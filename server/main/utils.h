#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>


typedef struct{
    int account;
    uint16_t balance;
    char password[5];
}central_client;

int validade(uint16_t balance, uint16_t value);
void withdraw(central_client* cliente1, uint16_t value);
void deposit(central_client* cliente1, uint16_t value);
void transfer(central_client* cliente1, central_client* cliente2, uint16_t value);
central_client recover_client(int id, central_client *clientes);
int correct_password(char *password, central_client *client);
int operation(central_client* aux1,central_client* aux2, uint16_t value, int transaction);
