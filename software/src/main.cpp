#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "esp_camera.h"
#include <WiFi.h>
#include "camera_pins.h"
#include "soc/soc.h"
#include <WebServer.h>
#include <TickTwo.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <time.h>
#include <ArduinoJson.h>

//NTP setting
const char* NTP_server ="pool.ntp.org" ;
const long getoffset = 12600;
const int daylight = 0;

//Endpoint server url
const char* endpoint_url = "http://192.168.1.5/IoT_color_detection.php";

// Network credentials
const char *ssid = "WiFi_ssid";
const char *password = "WIFi_password";

//Pins
#define cam_led_pin 4
#define led_status 14
#define press_pin 13
#define buzzer_pin 15
#define relay_pin 12


//flags
bool loopflag = false;
bool desition_flag = true;
bool press_flag = false;
bool color_detection_flag = false;

//number of byte per pixel, for RGB565 is 2 and for RGB888 is 3
const int bytes_per_pixel_number = 2; 

//pixel density
const int pixelarea= 20;              

//resize picture as:
int x_scale = 135;
int y_scale = 30;
int x_length = 80;
int y_length = 50;

//LED PWM control
//percent of camera led light
int cam_led_on_percent = 10;
int cam_led_off_percent = 0;
int cam_led_on = cam_led_on_percent*256/100;
int cam_led_off = cam_led_off_percent*256/100;
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

//press configuration
const int debounceTime=100;
void press_ticker_callback();
TickTwo press_ticker(press_ticker_callback,debounceTime,1,MILLIS);

//relay configuration
const int relay_time = 500;
void relay_ticker_callback();
TickTwo relay_ticker(relay_ticker_callback,relay_time,1,MILLIS);

//buzzer configuration
const int buzzer_time = 500;
void buzzer_ticker_callback();
TickTwo buzzer_ticker(buzzer_ticker_callback,buzzer_time,1,MILLIS);

//webserver handler
WebServer server(80);

//camera error config
esp_err_t cameraInitError;

//position structure
struct Position{
  int x_scale;
  int y_scale;
};

Position positionfinder(std::vector<std::vector<cv::Point>> contours){
  Position pos;
  std::vector <cv::Point> all_points;
  double center_x;
  double center_y;
  double sum_x = 0, sum_y = 0;
  
  // collect all points
  for (size_t i = 0; i < contours.size(); i++) {
    all_points.insert(all_points.end(), contours[i].begin(), contours[i].end());
  }
  if (all_points.empty()) {
    // Serial.println("no point found");
    center_x = 0;
    center_y = 0;
  }else{
    for (const auto& point : all_points) {
      sum_x += point.x;
      sum_y += point.y;
    }
    center_x = sum_x / all_points.size();
    center_y = sum_y / all_points.size();
  }
  pos.x_scale = center_x;
  pos.y_scale = center_y;
  return pos;
}

//functions
void cropImage(uint8_t *input, int width, int height, int w, int h,const int w_size,const int h_size,uint8_t *output) {
  for (int j = 0; j < h_size; j++) {
    for (int i = 0; i < w_size; i++) {
      int dest_index = (j* w_size + i) * bytes_per_pixel_number;
      int src_index = (((j+h-1)*(width) + (w + i))*bytes_per_pixel_number)-1;
      output[dest_index] = input[src_index-1];
      output[dest_index+1] =input[src_index];
      if (bytes_per_pixel_number==3)
      {
        output[dest_index + 2] = input[src_index + 1];
      }
    }
  }
}

//RGB converter
void convertRGB565ToBGR(uint8_t* rgb565, int width, int height, cv::Mat& bgrImage) {
  bgrImage = cv::Mat(height, width, CV_8UC3);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint16_t pixel = (rgb565[(y * width + x) * 2] << 8) | rgb565[(y * width + x) * 2 + 1];
      uint8_t r = (pixel >> 11) & 0x1F;
      uint8_t g = (pixel >> 5) & 0x3F;
      uint8_t b = pixel & 0x1F;
      r = (r << 3) | (r >> 2);
      g = (g << 2) | (g >> 4);
      b = (b << 3) | (b >> 2);
      cv::Vec3b& pixelBGR = bgrImage.at<cv::Vec3b>(y, x);
      pixelBGR[0] = b;
      pixelBGR[1] = g;
      pixelBGR[2] = r;
    }
  }
}


bool rangdetector(std::vector<std::vector<cv::Point>> green_contours,
                  std::vector<std::vector<cv::Point>> yellow_contours,
                  std::vector<std::vector<cv::Point>> blue_contours)
                  {
  cv::Moments MG = cv::moments(green_contours[0]);
  cv::Moments MY = cv::moments(yellow_contours[0]);
  cv::Moments MB = cv::moments(blue_contours[0]);
  int yellow_x,green_x,blue_x;
  if (MY.m00!=0)
  {
    yellow_x = static_cast<int>(MY.m10 / MY.m00);
  }else{
    yellow_x = 0;
  }
  if (MG.m00!=0)
  {
    green_x = static_cast<int>(MG.m10 / MG.m00);
  }else{
    green_x = 0;
  }
  if (MB.m00!=0)
  {
    blue_x = static_cast<int>(MB.m10 / MB.m00);
  }else{
    blue_x = 0;
  }
  if(yellow_x>min(green_x,blue_x) && yellow_x<max(green_x,blue_x)){
    return 1;
  }else{
    return 0;
  }
}

void buzzer_on(){
  digitalWrite(buzzer_pin,HIGH);
}
void buzzer_off(){
  digitalWrite(buzzer_pin,LOW);
}
void relay_on(){
  digitalWrite(relay_pin,HIGH);  
}
void relay_off(){
  digitalWrite(relay_pin,LOW);  
}

void desition_handler(Position blue, Position yellow, Position brown){
  if (blue.x_scale>brown.x_scale && brown.x_scale>yellow.x_scale){
    buzzer_on();
    buzzer_ticker.start();
  }else{
    relay_on();
    relay_ticker.start();
  }  
}


//function for color detection
String color_detection() {
  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb || !fb->buf || fb->width <= 0 || fb->height <= 0) {
    log_e("Framebuffer is null");
    Serial.println("Framebuffer is null");
  }

  uint8_t *cut_buff = (uint8_t *) malloc(x_length*y_length*bytes_per_pixel_number);
  cropImage(fb->buf,fb->width,fb->height,x_scale,y_scale,x_length,y_length,cut_buff);

  int width = x_length;
  int height = y_length;
  
  // Create an OpenCV Mat to store the image (convert to BGR for OpenCV)
  cv::Mat image(height, width, CV_8UC3);
  

  // Convert RGB565 framebuffer data to BGR
  convertRGB565ToBGR(cut_buff, width, height, image);
  free(cut_buff);
  

  // Release the frame buffer
  esp_camera_fb_return(fb); // Clean up the framebuffer  
  fb = NULL;
  

  // Convert the image to HSV color space
  cv::Mat hsv_image;
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

  std::vector<cv::Mat> hsv_channels;
  cv::split(hsv_image,hsv_channels);
  cv::Mat h = hsv_channels[0]; // Hue
  cv::Mat s = hsv_channels[1]; // Saturation
  cv::Mat v = hsv_channels[2]; // Value

  // Equalize histogram for S and V channels
  cv::Mat eqs, eqv;
  cv::equalizeHist(s, eqs);
  cv::equalizeHist(v, eqv);

  // Merge the channels back
  std::vector<cv::Mat> merged_channels = {h, eqs, eqv};
  cv::merge(merged_channels, hsv_image);

  // Define color ranges
  cv::Scalar brown_lowwer(0,50,0);
  cv::Scalar brown_upper(30,200,50);
  
  cv::Scalar yellow_lower(30,100,150);
  cv::Scalar yellow_upper(50,255,255);

  cv::Scalar blue_lower(100,200,150);
  cv::Scalar blue_upper(125,255,255);

  // Create masks for each color
  cv::Mat brown_mask, yellow_mask, blue_mask;
  cv::inRange(hsv_image, brown_lowwer, brown_upper, brown_mask);
  cv::inRange(hsv_image, yellow_lower, yellow_upper, yellow_mask);
  cv::inRange(hsv_image, blue_lower, blue_upper, blue_mask);
  hsv_image.release();
  
  // Find contours for each color
  std::vector<std::vector<cv::Point>> brown_contours, yellow_contours, blue_contours;
  cv::findContours(brown_mask, brown_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::findContours(yellow_mask, yellow_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::findContours(blue_mask, blue_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  
  double min_area = pixelarea;
  std::vector<std::vector<cv::Point>> filtered_brown_contours,filtered_blue_contours,filtered_yellow_contours;

  for (const auto& cnt : brown_contours) {
      if (cv::contourArea(cnt) > min_area) {
          filtered_brown_contours.push_back(cnt);
      }
  }

  for (const auto& cnt : yellow_contours) {
      if (cv::contourArea(cnt) > min_area) {
          filtered_yellow_contours.push_back(cnt);
      }
  }

  for (const auto& cnt : blue_contours) {
      if (cv::contourArea(cnt) > min_area) {
          filtered_blue_contours.push_back(cnt);
      }
  }

  brown_mask.release();
  yellow_mask.release();
  blue_mask.release();

  printf("\n brown_contours.size(), yellow_contours.size(), blue_contours.size(): %d, %d, %d  \n"
    , brown_contours.size(), yellow_contours.size(), blue_contours.size());


  Position blue_position = positionfinder(filtered_blue_contours);
  printf("Blue: X_scale is=%d, Y_scale is=%d\n", blue_position.x_scale, blue_position.y_scale);

  Position yellow_position = positionfinder(filtered_yellow_contours);
  printf("Yellow: X_scale is=%d, Y_scale is=%d\n", yellow_position.x_scale, yellow_position.y_scale);

  Position brown_position = positionfinder(filtered_brown_contours);
  printf("Brown: X_scale is=%d, Y_scale is=%d\n", brown_position.x_scale, brown_position.y_scale);
  desition_handler(blue_position,yellow_position,brown_position);
  String result_colorposition =String(blue_position.x_scale) + "," + String(blue_position.x_scale) + "/" +
                              String(yellow_position.x_scale) + "," + String(yellow_position.x_scale) + "/" +
                              String(brown_position.x_scale) + "," + String(brown_position.x_scale) + "/";
  return result_colorposition;
}

//Web server response
void webpagehandler(unsigned char *buffer,int length){
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: image/jpeg\r\n";         //for download
  client.write(response.c_str(), response.length());  // Convert String to const char* using c_str().
  server.send_P(200, "image/jpeg",(const char*) buffer, length);
  client.write("--frame\r\n", 8);
  client.write("Content-Type: image/jpeg\r\n", 26);
  String contentLength = "Content-Length: " + String(length) + "\r\n\r\n";
  client.write(contentLength.c_str(), contentLength.length());
  client.write(buffer, length);
  client.write("\r\n", 2);
}

//image taker
void pagehandlermain(){
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb){
    Serial.println("Camera capture failed");
    return;
  }
  uint8_t *jpeg_buff;
  size_t jpeg_len;
  frame2jpg(fb,80,&jpeg_buff,&jpeg_len);
  webpagehandler(jpeg_buff,jpeg_len);
  esp_camera_fb_return(fb);
  free(jpeg_buff);
}

//image tacker and ROI
void pagehandler_croped(){
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb){
    Serial.println("Camera capture failed");
    return;
  }
  size_t cut_len =x_length*y_length*bytes_per_pixel_number;
  uint8_t *cut_buff = (uint8_t *) malloc(x_length*y_length*bytes_per_pixel_number);
  uint8_t *jpeg_buff;
  size_t jpeg_len;
  cropImage(fb->buf,fb->width,fb->height,x_scale,y_scale,x_length,y_length,cut_buff);
  fmt2jpg(cut_buff,cut_len,x_length,y_length,fb->format,80,&jpeg_buff,&jpeg_len);
  webpagehandler(jpeg_buff,jpeg_len);
  esp_camera_fb_return(fb);
  free(jpeg_buff);
  free(cut_buff);
}

//interrupt handler
IRAM_ATTR void press_interrupt_handler(){
  press_flag = true;
}

//interrupt initiallizer
void PIN_init() {
  pinMode(press_pin, INPUT_PULLUP);
  attachInterrupt(press_pin, press_interrupt_handler, FALLING);

  pinMode(relay_pin,OUTPUT);
  digitalWrite(relay_pin,LOW);

  pinMode(buzzer_pin,OUTPUT);
  digitalWrite(buzzer_pin,LOW);
}

//wifi connection setting
void wifiinit(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

//camera flash LED control by PWM
void led_pwm_init(){
  ledcSetup(pwmChannel,pwmFreq,pwmResolution);
  ledcAttachPin(cam_led_pin,pwmChannel);
}



void press_ticker_callback(){
  press_ticker.stop();
  if (digitalRead(press_pin) == LOW)
  {
    color_detection_flag = true;
  }
}

void buzzer_ticker_callback(){
  buzzer_ticker.stop();
  buzzer_off();
}

void relay_ticker_callback(){
  relay_ticker.stop();
  relay_off();
}

String local_time2string(){
  struct tm timeinformation;
  if (!getLocalTime(&timeinformation))
  {
    printf("fail to red time!");
    return "NULL";
  }
  char return_time[100];
  strftime(return_time,sizeof(return_time),"%Y-%m-%d %H:%M:%S",&timeinformation);
  return return_time;
}

void http_post(String timestamp,String colorcodes){
  JsonDocument doc;
  doc["timestamp"] = timestamp;
  doc["colorcodes"] = colorcodes;
  String jsonpayload;
  serializeJson(doc,jsonpayload); 
  
  HTTPClient http;
  http.begin(endpoint_url);
  http.addHeader("Content-Type", "application/json");
  int httpresponse = http.POST(jsonpayload);
  if (httpresponse>0)
  {
    printf("HTTP response code: %s\n",String(httpresponse));
    printf("Response is: %s\n",http.getString());
  }else printf("Error on send data: ", String(httpresponse));
  http.end();  
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  pinMode(led_status,OUTPUT);
  PIN_init();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_RGB565;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  wifiinit();
  // server.on("/",color_detection);
  server.on("/",pagehandlermain);
  server.on("/cd",pagehandler_croped);
  server.begin();
  led_pwm_init();

  configTime(getoffset,daylight,NTP_server);
  printf("local time is %s  \n",local_time2string());
}

void crophandler(){
  printf("X_scale is=%d, Y_scale is=%d, X_length is=%d, Y_length is=%d \n",
        x_scale,y_scale,x_length,y_length);
  int count = 0;
  int state = 0;
  Serial.println("please insert the x_scale");
  while(count<4)
  {
    if (Serial.available()>0)
    {
      String input = Serial.readStringUntil('\n');
      input.trim();
      if (input.length() == 0) continue;
      int number = input.toInt();
      switch (state)
      {
      case 0:
        x_scale = number;
        state = 1;
        Serial.println("please insert the y_scale");
        break;
      case 1:
        y_scale = number;
        state = 2;
        Serial.println("please insert the x_length");
        break;
      case 2:
        x_length = number;
        state =3 ;
        Serial.println("please insert the y_length");
        break;
      case 3:
        y_length = number;
        Serial.println("its done");
        break;
      }
      count++;
    }
  }
  printf("X_scale is=%d, Y_scale is=%d, X_length is=%d, Y_length is=%d \n",
        x_scale,y_scale,x_length,y_length);
}

void loop() {
  if (WiFi.status()==WL_DISCONNECTED){
    wifiinit();
  }

  // color_detection();
  server.handleClient();

  //ticker update
  press_ticker.update();
  buzzer_ticker.update();
  relay_ticker.update();

  if (Serial.available()>0){
    String inputstring = Serial.readStringUntil('\n');
    inputstring.trim();
    Serial.println("input detected");
    Serial.println(Serial.readStringUntil('\n'));
    if (inputstring == "r"){
      Serial.println("color detection has been runed!");
      loopflag = true;
    }else if(inputstring == "s"){
      Serial.println("color detection stopped!");
      loopflag = false;
    }else if (inputstring == "m"){
      loopflag = false;
      Serial.println("color detection stopped!");
      Serial.println("resizer");
      crophandler();
    }
  }
  
  if (press_flag && desition_flag)
  {
    press_ticker.start();
    if (color_detection_flag)
    {
      ledcWrite(pwmChannel,cam_led_on);
      String colorcode = color_detection();
      ledcWrite(pwmChannel,cam_led_off);
      desition_flag = false;
      color_detection_flag = false;
      press_flag = false;
      String time_stamp = local_time2string();
      http_post(time_stamp,colorcode);
    }
  }
  desition_flag = (digitalRead(press_pin) == HIGH)?true:false;
}