/****************************************************************************
O arquivo Funcs.c vai conter todas as funcoes do projeto
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 1000

#define FileAP1 "Ap1.bin"
#define FileAP2 "Ap2.bin"
#define IdxAP1 "idx.bin"

#include "arqFuncs.h"


const int ListaVazia = -1; //indica se a lista disponivel est� vazia
int codControl = 1;
/*************************************************************
---------------- Definicao das estruturas---------------------
**************************************************************/
typedef struct registroAP1{
    int codigoControle;
    int codigoCachorro;
    char vacina[50];
    char dataVacina[50];
    char respVacina[50];
} RegAP1; //RegAP1 vai receber as estruturas do Arquivo Principal 1

typedef struct idxAP1{
    int idx_codControle;
    int offset;
    int flag;
} IDX_RegAP1; //IDX_RegAP1 sera a estrutura de indices do AP1


typedef struct registroAP2{
    int codigoCachorro;
    char raca[50];
    char nomeCachorro[50];
} RegAP2; //RegAP2 vai receber as estruturas do Arquivo Principal 2

typedef struct registroIndices{
    int codControle;
    int offset;
}RegIdx;
/*******************************************************************/
int headListAP1 = -1;//indica a cabe�a da lista de dispon�veis em AP1
int headListAP2 = -1;//indica a cabe�a da lista de dispon�veis em AP2
/******Quando estes valores acima forem igual a -1, inerimos no fim do arquivo*******/

RegIdx Idx1[MAX]; //Vetor que guarda as estruturas do indice
fimIdx1 = 0;

int pesquisaKeyPrimariaAP2(int ch, FILE *fileAP2)
{
    FILE *aux;
    aux = fileOpen(FileAP2, &headListAP2);

    char num[5];
    int offset = 0,achou = 0;

    aux = fileAP2;
    fread(num,sizeof(int),1,aux);
    int v = atoi(num);
    if (v == 0)
        return -1;

    while(!(feof(fileAP2))){
        if (ch == v){
            achou = 1;
            break;
        }
        offset++;
        fseek(fileAP2,sizeof(RegAP2),1);
        aux = fileAP2;
        fread(&num,sizeof(int),1,aux);
        v = atoi(num);
    }

    fclose(aux);

    if (achou)
        return offset;
    else
        return -1;
}


int insereAP2(RegAP2* novoAp2){

    int achou;
    char num[4];

    FILE *fpAP2;
    FILE *aux;

    fpAP2 = fileOpen(FileAP2, &headListAP2);
    aux = fileOpen(FileAP2, &headListAP2);

    achou = pesquisaKeyPrimariaAP2(novoAp2->codigoCachorro, aux);

    if(headListAP2 == -1 && achou == -1){
        fseek(fpAP2,0,SEEK_END);
        itoa(novoAp2->codigoCachorro,num,10);
        fwrite(num,sizeof(int),1,fpAP2);
        fwrite(novoAp2->nomeCachorro,50*sizeof(char),1,fpAP2);
        fwrite(novoAp2->raca,50*sizeof(char),1,fpAP2);
        fclose(fpAP2);
        fclose(aux);
        return 1;
    }

    else if(achou != -1){
        printf("\nCadastro ja existente!\n");
        fclose(fpAP2);
        fclose(aux);
        return 0;
    }

    /*achou = pesquisaKeyPrimariaAP2(novoAp2->codigoCachorro, aux);
    if(achou){
        printf("\nCadastro ja existente!\n");
        fclose(fpAP2);
        fclose(aux);
        return 0;
    }*/

    /*char *teste;
    teste = "isso e um teste";
    fwrite(teste,sizeof(char),strlen(teste),fpAP2);
    fclose(fpAP2);
    achou = pesquisaKeyPrimariaAP2(novoAp2->codigoCachorro, aux);*/
    //firstFitAP2(&fpAP2,sizeof(novoAP2))
}

int obterDadosCadastroAP2()
{
    RegAP2 novoAp2;

    printf("\nCodigo do cachorro: ");
    scanf("%d",&(novoAp2.codigoCachorro));
    printf("\nRaca do cachorro: ");
    fflush(stdin);
    gets(novoAp2.raca);
    printf("\nNome do cachorro: ");
    fflush(stdin);
    gets(novoAp2.nomeCachorro);

    int cod = insereAP2(&novoAp2);

    if(cod)
        return (novoAp2.codigoCachorro);
    else
        return 0;
}

int firstFit(int tam,FILE *fp){
    char tamRegDispo[5], valOffset[5];
    int tamDispo, offsetProx, achou = 0;
    FILE *aux;
    aux = fileOpen(FileAP1, &headListAP1);
    aux = fp;
    fseek(fp,headListAP1*sizeof(char),1);
    fread(tamRegDispo,sizeof(int),1,fp);//le o tamanho do esp�o disponivel
    tamDispo = atoi(tamRegDispo);

    fread(valOffset,sizeof(int),1,fp);//le o offset do proximo espa�o vazio
    offsetProx = atoi(valOffset);

    while(!(feof(fp))){
        if (tam <= tamDispo){
            achou = 1;
            headListAP1 = offsetProx;
            break;
        }
        fseek(fp,offsetProx*sizeof(char),1);
        aux = fp;
        fread(tamRegDispo,sizeof(int),1,fp);//le o tamanho do esp�o disponivel
        tamDispo = atoi(tamRegDispo);

        fread(valOffset,sizeof(int),1,fp);//le o offset do proximo espa�o vazio
        offsetProx = offsetProx + atoi(valOffset);
    }


    if (achou)
        return offsetProx;
    else
        return -1;
}

int insereAP1(RegAP1* novoAp1){

    int tam, proxOffset,auxCod, offsetAux;
    char buffer[256], num1[4],num2[4],valorCabecalho[4],tamReg[4];

    FILE *fpAP1;
    FILE *aux;

    fpAP1 = fileOpen(FileAP1, &headListAP1);
    aux = fileOpen(FileAP1, &headListAP1);

    rewind(aux);

    ebaidnohfiogsuefosfheoio

    fread(valorCabecalho,sizeof(int),1,aux);
    headListAP1 = atoi(valorCabecalho);

    itoa(novoAp1->codigoControle,num1,10);
    itoa(novoAp1->codigoCachorro,num2,10);
    sprintf(buffer,"*%s|%s|%s|%s|%s|",num1,num2,novoAp1->vacina,
                            novoAp1->dataVacina,novoAp1->respVacina);
    tam = strlen(buffer);

    if(fimIdx == 0)
        Idx1[fimIdx].codControle = 1;
    else{
        Idx1[fimIdx].codControle = Idx1[fimIdx-1].codControle + 1;
    }
    offsetAux = firstFit(tam,aux);
    if(headListAP1 == -1 || offsetAux == -1){
        fseek(aux,1,SEEK_END);
        Idx1[fimIdx].offset = (ftell(aux)-4);
        fimIdx++;
    }
    else{
        Idx1[fimIdx].offset = offsetAux;
        fimIdx++;
    }

    /*if(headListAP1 == -1){
        fseek(fpAP1,0,SEEK_END);
        itoa(tam,num1,10);*/
        fwrite(num1,sizeof(int),1,fpAP1);
        fwrite(buffer,sizeof(char),tam,fpAP1);
        fclose(fpAP1);
        fclose(aux);
        return 1;
    /*}
    else{
        proxOffset = firstFit(tam,fpAP1);
        fseek(aux,proxOffset*sizeof(char),1);
        itoa(tam,num1,10);
        fwrite(num1,sizeof(int),1,aux);
        fwrite(buffer,sizeof(char),tam,aux);
        fclose(fpAP1);
        fclose(aux);
        return 1;
    }*/
}

int obterDadosCadastroAP1(int codDog)
{
    RegAP1 novoAp1;

    novoAp1.codigoCachorro = codDog;
    printf("\nNome da Vacina: ");
    fflush(stdin);
    gets(novoAp1.vacina);
    printf("\nData Vacina(mes/ano): ");
    fflush(stdin);
    gets(novoAp1.dataVacina);
    printf("\nResponsavel Pela Aplicacao: ");
    fflush(stdin);
    gets(novoAp1.respVacina);

    return insereAP1(&novoAp1);
}

int alteraVacina()
{
    int ch, op, achou;
    FILE *fp;
    FILE *aux;

    RegAP2 reg2;

    printf("\nDigite o Codigo do cachorro que deseja alterar: ");
    scanf("%d", ch);

    fp = fileOpen(FileAP2,&headListAP2);
    aux = fileOpen(FileAP2,&headListAP2);

    achou = pesquisaKeyPrimariaAP2(ch, fp);
    if(achou == -1)
        return 0;

    fseek(aux,achou*sizeof(RegAP2),1);
    fread(reg2.raca,50*sizeof(char),1,aux);
    fread(reg2.nomeCachorro,50*sizeof(char),1,aux);

    printf("Raca do Cachorro: %s",reg2.raca);
    printf("Nome do Cachorro: %s",reg2.nomeCachorro);


    do{
        printf("1- Nome da Vacina\n");
        printf("2- Data da Vacina\n");
        printf("4- Responsavel Pela Aplicacao\n");
        printf("5- Raca do Cachorro\n");
        printf("6- Nome do Cachorro\n");

        printf("Digite o Campo que Deseja Alterar: ")
        scanf("%d",&op)
    }while(op < 0 || op > 6);

    switch(op){
        case 5:
            fseek(fp,achou*sizeof(RegAP2),1);
            fseek(fp,sizeof(int),1);
            printf("Digite a Nova Raca do Cachorro: ");
            gets(reg2.raca);
            fwrite(reg2.raca,50*sizeof(char),1,fp);
            break;
        case 6:
            fseek(fp,achou*sizeof(RegAP2),1);
            fseek(fp,sizeof(int),1,fp);
            fseek(fp,50*sizeof(char),1,fp)
            printf("Digite o Novo Nome do Cachorro: ");
            gets(reg2.nomeCachorro);
            fwrite(reg2.nomeCachorro,50*sizeof(char),1,fp);
            break;
    }
    system("cls");
    cabecalho();
    rewind(aux);
    fseek(aux,achou*sizeof(RegAP2),1);
    fseek(aux,sizeof(int),1);
    fread(reg2.raca,50*sizeof(char),1,aux);
    fread(reg2.nomeCachorro,50*sizeof(char),1,aux);

    printf("Raca do Cachorro: %s",reg2.raca);
    printf("Nome do Cachorro: %s",reg2.nomeCachorro);
    printf("Dados do Cadastro Ja Alterado: \n")

    return 1;
}