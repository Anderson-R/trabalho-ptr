#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_gattc_api.h"

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
    uint32_t reason;
} response;

extern QueueHandle_t requestQueue;
extern QueueHandle_t responseQueue;