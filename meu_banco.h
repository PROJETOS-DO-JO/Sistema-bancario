#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#define ARQUIVO "banco.txt"
#define MAX_HIST 5
#define LIMITE_CHEQUE 500.0

#define CORRENTE 1
#define SALARIO 2
#define POUPANCA 3

typedef struct {
    char op;
    double valor;
} Movimento;

typedef struct {
    int tipo;
    double saldo;
    Movimento historico[MAX_HIST];
    int qtdHist;
} Conta;

typedef struct {
    char nome[50];
    int numero;
    Conta contas[2];
    int qtdContas;
    int temEmprestimo;
    double valorEmprestimo;
    int temTitulo;
    int sorteado;
    double premio;
} Cliente;

void limparBuffer();
int lerInteiro();
double lerValor(double minimo);
void lerNome(char nome[]);

void registrarMovimento(int c, int j, char op, double valor);
void mostrarTipo(int tipo);
int podeCombinar(int tipo1, int tipo2);
int posicaoDoTipo(int c, int tipo);
double saldoTotalCliente(int c);
int buscarCliente(int numero);
void criarConta(int c, int j, int tipo);
int aptoEmprestimo(int c);
double limiteEmprestimo(int c);
int aptoTitulo(int c);

void cadastrarCliente();
void listarClientes();

int lerConta(int *c, int *j);
void depositar();
void sacar();
void solicitarEmprestimo();

double saldoTotalBanco();
void mostrarAptos();
void mostrarNegativos();
void mostrarTitulos();
void mostrarContemplados();
void historicoDoBanco();

int fibonacci(int n);
int somaFibonacci(int n);
int ehPrimo(int n);
void efetuarSorteio();

void salvarDados();
void carregarDados();

void abrirBanco();
void fecharBanco();
int bancoEstaAberto();
void liberarMemoria();

#endif
