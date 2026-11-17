#include <stdio.h>
#include "meu_banco.h"

void menuBanco() {
    int opcao;

    do {
        printf("\n------ BANCO ABERTO ------\n");
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
