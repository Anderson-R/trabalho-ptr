#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_LINE_0         GPIO_NUM_13
#define GPIO_LINE_1         GPIO_NUM_12
#define GPIO_LINE_2         GPIO_NUM_14
#define GPIO_LINE_3         GPIO_NUM_27
#define GPIO_LINE_PIN_SEL   ((1ULL<<GPIO_LINE_0) | (1ULL<<GPIO_LINE_1) | (1ULL<<GPIO_LINE_2) | (1ULL<<GPIO_LINE_3))
#define GPIO_COL_0          GPIO_NUM_26
#define GPIO_COL_1          GPIO_NUM_25
#define GPIO_COL_2          GPIO_NUM_33
#define GPIO_COL_3          GPIO_NUM_32
#define GPIO_COL_PIN_SEL    ((1ULL<<GPIO_COL_0) | (1ULL<<GPIO_COL_1) | (1ULL<<GPIO_COL_2) | (1ULL<<GPIO_COL_3))