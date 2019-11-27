#include<stdio.h>
#include<string.h>

void app_main(void){
    char cliente[18][3];
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
    fgets(cliente[0], sizeof(cliente), fp);
    fgets(cliente[1], sizeof(cliente), fp);
    fgets(cliente[2], sizeof(cliente), fp);


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