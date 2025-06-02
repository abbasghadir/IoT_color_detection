<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:30px;font-family:"Aptos",sans-serif;text-align:center;font-family:"Times New Roman";'><strong>ESP32 Color Detection</strong></p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:"Times New Roman";'><strong>Overview</strong></p>
<p style='font-family:"Times New Roman";margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:"Times New Roman";'>This project implements a color detection system using an ESP32 microcontroller with an OV2640 camera module. The system captures images, processes them to detect specific colors (brown, yellow, blue), determines their relative positions, and triggers actions (buzzer or relay) based on the detected positions. It also provides a web server to stream images and supports HTTP POST requests to send detection results to a specified endpoint.</p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:"Times New Roman";'><strong>Prerequisites</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:"Times New Roman";'><strong>Hardware</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">ESP32 module with OV2640 camera</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">LED (for camera flash, pin 4)</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;font-family:Times New Roman;">Status LED (pin 14)</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Push button (pin 13)</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Buzzer (pin 15)</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Relay (pin 12)</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Software</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Arduino IDE or Platform-IO with ESP32 support</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Libraries:&nbsp;<ul style="margin-bottom:0in;margin-top:0in;" type="square">
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">ESP_camera (for camera control)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">WiFi (for network connectivity)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">WebServer (for HTTP server)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">TickTwo (for timer-based callbacks)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">HTTPClient (for HTTP POST requests)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">ArduinoJson (for JSON serialization)</li>
                    <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">OpenCV (core, imgproc, imgcodecs for image processing)</li>
                </ul>
            </li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Network</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">WiFi credentials (SSID and password)</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Access to an NTP server (pool.ntp.org) for time synchronization</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;line-height: 115%;font-family:Times New Roman;">A server endpoint for HTTP POST requests (e.g., http://192.168.1.5/IoT_color_detection.php)</li>
        </ul>
    </li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Installation and Setup</strong></p>
<ol style="margin-bottom:0in;margin-top:0in;" start="1" type="1">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;font-family:Times New Roman;'><strong>Install Required Libraries</strong>:<br>&nbsp;Install the necessary libraries via the Arduino Library Manager or Platform-IO:</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Configure Hardware</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Connect the OV2640 camera to the ESP32 using the pin definitions in camera_pins.h.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Connect the LED, status LED, push button, buzzer, and relay to the specified pins (4, 14, 13, 15, 12, respectively).</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;font-family:Times New Roman;'><strong>Update Network Credentials</strong>:</li>
</ol>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:.5in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Modify the following constants in the code and replace with your WiFi SSID and password:</p>

* WiFi setting
  ```sh
  // Network credentials
  const char *ssid = "WiFi_ssid";
  const char *password = "WIFi_password";
  ```
<ol style="margin-bottom:0in;margin-top:0in;" start="4" type="1">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;font-family:Times New Roman;'><strong>Set Endpoint URL</strong>:</li>
</ol>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:.5in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Update the server endpoint for HTTP POST requests:</p>

* End-point URL
  ```sh
    //Endpoint server url
    const char* endpoint_url = "http://192.168.1.5/IoT_color_detection.php";
  ```

<ol style="margin-bottom:0in;margin-top:0in;" start="5" type="1">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Upload Code</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Upload the code to the ESP32 using the Platform-IO.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Ensure the board is set to the correct ESP32 model (e.g., ESP32-CAM).</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Run the Program</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Open the Serial Monitor (115200 baud) to view logs and the device&apos;s IP address.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Access the web server at http://&lt;ESP32_IP&gt; to view the camera feed or http://&lt;ESP32_IP&gt;/cd for cropped images.</li>
        </ul>
    </li>
</ol>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Code Structure</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Main File</strong>: The primary source file contains all functionality.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Key Components</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Camera Configuration</strong>: Initializes the OV2640 camera with QVGA resolution and RGB565 format.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Color Detection</strong>: Processes images to detect brown, yellow, and blue colors using OpenCV.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Position Detection</strong>: Calculates the center of detected color contours.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Decision Logic</strong>: Triggers a buzzer or relay based on the relative positions of detected colors.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Web Server</strong>: Serves full or cropped images via HTTP.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Interrupt Handling</strong>: Handles button presses with debouncing.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>HTTP POST</strong>: Sends detection results (color positions and timestamp) to a server.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>NTP Client</strong>: Synchronizes time for timestamping results.</li>
        </ul>
    </li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Key Functions</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Position positionfinder(std::vector&lt;std::vector&lt;cv::Point&gt;&gt; contours): Calculates the center of contours for detected colors.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void cropImage(uint8_t *input, int width, int height, int w, int h, const int w_size, const int h_size, uint8_t *output): Crops an image to a specified region of interest (ROI).</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void convertRGB565ToBGR(uint8_t* rgb565, int width, int height, cv::Mat&amp; bgrImage): Converts RGB565 image data to BGR format for OpenCV processing.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>bool rangdetector(...): Determines if the yellow contour is between green and blue contours (not used in the main loop).</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>String color_detection(): Captures an image, detects colors, calculates positions, and triggers actions.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void webpagehandler(unsigned char *buffer, int length): Sends JPEG images to clients via the web server.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void pagehandlermain(): Serves the full camera image.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void pagehandler_croped(): Serves a cropped image based on defined ROI.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>void http_post(String timestamp, String colorcodes): Sends detection results to the server.</li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Usage</strong></p>
<ol style="margin-bottom:0in;margin-top:0in;" start="1" type="1">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Start the System</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Power on the ESP32. It will connect to the specified WiFi network and initialize the camera.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">The IP address will be printed to the Serial Monitor.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Access the Web Server</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Open a browser and navigate to http://&lt;ESP32_IP&gt; to view the full camera feed.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Use http://&lt;ESP32_IP&gt;/cd to view the cropped ROI image.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Trigger Color Detection</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Press the button (pin 13) to initiate color detection.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Alternatively, send the command r via the Serial Monitor to start detection.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Send s to stop detection or m to adjust the ROI parameters (x_scale, y_scale, x_length, y_length) via Serial input.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>View Results</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">The system detects brown, yellow, and blue colors in the ROI.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">If the blue position is to the right of brown, and brown is to the right of yellow, the buzzer is activated; otherwise, the relay is triggered.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Results (color positions and timestamp) are sent to the specified endpoint via HTTP POST.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Serial Commands</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">r: Run color detection.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">s: Stop color detection.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">m: Modify ROI parameters interactively.</li>
        </ul>
    </li>
</ol>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><strong>&nbsp;</strong></p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Example Output</strong></p>

* Serial Monitor (example):
  ```sh
  WiFi connected 
  Camera Ready! Use 'http://192.168.1.10' to connect
  local time is 2025-06-01 20:33:45
  Blue: X_scale is=50, Y_scale is=25
  Yellow: X_scale is=30, Y_scale is=20
  Brown: X_scale is=40, Y_scale is=22
  HTTP response code: 200
  ```

* HTTP POST Payload (example):
  ```sh
  "timestamp": "2025-06-01 20:33:45",
  "colorcode": "50,50/30,30/40,40"
  ```

<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Performance</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Camera</strong>: Captures QVGA (320x240) images in RGB565 format.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Color Detection</strong>: Uses HSV color space to detect brown, yellow, and blue colors with predefined ranges.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Position Analysis</strong>: Computes the center of detected contours to determine relative positions.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Actions</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Buzzer activates for 500ms if blue &gt; brown &gt; yellow in x-coordinates.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Relay activates for 500ms otherwise.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Web Server</strong>: Streams JPEG images (full or cropped) with 80% quality.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Network</strong>: Sends JSON payloads with detection results to the specified endpoint.</li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><strong><span dir="RTL" style='font-family:"Arial",sans-serif;'>&nbsp;</span></strong></p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><strong>&nbsp;</strong></p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;'><strong>&nbsp;</strong></p>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Testing and Debugging</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Testing</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Verify camera functionality by accessing the web server.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Test color detection by pressing the button or sending r via Serial.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;">Check HTTP POST responses in the Serial Monitor.</li>
        </ul>
    </li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Common Issues</strong>:<ul style="margin-bottom:0in;margin-top:0in;" type="circle">
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Camera Failure</strong>: Check pin connections and ensure camera_pins.h matches your hardware.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>WiFi Disconnection</strong>: Ensure correct SSID/password and stable network.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Invalid ROI</strong>: Use the m command to set appropriate ROI parameters.</li>
            <li style="margin-top: 0in;margin-right: 0in;margin-bottom: 8pt;font-size:16px;font-family: Aptos, sans-serif;text-align: justify;line-height: 115%;font-family:Times New Roman;"><strong>Memory Issues</strong>: Ensure sufficient PSRAM for frame buffers.</li>
        </ul>
    </li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;font-family:Times New Roman;'><strong>Limitations</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Only detects brown, yellow, and blue colors with fixed HSV ranges.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Limited to QVGA resolution due to ESP32 memory constraints.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>ROI parameters must be manually adjusted via Serial input.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>No real-time streaming for color detection results (only on button press or Serial command).</li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Future Improvements</strong></p>
<ul style="margin-bottom:0in;margin-top:0in;" type="disc">
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Add support for dynamic HSV range calibration.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Implement real-time color detection without button presses.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Enhance web server to display detection results.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Add error handling for network failures.</li>
    <li style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:16px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'>Support higher resolutions or additional color detection.</li>
</ul>
<p style='margin-top:0in;margin-right:0in;margin-bottom:8.0pt;margin-left:0in;line-height:115%;font-size:20px;font-family:"Aptos",sans-serif;text-align:justify;font-family:Times New Roman;'><strong>Resources</strong></p>
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
<p>&nbsp;</p>
<h3 align="left">Languages and Tools:</h3>
<p align="left">
<a href="https://www.cprogramming.com/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/> </a>
<a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a>
<a href="https://git-scm.com/" target="_blank" rel="noreferrer"> <img src="https://www.vectorlogo.zone/logos/git-scm/git-scm-icon.svg" alt="git" width="40" height="40"/> </a>
<a href="https://www.arduino.cc/" target="_blank" rel="noreferrer"> <img src="https://cdn.worldvectorlogo.com/logos/arduino-1.svg" alt="arduino" width="40" height="40"/> </a>
<a href="https://platformio.org//" target="_blank" rel="noreferrer"> <img src="https://cdn.platformio.org/images/platformio-logo.17fdc3bc.png" alt="arduino" width="40" height="40"/> </a>
</p>
