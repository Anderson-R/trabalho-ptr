#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "driver/uart.h"

static const char *TAG = "example";


void create_initial_file();
void writeFile(central_client cliente[4]);
void recover_client(char cl[18], central_client* cliente, const char s[2]);
void initFile(void);
void create_initial_file();
void writeFile(central_client cliente[4]);