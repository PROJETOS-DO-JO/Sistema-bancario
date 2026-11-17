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

void cadastrarCliente() {
    Cliente *aux;
    int c, tipo1, tipo2;
    char resposta;
    double saldo;

    aux = realloc(clientes, (qtdClientes + 1) * sizeof(Cliente));
    if (aux == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    clientes = aux;
    c = qtdClientes;

    printf("\n--- CADASTRO DE CLIENTE ---\n");
    printf("Nome do cliente: ");
    lerNome(clientes[c].nome);

    clientes[c].qtdContas = 0;
    clientes[c].temEmprestimo = 0;
    clientes[c].valorEmprestimo = 0;

    do {
        printf("Tipo da conta (1-Corrente / 2-Salario / 3-Poupanca): ");
        tipo1 = lerInteiro();
    } while (tipo1 < 1 || tipo1 > 3);

    printf("Saldo inicial: R$ ");
    saldo = lerValor(0);
    criarConta(c, 0, tipo1, saldo);
    clientes[c].qtdContas = 1;

    printf("Deseja abrir uma segunda conta? (s/n): ");
    scanf(" %c", &resposta);
    limparBuffer();

    if (resposta == 's' || resposta == 'S') {
        do {
            printf("Tipo da segunda conta (1-Corrente / 2-Salario / 3-Poupanca): ");
            tipo2 = lerInteiro();
            if (tipo2 >= 1 && tipo2 <= 3 && podeCombinar(tipo1, tipo2) == 0) {
                printf("Combinacao nao permitida! So pode Corrente + Poupanca ou Salario + Poupanca.\n");
            }
        } while (tipo2 < 1 || tipo2 > 3 || podeCombinar(tipo1, tipo2) == 0);

        printf("Saldo inicial: R$ ");
        saldo = lerValor(0);
        criarConta(c, 1, tipo2, saldo);
        clientes[c].qtdContas = 2;
    }

    qtdClientes++;
    registrarBanco(NOVO_CLIENTE, c, clientes[c].contas[0].numero, 0);

    printf("\nCliente cadastrado!\n");
    for (int j = 0; j < clientes[c].qtdContas; j++) {
        printf("  Conta %d - ", clientes[c].contas[j].numero);
        mostrarTipo(clientes[c].contas[j].tipo);
        printf("\n");
    }
}

void listarClientes() {
    int i, j;

    printf("\n--- CLIENTES ---\n");
    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (i = 0; i < qtdClientes; i++) {
        printf("%s\n", clientes[i].nome);
        for (j = 0; j < clientes[i].qtdContas; j++) {
            printf("   Conta %d | ", clientes[i].contas[j].numero);
            mostrarTipo(clientes[i].contas[j].tipo);
            printf(" | Saldo: R$ %.2f\n", clientes[i].contas[j].saldo);
        }
        if (clientes[i].temEmprestimo == 1) {
            printf("   Emprestimo: R$ %.2f\n", clientes[i].valorEmprestimo);
        }
    }
}

void depositar() {
    int numero, c, j;
    double valor;

    printf("\nNumero da conta: ");
    numero = lerInteiro();
    c = buscarConta(numero, &j);
    if (c == -1) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("Valor do deposito: R$ ");
    valor = lerValor(0.01);
    clientes[c].contas[j].saldo += valor;

    registrarMovimento(c, j, DEPOSITO, valor);
    registrarBanco(DEPOSITO, c, numero, valor);
    printf("Deposito feito. Novo saldo: R$ %.2f\n", clientes[c].contas[j].saldo);
}

void sacar() {
    int numero, c, j;
    double valor, minimo;

    printf("\nNumero da conta: ");
    numero = lerInteiro();
    c = buscarConta(numero, &j);
    if (c == -1) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("Valor do saque: R$ ");
    valor = lerValor(0.01);

    if (clientes[c].contas[j].tipo == CORRENTE) {
        minimo = -LIMITE_CHEQUE;
    } else {
        minimo = 0;
    }

    if (clientes[c].contas[j].saldo - valor < minimo) {
        printf("Saldo insuficiente.\n");
        return;
    }
    clientes[c].contas[j].saldo -= valor;

    registrarMovimento(c, j, SAQUE, valor);
    registrarBanco(SAQUE, c, numero, valor);
    printf("Saque feito. Novo saldo: R$ %.2f\n", clientes[c].contas[j].saldo);
}

void solicitarEmprestimo() {
    int numero, c, j, pc, pp;
    double limite, valor;

    printf("\nNumero de uma conta do cliente: ");
    numero = lerInteiro();
    c = buscarConta(numero, &j);
    if (c == -1) {
        printf("Conta nao encontrada.\n");
        return;
    }

    pc = posicaoDoTipo(c, CORRENTE);
    pp = posicaoDoTipo(c, POUPANCA);

    if (clientes[c].temEmprestimo == 1) {
        printf("Emprestimo negado: o cliente ja possui um emprestimo.\n");
        return;
    }
    if (pc == -1) {
        printf("Emprestimo negado: o cliente precisa ter conta corrente.\n");
        return;
    }
    if (pp == -1 || clientes[c].contas[pp].saldo <= 0) {
        printf("Emprestimo negado: o cliente precisa ter salario na poupanca.\n");
        return;
    }
    if (saldoTotalCliente(c) <= 0) {
        printf("Emprestimo negado: o saldo total do cliente nao e positivo.\n");
        return;
    }

    limite = limiteEmprestimo(c);
    printf("Cliente: %s\n", clientes[c].nome);
    printf("Saldo total: R$ %.2f\n", saldoTotalCliente(c));
    printf("Limite de emprestimo: R$ %.2f\n", limite);

    printf("Valor desejado: R$ ");
    valor = lerValor(0.01);
    if (valor > limite) {
        printf("Valor acima do limite. Emprestimo negado.\n");
        return;
    }

    clientes[c].contas[pc].saldo += valor;
    clientes[c].temEmprestimo = 1;
    clientes[c].valorEmprestimo = valor;

    registrarMovimento(c, pc, EMPRESTIMO, valor);
    registrarBanco(EMPRESTIMO, c, clientes[c].contas[pc].numero, valor);
    printf("Emprestimo aprovado e creditado na conta corrente %d.\n", clientes[c].contas[pc].numero);
}

void historicoCliente() {
    int numero, c, j, k;

    printf("\nNumero de uma conta do cliente: ");
    numero = lerInteiro();
    c = buscarConta(numero, &j);
    if (c == -1) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("\n--- HISTORICO DE %s ---\n", clientes[c].nome);
    for (j = 0; j < clientes[c].qtdContas; j++) {
        printf("Conta %d (", clientes[c].contas[j].numero);
        mostrarTipo(clientes[c].contas[j].tipo);
        printf("):\n");
        for (k = 0; k < clientes[c].contas[j].qtdHist; k++) {
            printf("   %d. ", k + 1);
            mostrarOperacao(clientes[c].contas[j].historico[k].operacao);
            printf(" de R$ %.2f | saldo R$ %.2f\n",
                   clientes[c].contas[j].historico[k].valor,
                   clientes[c].contas[j].historico[k].saldoApos);
        }
    }
}

double saldoTotalBanco() {
    double total = 0;
    int i;
    for (i = 0; i < qtdClientes; i++) {
        total += saldoTotalCliente(i);
    }
    return total;
}

void mostrarAptos() {
    int i, j, achou = 0;

    printf("\nClientes aptos a emprestimo:\n");
    for (i = 0; i < qtdClientes; i++) {
        if (aptoEmprestimo(i) == 1) {
            printf("   %s | contas:", clientes[i].nome);
            for (j = 0; j < clientes[i].qtdContas; j++) {
                printf(" %d", clientes[i].contas[j].numero);
            }
            printf(" | limite: R$ %.2f\n", limiteEmprestimo(i));
            achou = 1;
        }
    }
    if (achou == 0) {
        printf("   Nenhum.\n");
    }
}

void mostrarNegativos() {
    int i, j, achou = 0;

    printf("\nClientes negativos:\n");
    for (i = 0; i < qtdClientes; i++) {
        for (j = 0; j < clientes[i].qtdContas; j++) {
            if (clientes[i].contas[j].saldo < 0) {
                printf("   %s | conta %d | saldo: R$ %.2f\n", clientes[i].nome,
                       clientes[i].contas[j].numero, clientes[i].contas[j].saldo);
                achou = 1;
            }
        }
    }
    if (achou == 0) {
        printf("   Nenhum.\n");
    }
}

void mostrarTitulos() {
    int i, ps, achou = 0;
    double saldo;

    printf("\nClientes titulos (conta salario com saldo entre R$ 1000 e R$ 1500):\n");
    for (i = 0; i < qtdClientes; i++) {
        ps = posicaoDoTipo(i, SALARIO);
        if (ps != -1) {
            saldo = clientes[i].contas[ps].saldo;
            if (saldo >= 1000 && saldo <= 1500) {
                printf("   %s | conta %d | saldo: R$ %.2f\n", clientes[i].nome,
                       clientes[i].contas[ps].numero, saldo);
                achou = 1;
            }
        }
    }
    if (achou == 0) {
        printf("   Nenhum.\n");
    }
}

void relatorios() {
    printf("\n========== RELATORIOS ==========\n");
    printf("\nSaldo total do banco: R$ %.2f\n", saldoTotalBanco());
    mostrarAptos();
    mostrarNegativos();
    mostrarTitulos();
    printf("================================\n");
}

void abrirBanco() {
    if (bancoAberto == 0) {
        bancoAberto = 1;
        registrarBanco(BANCO_ABERTO, -1, 0, 0);
        printf("Banco aberto!\n");
    }
}

void fecharBanco() {
    bancoAberto = 0;
    registrarBanco(BANCO_FECHADO, -1, 0, 0);
    printf("Banco fechado.\n");
}

int bancoEstaAberto() {
    return bancoAberto;
}

void historicoDoBanco() {
    int i;

    if (bancoAberto == 0) {
        printf("\nO historico so pode ser consultado com o banco aberto.\n");
        return;
    }

    printf("\n--- HISTORICO DO BANCO ---\n");
    for (i = 0; i < qtdHistBanco; i++) {
        printf("%d. ", i + 1);
        mostrarOperacao(historicoBanco[i].operacao);

        if (historicoBanco[i].operacao == NOVO_CLIENTE) {
            printf(": %s", clientes[historicoBanco[i].cliente].nome);
        } else if (historicoBanco[i].operacao == DEPOSITO ||
                   historicoBanco[i].operacao == SAQUE ||
                   historicoBanco[i].operacao == EMPRESTIMO) {
            printf(" de R$ %.2f | conta %d (%s)", historicoBanco[i].valor,
                   historicoBanco[i].numeroConta, clientes[historicoBanco[i].cliente].nome);
        }
        printf("\n");
    }
}

void liberarMemoria() {
    free(clientes);
    clientes = NULL;
    qtdClientes = 0;
}
