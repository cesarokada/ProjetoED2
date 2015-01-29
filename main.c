#include <stdio.h>
#include <stdlib.h>

#include "Funcs.h"

void cabecalho()
{
    printf("------------------------------------------------------------------------------\n");
    printf("|                           VETERINARIA                                       |\n");
    printf("|                               ED II                                         |\n");
    printf("------------------------------------------------------------------------------\n");
}

int menu()
{
    int op;
    do{
        printf("1 - CADASTRO DE NOVA VACINA \n");
        printf("2 - ALTERAR O CAMPO DE ALGUM CADASTRO\n");
        printf("0 - SAIR \n");
        printf("\n\nDIGITE A OPERACAO: ");
        scanf("%d",&op);
    }while(op < 0 || op > 1);

    return op;
}

void validaOpcao(int opcao)
{
    int codDog, retorno;

    switch(opcao){
        case 1:
            system("cls");
            cabecalho();
            codDog = obterDadosCadastroAP2();
            if(codDog){
                obterDadosCadastroAP1(codDog);
                printf("\nCadastro Efetuado Com Sucesso!\n");
            }
            break;
        case 2:
            system("cls");
            cabecalho();
            retorno = alteraVacina();
            if (retorno)
                printf("\nRegistro Alterado Com Sucesso");
            else
                printf("Registro Não Encontrado");
            break;
        case 0:
            break;
    }
}

int main()
{
    int opcao;

    do{
        cabecalho();
        opcao = menu();
        validaOpcao(opcao);
        system("cls");
    }while(opcao != 0);
    return 0;
}
