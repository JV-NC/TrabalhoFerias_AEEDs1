#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#define TAB 52  //quantidade de casas do tabuleiro



typedef struct{
    int peao[4],casa_esp[4];
    int cont[4];
    int dc[3],passou;
}jogador;

int start();
int roll();
void limpar(jogador j[],int c);
void status(jogador j[],int c,int p);
int iniciar_peao(jogador j[],int c,int p,int d);
int chegada(jogador j[],int c,int p,int qpeao,int d);
int select(jogador j[],int c,int p,int d);
int captura(jogador j[],int c,int p,int qpeao);
void tabuleiro(jogador j[],int c);

int main()
{
    setlocale(LC_ALL,"Portuguese");
    srand(time(NULL));

    int q;

    q=start();

    jogador *j;

    j=(jogador*)calloc(q,sizeof(jogador));

    printf("\n%d\n",q);

    limpar(j,q);

    tabuleiro(j,q);

    return 0;
}

int start(){
    int player;

    printf("**************************************************\n                       LUDO                       \n**************************************************\n\n");
    printf("Informe a quantidade de jogadores (2-4): ");
    scanf("%d",&player);
    if(player<2 || player>4){
        printf("quantidade inválida\n");
        exit(1);
    }
    return player;
}

int roll(){
    int d;
    d=1+(rand()%6);
    return d;
}

void status(jogador j[],int c,int p){
    int i;
	for(i=0;i<4;i++){
        if(j[p].casa_esp[i]==-1){
            printf("Peao %d - Casa %d\n",i+1,j[p].peao[i]);
        }
        else if(j[p].casa_esp[i]==6){
            printf("Peao %d - Completou\n",i+1);
        }
        else{
            printf("Peao %d - Casa Colorida %d\n",i+1,j[p].casa_esp[i]);
        }
    }
}

void limpar(jogador j[],int c){
    int i,k;
	for(i=0;i<c;i++){
        for(k=0;k<4;k++){
            j[i].peao[k]=-1;
            j[i].cont[k]=56;
            j[i].casa_esp[k]=-1;
        }
    }
}

int iniciar_peao(jogador j[],int c,int p,int d){
    int check=0,di,qpeao,i;

    for(i=0;i<4;i++){
        if(j[p].peao[i]==-1 && d==6 && check==0){
            switch(p){
                case 0:
                    j[p].peao[i]=0;
                break;

                case 1:
                    j[p].peao[i]=13;
                break;

                case 2:
                    j[p].peao[i]=26;
                break;

                case 3:
                    j[p].peao[i]=39;
                break;
            }
            qpeao=i;
            printf("\nPeao do jogador %d iniciado.\n",p+1);
            j[p].dc[2]=j[p].dc[1];j[p].dc[1]=j[p].dc[0];
            di=roll();
            j[p].dc[0]=di;
            j[p].peao[i]+=di;
            j[p].cont[i]-=di;
            printf("Peao %d andou %d casas\n",i+1,di);
            system("pause");
            printf("\n");
            check=1;
        }
    }
    if(check==0){
        printf("Não foi possivel iniciar um peao\n");
        system("pause");
        qpeao=0;
    }
    return qpeao;
}

int chegada(jogador j[],int c,int p,int qpeao,int d){
    int dif;
    dif=j[p].cont[qpeao-1]-d;
    if(j[p].cont[qpeao-1]<6){
        j[p].casa_esp[qpeao-1]=6-j[p].cont[qpeao-1];
        j[p].peao[qpeao-1]=-99;
        printf("Peão %d do jogador %d entrou na linha de sua respectiva cor\nFalta %d casas\n",qpeao,p+1,j[p].cont[qpeao-1]);
        if(j[p].cont[qpeao-1]<d && j[p].cont[qpeao-1]!=0){
            j[p].cont[qpeao-1]-=d-(2*dif);
            j[p].casa_esp[qpeao-1]+=d+(2*dif); //casas diferentes , travar em 0 , corrigir casas
        }
    }

    else{
        j[p].peao[qpeao-1]+=d;
                j[p].cont[qpeao-1]-=d;
                j[p].dc[2]=j[p].dc[1];j[p].dc[1]=j[p].dc[0];j[p].dc[0]=d;
            printf("Peao %d andou %d casas\n",qpeao,d);
    }
    return 1;
}

int select(jogador j[],int c,int p,int d){
    int op,qpeao,check=0;
    while(check==0){
        printf("Digite:\n\t1 para iniciar um peao\n\t2 para mover um peao que ja esta no tabuleiro\n");
        status(j,c,p);
        scanf("%d",&op);
        if(op==1){
            qpeao=iniciar_peao(j,c,p,d);
            check=1;
        }
        else if(op==2){

            printf("\nQual peao deseja mover?\nDigite 0 caso deseje retornar.\n");
            scanf("%d",&qpeao);
            if(j[p].peao[qpeao-1]==-1){
                printf("Esse peão ainda não foi iniciado, selecione outro.\n");
            }
            else{
                check=chegada(j,c,p,qpeao,d);
            }
        }
    }
    return qpeao;
}

int captura(jogador j[],int c,int p,int qpeao){
    int h,i,k,S[9]={-1,0,8,13,21,26,34,39,47};

    for(h=0;h<9;h++){
        if(j[p].peao[qpeao-1]==S[h]){
            printf("Zona segura!\n");
            return 0;
        }
    }
    for(i=0;i<c;i++){
        for(k=0;k<4;k++){
            if(j[p].peao[qpeao-1]==j[i].peao[k] && p!=i){ //n quer capturar
                j[i].peao[k]=-1;
                printf("Peao %d do jogador %d foi capturado!\n",k+1,i+1);
                return 1;
            }
        }
    }
    return 0;
}

void tabuleiro(jogador j[],int c){
    int win=0,d,qpeao;

    while(win==0){
    	int i;
        for(i=0;i<c;i++){
            if(j[i].dc[0]==6 && j[i].dc[1]==6 && j[i].dc[2]==6 && j[i].passou==0){
                printf("Jogador %d rolou três 6 seguidos, perdeu a vez\n",i);
                j[i].passou=1;
            }
            if(j[i].passou==0){
                printf("Vez do Jogador %d:\n",i+1);
                d=roll();
                printf("Dado=%d\n",d);
                qpeao=select(j,c,i,d);
                captura(j,c,i,qpeao);
                printf("\n");
                if(j[i].peao[qpeao-1]>51){
                    j[i].peao[qpeao-1]-=52;
                }
                if(j[i].cont[0]==0 && j[i].cont[1]==0 && j[i].cont[2]==0 && j[i].cont[3]==0){
                    printf("Todos os 4 peãos alcançaram a linha de chegada!\nVitória do Jogador %d\n",i+1);
                    win=1;
                }
            }
            j[i].passou=0;
        }
    }

}
