<h1 align="center" style = "font-weight:bold; font-size: 32px; font-family: Times New Roman;">
ESP32 Color Detection
</h1>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Overview
</h2>
<p align="justify" class = "paraghraph">
This project implements a color detection system using an ESP32 microcontroller with an OV2640 camera module. The system captures images, processes them to detect specific colors (brown, yellow, blue), determines their relative positions, and triggers actions (buzzer or relay) based on the detected positions. It also provides a web server to stream images and supports HTTP POST requests to send detection results to a specified endpoint.
</p>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Prerequisites
</h2>
<ol class = "paraghraph">
  <li>Hardware:</li>
  	<ul>
  		<li>
			ESP32 module with OV2640 camera
  		</li>
		<li>
			LED (for camera flash, pin 4)
  		</li>
		<li>
			Status LED (pin 14)
  		</li>
		<li>
			Push button (pin 13)
  		</li>
		<li>
			Buzzer (pin 15)
  		</li>
		<li>
			Relay (pin 12)
  		</li>
	</ul>
	<li>Software:</li>
	<ul>
		<li>
			Arduino IDE or Platform-IO with ESP32 support
		</li>
		<li>
			Libraries:
		</li>
		<ul>
			<li>
				ESP_camera (for camera control).
			</li>
			<li>
				WiFi (for network connectivity).
			</li>
			<li>
				WebServer (for HTTP server).
			</li>
			<li>
				TickTwo (for timer-based callbacks).
			</li>
			<li>
				HTTPClient (for HTTP POST requests).
			</li>
			<li>
				ArduinoJson (for JSON serialization).
			</li>
			<li>
				OpenCV (core, imgproc, imgcodecs for image processing).
			</li>
		</ul>
	</ul>
	<li>Network:</li>
	<ul>
		<li>
			WiFi credentials (SSID and password).
		</li>
		<li>
			mAccess to an NTP server (pool.ntp.org) for time synchronizationm.
		</li>
		<li>
			A server endpoint for HTTP POST requests (e.g., http://192.168.1.5/IoT_color_detection.php).
		</li>
	</ul>
</ol>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Installation and Setup
</h2>
<ol>
	<li>
		<strong>Install Required Libraries:</strong>
		<br>Install the necessary libraries via the Arduino Library Manager or Platform-IO.</br>
	</li>
	<li>
		<strong>Configure Hardware:</strong>
		<ul>
			<li>	
				Connect the OV2640 camera to the ESP32 using the pin definitions in camera_pins.h.
			</li>
			<li>
				Connect the LED, status LED, push button, buzzer, and relay to the specified pins (4, 14, 13, 15, 12, respectively).
			</li>
		</ul>
	</li>
	<li>
		<strong>Update Network Credentials:</strong>
		<br>Modify the following constants in the code and replace with your WiFi SSID and password:</br>

```sv
// Network credentials
const char *ssid = "WiFi_ssid";
const char *password = "WIFi_password";
```
</li>
	<li>
		<strong>Set Endpoint URL:</strong>
		<br>Update the server endpoint for HTTP POST requests:</br>

  ```sv
    //Endpoint server url
    const char* endpoint_url = "http://192.168.1.5/IoT_color_detection.php";
  ```
</li>
	<li>
		<strong>Upload Code:</strong>
	</li>
	<ul>
		<li>
			Upload the code to the ESP32 using the Platform-IO.
		</li>
		<li>
			Ensure the board is set to the correct ESP32 model (e.g., ESP32-CAM).
		</li>
	</ul>
	<li>
		<strong>Run the Program</strong>
	</li>
	<ul>
		<li>
			Open the Serial Monitor (115200 baud) to view logs and the device's IP address.
		</li>
		<li>
			Access the web server at http://"ESP32_IP" to view the camera feed or http://"ESP32_IP"/cd for cropped images.
		</li>
	</ul>
</ol>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Code Structure
</h2>
<ul>
	<li>
		<strong>Main File:</strong> The primary source file contains all functionality.
	</li>
	<li>
		<strong>Key Components:</strong>
	</li>
	<ul>
		<li>
			<strong>Camera Configuration:</strong> Initializes the OV2640 camera with QVGA resolution and RGB565 format.
		</li>
		<li>
			<strong>Color Detection:</strong> Processes images to detect brown, yellow, and blue colors using OpenCV.
		</li>
		<li>
			<strong>Position Detection:</strong> Calculates the center of detected color contours.
		</li>
		<li>
			<strong>Decision Logic:</strong> Triggers a buzzer or relay based on the relative positions of detected colors.
		</li>
		<li>
			<strong>Web Server:</strong> Serves full or cropped images via HTTP.
		</li>
		<li>
			<strong>Interrupt Handling:</strong> Handles button presses with debouncing.
		</li>
		<li>
			<strong>HTTP POST:</strong> Sends detection results (color positions and timestamp) to a server.
		</li>
		<li>
			<strong>NTP Client:</strong> Synchronizes time for timestamping results
		</li>
	</ul>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Key Functions
</h2>
<ul>
	<li>
		Position positionfinder(std::vector<std::vector<cv::Point>> contours): Calculates the center of contours for detected colors.
	</li>
	<li>
		void cropImage(uint8_t *input, int width, int height, int w, int h, const int w_size, const int h_size, uint8_t *output): Crops an image to a specified region of interest (ROI).
	</li>
	<li>
		void convertRGB565ToBGR(uint8_t* rgb565, int width, int height, cv::Mat& bgrImage): Converts RGB565 image data to BGR format for OpenCV processing.
	</li>
	<li>
		bool rangdetector(...): Determines if the yellow contour is between green and blue contours (not used in the main loop).
	</li>
	<li>
		String color_detection(): Captures an image, detects colors, calculates positions, and triggers actions.
	</li>
	<li>
		void webpagehandler(unsigned char *buffer, int length): Sends JPEG images to clients via the web server.
	</li>
	<li>
		void pagehandlermain(): Serves the full camera image.
	</li>
	<li>
		void pagehandler_croped(): Serves a cropped image based on defined ROI.
	</li>
	<li>
		void http_post(String timestamp, String colorcodes): Sends detection results to the server.
	</li>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Usage
</h2>
<ol>
	<li>
		<strong>Start the System:</strong>
	</li>
	<ul>
		<li>
			Power on the ESP32. It will connect to the specified WiFi network and initialize the camera.
		</li>
		<li>
			The IP address will be printed to the Serial Monitor.
		</li>
	</ul>
	<li>
		<strong>Access the Web Server:</strong>
	</li>
	<ul>
		<li>Open a browser and navigate to http://"ESP32_IP" to view the full camera feed.
		</li>
		<li>Use http://"ESP32_IP"/cd to view the cropped ROI image.
		</li>
	</ul>
	<li>
		<strong>Trigger Color Detection:</strong>
	</li>
	<ul>
		<li>Press the button (pin 13) to initiate color detection.
		</li>
		<li>Alternatively, send the command r via the Serial Monitor to start detection.
		</li>
		<li>Send s to stop detection or m to adjust the ROI parameters (x_scale, y_scale, x_length, y_length) via Serial input.
		</li>
	</ul>
	<li>
		<strong>View Results:</strong>
	</li>
	<ul>
		<li>The system detects brown, yellow, and blue colors in the ROI.
		</li>
		<li>If the blue position is to the right of brown, and brown is to the right of yellow, the buzzer is activated; otherwise, the relay is triggered.
		</li>
		<li>Results (color positions and timestamp) are sent to the specified endpoint via HTTP POST.
		</li>
	</ul>
	<li>
		<strong>Serial Commands:</strong>
	</li>
	<ul>
		<li>r: Run color detection.
		</li>
		<li>s: Stop color detection.
		</li>
		<li>m: Modify ROI parameters interactively.
		</li>
	</ul>
</ol>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Example Output
</h2>
<ul>
	<li>Serial Monitor (example):
	</li>

  ```sv
    WiFi connected 
	Camera Ready! Use 'http://192.168.1.10' to connect
	local time is 2025-06-01 20:33:45
	Blue: X_scale is=50, Y_scale is=25
	Yellow: X_scale is=30, Y_scale is=20
	Brown: X_scale is=40, Y_scale is=22
	HTTP response code: 200
  ```
  <li>HTTP POST Payload (example):
	</li>

  ```sv
	"timestamp": "2025-06-01 20:33:45",
	"colorcode": "50,50/30,30/40,40"
  ```
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Performance
</h2>
<ul>
	<li>
		<strong>Camera:</strong> Captures QVGA (320x240) images in RGB565 format.
	</li>
	<li>
		<strong>Color Detection:</strong> Uses HSV color space to detect brown, yellow, and blue colors with predefined ranges.
	</li>
	<li>
		<strong>Position Analysis:</strong> Computes the center of detected contours to determine relative positions.
	</li>
	<li>
		<strong>Actions:</strong>
	</li>
	<ul>
		<li>Buzzer activates for 500ms if blue > brown > yellow in x-coordinates.
		</li>
		<li>Relay activates for 500ms otherwise.
		</li>
	</ul>
	<li>
		<strong>Web Server:</strong> Streams JPEG images (full or cropped) with 80% quality.
	</li>
	<li>
		<strong>Network:</strong> Sends JSON payloads with detection results to the specified endpoint.
	</li>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Testing and Debugging
</h2>
<ul>
	<li><strong>Testing:</strong>
	</li>
	<ul>
		<li>Verify camera functionality by accessing the web server.
		</li>
		<li>Test color detection by pressing the button or sending r via Serial.
		</li>
		<li>Check HTTP POST responses in the Serial Monitor.
		</li>
	</ul>
	<li><strong>Common Issues:</strong>
	</li>
	<ul>
		<li><strong>Camera Failure:</strong> Check pin connections and ensure camera_pins.h matches your hardware.
		</li>
		<li><strong>WiFi Disconnection:</strong> Ensure correct SSID/password and stable network.
		</li>
		<li><strong>Invalid ROI:</strong> Use the m command to set appropriate ROI parameters.
		</li>
		<li><strong>Memory Issues:</strong> Ensure sufficient PSRAM for frame buffers.
		</li>
	</ul>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Limitations
</h2>
<ul>
	<li>
		Only detects brown, yellow, and blue colors with fixed HSV ranges.
	</li>
	<li>
		Limited to QVGA resolution due to ESP32 memory constraints.
	</li>
	<li>
		ROI parameters must be manually adjusted via Serial input.
	</li>
	<li>No real-time streaming for color detection results (only on button press or Serial command).
	</li>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Future Improvements
</h2>
<ul>
	<li>Add support for dynamic HSV range calibration.
	</li>
	<li>Implement real-time color detection without button presses.
	</li>
	<li>Enhance web server to display detection results.
	</li>
	<li>Add error handling for network failures.
	</li>
	<li>Support higher resolutions or additional color detection.
	</li>
</ul>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Resources
</h2>

<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><a href="https://github.com/espressif/esp32-camera">ESP32 Camera Library</a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><a href="https://docs.opencv.org/">OpenCV Documentation</a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><a href="https://arduinojson.org/">ArduinoJson Documentation</a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><a href="https://github.com/arduino-libraries/NTPClient">NTP Client Documentation</a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><span style='font-family:"Times New Roman",serif;'>Full BOM&nbsp;</span><a href="https://github.com/abbasghadir/IoT_color_detection/blob/main/hardware/IoT_color_detection_BOM.xlsx"><span style='font-family:"Times New Roman",serif;'>(Bill of Materials links to GitHub repository)</span></a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><span style='font-family:"Times New Roman",serif;'>Source code link&nbsp;</span><a href="https://github.com/abbasghadir/IoT_color_detection/tree/main/software"><span style='font-family:"Times New Roman",serif;'>(GitHub repository)</span></a></li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><span style='font-family:"Times New Roman",serif;'>Schematic diagram and PCB layout&nbsp;</span><a href="https://github.com/abbasghadir/IoT_color_detection/tree/main/hardware"><span style='font-family:"Times New Roman",serif;'>(GitHub repository)</span></a></li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><span style='font-family:"Times New Roman",serif;'>&nbsp;</span></p>
<h2 style = "font-weight:bold; font-size: 24px; font-family: Times New Roman;">
Languages and Tools:
</h2>
<p align="left">
<a href="https://www.cprogramming.com/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/> </a>
<a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a>
<a href="https://git-scm.com/" target="_blank" rel="noreferrer"> <img src="https://www.vectorlogo.zone/logos/git-scm/git-scm-icon.svg" alt="git" width="40" height="40"/> </a>
<a href="https://www.arduino.cc/" target="_blank" rel="noreferrer"> <img src="https://cdn.worldvectorlogo.com/logos/arduino-1.svg" alt="arduino" width="40" height="40"/> </a>
<a href="https://platformio.org//" target="_blank" rel="noreferrer"> <img src="https://cdn.platformio.org/images/platformio-logo.17fdc3bc.png" alt="arduino" width="40" height="40"/> </a>
</p>