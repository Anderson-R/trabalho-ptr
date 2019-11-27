#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_gattc_api.h"

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


#define GATTC_TAG                   "GATTC_SPP"
#define PROFILE_NUM                 1
#define PROFILE_APP_ID              0
#define BT_BD_ADDR_STR              "%02x:%02x:%02x:%02x:%02x:%02x"
#define BT_BD_ADDR_HEX(addr)        addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]
#define ESP_GATT_SPP_SERVICE_UUID   0xABF0
#define SCAN_ALL_THE_TIME           0

struct gattc_profile_inst {
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};

enum{
    SPP_IDX_SVC,

    SPP_IDX_SPP_DATA_RECV_VAL,

    SPP_IDX_SPP_DATA_NTY_VAL,
    SPP_IDX_SPP_DATA_NTF_CFG,

    SPP_IDX_SPP_COMMAND_VAL,

    SPP_IDX_SPP_STATUS_VAL,
    SPP_IDX_SPP_STATUS_CFG,
    
    SPP_IDX_NB,
};

typedef enum{WITHDRAW, DEPOSIT, TRANSFER, BALANCE, STATEMENT} CMD;
typedef enum{NO_FUNDS, WRONG_PASSWORD, INVALID_ACOUNT, INVALID_DEST_ACOUNT} REASON;
typedef enum{APROVED, DESAPROVED, TIMEOUT} STAT;

typedef struct {
    CMD cmd;
    uint32_t acount;
    uint32_t secAcount;
    uint16_t value;
    uint8_t password[4];
} request;

typedef struct {
    uint8_t status;
    uint8_t reason;
} response;

extern QueueHandle_t commQueue;
extern QueueHandle_t responseQueue;