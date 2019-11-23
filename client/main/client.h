#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "driver/uart.h"
#include "esp_bt.h"
#include "nvs_flash.h"
#include "esp_bt_device.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "keypad.h"
#include "typedef.h"

void spp_client_reg_task(void* arg);
void ble_client_appRegister(void);
void uart_task(void *pvParameters);
static void bleCommInit(void);