#ifndef COLOR_DETECTION_MANAGER_H
#define COLOR_DETECTION_MANAGER_H
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "camera_manager.h"

//position structure
struct Position{
  int x_scale;
  int y_scale;
};

void color_range_detection(void *pvParameters);

#endif
