#include <stdio.h>
#include <stdlib.h>

#include "Funcs.h"

void cabecalho()//Metodo com cabeçalho para ser impresso no porgrama
{
    printf("------------------------------------------------------------------------------\n");
    printf("|                           VETERINARIA                                       |\n");
    printf("|                               ED II                                         |\n");
    printf("------------------------------------------------------------------------------\n");
}

int menu()//Metodo para imprimir as opções do programa
{
    int op;//variável para armazenar opção
    do{
        printf("1 - CADASTRO DE NOVA VACINA \n");
        printf("2 - ALTERAR O CAMPO DE ALGUM CADASTRO\n");
        printf("3 - REMOVER REGISTRO\n");
        printf("4 - COMPACTAR O ARQUIVO\n");
        printf("5 - CONSULTAR CADASTRO(Pesquisa Idx Primario)\n");
        printf("6 - CONSULTAR CADASTRO PELA VACINA(Pesquisa Idx secundario)\n");
        printf("0 - SAIR \n");
        printf("\n\nDIGITE A OPERACAO: ");
        scanf("%d",&op);//Lê opção do usuário
    }while(op < 0 || op > 6);//Laço de verificação da opção digitada pelo usuário

    return op;
}

void validaOpcao(int opcao)
{
    int codDog, retorno;
    char vacina[40];
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
        case 4:
            system("cls");
            cabecalho();
            compactaArquivo();
            printf("\nArquivo Compactado Com Sucesso!");
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

        case 6:
            system("cls");
            cabecalho();
            fflush(stdin);
            printf("\nDigite a Vacina a Ser Buscada: ");
            gets(vacina);
            pesquisaKeySec(vacina);
            break;

        case 0:
            salvaIdx();
            break;
    }
}

int main() {//Metodo Main
    int opcao;//Variável para armazenar a opção do usuário

    criaVetorIdx();
    criaVetorIdxSec();

    do{
        cabecalho();
        opcao = menu();
        validaOpcao(opcao);

    }while(opcao != 0);

    return 0;
}
