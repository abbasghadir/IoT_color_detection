#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H
#include <esp_camera.h>
#include "Storage_manager.h"

struct ROI_config {
    uint8_t *buffer;
    int x;
    int y;
    int x_width;
    int y_height;
    int BPPN; // bytes_per_pixel_number
    pixformat_t format;
};

void camera_init();
void camera_capture(void *pvParameters);
#endif