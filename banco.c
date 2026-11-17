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
