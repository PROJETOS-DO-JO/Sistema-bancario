#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meu_banco.h"

Cliente *clientes = NULL;
int qtdClientes = 0;
int proximoNumero = 1001;
int bancoAberto = 0;

Evento historicoBanco[MAX_HIST_BANCO];
int qtdHistBanco = 0;

void limparBuffer() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int lerInteiro() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        limparBuffer();
        printf("Digite um numero valido: ");
    }
    limparBuffer();
    return valor;
}

double lerValor(double minimo) {
    double valor;
    do {
        while (scanf("%lf", &valor) != 1) {
            limparBuffer();
            printf("Digite um valor valido: R$ ");
        }
        limparBuffer();
        if (valor < minimo) {
            printf("Valor invalido. Digite novamente: R$ ");
        }
    } while (valor < minimo);
    return valor;
}

void lerNome(char nome[]) {
    int tam;
    do {
        fgets(nome, 50, stdin);
        tam = strlen(nome);
        if (tam > 0 && nome[tam - 1] == '\n') {
            nome[tam - 1] = '\0';
            tam--;
        }
        if (tam == 0) {
            printf("O nome nao pode ficar vazio: ");
        }
    } while (tam == 0);
}

void registrarMovimento(int c, int j, int operacao, double valor) {
    int pos = clientes[c].contas[j].qtdHist;
    if (pos < MAX_HIST) {
        clientes[c].contas[j].historico[pos].operacao = operacao;
        clientes[c].contas[j].historico[pos].valor = valor;
        clientes[c].contas[j].historico[pos].saldoApos = clientes[c].contas[j].saldo;
        clientes[c].contas[j].qtdHist++;
    }
}

void registrarBanco(int operacao, int c, int numeroConta, double valor) {
    if (qtdHistBanco < MAX_HIST_BANCO) {
        historicoBanco[qtdHistBanco].operacao = operacao;
        historicoBanco[qtdHistBanco].cliente = c;
        historicoBanco[qtdHistBanco].numeroConta = numeroConta;
        historicoBanco[qtdHistBanco].valor = valor;
        qtdHistBanco++;
    }
}

void mostrarOperacao(int operacao) {
    switch (operacao) {
        case ABERTURA:      printf("Abertura de conta"); break;
        case DEPOSITO:      printf("Deposito"); break;
        case SAQUE:         printf("Saque"); break;
        case EMPRESTIMO:    printf("Emprestimo"); break;
        case BANCO_ABERTO:  printf("Banco aberto"); break;
        case BANCO_FECHADO: printf("Banco fechado"); break;
        case NOVO_CLIENTE:  printf("Novo cliente"); break;
    }
}

void mostrarTipo(int tipo) {
    switch (tipo) {
        case CORRENTE: printf("01-Corrente"); break;
        case SALARIO:  printf("02-Salario"); break;
        case POUPANCA: printf("03-Poupanca"); break;
    }
}

int podeCombinar(int tipo1, int tipo2) {
    if (tipo1 == tipo2) {
        return 0;
    }
    if (tipo1 == CORRENTE && tipo2 == SALARIO) {
        return 0;
    }
    if (tipo1 == SALARIO && tipo2 == CORRENTE) {
        return 0;
    }
    return 1;
}

int posicaoDoTipo(int c, int tipo) {
    int j;
    for (j = 0; j < clientes[c].qtdContas; j++) {
        if (clientes[c].contas[j].tipo == tipo) {
            return j;
        }
    }
    return -1;
}

double saldoTotalCliente(int c) {
    double total = 0;
    int j;
    for (j = 0; j < clientes[c].qtdContas; j++) {
        total += clientes[c].contas[j].saldo;
    }
    return total;
}

int buscarConta(int numero, int *posConta) {
    int i, j;
    for (i = 0; i < qtdClientes; i++) {
        for (j = 0; j < clientes[i].qtdContas; j++) {
            if (clientes[i].contas[j].numero == numero) {
                *posConta = j;
                return i;
            }
        }
    }
    return -1;
}

void criarConta(int c, int j, int tipo, double saldoInicial) {
    clientes[c].contas[j].numero = proximoNumero;
    clientes[c].contas[j].tipo = tipo;
    clientes[c].contas[j].saldo = saldoInicial;
    clientes[c].contas[j].qtdHist = 0;
    proximoNumero++;
    registrarMovimento(c, j, ABERTURA, saldoInicial);
}

int aptoEmprestimo(int c) {
    int pc = posicaoDoTipo(c, CORRENTE);
    int pp = posicaoDoTipo(c, POUPANCA);

    if (clientes[c].temEmprestimo == 1) {
        return 0;
    }
    if (pc == -1 || pp == -1) {
        return 0;
    }
    if (clientes[c].contas[pp].saldo <= 0) {
        return 0;
    }
    if (saldoTotalCliente(c) <= 0) {
        return 0;
    }
    return 1;
}

double limiteEmprestimo(int c) {
    return 0.25 * (10 * saldoTotalCliente(c));
}


