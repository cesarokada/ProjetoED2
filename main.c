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
        printf("3 - REMOVER REGISTRO\n");
        printf("5 - CONSULTAR CADASTRO(Pesquisa Idx Primario)\n");
        printf("0 - SAIR \n");
        printf("\n\nDIGITE A OPERACAO: ");
        scanf("%d",&op);
    }while(op < 0 || op > 5);

    return op;
}

void validaOpcao(int opcao)
{
    int codDog, retorno;
    FILE *fp;

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
                printf("Registro Nao Encontrado");
            break;

        case 3:
            system("cls");
            cabecalho();
            printf("\nDigite o Codigo do Cachorro a Ser Removido: ");
            scanf("%d",&codDog);
            retorno = pesquisaKeyPrimariaAP1(codDog);
            if (retorno != -1){
                imprime(retorno);
                removeAp1(retorno,codDog);
            }
            else
                printf("\nRegistro Nao Encontrado");
            break;

        case 5:
            system("cls");
            cabecalho();
            printf("\nDigite o Codigo do Cachorro a ser buscado: ");
            scanf("%d",&codDog);
            retorno = pesquisaKeyPrimariaAP1(codDog);
            if (retorno != -1)
                imprime(retorno);
            else
                printf("\nRegistro Nao Encontrado");
            break;
        case 0:
            salvaIdx();
            break;
    }
}

int main()
{
    int opcao;

    criaVetorIdx();

    do{
        cabecalho();
        opcao = menu();
        validaOpcao(opcao);
        //system("cls");
    }while(opcao != 0);
    return 0;
}
