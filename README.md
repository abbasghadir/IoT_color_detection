ESP32 Color Detection
Overview
This project implements a color detection system using an ESP32 microcontroller with an OV2640 camera module. The system captures images, processes them to detect specific colors (brown, yellow, blue), determines their relative positions, and triggers actions (buzzer or relay) based on the detected positions. It also provides a web server to stream images and supports HTTP POST requests to send detection results to a specified endpoint.
Prerequisites
•	Hardware:
    o ESP32 module with OV2640 camera
    o	LED (for camera flash, pin 4)
    o	Status LED (pin 14)
    o	Push button (pin 13)
    o	Buzzer (pin 15)
    o	Relay (pin 12)

•	Software:
    o	Arduino IDE or Platform-IO with ESP32 support
    o	Libraries: 
        	ESP_camera (for camera control)
        	WiFi (for network connectivity)
        	WebServer (for HTTP server)
        	TickTwo (for timer-based callbacks)
        	HTTPClient (for HTTP POST requests)
        	ArduinoJson (for JSON serialization)
        	OpenCV (core, imgproc, imgcodecs for image processing)

•	Network:
    o	WiFi credentials (SSID and password)
    o	Access to an NTP server (pool.ntp.org) for time synchronization
    o	A server endpoint for HTTP POST requests (e.g., http://192.168.1.5/IoT_color_detection.php)

Installation and Setup
1.	Install Required Libraries:
    Install the necessary libraries via the Arduino Library Manager or Platform-IO:

2.	Configure Hardware:
    o	Connect the OV2640 camera to the ESP32 using the pin definitions     in camera_pins.h.
    o	Connect the LED, status LED, push button, buzzer, and relay to the specified pins (4, 14, 13, 15, 12, respectively).

3.	Update Network Credentials:
    Modify the following constants in the code and replace with your WiFi SSID and password:
    {
        // Network credentials
        const char *ssid = "WiFi_ssid";
        const char *password = "WIFi_password";
    }

4.	Set Endpoint URL:
    Update the server endpoint for HTTP POST requests:
    {
        const char* endpoint_url = "http://192.168.1.5/IoT_color_detection.php";
    }

5.	Upload Code:
    o	Upload the code to the ESP32 using the Platform-IO.
    o	Ensure the board is set to the correct ESP32 model (e.g., ESP32-CAM).

6.	Run the Program:
    o	Open the Serial Monitor (115200 baud) to view logs and the device's IP address.
    o	Access the web server at http://<ESP32_IP> to view the camera feed or http://<ESP32_IP>/cd for cropped images.

Code Structure
•	Main File: The primary source file contains all functionality.
•	Key Components:
    o	Camera Configuration: Initializes the OV2640 camera with QVGA resolution and RGB565 format.
    o	Color Detection: Processes images to detect brown, yellow, and blue colors using OpenCV.
    o	Position Detection: Calculates the center of detected color contours.
    o	Decision Logic: Triggers a buzzer or relay based on the relative positions of detected colors.
    o	Web Server: Serves full or cropped images via HTTP.
    o	Interrupt Handling: Handles button presses with debouncing.
    o	HTTP POST: Sends detection results (color positions and timestamp) to a server.
    o	NTP Client: Synchronizes time for timestamping results.


Key Functions
    •	Position positionfinder(std::vector<std::vector<cv::Point>> contours): Calculates the center of contours for detected colors.
    •	void cropImage(uint8_t *input, int width, int height, int w, int h, const int w_size, const int h_size, uint8_t *output): Crops an image to a specified region of interest (ROI).
    •	void convertRGB565ToBGR(uint8_t* rgb565, int width, int height, cv::Mat& bgrImage): Converts RGB565 image data to BGR format for OpenCV processing.
    •	bool rangdetector(...): Determines if the yellow contour is between green and blue contours (not used in the main loop).
    •	String color_detection(): Captures an image, detects colors, calculates positions, and triggers actions.
    •	void webpagehandler(unsigned char *buffer, int length): Sends JPEG images to clients via the web server.
    •	void pagehandlermain(): Serves the full camera image.
    •	void pagehandler_croped(): Serves a cropped image based on defined ROI.
    •	void http_post(String timestamp, String colorcodes): Sends detection results to the server.

Usage
1.	Start the System:
    o	Power on the ESP32. It will connect to the specified WiFi network and initialize the camera.
    o	The IP address will be printed to the Serial Monitor.
    
2.	Access the Web Server:
    o	Open a browser and navigate to http://<ESP32_IP> to view the full camera feed.
    o	Use http://<ESP32_IP>/cd to view the cropped ROI image.

3.	Trigger Color Detection:
    o	Press the button (pin 13) to initiate color detection.
    o	Alternatively, send the command r via the Serial Monitor to start detection.
    o	Send s to stop detection or m to adjust the ROI parameters (x_scale, y_scale, x_length, y_length) via Serial input.

4.	View Results:
    o	The system detects brown, yellow, and blue colors in the ROI.
    o	If the blue position is to the right of brown, and brown is to the right of yellow, the buzzer is activated; otherwise, the relay is triggered.
    o	Results (color positions and timestamp) are sent to the specified endpoint via HTTP POST.

5.	Serial Commands:
    o	r: Run color detection.
    o	s: Stop color detection.
    o	m: Modify ROI parameters interactively.
    
Example Output
•	Serial Monitor (example):
    {
        WiFi connected
        Camera Ready! Use 'http://192.168.1.100' to connect
        Blue: X_scale is=50, Y_scale is=25
        Yellow: X_scale is=30, Y_scale is=20
        Brown: X_scale is=40, Y_scale is=22
    }

•	HTTP POST Payload (example):
    {
        "timestamp": "2025-06-01 20:33:45",
        "colorcodes": "50,50/30,30/40,40/"
    }
 
Performance
•	Camera: Captures QVGA (320x240) images in RGB565 format.
•	Color Detection: Uses HSV color space to detect brown, yellow, and   blue colors with predefined ranges.
•	Position Analysis: Computes the center of detected contours to determine relative positions.
•	Actions:
    o	Buzzer activates for 500ms if blue > brown > yellow in x-coordinates.
    o	Relay activates for 500ms otherwise.
•	Web Server: Streams JPEG images (full or cropped) with 80% quality.
•	Network: Sends JSON payloads with detection results to the specified endpoint.

Testing and Debugging
•	Testing:
    o	Verify camera functionality by accessing the web server.
    o	Test color detection by pressing the button or sending r via Serial.
    o	Check HTTP POST responses in the Serial Monitor.
•	Common Issues:
    o	Camera Failure: Check pin connections and ensure camera_pins.h matches your hardware.
    o	WiFi Disconnection: Ensure correct SSID/password and stable network.
    o	Invalid ROI: Use the m command to set appropriate ROI parameters.
    o	Memory Issues: Ensure sufficient PSRAM for frame buffers.
Limitations
•	Only detects brown, yellow, and blue colors with fixed HSV ranges.
•	Limited to QVGA resolution due to ESP32 memory constraints.
•	ROI parameters must be manually adjusted via Serial input.
•	No real-time streaming for color detection results (only on button press or Serial command).
Future Improvements
•	Add support for dynamic HSV range calibration.
•	Implement real-time color detection without button presses.
•	Enhance web server to display detection results.
•	Add error handling for network failures.
•	Support higher resolutions or additional color detection.

Contributing
•   Contributions are welcome! Please submit issues or pull requests via the GitHub repository.
