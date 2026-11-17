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

void limparBuffer();
int lerInteiro();
double lerValor(double minimo);
void lerNome(char nome[]);

void registrarMovimento(int c, int j, int operacao, double valor);
void registrarBanco(int operacao, int c, int numeroConta, double valor);
void mostrarOperacao(int operacao);

void mostrarTipo(int tipo);
int podeCombinar(int tipo1, int tipo2);
int posicaoDoTipo(int c, int tipo);
double saldoTotalCliente(int c);
int buscarConta(int numero, int *posConta);
void criarConta(int c, int j, int tipo, double saldoInicial);
int aptoEmprestimo(int c);
double limiteEmprestimo(int c);

void cadastrarCliente();
void listarClientes();
void depositar();
void sacar();
void solicitarEmprestimo();
void historicoCliente();

double saldoTotalBanco();
void mostrarAptos();
void mostrarNegativos();
void mostrarTitulos();
void relatorios();

void abrirBanco();
void fecharBanco();
int bancoEstaAberto();
void historicoDoBanco();
void liberarMemoria();

#endif
