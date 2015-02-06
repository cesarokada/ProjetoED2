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
#define FileIdx1 "Idx1.bin"
#define FileIdx2 "Idx2Vacina.bin"
#define FileIdx2Cod "Idx2CodControle.bin"

#include "arqFuncs.h"


const int ListaVazia = -1; //indica se a lista disponivel está vazia
//int codControl = 1;
int flagIdx1 = 0;//1 - se o arquivo está atualizado e 0 se não está

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


typedef struct registroAP2{
    int codigoCachorro;
    char raca[50];
    char nomeCachorro[50];
} RegAP2; //RegAP2 vai receber as estruturas do Arquivo Principal 2

typedef struct registroIndices{
    int codControle;
    int offset;
}RegIdx;

typedef struct registroIndicesSec{
    char vacina[50];
    int offset;
}RegIdx2;
/*******************************************************************/
int headListAP1 = -1;//indica a cabeça da lista de disponíveis em AP1
int headListAP2 = -1;//indica a cabeça da lista de disponíveis em AP2
/******Quando estes valores acima forem igual a -1, inerimos no fim do arquivo*******/

RegIdx Idx1[MAX]; //Vetor que guarda as estruturas do indice
int fimIdx1 = 0;

RegIdx2 Idx2[MAX];
int fimIdx2 = 0;

int pesquisaKeyPrimariaAP2(int ch, FILE *fp)
{
    FILE *auxFp;
    auxFp = fileOpen(FileAP2, &headListAP2);

    char num[20];
    int offset = 0,achou = 0;

    rewind(auxFp);
    fseek(auxFp,sizeof(int),1);

    fp = auxFp;

    fread(num,sizeof(int),1,auxFp);
    int v = atoi(num);

    if (v == 0){
        return -1;
    }
    fseek(fp,-4,1);

    while(!(feof(fp))){
        if (ch == v){
            achou = 1;
            break;
        }
        offset++;
        rewind(fp);
        fseek(fp,offset*sizeof(RegAP2)+4,1);
        auxFp = fp;
        fread(num,2*sizeof(int),1,auxFp);
        v = atoi(num);
    }

    fclose(auxFp);

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
    //fclose(fpAP2);
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

void escreveFlag()
{
    FILE *fp;
    fp = fileOpenIdx(FileIdx1, &flagIdx1);

    char flag[5];

    rewind(fp);
    itoa(flagIdx1,flag,10);
    fwrite(flag,sizeof(int),1,fp);
}

int firstFit(int tam,FILE *fp){

    char tamRegDispo[5], valOffset[5];
    int tamDispo, offsetProx, achou = 0,offsetDispo,i;
    FILE *aux;

    aux = fileOpen(FileAP1, &headListAP1);

    i = fgetc(aux);
    if (i == -1)
        return -1;

    if(headListAP1 == -1)
        return -1;

    fseek(fp,headListAP1*sizeof(char),0);
    fseek(aux,headListAP1*sizeof(char),0);
    printf("head %d",headListAP1);

    do{
        printf("teste7");
        fread(tamRegDispo,sizeof(int),1,fp);//le o tamanho do espço disponivel
        tamDispo = atoi(tamRegDispo);
        fseek(fp,1,1);//pula o caracter '!'
        fread(valOffset,sizeof(int),1,fp);//le o offset do proximo espaço vazio
        offsetProx = atoi(valOffset);
        if (tam <= tamDispo){
            printf("teste 8");
            achou = 1;
            printf("teste 8");
            /*headListAP1 = offsetProx;
            printf("teste 8");
            itoa(headListAP1,valOffset,10);
            printf("teste 8");*/
            rewind(fp);
            printf("teste 8");
            fwrite(valOffset,sizeof(int),1,fp);
            printf("teste 8");
            offsetDispo = ftell(aux);
            printf("teste 8");
            break;
        }

        fseek(fp,offsetProx*sizeof(char),0);
        fseek(aux,offsetProx*sizeof(char),0);
    }while(offsetProx != -1);


    if (achou){
        printf("dispooo %d",offsetDispo);
        return offsetDispo;
    }
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
    printf("head1 %d",headListAP1);
    rewind(aux);

    fread(valorCabecalho,sizeof(int),1,aux);
    headListAP1 = atoi(valorCabecalho);
    printf("head2 %d",headListAP1);
    if(fimIdx1 == 0){
        Idx1[fimIdx1].codControle = 1;
        novoAp1->codigoControle = Idx1[fimIdx1].codControle;
    }
    else{
        Idx1[fimIdx1].codControle = Idx1[fimIdx1-1].codControle + 1;
        novoAp1->codigoControle = Idx1[fimIdx1].codControle;
    }

    itoa(novoAp1->codigoControle,num1,10);
    itoa(novoAp1->codigoCachorro,num2,10);
    sprintf(buffer,"*%s|%s|%s|%s|%s|",num1,num2,novoAp1->vacina,
                            novoAp1->dataVacina,novoAp1->respVacina);
    tam = strlen(buffer);

    offsetAux = firstFit(tam,aux);
    printf("head %d",headListAP1);
    if(headListAP1 == -1 || offsetAux == -1){
        fseek(aux,1,SEEK_END);
        Idx1[fimIdx1].offset = (ftell(aux)-4);
        fimIdx1++;

        fseek(fpAP1,0,SEEK_END);
        itoa(tam,num1,10);
        fwrite(num1,sizeof(int),1,fpAP1);
        fwrite(buffer,sizeof(char),tam,fpAP1);
        flagIdx1 = 0;
        escreveFlag();
        fclose(fpAP1);
        fclose(aux);
        return 1;
    }
    else
        {
        Idx1[fimIdx1].offset = offsetAux;
        fimIdx1++;

        rewind(aux);
        fseek(aux,offsetAux*sizeof(char),0);
        itoa(tam,num1,10);
        fwrite(num1,sizeof(int),1,aux);
        fwrite(buffer,sizeof(char),tam,aux);
        flagIdx1 = 0;
        escreveFlag();
        fclose(fpAP1);
        fclose(aux);
        return 1;
    }
}

int obterDadosCadastroAP1(int codDog)
{
    RegAP1 novoAp1;

    novoAp1.codigoControle = 1;
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
void pegaCampo(FILE *fp, char *buffer)
{
    char c;
    int i = 0;

    buffer[i] = '\0';

    c = fgetc(fp);
    while(c != '|'){
        buffer[i] = c;
        c = fgetc(fp);
        i++;
    }
    buffer[i] = '\0';
}

void criaVetorIdx(){
    FILE *fpIdx1;
    FILE *fp;
    FILE* aux;

    char tam[5],c,codControle[5],offset[5];
    int offsetProx,i = 0,cont = 0;

    fpIdx1 = fileOpenIdx(FileIdx1, &flagIdx1);

    if(flagIdx1 == 0){
        printf("teste1");
        aux = fileOpen(FileAP1,&headListAP1);
        fp = fileOpen(FileAP1,&headListAP1);

        if(feof(fp)){
            flagIdx1 = 1;
            escreveFlag();
            return 0;
        }

        fseek(fp,sizeof(int),0);
        offsetProx = 4;
        while(i!=-1){
            fread(tam,sizeof(int),1,fp);
            offsetProx = offsetProx + atoi(tam) + 4;
            c = fgetc(fp);
            printf("\ncaracter    %c",c);
            if (c == '!'){
                fseek(aux,offsetProx*sizeof(char),0);
                *fp = *aux;
                i = fgetc(aux);
                fseek(aux,offsetProx*sizeof(char),0);
            }
            else{
                pegaCampo(fp,codControle);
                Idx1[fimIdx1].codControle = atoi(codControle);
                if(fimIdx1 == 0){
                    Idx1[fimIdx1].offset = ftell(aux);
                }
                else{
                    Idx1[fimIdx1].offset = ftell(fp) - 7;
                }
                fimIdx1++;

                fseek(aux,offsetProx*sizeof(char),0);
                *fp = *aux;
                i = fgetc(aux);
                fseek(aux,offsetProx*sizeof(char),0);
            }
        }
        printf("\n1");
        for(i=0;i<fimIdx1;i++){
            printf("cod %d\n",Idx1[i].codControle);
            printf("off %d\n",Idx1[i].offset);
        }
        fclose(fp);
        fclose(aux);
        fclose(fpIdx1);
        return 1;
    }

    else{
        printf("\n2");
        printf("teste 1");

        i = fgetc(fpIdx1);
        while(i != -1){
            fseek(fpIdx1,(cont*sizeof(RegIdx))+4,0);
            fread(codControle,sizeof(int),1,fpIdx1);
            Idx1[fimIdx1].codControle = atoi(codControle);
            //puts(codControle);
            //printf("%d",atoi(codControle));
            fread(offset,sizeof(int),1,fpIdx1);
            Idx1[fimIdx1].offset = atoi(offset);
            fimIdx1++;
            cont++;
            i = fgetc(fpIdx1);
        }
        for(i=0;i<fimIdx1;i++){
            printf("cod %d\n",Idx1[i].codControle);
            printf("off %d\n",Idx1[i].offset);
        }
        fclose(fpIdx1);
        return 1;
    }
}

void salvaIdx()
{
    flagIdx1 = 1;

    FILE *fp;
    fp = fileOpenIdx(FileIdx1, &flagIdx1);

    int i;
    char controle[5],offset[5],flag[5];

    rewind(fp);
    itoa(flagIdx1,flag,10);
    fwrite(flag,sizeof(int),1,fp);
    //fclose(fp);
    //return 1;
    //fread(controle,sizeof(int),1,fp);
    for(i = 0; i < fimIdx1; i++){
        itoa(Idx1[i].codControle,controle,10);
        //puts(controle);
        fwrite(controle,sizeof(int),1,fp);
        itoa(Idx1[i].offset,offset,10);
        //puts(offset);
        fwrite(offset,sizeof(int),1,fp);
    }
    /*for(i = 0; i<fimIdx1;i++){
        printf("cod %d",Idx1[i].codControle);
        printf("offset %d",Idx1[i].offset);
    }*/
    rewind(fp);
    fread(flag,sizeof(int),1,fp);
    //printf("teste1%d",atoi(flag));
    fread(flag,sizeof(int),1,fp);
    //printf("teste2 %d",atoi(flag));
    fread(flag,sizeof(int),1,fp);
    //printf("teste3 %d",atoi(flag));
    fclose(fp);

    return 0;
}

int pesquisaKeyPrimariaAP1(int ch)
{
    int i, achou = 0,offset;
    char buffer[50];

    for(i = 0; i < fimIdx1; i++){
        if(ch == Idx1[i].codControle){
            achou = 1;
            offset = Idx1[i].offset;
            break;
        }
    }

    if(achou)
        return offset;
    else
        return -1;
}

void removeAp1(int offset, int ch)
{
    FILE *fp;
    char c = '!';
    char dispo[5];
    int achou;

    if(fimIdx1 == 0)
        return -1;

    fp = fileOpen(FileAP1,&headListAP1);
    printf("teste1");
    fseek(fp,offset*sizeof(char) + 4,0);
    //char *teste = "testando";
    //fwrite(teste,sizeof(char),strlen(teste),fp);
    //fread(dispo,sizeof(int),1,fp);
    //printf("\n\n disponivel %d\n\n",atoi(dispo));
    //puts(dispo);
    printf("teste2");
    fwrite(&c,sizeof(char),1,fp);
    printf("teste3");
    itoa(headListAP1,dispo,10);
    printf("teste4");
    fwrite(dispo,sizeof(int),1,fp);
    printf("teste5");
    headListAP1 = offset;
    rewind(fp);
    itoa(offset,dispo,10);
    fwrite(dispo,sizeof(int),1,fp);
    fclose(fp);

    fp = fileOpenIdx(FileIdx1,&flagIdx1);
    flagIdx1 = 0;
    rewind(fp);
    itoa(flagIdx1,dispo,10);
    fwrite(dispo,sizeof(int),1,fp);
    fclose(fp);

    int i = 0,j;
    for(i = 0; i < fimIdx1; i++){
        if(ch == Idx1[i].codControle){
            for(j = i; j < fimIdx1 - 1; j++){
                Idx1[j] = Idx1[i + 1];
                i++;
            }
            fimIdx1--;
            break;
        }
    }

    for(i = 0; i<fimIdx1;i++){
        printf("cod %d",Idx1[i].codControle);
        printf("offset %d",Idx1[i].offset);
    }
}

void imprime(int offset)
{
    char buffer[50],n[5];
    FILE *fp;

    fp = fileOpen(FileAP1,&headListAP1);
    fseek(fp,offset*sizeof(char),0);
    fseek(fp,5 * sizeof(char),1);
    pegaCampo(fp,buffer);
    pegaCampo(fp,buffer);
    pegaCampo(fp,buffer);
    printf("\nNome da Vacina: %s",buffer);
    pegaCampo(fp,buffer);
    printf("\nData da Vacinacao: %s",buffer);
    pegaCampo(fp,buffer);
    printf("\nResponsavel Pela Aplicacao: %s",buffer);

    fclose(fp);
}

int alteraVacina()
{
    int ch, op, achou, i,num, tam1,tam2,chAux;
    char vacina[50],data[50],resp[50],campoAux[50],s[10],controle[10],codPet[10];
    char completa = '\0';
    FILE *fp;
    FILE *aux;

    RegAP2 reg2;

    printf("\nDigite o Codigo do cachorro que deseja alterar: ");
    scanf("%d",&ch);

    fp = fileOpen(FileAP1,&headListAP1);
    aux = fileOpen(FileAP2,&headListAP2);

    achou = pesquisaKeyPrimariaAP2(ch, aux);
    if(achou == -1)
        return 0;

    for(i = 0; i < fimIdx1; i++){
        printf("teste1");
        fseek(fp,Idx1[i].offset*sizeof(char)+4,0);
        pegaCampo(fp,s);
        puts(s);
        pegaCampo(fp,s);
        puts(s);
        chAux = Idx1[i].codControle;
        if(ch == chAux){
            num = Idx1[i].offset;
            break;
        }
    }
    printf("ch %d",ch);

    fseek(aux,achou*sizeof(RegAP2) + 8,0);
    printf("teste4");
    fread(reg2.raca,50*sizeof(char),1,aux);
    puts(reg2.raca);
    fread(reg2.nomeCachorro,50*sizeof(char),1,aux);
    puts(reg2.nomeCachorro);
    printf("num %d",num);
    fseek(fp,num * sizeof(char) + 5,0);
    pegaCampo(fp,controle);//a funcao e chama 3 vezes
    pegaCampo(fp,codPet);//por conta de pular o caracter *
    pegaCampo(fp,vacina);//e depois pular o codigo do cachorro
    pegaCampo(fp,data);
    pegaCampo(fp,resp);

    printf("\nRaca do Cachorro: %s",reg2.raca);
    printf("\nNome do Cachorro: %s",reg2.nomeCachorro);
    printf("\nNome do Cachorro: %s",reg2.nomeCachorro);
    printf("\n\nVacina: %s",vacina);
    printf("\nData da Vacina: %s",data);
    printf("\nResponsavel pela vacina: %s",resp);


    do{
        printf("1- Nome da Vacina\n");
        printf("2- Data da Vacina\n");
        printf("3- Responsavel Pela Aplicacao\n");
        printf("4- Raca do Cachorro\n");
        printf("5- Nome do Cachorro\n");

        printf("Digite o Campo que Deseja Alterar: ");
        scanf("%d",&op);
    }while(op < 0 || op > 5);

    switch(op){
        case 1:
            tam1 = strlen(vacina);
            fflush(stdin);
            printf("\nDigite a Nova Vacina: ");
            gets(vacina);
            tam2 = strlen(vacina);
            if(tam2 > tam1){
                removeAp1(num,chAux);
                RegAP1 novo;

                novo.codigoControle = atoi(controle);
                novo.codigoCachorro = ch;
                strcpy(novo.vacina,vacina);
                strcpy(novo.dataVacina,data);
                strcpy(novo.respVacina,resp);

                insereAP1(&novo);
            }
            else{
                fseek(fp,num * sizeof(char) + 5,0);
                pegaCampo(fp,campoAux);
                fseek(fp,strlen(codPet)*sizeof(char)+1,1);
                fwrite(vacina,sizeof(char),strlen(vacina),fp);
                fwrite(&completa,sizeof(char),1,fp);
                fclose(fp);
            }
            break;
        case 2:
            tam1 = strlen(data);
            fflush(stdin);
            printf("\nDigite a Nova Data da Vacina: ");
            gets(data);
            tam2 = strlen(data);
            if(tam2 > tam1){
                removeAp1(num,chAux);
                RegAP1 novo;

                novo.codigoControle = atoi(controle);
                novo.codigoCachorro = ch;
                strcpy(novo.vacina,vacina);
                strcpy(novo.dataVacina,data);
                strcpy(novo.respVacina,resp);


                insereAP1(&novo);
            }
            else{
                fseek(fp,num * sizeof(char) + 5,0);
                fseek(fp,(strlen(codPet) + strlen(vacina))*sizeof(char),1);
                fwrite(vacina,sizeof(char),strlen(data),fp);
                fclose(fp);
            }
            break;
        case 3:
            tam1 = strlen(resp);
            fflush(stdin);
            printf("\nDigite a Nova Data da Vacina: ");
            gets(resp);
            tam2 = strlen(resp);
            if(tam2 > tam1){
                removeAp1(num,chAux);
                RegAP1 novo;

                novo.codigoControle = atoi(controle);
                novo.codigoCachorro = ch;
                strcpy(novo.vacina,vacina);
                strcpy(novo.dataVacina,data);
                strcpy(novo.respVacina,resp);


                insereAP1(&novo);
            }
            else{
                fseek(fp,num * sizeof(char) + 5,0);
                fseek(fp,(strlen(codPet) + strlen(vacina)+strlen(data))*sizeof(char),1);
                fwrite(vacina,sizeof(char),strlen(resp),fp);
                fclose(fp);
            }
            break;
        case 4:
            fseek(fp,achou*sizeof(RegAP2),1);
            fseek(fp,sizeof(int),1);
            fflush(stdin);
            printf("Digite a Nova Raca do Cachorro: ");
            gets(reg2.raca);
            fwrite(reg2.raca,50*sizeof(char),1,fp);
            break;
        case 5:
            fseek(fp,achou*sizeof(RegAP2),1);
            fseek(fp,sizeof(int),1);
            fseek(fp,50*sizeof(char),1);
            fflush(stdin);
            printf("Digite o Novo Nome do Cachorro: ");
            gets(reg2.nomeCachorro);
            fwrite(reg2.nomeCachorro,50*sizeof(char),1,fp);
            break;
    }
    //system("cls");
    cabecalho();
    rewind(aux);

    fseek(aux,achou*sizeof(RegAP2) + 8,0);
    printf("teste4");
    fread(reg2.raca,50*sizeof(char),1,aux);
    puts(reg2.raca);
    fread(reg2.nomeCachorro,50*sizeof(char),1,aux);
    puts(reg2.nomeCachorro);

    printf("Raca do Cachorro: %s",reg2.raca);
    printf("Nome do Cachorro: %s",reg2.nomeCachorro);
    printf("Dados do Cadastro Ja Alterado: \n");

    return 1;
}

void compactaArquivo()
{
    FILE *fp;
    FILE *aux;
    int i, offset, c, result;
    char tam[5],headAux[5],buffer[100];

    fp = fileOpen(FileAP1,&headListAP1);

    /*if(!(feof(fp))){
        fclose(fp);
        fclose(aux);
        return 1;
    }*/

    if((aux = fopen("arqAux.bin","w+b")) == NULL)
        printf("erro");

    //rewind(fp);
    //fread(head,sizeof(int),1,fp);
    c = -1;
    itoa(c,headAux,10);
    fwrite(headAux,sizeof(int),1,aux);

    for(i = 0; i < fimIdx1; i++){
        fseek(fp,Idx1[i].offset*sizeof(char),0);
        fread(tam,sizeof(int),1,fp);
        fwrite(tam,sizeof(int),1,aux);
        fread(buffer,sizeof(char),atoi(tam),fp);
        fwrite(buffer,sizeof(char),atoi(tam),aux);
    }

    fclose(fp);
    fclose(aux);

    if(remove(FileAP1) != 0)
        printf("\nErro ao Apagar o Arquivo\n");

    result = rename("arqAux.bin",FileAP1);
    if (result != 0)
        printf("\nErro ao Renomear o Arquivo\n");
}

void criaVetorIdxSec()
{
    char tam[5],codControl[5],codDog[5],vacina[50],offset[5],offsetProx[5];
    int ultimo = -1,i,j,pos,achou = 0;

    FILE *fp;
    FILE *fpIdx2;
    FILE *fpIdxAux;

    fp = fileOpen(FileAP1,&headListAP1);
    fpIdx2 = fopen(FileIdx2,"w+b");
    fpIdxAux = fopen(FileIdx2Cod,"w+b");

    if(fpIdx2 == NULL)
        printf("Erro ao Abrir o Arquivo!\n");

    if(fpIdxAux == NULL)
        printf("Erro ao Abrir o Arquivo!\n");

    for(i = 0; i < fimIdx1; i++){
        fseek(fp,Idx1[i].offset*sizeof(char),0);
        fread(tam,sizeof(int),1,fp);
        fseek(fp,sizeof(char),1);
        pegaCampo(fp,codControl);
        pegaCampo(fp,codDog);
        pegaCampo(fp,vacina);

        if(i == 0){
            fwrite(codControl,sizeof(int),1,fpIdxAux);
            itoa(ultimo,offsetProx,10);
            fwrite(offsetProx,sizeof(int),1,fpIdxAux);
            strcpy(Idx2[fimIdx2].vacina,vacina);
            Idx2[fimIdx2].offset = 0;
            fimIdx2++;
        }
        else{
            for(j = 0; j < fimIdx2; j++){
                if((strcmp(Idx2[j].vacina,vacina)) == 0){
                    achou = 1;
                    break;
                }
            }
            if(achou){
                fseek(fpIdxAux,0,SEEK_END);
                pos = ftell(fpIdxAux);
                //fseek(fpIdxAux,Idx2[j].offset*sizeof(char),0);
                itoa(Idx2[j].offset,offsetProx,10);
                while(atoi(offsetProx) != -1){
                    fseek(fpIdxAux,atoi(offsetProx)*sizeof(char),0);
                    fseek(fpIdxAux,sizeof(int),1);
                    fread(offsetProx,sizeof(int),1,fpIdxAux);
                }
                fseek(fpIdxAux,-(sizeof(int)),1);
                itoa(pos,offsetProx,10);
                fwrite(offsetProx,sizeof(int),1,fpIdxAux);
                fseek(fpIdxAux,0,SEEK_END);
                fwrite(codControl,sizeof(int),1,fpIdxAux);
                itoa(ultimo,offsetProx,10);
                fwrite(offsetProx,sizeof(int),1,fpIdxAux);
                achou = 0;
            }
            else{
                fseek(fpIdxAux,0,SEEK_END);
                pos = ftell(fpIdxAux);
                fwrite(codControl,sizeof(int),1,fpIdxAux);
                itoa(ultimo,offsetProx,10);
                fwrite(offsetProx,sizeof(int),1,fpIdxAux);
                strcpy(Idx2[fimIdx2].vacina,vacina);
                Idx2[fimIdx2].offset = pos;
                fimIdx2++;
            }
        }
    }

    for(i = 0; i <= fimIdx2; i++){
        fwrite(Idx2[i].vacina,sizeof(char),strlen(vacina),fpIdx2);
        puts(Idx2[i].vacina);
        itoa(Idx2[i].offset,offset,10);
        fwrite(offset,sizeof(int),1,fpIdx2);
        printf("  %d\n",Idx2[i].offset);
    }
    printf("  %d\n",fimIdx2);
    fclose(fpIdx2);
    fclose(fpIdxAux);
    fclose(fp);
}
