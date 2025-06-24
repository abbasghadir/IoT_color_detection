#include "color_detection_manager.h"

extern ROI_config ROI;
double pixelarea = ROI.x_width/2;

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

//convert RGB565 2 BGR (need for opencv input)
void convertRGB565ToBGR(uint8_t* rgb565, int width, int height, cv::Mat& bgrImage) {
    bgrImage = cv::Mat(height, width, CV_8UC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
        uint16_t pixel = (rgb565[(y * width + x) * 2] << 8) | rgb565[(y * width + x) * 2 + 1];
        uint8_t r = (pixel >> 11) & 0x1F; //
        uint8_t g = (pixel >> 5) & 0x3F;  //
        uint8_t b = pixel & 0x1F;         //
        r = (r << 3) | (r >> 2); //
        g = (g << 2) | (g >> 4); //
        b = (b << 3) | (b >> 2); //
        cv::Vec3b& pixelBGR = bgrImage.at<cv::Vec3b>(y, x);
        pixelBGR[0] = b; // 
        pixelBGR[1] = g; // 
        pixelBGR[2] = r; // 
        }
    }
}

void color_range_detection(void *pvParameters) {
// bool color_range_detection() {
    Serial.println("start color detection");
    cv::Mat image(ROI.x_width, ROI.y_height, CV_8UC3);
    convertRGB565ToBGR(ROI.buffer, ROI.x_width, ROI.y_height, image);
    
    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
    image.release(); 
    
    cv::Scalar brown_lower(0, 0, 50);
    cv::Scalar brown_upper(30, 100, 200);

    // cv::Scalar brown_lower_2(150, 0, 50);
    // cv::Scalar brown_upper_2(200, 100, 200);

    cv::Scalar yellow_lower(30,100,150);
    cv::Scalar yellow_upper(100,200,255);

    cv::Scalar blue_lower(100, 140, 150);
    cv::Scalar blue_upper(130, 255, 255);

    cv::Mat brown_mask, yellow_mask, blue_mask;//,brown_mask_2;
    cv::inRange(hsv_image, brown_lower, brown_upper, brown_mask);
    // cv::inRange(hsv_image, brown_lower_2, brown_upper_2, brown_mask_2);
    cv::inRange(hsv_image, yellow_lower, yellow_upper, yellow_mask);
    cv::inRange(hsv_image, blue_lower, blue_upper, blue_mask);
    hsv_image.release();

    std::vector<std::vector<cv::Point>> brown_contours, yellow_contours, blue_contours;//,brown_contours_2;
    cv::findContours(brown_mask, brown_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // cv::findContours(brown_mask_2, brown_contours_2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(yellow_mask, yellow_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(blue_mask, blue_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    brown_mask.release();
    // brown_mask_2.release();
    yellow_mask.release();
    blue_mask.release();

    std::vector<std::vector<cv::Point>> filtered_brown_contours, filtered_yellow_contours, filtered_blue_contours;//,filtered_brown_contours_2;
    for (const auto& cnt : brown_contours) {
        if (cv::contourArea(cnt) > pixelarea) {
        filtered_brown_contours.push_back(cnt);
        }
    }
    // for (const auto& cnt : brown_contours_2) {
    //     if (cv::contourArea(cnt) > pixelarea) {
    //     filtered_brown_contours_2.push_back(cnt);
    //     }
    // }
    for (const auto& cnt : yellow_contours) {
        if (cv::contourArea(cnt) > pixelarea) {
        filtered_yellow_contours.push_back(cnt);
        }
    }
    for (const auto& cnt : blue_contours) {
        if (cv::contourArea(cnt) > pixelarea) {
        filtered_blue_contours.push_back(cnt);
        }
    }

    Position blue_position = positionfinder(filtered_blue_contours);
    Position yellow_position = positionfinder(filtered_yellow_contours);
    Position brown_position = positionfinder(filtered_brown_contours);
    
    // Position brown_position_2 = positionfinder(filtered_brown_contours_2);

    // printf("Blue: X_scale is=%d, Y_scale is=%d\n", blue_position.x_scale, blue_position.y_scale);
    // printf("Yellow: X_scale is=%d, Y_scale is=%d\n", yellow_position.x_scale, yellow_position.y_scale);
    // printf("Brown: X_scale is=%d, Y_scale is=%d\n", brown_position.x_scale, brown_position.y_scale);
    // printf("Brown: X_scale is=%d, Y_scale is=%d\n", brown_position_2.x_scale, brown_position_2.y_scale);
    
    // printf("yellow is=%d, min is=%d, max is=%d \n", yellow_position.x_scale, min(blue_position.x_scale, brown_position.x_scale),max(blue_position.x_scale, brown_position.x_scale));

    if( (yellow_position.x_scale)<=0 && (blue_position.x_scale)>0 && ((brown_position.x_scale)>0)){// || (brown_position_2.x_scale)>0)){
        Serial.println("stop color detection");
        // return true;
    }else
    {
        Serial.println("stop color detection");
        // return false;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);  
}