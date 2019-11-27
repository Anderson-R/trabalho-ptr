#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "typedef.h"
#include "driver/uart.h"
#include "esp_log.h"

void keypadInit(void);
void lineStep(void);
void scan(void* args);
void welcome(void);
char operations(void);
void interface(void);
char getKey(void);
void withdraw(void);
char getWord(char* word, bool hide, uint8_t maxSize);
void deposit(void);
void transfer(void);
void balance(void);
void statement(void);
response waitResponse(uint8_t t);
void responseInterpreter(response response);
void clearTerminal(void);