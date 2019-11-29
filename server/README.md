* verificar se a conta origem existe
    * se não existe responde com negativa e motivo

* switch(operação)
    * case retirada:
        * verifica se saldo suficiente
            * se não, retorna com negativa e motivo
        * reduz do saldo o valor
        * retorna com OK
    * case deposito:
        * soma o valor ao saldo
        * retorna com OK
    * case transferência:
        * verifica se a conta destino exite
            * se não, retorna com negativa e motivo
        * verifica se saldo suficiente
            * se não, retorna com negativa e motivo
        * reduz do saldo o valor da conta origem
        * soma o valor ao saldo da conta destino
    * case saldo:
        * retorna com OK e valor do saldo


