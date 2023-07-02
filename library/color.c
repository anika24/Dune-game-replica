#include "color.h"
#include <stdio.h>
#include <stdlib.h>

rgb_color_t rgb_color_init(float r, float g, float b) {
  rgb_color_t color = {color.r = r, color.g = g, color.b = b};
  return color;
}