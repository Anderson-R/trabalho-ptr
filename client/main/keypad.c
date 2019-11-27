#include "keypad.h"

uint8_t lines[4] = {GPIO_LINE_0, GPIO_LINE_1, GPIO_LINE_2, GPIO_LINE_3};
uint8_t columns[4] = {GPIO_COL_0, GPIO_COL_1, GPIO_COL_2, GPIO_COL_3};
uint8_t lineSel[4] = {1, 0, 0, 0};
char hiden[1] = {'*'};
char newline[2] = {'\r', '\n'};
xQueueHandle keyQueue = NULL;

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
    printf("Welcome to UNBank!\n");
}
char operations(void){
    printf("Choose a operation:\n");
    printf("1 - Withdrawal\n");
    printf("2 - Deposit\n");
    printf("3 - Transfer\n");
    printf("4 - Balance\n");
    printf("5 - Statement\n");
    
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
            word[i] = digit;
            if(hide) uart_write_bytes(UART_NUM_0, hiden, 1); 
            else uart_write_bytes(UART_NUM_0, word+i, 1);
            i++;
        }
    } while(digit != '#' && digit != '*' && i < maxSize);
    word[i] = '\0';
    uart_write_bytes(UART_NUM_0, newline, 2);
    return digit;
}

void withdraw(void){
    request request;
    request.cmd = WITHDRAW;
    char* word = malloc(sizeof(char)*15);

    printf("WITHDRAW\n");
    
    printf("Please enter your acount number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    printf("Please enter the value: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);
    
    printf("Please enter your password: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord((char*)request.password, true, 4) == '#') return;

    printf("op: %d \t conta: %d \t valor: %d \t senha: %s\n\n", request.cmd, request.acount, request.value, request.password);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
}

void deposit(void){
    request request;
    request.cmd = DEPOSIT;
    char* word = malloc(sizeof(char)*12);

    printf("DEPOSIT\n");
    
    printf("Please enter the acount number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    printf("Please enter the value: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);

    printf("op: %d \t conta: %d \t valor: %d\n\n", request.cmd, request.acount, request.value);
    uart_write_bytes(UART_NUM_0, newline, 2);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
}

void transfer(void){
    request request;
    request.cmd = TRANSFER;
    char* word = malloc(sizeof(char)*15);

    printf("TRANSFER\n");

    printf("Please enter your acount number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    memset(word, 0, 15);
    
    printf("Please enter the destination account number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.secAcount = atoi(word);
    memset(word, 0, 15);

    printf("Please enter the value: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.value = atoi(word);
    memset(word, 0, 15);
    
    printf("Please enter your password: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord((char*)request.password, true, 4) == '#') return;

    printf("op: %d \t conta1: %d \t conta2: %d \t valor: %d \t senha: %s\n\n", request.cmd, request.acount, request.secAcount, request.value, request.password);
    uart_write_bytes(UART_NUM_0, newline, 2);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
}

void balance(void){
    request request;
    request.cmd = BALANCE;
    printf("BALANCE\n");
    printf("Please enter the acount number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    char* word = malloc(sizeof(char)*12);
    char ret = getWord(word, false, 10);
    if(ret == '#') return;
    request.acount = atoi(word);
    
    printf("Please enter your password: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    ret = getWord((char*)request.password, true, 4);
    if(ret == '#') return;

    printf("op: %d \t conta: %d \t senha: %s\n\n", request.cmd, request.acount, request.password);
    uart_write_bytes(UART_NUM_0, newline, 2);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
}

void statement(void){
    request request;
    request.cmd = STATEMENT;
    char* word = malloc(sizeof(char)*12);
    
    printf("STATEMENT\n");
    
    printf("Please enter the acount number: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord(word, false, 10) == '#') return;
    request.acount = atoi(word);
    
    printf("Please enter your password: ('*' = enter; '#' = cancel)\n");
    uart_write_bytes(UART_NUM_0, newline, 2);
    if(getWord((char*)request.password, true, 4) == '#') return;

    printf("op: %d \t conta: %d \t senha: %s\n\n", request.cmd, request.acount, request.password);
    uart_write_bytes(UART_NUM_0, newline, 2);
    xQueueSend(commQueue, &request, 10/portTICK_PERIOD_MS);
}


