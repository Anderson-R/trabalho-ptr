#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "driver/uart.h"

static const char *TAG = "example";

void app_main(void){
    char cliente1[20], cliente2[20], cliente3[20];
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    FILE *fp = fopen("/spiffs/clientes", "r");
    if(fp == NULL){
        create_initial_file();
    }
    ESP_LOGI(TAG, "The Client File Exists!");
    fgets(cliente1, sizeof(cliente1), fp);
    fgets(cliente2, sizeof(cliente2), fp);
    fgets(cliente3, sizeof(cliente3), fp);

    printf("Clientes:1- %s, 2- %s, 3- %s", cliente1, cliente2, cliente3);

    fclose(fp);
}

void create_initial_file(){
    ESP_LOGI(TAG, "Creating file!");
    FILE *fp = fopen("/spiffs/clientes", "w+");
    if (fp == NULL){
        printf("Falhou ao criar arquivo");
    }
    fprintf(fp, "0001,2000.00,1234\n0002,500.50,8080\n0003,8000.00,7777");
    fclose(fp);
}