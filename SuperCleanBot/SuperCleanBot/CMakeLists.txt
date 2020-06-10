#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.8)
project (SuperCleanBot C)

# Create executable
add_executable (${PROJECT_NAME} main.c  epoll_timerfd_utilities.c oled.c oled.h font.h i2c.c lps22hh_reg.c lsm6dso_reg.c "ultrasonicsensor.h" "ultrasonicsensor.c" "inputsense.c" "inputsense.h" "botdrive.c" "botdrive.h" sd1306.c sd1306.h "lightsensor.c" "lightsensor.h" "sanitizeAlgo.c" "sanitizeAlgo.h")
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c)

# Add MakeImage post-build command
include (${AZURE_SPHERE_MAKE_IMAGE_FILE})
