#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int codigo;
    char nome[50];
    char email[50];
    char cpf[20];
    char dataNascimento[20];
    char dataCadastro[20];
} Cliente;

typedef struct {
    int numero;
    Cliente cliente;
    float saldo;
    float limite;
    float saldoTotal;
} Conta;

void menu();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();

float atualizaSaldoTotal(Conta conta);
Conta buscarContaPorNumero(int numero);

void sacar(Conta conta, float valor);
void depositar(Conta conta, float valor);
void transferir(Conta conta_origem, Conta conta_destino, float valor);

static Conta contas[50];
static int contador_contas = 0;
static int contador_clientes = 0;

int main(){
    menu();


    return 0;
}

void menu(){
    int opcao = 0;
    printf("=====================.===.==========================\n");
    printf("=====================|ATM|==========================\n");
    printf("=====================|===|==========================\n");
    printf("[1] Criar Conta......|___| Efetuar Transferência.[4]\n");
    printf("[2] Efetuar Saque....|   | Listar Contas.........[5]\n");
    printf("[3] Efetuar Depósito.|___| Sair do Sistema.......[6]\n");
    printf("====================='==='==========================\n");
    printf("Selecione uma opção:");
    scanf("%d", &opcao);
    switch(opcao){
        case 1:
            criarConta();
            break;
        case 2:
            efetuarSaque();
            break;
        case 3:
            efetuarDeposito();
            break;
        case 4:
            efetuarTransferencia();
            break;
        case 5:
            listarContas();
            break;
        case 6:
            printf("Até a próxima!");
            exit(0);
        default:
            printf("Opção Invalida!");
            menu();
    }
}
void infoCliente(Cliente cliente){
    printf("Código:%d \nNome: %s \nData de Nascimento:%s \nCadastro: %s\n", cliente.codigo, strtok(cliente.nome, "\n"), strtok(cliente.dataNascimento, "\n"), strtok(cliente.dataCadastro, "\n"));
}
void infoConta(Conta conta){
    printf("Numero da Conta:%d \nCliente:%s \nData Nascimento:%s \nData Cadastro:%s \nSaldo Total:R$%.2f\n", conta.numero, strtok(conta.cliente.nome, "\n"),strtok(conta.cliente.dataNascimento, "\n"),strtok(conta.cliente.dataCadastro, "\n"), conta.saldoTotal);
}
void criarConta(){
    Cliente cliente;
    // Capturando a data via sistema.
    char dia[3];
    char mes[3];
    char ano[5];
    char data_cadastro[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    //dia
    if(tm.tm_mday < 10){
        sprintf(dia, "0%d", tm.tm_mday);
    }else{
        sprintf(dia, "%d", tm.tm_mday);
    }
    //mes
    if((tm.tm_mon + 1) - 10 ){
        sprintf(mes, "0%d",tm.tm_mon + 1);
    }else{
        sprintf(mes,"%d", tm.tm_mon + 1);
    }
    //ano
    sprintf(ano,"%d",tm.tm_year + 1900);
    //Concatenando tudo.
    strcpy(data_cadastro, "");
    strcat(data_cadastro, dia);
    strcat(data_cadastro, "/");
    strcat(data_cadastro, mes);
    strcat(data_cadastro, "/");
    strcat(data_cadastro, ano);
    strcat(data_cadastro, "\0");
    strcpy(cliente.dataCadastro, data_cadastro);

    // Criando o cliente.
    printf("Informe os dados do cliente\n");
    cliente.codigo = contador_clientes + 1;
    
    printf("Nome do cliente:");
    getchar();
    fgets(cliente.nome,50,stdin);

    
    printf("E-Mail do Cliente:");
    fgets(cliente.email,50,stdin);

    printf("CPF do Cliente:");
    fgets(cliente.cpf,20,stdin);

    printf("Data de Nascimento do Cliente:");
    fgets(cliente.dataNascimento,20,stdin);

    contador_clientes++;

    // Criando a conta.
    contas[contador_contas].numero = contador_contas + 1;
    contas[contador_contas].cliente = cliente;
    contas[contador_contas].saldo = 0.0;
    contas[contador_contas].limite = 0.0;
    contas[contador_contas].saldoTotal = atualizaSaldoTotal(contas[contador_contas]);

    printf("Conta criada com sucesso!\n");
    printf("\n");
    printf("Dados da conta criada:\n");
    infoConta(contas[contador_contas]);
    contador_contas++;
    sleep(4);
    menu();
}

float atualizaSaldoTotal(Conta conta){
    return conta.saldo + conta.limite;
}
Conta buscarContaPorNumero(int numero){
    Conta c;
    if(contador_contas > 0){
        for(int i = 0; i < contador_contas; i++){
            if(contas[i].numero == numero){
                c = contas[i];
            }
        }
    }
    return c;
}

void sacar(Conta conta, float valor){
    if(valor > 0 && conta.saldoTotal >= valor){
        for(int i = 0; i < contador_contas; i++){
            if(contas[i].numero == conta.numero){
                if(contas[i].saldo >= valor){
                    contas[i].saldo = contas[i].saldo - valor;
                    contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                    printf("Saque efetuado com sucesso!\n");
                }else{
                    float restante = contas[i].saldo - valor;
                    contas[i].limite = contas[i].limite + restante;
                    contas[i].saldo = 0.0;
                    contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                    printf("Saque efetuado com sucesso!\n");
                }
            }
        }
    }else{
        printf("Saldo insuficiente!\n");
    }
}
void depositar(Conta conta, float valor){
    if(valor > 0){
        for(int i = 0; i < contador_contas; i++){
            if(contas[i].numero == conta.numero){
                contas[i].saldo = contas[i].saldo + valor;
                contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                printf("Depósito efetuado com sucesso!\n");
            }
        }
    }else{
        printf("Erro ao efetuar o depósito. Tente novamente!\n");
    }
}
void transferir(Conta conta_origem, Conta conta_destino, float valor){
    if(valor > 0 && conta_origem.saldoTotal >= valor){
        for(int co = 0 ; co < contador_contas; co++){
            if(contas[co].numero == conta_origem.numero){
                for(int cd = 0; cd < contador_contas; cd++){
                    if(contas[cd].numero == conta_destino.numero){
                        if(contas[co].saldo >= valor){
                            contas[co].saldo = contas[co].saldo - valor;
                            contas[cd].saldo = contas[cd].saldo + valor;
                            contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
                            contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
                            printf("Transferência realizada com sucesso!\n");
                        }else{
                            float restante = contas[co].saldo - valor;
                            contas[co].limite = contas[co].limite + restante;
                            contas[co].saldo = 0.0;
                            contas[cd].saldo = contas[cd].saldo + valor;
                            contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
                            contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
                            printf("Transferência realizada com sucesso!\n");
                        }
                    }
                }
            }
        }
    }else{
        printf("Saldo insuficiente!\n");
    }
}
void efetuarSaque(){
    if(contador_contas > 0){
        int numero;
        printf("Informe o número da conta:");
        scanf("%d", &numero);

        Conta conta = buscarContaPorNumero(numero);

        if(conta.numero == numero){
            float valor;
            printf("Informe o valor do saque:");
            scanf("%f", &valor);

            sacar(conta, valor);
        }else{
            printf("Conta não encontrada: %d\n", numero);
        }

    }else{
        printf("Ainda não existem contas para saque!\n");
    }
    printf(".");
    sleep(1);
    printf(".");
    sleep(2);
    printf(".");
    printf("\n");
    menu();
}
void efetuarDeposito(){
        if(contador_contas > 0){
        int numero;
        printf("Informe o número da conta:");
        scanf("%d", &numero);

        Conta conta = buscarContaPorNumero(numero);

        if(conta.numero == numero){
            float valor;
            printf("Informe o valor do saque:");
            scanf("%f", &valor);

            depositar(conta, valor);
        }else{
            printf("Conta não encontrada: %d\n", numero);
        }

    }else{
        printf("Ainda não existem contas para Depósito!\n");
    }
    printf(".");
    sleep(1);
    printf(".");
    sleep(2);
    printf(".");
    printf("\n");
    menu();
}
void efetuarTransferencia(){
    if(contador_contas > 0){
        int numero_o, numero_d;
        printf("Informe o numero da sua conta:");
        scanf("%d", &numero_o);
        Conta conta_o = buscarContaPorNumero(numero_o);
        if(conta_o.numero == numero_o){
            printf("Informe o numero da conta de Destino:");
            scanf("%d", &numero_d);

            Conta conta_d = buscarContaPorNumero(numero_d);
            if(conta_d.numero == numero_d){
                float valor;
                printf("Informe o valor para Transferência:");
                scanf("%f", &valor);
                transferir(conta_o,conta_d,valor);
            }else{
                printf("Conta Destino não encontrada!\n");
            }
        }else{
            printf("Conta Origem não encontrada!\n");
        }
    }else{
        printf("Ainda não existem contas para Transferências!\n");
    }
    printf(".");
    sleep(1);
    printf(".");
    sleep(2);
    printf(".");
    printf("\n");
    menu();
}
void listarContas(){
    if(contador_contas > 0){
        for(int i = 0; i < contador_contas; i++){
            infoConta(contas[i]);
            printf("\n");
            sleep(1);
        }
    }else{
        printf("Não existem contas cadastradas ainda!\n");
    }
    printf(".");
    sleep(1);
    printf(".");
    sleep(2);
    printf(".");
    printf("\n");
    menu();
}