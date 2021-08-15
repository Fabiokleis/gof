#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef  __unix__
    #include <unistd.h>
#elif defined(_WIN32)
    #define OS_Windows
    #include <windowns.h>
#endif

#define ORG '#'
#define VAZ '.'
#define TAM 101
#define LINESIZE 1024

typedef struct {
    char nomeJogo[TAM];
    int ciclosVida;
    int dim1, dim2;
    char **m;
} Tab;

void alocaMatriz(char ***mt, int nL, int nC);
void limpaMatriz(char ***m, int nL, int nC);
void imprimeMatriz(char ***mat, int nL, int nC);
void copiaMatriz(char ***mat, char ***mc, int nL, int nC);
void desalocaMatriz(char ***mt, int nL);
void iniciaMatriz(char ***mt, int nL, int nC, char fileName[]);
void menuInicJogo(Tab *tabuleiro);
void jogaJogoVida(Tab *tabuleiro, int *deltaX, int *deltaY);
void novaMatriz(char ***mA, char ***mC, int nL, int nC);
void matrizAleatoria(char ***mat, int nL, int nC);
void rust(Tab *tabuleiro, int deltaX, int deltaY);

int main(){
    Tab tabuleiro;
    int nl, nc, i, flag, deltaX=0, deltaY=0;
    do{
        printf("nl e nc: ");
        scanf("%d %d", &nl, &nc);
        tabuleiro.dim1 = nl;
        tabuleiro.dim2 = nc; 

        alocaMatriz(&tabuleiro.m, tabuleiro.dim1, tabuleiro.dim2);
    
        menuInicJogo(&tabuleiro);
    
        jogaJogoVida(&tabuleiro, &deltaX, &deltaY);
        deltaX++;
        deltaY++;
        
        desalocaMatriz(&tabuleiro.m, tabuleiro.dim1);
        printf("começar um novo jogo da vida?(1/0): ");
        scanf("%d", &flag);
    }while(flag);
    printf("o ciclo encerrou...\n");
    return 0;
}

void alocaMatriz(char ***mt, int nL, int nC){
    int i;

    *mt = (char**) malloc(nL*sizeof(char*));

    for(i=0; i < nL; i++){
        (*mt)[i] = (char*) malloc(nC);
    }

}

void desalocaMatriz(char ***mt, int nL){
    int i;
    for(i=0; i < nL; i++){
        free((*mt)[i]);
    }
    free(*mt);
}


void imprimeMatriz(char ***mat, int nL, int nC){
    int i, j;

    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            printf("%c", (*mat)[i][j]);
        }
        printf("\n");
    }
}

void limpaMatriz(char ***m, int nL, int nC){
    int i, j;
    for(i=0;i<nL;i++){
        for(j=0;j<nC;j++){
            (*m)[i][j] = VAZ;
        }
    }
}

void copiaMatriz(char ***mat, char ***mc, int nL, int nC){
    int i, j;
    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            (*mat)[i][j] = (*mc)[i][j];
        }
    }
}
// le e coloca as posicoes do .csv na matriz de forma "aleatoria".
void iniciaMatriz(char ***mt, int nL, int nC, char fileName[]){
    FILE *fp;
    int i, n, r, r2, j=0;
    char line[LINESIZE];
    
    fp = fopen(fileName, "r");

    if(fp == NULL){
        printf("ERROR\n");
        exit(1);
    }
    
    limpaMatriz(mt, nL, nC);
        
    srand(time(NULL));
    r = rand()%(nL/2);
    r2 = rand()%(nC/2);
    while(fgets(line, LINESIZE, fp) != NULL){
        for(i=0; line[i] != '\0'; i++){
            if(!((int)line[i] == 44 || line[i+1] == '\0')){
                n = atoi(&line[i])-1;
                (*mt)[j+r][n+r2] = ORG;
            }
        }
        j++;
    }
    fclose(fp);
}

// coloca todas as posiçoes de forma "aleatoria".
void matrizAleatoria(char ***mat, int nL, int nC){
    int i, j, r;
    srand(time(NULL));
    limpaMatriz(mat, nL, nC);
    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            r = rand()%2;
            if(r){
                (*mat)[i][j] = ORG;
            }else{
                (*mat)[i][j] = VAZ;
            }
        }
    }
}

void menuInicJogo(Tab *tabuleiro){
    int opcao;
    char opt;
    char fileName[20];
    
    printf("1. Vidas paradas\n");
    printf("2. Osciladores\n");
    printf("3. Naves espaciais\n");
    printf("4. Arquivo .csv 20 chars\n");
    printf("default aleatoria... qualquer tecla.\n");
    printf("opcao: ");
    scanf("%d", &opcao);
    
    switch(opcao){
        case 1:
            printf("a) vida eterna 1\nb) vida eterna 2\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "bloco.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "et.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
            }
            break;
        case 2:
            printf("a) blinker\nb) sapo\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "blinker.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "sapo.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
            }
            break;
        case 3:
            printf("a) glider\nb) lightweight spaceship\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "glider.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, "lwss.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
            }
            break;
        case 4:
            printf("nome do arquivo: ");
            scanf("%s", &fileName);
            iniciaMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2, fileName);
            break;
        default:
            printf("opcao invalida!, carregando a default\n");
            matrizAleatoria(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
            break;
    }
    imprimeMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo..."); 
    while(getchar() !='\n'); getchar();
}

void novaMatriz(char ***mA, char ***mC, int nL, int nC){
    int i, j, s, k, u;
    
    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            s=0;
            // variando de -1 até 1 tirando o 0 que é o próprio elemento.
            for(u=-1; u < 2; u++){
                for(k=-1; k < 2; k++){
                   if(u == 0 && k == 0){
                       continue;
                   }
                   // para eliminar a tentativa de ler espaços fora da alocação da matriz.
                   if(i-u < 0 || i-u == nL || j-k < 0 || j-k == nC){
                       continue;
                   }
                   // verificando se está vivo um vizinho.
                   if((*mA)[i-u][j-k] == ORG){
                       s++;
                    }
                }
            }
            if((*mA)[i][j] == ORG && s < 2){
                (*mC)[i][j] = VAZ;
            }else if((*mA)[i][j] == ORG && s > 3){
                (*mC)[i][j] = VAZ;
            }else if((*mA)[i][j] == ORG && (s == 3 || s == 2)){
                (*mC)[i][j] = ORG;
            }else if((*mA)[i][j] == VAZ && s == 3){
                (*mC)[i][j] = ORG;
            }else{
                (*mC)[i][j] = VAZ;
            }
        }
    }
}


void jogaJogoVida(Tab *tabuleiro, int *deltaX, int *deltaY){
    char **aux;
    int i, k;

    printf("numero de ciclos: ");
    scanf("%d", &k);
    tabuleiro->ciclosVida = k;
    alocaMatriz(&aux, tabuleiro->dim1, tabuleiro->dim2);
    
    for(i=0; i < tabuleiro->ciclosVida; i++){
        // windows ou qualquer outro sistema unix like...
        #ifdef OS_windows
            Sleep(200);
            system("cls");
        #else
            usleep(200000);
            system("clear");
        #endif
        novaMatriz(&tabuleiro->m, &aux, tabuleiro->dim1, tabuleiro->dim2);
        copiaMatriz(&tabuleiro->m, &aux, tabuleiro->dim1, tabuleiro->dim2);
        imprimeMatriz(&tabuleiro->m, tabuleiro->dim1, tabuleiro->dim2);
    }    
    desalocaMatriz(&aux, tabuleiro->dim1);
}

