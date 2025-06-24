#include "camera_manager.h"

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
// 4 for flash led or 33 for normal led
#define LED_GPIO_NUM 4      // GPIO pin for the flash LED
#define PWM_FREQUENCY 5000  // Set PWM (Pulse Width Modulation) frequency
#define PWM_RESOLUTION 8    // Set PWM resolution (8-bit)
#define PWM_DUTY_CYCLE 128  // Set moderate brightness (50% of 255)

extern ROI_config ROI;

void camera_init(){
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
    config.frame_size = FRAMESIZE_VGA;
    //change max_x and max_y simultaneously
    config.pixel_format = PIXFORMAT_RGB565;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1; 
    if (!psramFound()) {
        Serial.println("PSRAM not found! Falling back to lower resolution.");
        config.frame_size = FRAMESIZE_QVGA; 
    }
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return;
    }
}

void cropImage(uint8_t *orig_buff, int orig_x_width, int orig_y_height){//, const int ROI_x,const int ROI_y,const int ROI_x_width,const int ROI_y_height,const int BPP,uint8_t *ROI_buff) {
  for (int y = 0; y < (ROI.y_height); y++) {
    for (int x = 0; x < (ROI.x_width*ROI.BPPN); x++) {
        ROI.buffer[x + (y * ROI.BPPN * ROI.x_width)] = orig_buff[((ROI.x*ROI.BPPN)+x)+(orig_x_width*ROI.BPPN*(y+ROI.y))];
    }
  }
}
void camera_capture(void *pvParameters) {
// void camera_capture(){//uint8_t *ROI_buff ,const int ROI_x, const int ROI_y, const int ROI_height, const int ROI_width,const int BPP){
    Serial.println("start camera capture");
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb || !fb->buf || fb->width <= 0 || fb->height <= 0) {
        log_e("Framebuffer is null");
        Serial.println("Framebuffer is null");
        esp_camera_fb_return(fb);
        // return;
    }
    // uint8_t *ROI_buff = (uint8_t *) malloc(ROI_x*ROI_y*BPP);
    if (ROI.buffer != NULL) {
        free(ROI.buffer);
        ROI.buffer = NULL;
    }
    ROI.buffer = (uint8_t*)malloc(ROI.x_width * ROI.y_height * ROI.BPPN);
    cropImage(fb->buf,fb->width,fb->height);//,ROI.x,ROI.y,ROI.y_height,ROI.x_width,ROI.BPPN,ROI.buffer);
    ROI.format = fb->format;
    esp_camera_fb_return(fb);
    Serial.println("stop camera capture");
    vTaskDelay(10 / portTICK_PERIOD_MS);  
    // return cut_buff;
}