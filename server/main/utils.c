#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct{
    int account;
    uint16_t balance;
    char password[5];
}central_client;

int validade(uint16_t balance, uint16_t value){
    return balance >= value;
}

void withdraw(central_client* cliente1, uint16_t value){
    cliente1->balance -= value;
}

void deposit(central_client* cliente1, uint16_t value){
    cliente1->balance += value;
}

void transfer(central_client* cliente1, central_client* cliente2, uint16_t value){
    cliente1->balance -= value;
    cliente2->balance += value;
}

central_client recover_client(int id, central_client *clientes){
    return clientes[id];
}

int correct_password(char *password, central_client *client){
    //o strcmp retorna 0 para verdadeiro
    return (!strcmp(password, client->password));
}

int operation(central_client* aux1,central_client* aux2, uint16_t value, int transaction){
    
    printf("Senha eh: %s\n", aux1->password);
    if (correct_password("0123", aux1))
    {
        printf("Senha Correta!\n");
        switch (transaction)
        {
            case 1:
                if(aux1->balance - value >= 0.0)
                {
                    withdraw(aux1, value);
                    printf("Saldo de %d = R$ %.2f\n", aux1->account, aux1->balance);
                    return 1;
                }
                printf("Saldo Insuficiente\n");
                break;
            case 2:
                deposit(aux1, value);
                printf("Saldo de %d = R$ %.2f\n", aux1->account, aux1->balance);
                return 1;
            case 3:
                if (aux1->balance - value >= 0.0)
                {
                    transfer(aux1, aux2, value);
                    printf("Saldo de %d = R$ %.2f\n", aux1->account, aux1->balance);
                    return 1;                    
                }
                printf("Saldo Insuficiente\n");
                break;
                
            default:
                break;
        }
    }else{
        printf("Senha Incorreta!\n");
    }

    return 0;
}
