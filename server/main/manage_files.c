#include "manage_files.h"

void create_initial_file();
void writeFile(central_client cliente[4]);

void recover_client(char cl[18], central_client* cliente, const char s[2]){
    int i = 0; 
    float j = 0.0;
    char * str;
    str = strtok(cl, s);

    sscanf(str, "%d", &i);
    cliente->account = i;
    printf("%d\n", cliente->account);
    str = strtok(NULL, s);   
    sscanf(str, "%f\n", &j);
    cliente->balance = j;
    printf("%.2f\n", cliente->balance);
    str = strtok(NULL, s);
    strncpy(cliente->password, str, 4);
    printf ("%s\n", cliente->password);
    printf("\n------------\n");

}

void initFile(void){
    central_client clientes[4];
    const char s[2] = ",";
    char cliente1[20], cliente2[20], cliente3[20], cliente4[20];

    for(int j = 0; j < 4; j++){
        clientes[j].account = 0;
        clientes[j].balance = 0.0;
        strcpy(clientes[j].password, "0000");
    }

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_RTS,
        .rx_flow_ctrl_thresh = 122,
    };

    //Set UART parameters
    uart_param_config(UART_NUM_0, &uart_config);
    //Set UART pins
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver, and get the queue.
    uart_driver_install(UART_NUM_0, 4096, 8192, 10, NULL, 0);


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

    unlink("/spiffs/clientes.txt");
    FILE* f = fopen("/spiffs/clientes.txt", "r");
    if(f == NULL){
        create_initial_file();
        f = fopen("/spiffs/clientes.txt", "r");
    }

    ESP_LOGI(TAG, "The Client File Exists!");
    
    fgets(cliente1, 19, f);
    fgets(cliente2, 19, f);
    fgets(cliente3, 19, f);
    fgets(cliente4, 19, f);

    recover_client(cliente1, &clientes[0], s);
    recover_client(cliente2, &clientes[1], s);
    recover_client(cliente3, &clientes[2], s);
    recover_client(cliente4, &clientes[3], s);
    fclose(f);


    writeFile(clientes);

}

void create_initial_file(){
    ESP_LOGI(TAG, "Creating file!");
    FILE *fp = fopen("/spiffs/clientes.txt", "w");
    if (fp == NULL){
        printf("Falhou ao criar arquivo");
    }
    fprintf(fp, "0001,2000.00,1234\n");
    fprintf(fp, "0002,500.50,8080\n");
    fprintf(fp, "0003,8000.00,7777\n");
    fprintf(fp, "0004,1250.00,8585\n");

    fclose(fp);
}

void writeFile(central_client cliente[4]){
    
    ESP_LOGI(TAG, "Writing the data on the file!");

    FILE *fp = fopen("/spiffs/clientes.txt", "w");
    if (fp == NULL){
        ESP_LOGE(TAG, "Skyler, where is my FILE!");
    }
    for(int i = 0; i < 4; i++){
        fprintf(fp, "%d,%.2f,%s\n", cliente[i].account, cliente[i].balance, cliente[i].password);
    }
    fclose(fp);
    fp = fopen("/spiffs/clientes.txt", "r+");
    for (int i = 0; i < 4; i++)
    {
        char *buff = malloc(sizeof(char)*40);
        fgets(buff, 30, fp);
        ESP_LOGI(TAG, "%s", buff);
        free(buff);
        
    }
    
    fclose(fp);

    
    ESP_LOGI(TAG, "Terminou com sucesso(?)");

}


