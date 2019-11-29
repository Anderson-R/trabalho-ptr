#include "keypad.h"

uint8_t lines[4] = {GPIO_LINE_0, GPIO_LINE_1, GPIO_LINE_2, GPIO_LINE_3};
uint8_t columns[4] = {GPIO_COL_0, GPIO_COL_1, GPIO_COL_2, GPIO_COL_3};
uint8_t lineSel[4] = {1, 0, 0, 0};
char hiden[1] = {'*'};
char newline[1] = {'\n'};
xQueueHandle keyQueue = NULL;
static const char* bankTag = "UnBank";

char keys[4][4] =   {{'1', '2', '3', 'A'},
                    {'4', '5', '6', 'B'},
                    {'7', '8', '9', 'C'},
                    {'*', '0', '#', 'D'}};

void keypadInit(void){
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = GPIO_LINE_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = GPIO_COL_PIN_SEL;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

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

    keyQueue = xQueueCreate(1, sizeof(char));
    xTaskCreate(scan, "keypadScanning", 2048, NULL, 1, NULL);
    while(commQueue == NULL);
    interface();
}

void lineStep(void){
    static int8_t i = 0;
    for(i=0; i<3; i++){
        if(lineSel[i] == 1){
            lineSel[i++] = 0;
            break;
        }
    }
    if(i == 3) i = 0;
    lineSel[i] = 1;

    for(size_t j=0; j<4; j++)
        gpio_set_level(lines[j], lineSel[j]);
}

void scan(void* args){
    while(true){
        for(size_t i=0; i<4; i++){
            lineStep();
            vTaskDelay(10 / portTICK_RATE_MS);
            if(lineSel[i] == 1){
                for(size_t j=0; j<4; j++){
                    if(gpio_get_level(columns[j]) == 1){
                        xQueueSend(keyQueue, &keys[j][i], 10/portTICK_PERIOD_MS);
                        vTaskDelay(200 / portTICK_RATE_MS);
                    }
                }
            }
        }
        //gpio_set_level(lines[0], 1); gpio_set_level(lines[1], 0); gpio_set_level(lines[2], 0); gpio_set_level(lines[3], 0);

    }
}

void welcome(void){
    clearTerminal();
    ESP_LOGI(bankTag, "**********Welcome to UNBank!**********");
}
void clearTerminal(void){
    ESP_LOGI(bankTag, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
char operations(void){
    ESP_LOGI(bankTag, "Choose an operation:");
    ESP_LOGI(bankTag, "1 - Withdrawal");
    ESP_LOGI(bankTag, "2 - Deposit");
    ESP_LOGI(bankTag, "3 - Transfer");
    ESP_LOGI(bankTag, "4 - Balance");
    ESP_LOGI(bankTag, "5 - Statement");
    
    return getKey();
}

char getKey(void){
    char key;
    while(true){
        if(xQueueReceive(keyQueue, &key, portMAX_DELAY)) break;
    }
    return key;
}

void interface(void){
    welcome();
    while(true){
        char k = operations();
        switch(k){
            case '1':
                withdraw();
                break;
            case '2':
                deposit();
                break;
            case '3':
                transfer();
                break;
            case '4':
                balance();
                break;
            case '5':
                statement();
                break;
            default:
                printf("Invalid choice!\n");
        }
    }

}

char getWord(char* word, bool hide, uint8_t maxSize){
    uint8_t i = 0;
    char digit;
    do{
        digit = getKey();
        if(digit != '#' && digit != '*'){
            if(hide) uart_write_bytes(UART_NUM_0, hiden, 1); 
            else uart_write_bytes(UART_NUM_0, word+i, 1);
            if(i < maxSize){
                word[i] = digit;
                i++;
            }
        }
    } while(digit != '#' && digit != '*');
    word[i] = '\0';
    //uart_write_bytes(UART_NUM_0, newline, 1);
    printf("\n");
    ESP_LOGI(bankTag, "\n");
    return digit;
}

void withdraw(void){
    request request;
    request.cmd = WITHDRAW;
    char* word = malloc(sizeof(char)*15);

    ESP_LOGI(bankTag, "WITHDRAW");
    
    ESP_LOGI(bankTag, "Please enter your acount number: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    ESP_LOGI(bankTag, "Please enter the value: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);
    
    ESP_LOGI(bankTag, "Please enter your password: ('*' = enter; '#' = cancel)");
    if(getWord((char*)request.password, true, 4) == '#') return;

    //ESP_LOGI(bankTag, "op: %d \t conta: %d \t valor: %d \t senha: %s\n\n", request.cmd, request.acount, request.value, request.password);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
    response response = waitResponse(10);
    responseInterpreter(response);
}

void deposit(void){
    request request;
    request.cmd = DEPOSIT;
    char* word = malloc(sizeof(char)*12);

    ESP_LOGI(bankTag, "DEPOSIT");
    
    ESP_LOGI(bankTag, "Please enter the acount number: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    ESP_LOGI(bankTag, "Please enter the value: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);

    //ESP_LOGI(bankTag, "op: %d \t conta: %d \t valor: %d\n\n", request.cmd, request.acount, request.value);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
    response response = waitResponse(10);
    responseInterpreter(response);
}

void transfer(void){
    request request;
    request.cmd = TRANSFER;
    char* word = malloc(sizeof(char)*15);

    ESP_LOGI(bankTag, "TRANSFER");

    ESP_LOGI(bankTag, "Please enter your acount number: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    memset(word, 0, 15);
    
    ESP_LOGI(bankTag, "Please enter the destination account number: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.secAcount = atoi(word);
    memset(word, 0, 15);

    ESP_LOGI(bankTag, "Please enter the value: ('*' = enter; '#' = cancel)");
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);
    memset(word, 0, 15);
    
    ESP_LOGI(bankTag, "Please enter your password: ('*' = enter; '#' = cancel)");
    if(getWord((char*)request.password, true, 4) == '#') return;

    //ESP_LOGI(bankTag, "op: %d \t conta1: %d \t conta2: %d \t valor: %d \t senha: %s\n\n", request.cmd, request.acount, request.secAcount, request.value, request.password);
    uart_write_bytes(UART_NUM_0, newline, 1);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
    response response = waitResponse(10);
    responseInterpreter(response);
}

void balance(void){
    request request;
    request.cmd = BALANCE;
    ESP_LOGI(bankTag, "BALANCE");
    ESP_LOGI(bankTag, "Please enter the acount number: ('*' = enter; '#' = cancel)");
    uart_write_bytes(UART_NUM_0, newline, 1);
    char* word = malloc(sizeof(char)*12);
    char ret = getWord(word, false, 10);
    if(ret == '#') return;
    request.acount = atoi(word);
    
    ESP_LOGI(bankTag, "Please enter your password: ('*' = enter; '#' = cancel)");
    uart_write_bytes(UART_NUM_0, newline, 1);
    ret = getWord((char*)request.password, true, 4);
    if(ret == '#') return;

    //ESP_LOGI(bankTag, "op: %d \t conta: %d \t senha: %s\n\n", request.cmd, request.acount, request.password);
    uart_write_bytes(UART_NUM_0, newline, 1);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
    response response = waitResponse(10);
    responseInterpreter(response);
}

void statement(void){
    request request;
    request.cmd = STATEMENT;
    char* word = malloc(sizeof(char)*12);
    
    ESP_LOGI(bankTag, "STATEMENT");
    
    ESP_LOGI(bankTag, "Please enter the acount number: ('*' = enter; '#' = cancel)");
    uart_write_bytes(UART_NUM_0, newline, 1);
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    ESP_LOGI(bankTag, "Please enter your password: ('*' = enter; '#' = cancel)");
    uart_write_bytes(UART_NUM_0, newline, 1);
    if(getWord((char*)request.password, true, 4) == '#') return;

    //ESP_LOGI(bankTag, "op: %d \t conta: %d \t senha: %s\n\n", request.cmd, request.acount, request.password);
    uart_write_bytes(UART_NUM_0, newline, 1);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
    response response = waitResponse(10);
    responseInterpreter(response);
}

response waitResponse(uint8_t t){
    uint8_t answered = false;
    response response;
    for(size_t j=0; j<t; j++){
        if(xQueueReceive(responseQueue, &response, 1000/portTICK_PERIOD_MS)) {
            answered = true;
            break;
        }
    }
    if(answered){
        ESP_LOGI(bankTag, "got answer!");
        return response;
    }
    ESP_LOGE(bankTag, "TIMEOUT!");
    response.status = TIMEOUT;
    return response;
}

void responseInterpreter(response response){
    if(response.status == APROVED){
        ESP_LOGI(bankTag, "The Transaction Was Aproved!");
        return;
    }
    else if(response.status == TIMEOUT){
        ESP_LOGE(bankTag, "The Transaction Got No Response From The Server!");
        return;
    }

    ESP_LOGE(bankTag, "The Transaction Was NOT Aproved!");
    ESP_LOGE(bankTag, "Reason: ");
    switch(response.reason){
        case NO_FUNDS:
            ESP_LOGE(bankTag, "insufficient founds.");
            break;
        case WRONG_PASSWORD:
            ESP_LOGE(bankTag, "wrong password.");
            break;
        case INVALID_ACOUNT:
            ESP_LOGE(bankTag, "the acount does not exist.");
            break;
        case INVALID_DEST_ACOUNT:
            ESP_LOGE(bankTag, "the destination acount does not exist.");
            break;
        default:
            break;
    }
}


