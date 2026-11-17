#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#define MAX_HIST 50
#define MAX_HIST_BANCO 200
#define LIMITE_CHEQUE 500.0

#define CORRENTE 1
#define SALARIO 2
#define POUPANCA 3

#define ABERTURA 1
#define DEPOSITO 2
#define SAQUE 3
#define EMPRESTIMO 4
#define BANCO_ABERTO 5
#define BANCO_FECHADO 6
#define NOVO_CLIENTE 7

typedef struct {
    int operacao;
    double valor;
    double saldoApos;
} Movimento;

typedef struct {
    int numero;
    int tipo;
    double saldo;
    Movimento historico[MAX_HIST];
    int qtdHist;
} Conta;

typedef struct {
    char nome[50];
    Conta contas[2];
    int qtdContas;
    int temEmprestimo;
    double valorEmprestimo;
} Cliente;

typedef struct {
    int operacao;
    int cliente;
    int numeroConta;
    double valor;
} Evento;

#endif
