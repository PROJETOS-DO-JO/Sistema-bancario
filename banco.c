#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meu_banco.h"

Cliente *clientes = NULL;
int qtdClientes = 0;
int bancoAberto = 0;
int dadosCarregados = 0;

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

void registrarMovimento(int c, int j, char op, double valor) {
    int k;
    Conta *ct = &clientes[c].contas[j];

    if (ct->qtdHist == MAX_HIST) {
        for (k = 0; k < MAX_HIST - 1; k++) {
            ct->historico[k] = ct->historico[k + 1];
        }
        ct->qtdHist--;
    }
    ct->historico[ct->qtdHist].op = op;
    ct->historico[ct->qtdHist].valor = valor;
    ct->qtdHist++;
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

int buscarCliente(int numero) {
    int i;
    for (i = 0; i < qtdClientes; i++) {
        if (clientes[i].numero == numero) {
            return i;
        }
    }
    return -1;
}

void criarConta(int c, int j, int tipo) {
    clientes[c].contas[j].tipo = tipo;
    clientes[c].contas[j].saldo = 0;
    clientes[c].contas[j].qtdHist = 0;
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

int aptoTitulo(int c) {
    int ps = posicaoDoTipo(c, SALARIO);
    double saldo;

    if (clientes[c].temTitulo == 1) {
        return 0;
    }
    if (ps == -1) {
        return 0;
    }
    saldo = clientes[c].contas[ps].saldo;
    if (saldo >= 1000 && saldo <= 1500) {
        return 1;
    }
    return 0;
}

void cadastrarCliente() {
    Cliente *aux;
    int c, j, tipo1, tipo2;
    char resposta;

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

    clientes[c].numero = 1000 + qtdClientes;
    clientes[c].qtdContas = 0;
    clientes[c].temEmprestimo = 0;
    clientes[c].valorEmprestimo = 0;
    clientes[c].temTitulo = 0;
    clientes[c].sorteado = 0;
    clientes[c].premio = 0;

    do {
        printf("Tipo da conta (1-Corrente / 2-Salario / 3-Poupanca): ");
        tipo1 = lerInteiro();
    } while (tipo1 < 1 || tipo1 > 3);

    criarConta(c, 0, tipo1);
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

        criarConta(c, 1, tipo2);
        clientes[c].qtdContas = 2;
    }

    qtdClientes++;

    printf("\nCliente cadastrado!\n");
    for (j = 0; j < clientes[c].qtdContas; j++) {
        printf("  Conta %d-%02d (", clientes[c].numero, clientes[c].contas[j].tipo);
        mostrarTipo(clientes[c].contas[j].tipo);
        printf(")\n");
    }

    salvarDados();
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
            printf("   Conta %d-%02d | ", clientes[i].numero, clientes[i].contas[j].tipo);
            mostrarTipo(clientes[i].contas[j].tipo);
            printf(" | Saldo: R$ %.2f\n", clientes[i].contas[j].saldo);
        }
        if (clientes[i].temEmprestimo == 1) {
            printf("   Emprestimo: R$ %.2f\n", clientes[i].valorEmprestimo);
        }
        if (clientes[i].temTitulo == 1) {
            printf("   Possui titulo de capitalizacao\n");
        }
    }
}

int lerConta(int *c, int *j) {
    int numero, tipo;

    printf("\nNumero da conta (ex: 1000-01): ");
    if (scanf("%d-%d", &numero, &tipo) != 2) {
        limparBuffer();
        printf("Formato invalido.\n");
        return 0;
    }
    limparBuffer();

    *c = buscarCliente(numero);
    if (*c == -1) {
        printf("Conta nao encontrada.\n");
        return 0;
    }
    *j = posicaoDoTipo(*c, tipo);
    if (*j == -1) {
        printf("Conta nao encontrada.\n");
        return 0;
    }
    return 1;
}

void depositar() {
    int c, j;
    double valor;

    if (lerConta(&c, &j) == 0) {
        return;
    }

    printf("Valor do deposito: R$ ");
    valor = lerValor(0.01);
    clientes[c].contas[j].saldo += valor;

    registrarMovimento(c, j, 'D', valor);
    printf("Deposito feito. Novo saldo: R$ %.2f\n", clientes[c].contas[j].saldo);
    salvarDados();
}

void sacar() {
    int c, j;
    double valor, minimo;

    if (lerConta(&c, &j) == 0) {
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

    registrarMovimento(c, j, 'S', valor);
    printf("Saque feito. Novo saldo: R$ %.2f\n", clientes[c].contas[j].saldo);
    salvarDados();
}

void solicitarEmprestimo() {
    int c, j, pc, pp;
    double limite, valor;

    if (lerConta(&c, &j) == 0) {
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
        printf("Emprestimo negado: o cliente precisa ter saldo na poupanca.\n");
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

    registrarMovimento(c, pc, 'E', valor);
    printf("Emprestimo aprovado e creditado na conta %d-01.\n", clientes[c].numero);
    salvarDados();
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
    int i;

    printf("\n== Clientes Aptos a Emprestimos ==\n");
    for (i = 0; i < qtdClientes; i++) {
        if (aptoEmprestimo(i) == 1) {
            printf("%s|%d-01|R$ %.2f\n", clientes[i].nome, clientes[i].numero,
                   limiteEmprestimo(i));
        }
    }
}

void mostrarNegativos() {
    int i, pc;

    printf("\n== Clientes com Saldo Negativo na Conta Corrente ==\n");
    for (i = 0; i < qtdClientes; i++) {
        pc = posicaoDoTipo(i, CORRENTE);
        if (pc != -1 && clientes[i].contas[pc].saldo < 0) {
            printf("%s|%d-01|R$ %.2f\n", clientes[i].nome, clientes[i].numero,
                   clientes[i].contas[pc].saldo);
        }
    }
}

void mostrarTitulos() {
    int i, ps;

    printf("\n== Clientes Aptos a Compra de Titulos ==\n");
    for (i = 0; i < qtdClientes; i++) {
        if (aptoTitulo(i) == 1) {
            ps = posicaoDoTipo(i, SALARIO);
            printf("%s|%d-02|R$ %.2f\n", clientes[i].nome, clientes[i].numero,
                   clientes[i].contas[ps].saldo);
        }
    }
}

void mostrarContemplados() {
    int i;

    printf("\n== Clientes Contemplado no Sorteio ==\n");
    for (i = 0; i < qtdClientes; i++) {
        if (clientes[i].sorteado == 1) {
            printf("%s|%d-02|R$ %.2f\n", clientes[i].nome, clientes[i].numero,
                   clientes[i].premio);
        }
    }
}

void historicoDoBanco() {
    printf("\n== Saldo Total do banco ==\n");
    printf("%d | R$ %.2f\n", qtdClientes, saldoTotalBanco());
    mostrarAptos();
    mostrarNegativos();
    mostrarTitulos();
    mostrarContemplados();
}

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int somaFibonacci(int n) {
    int i, soma = 0;
    for (i = 0; i < n; i++) {
        soma += fibonacci(i);
    }
    return soma;
}

int ehPrimo(int n) {
    int i;
    if (n < 2) {
        return 0;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void efetuarSorteio() {
    int *ordem;
    int i, j, aux, n, vencedor, ps;
    int resultado[5];
    double media = 0, premio;

    if (qtdClientes < 5) {
        printf("\nO sorteio precisa de pelo menos 5 clientes.\n");
        return;
    }

    ordem = (int *) malloc(qtdClientes * sizeof(int));
    if (ordem == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    for (i = 0; i < qtdClientes; i++) {
        ordem[i] = i;
    }

    for (i = 0; i < qtdClientes - 1; i++) {
        for (j = 0; j < qtdClientes - 1 - i; j++) {
            if (saldoTotalCliente(ordem[j]) < saldoTotalCliente(ordem[j + 1])) {
                aux = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = aux;
            }
        }
    }

    for (i = 0; i < 5; i++) {
        if (clientes[ordem[i]].temTitulo == 0) {
            printf("\nSorteio nao realizado: %s esta entre os 5 maiores saldos e nao tem titulo.\n",
                   clientes[ordem[i]].nome);
            free(ordem);
            return;
        }
    }

    printf("\n--- SORTEIO ENTRE OS 5 MAIORES SALDOS ---\n");
    for (i = 0; i < 5; i++) {
        do {
            printf("%s, digite um numero de 1 a 30: ", clientes[ordem[i]].nome);
            n = lerInteiro();
        } while (n < 1 || n > 30);
        resultado[i] = somaFibonacci(n);
        media += resultado[i];
        printf("Soma de Fibonacci: %d\n", resultado[i]);
    }
    media = media / 5;

    vencedor = -1;
    for (i = 0; i < 5; i++) {
        if (ehPrimo(resultado[i]) == 1) {
            if (vencedor == -1 || resultado[i] > resultado[vencedor]) {
                vencedor = i;
            }
        }
    }

    if (vencedor == -1) {
        for (i = 0; i < 5; i++) {
            if (resultado[i] % 2 == 0) {
                if (vencedor == -1 || resultado[i] > resultado[vencedor]) {
                    vencedor = i;
                }
            }
        }
    }

    if (vencedor == -1) {
        for (i = 0; i < 5; i++) {
            if (resultado[i] > media) {
                if (vencedor == -1 || resultado[i] > resultado[vencedor]) {
                    vencedor = i;
                }
            }
        }
    }

    if (vencedor == -1) {
        vencedor = 0;
    }

    i = ordem[vencedor];
    ps = posicaoDoTipo(i, SALARIO);
    premio = 10 * clientes[i].contas[ps].saldo;
    clientes[i].contas[ps].saldo += premio;
    registrarMovimento(i, ps, 'D', premio);
    clientes[i].sorteado = 1;
    clientes[i].premio += premio;

    printf("\nVencedor: %s (%d-02) | Premio: R$ %.2f\n", clientes[i].nome,
           clientes[i].numero, premio);

    free(ordem);
    salvarDados();
}

void salvarDados() {
    FILE *arq;
    int i, j, k;
    Conta *ct;

    if (dadosCarregados == 0) {
        return;
    }

    arq = fopen(ARQUIVO, "w");
    if (arq == NULL) {
        printf("Erro ao salvar os dados.\n");
        return;
    }

    fprintf(arq, "%d\n", qtdClientes);
    for (i = 0; i < qtdClientes; i++) {
        fprintf(arq, "%s\n", clientes[i].nome);
        fprintf(arq, "%d %d %d %.2f %d %d %.2f\n", clientes[i].numero,
                clientes[i].qtdContas, clientes[i].temEmprestimo,
                clientes[i].valorEmprestimo, clientes[i].temTitulo,
                clientes[i].sorteado, clientes[i].premio);
        for (j = 0; j < clientes[i].qtdContas; j++) {
            ct = &clientes[i].contas[j];
            fprintf(arq, "%d %.2f %d", ct->tipo, ct->saldo, ct->qtdHist);
            for (k = 0; k < ct->qtdHist; k++) {
                fprintf(arq, " %.2f %c", ct->historico[k].valor, ct->historico[k].op);
            }
            fprintf(arq, "\n");
        }
    }

    fclose(arq);
}

void carregarDados() {
    FILE *arq;
    int i, j, k, tam, ch;
    Conta *ct;

    dadosCarregados = 1;

    arq = fopen(ARQUIVO, "r");
    if (arq == NULL) {
        return;
    }

    if (fscanf(arq, "%d", &qtdClientes) != 1 || qtdClientes <= 0) {
        qtdClientes = 0;
        fclose(arq);
        return;
    }

    clientes = (Cliente *) malloc(qtdClientes * sizeof(Cliente));
    if (clientes == NULL) {
        printf("Erro: memoria insuficiente.\n");
        qtdClientes = 0;
        fclose(arq);
        return;
    }

    for (i = 0; i < qtdClientes; i++) {
        do {
            ch = fgetc(arq);
        } while (ch != '\n' && ch != EOF);

        fgets(clientes[i].nome, 50, arq);
        tam = strlen(clientes[i].nome);
        if (tam > 0 && clientes[i].nome[tam - 1] == '\n') {
            clientes[i].nome[tam - 1] = '\0';
        }

        fscanf(arq, "%d %d %d %lf %d %d %lf", &clientes[i].numero,
               &clientes[i].qtdContas, &clientes[i].temEmprestimo,
               &clientes[i].valorEmprestimo, &clientes[i].temTitulo,
               &clientes[i].sorteado, &clientes[i].premio);

        for (j = 0; j < clientes[i].qtdContas; j++) {
            ct = &clientes[i].contas[j];
            fscanf(arq, "%d %lf %d", &ct->tipo, &ct->saldo, &ct->qtdHist);
            for (k = 0; k < ct->qtdHist; k++) {
                fscanf(arq, " %lf %c", &ct->historico[k].valor, &ct->historico[k].op);
            }
        }
    }

    fclose(arq);
    printf("%d cliente(s) carregado(s) do arquivo.\n", qtdClientes);
}

void abrirBanco() {
    if (dadosCarregados == 0) {
        carregarDados();
    }
    if (bancoAberto == 0) {
        bancoAberto = 1;
        printf("Banco aberto!\n");
    }
}

void fecharBanco() {
    salvarDados();
    bancoAberto = 0;
    printf("Banco fechado.\n");
}

int bancoEstaAberto() {
    return bancoAberto;
}

void liberarMemoria() {
    free(clientes);
    clientes = NULL;
    qtdClientes = 0;
}
