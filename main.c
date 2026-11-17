#include <stdio.h>
#include "meu_banco.h"

void menuBanco() {
    int opcao;

    do {
        printf("BANCO ABERTO\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("3 - Depositar\n");
        printf("4 - Sacar\n");
        printf("5 - Solicitar emprestimo\n");
        printf("6 - Historico de um cliente\n");
        printf("7 - Relatorios\n");
        printf("8 - Fechar o banco\n");
        printf("0 - Voltar ao menu do banqueiro\n");
        printf("Opcao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: listarClientes(); break;
            case 3: depositar(); break;
            case 4: sacar(); break;
            case 5: solicitarEmprestimo(); break;
            case 6: historicoCliente(); break;
            case 7: relatorios(); break;
            case 8: fecharBanco(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0 && opcao != 8);
}

int main() {
    char opcao;

    do {
        printf("\nMENU DO BANQUEIRO\n");
        if (bancoEstaAberto() == 1) {
            printf("1 - Entrar no banco (aberto)\n");
        } else {
            printf("1 - Abrir o banco\n");
        }
        printf("2 - Historico do banco\n");
        printf("S - Sair\n");
        printf("Opcao: ");

        opcao = 's';
        scanf(" %c", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case '1':
                abrirBanco();
                menuBanco();
                break;
            case '2':
                historicoDoBanco();
                break;
            case 's':
            case 'S':
                break;
            default:
                printf("Opcao invalida.\n");
        }
    
    } while (opcao != 's' && opcao != 'S');

    liberarMemoria();
    return 0;
}
