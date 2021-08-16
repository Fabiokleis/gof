#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// verificando e definindo tempo para cada plataforma...
#ifdef  __unix__
    #include <unistd.h>
    #define U_time 100000
#elif defined(_WIN32)
    #define OS_Windows
    #include <windowns.h>
    #define 100
#endif

#define ORG 'X'
#define VAZ '.'
#define RUST '%'
#define TAM 101
#define LINESIZE 1024

typedef struct {
    char nomeJogo[TAM];
    int ciclosVida;
    int dim1, dim2;
    char **m;
} Tab;

void alocaMatriz(Tab *tabuleiro);
void limpaMatriz(Tab *tabuleiro);
void imprimeMatriz(Tab *tabuleiro);
void copiaMatriz(Tab *Atabuleiro, Tab *tabuleiro);
void desalocaMatriz(Tab *tabuleiro);
void iniciaMatriz(Tab *tabuleiro);
void menuInicJogo(Tab *tabuleiro);
void jogaJogoVida(Tab *tabuleiro, int *delta);
void novaMatriz(Tab *Atabuleiro, Tab *tabuleiro);
void matrizAleatoria(Tab *tabuleiro);
void rust(Tab *tabuleiro, int seed);

int main(){
    Tab tabuleiro;
    int nl, nc, i, flag, delta=0;
    do{
        printf("dimensoes do tabuleiro\n");
        printf("nl e nc: ");
        scanf("%d %d", &nl, &nc);
        tabuleiro.dim1 = nl;
        tabuleiro.dim2 = nc; 

        alocaMatriz(&tabuleiro);
    
        menuInicJogo(&tabuleiro);
    
        jogaJogoVida(&tabuleiro, &delta);
 
        desalocaMatriz(&tabuleiro);

        printf("começar um novo jogo da vida?(1/0): ");
        scanf("%d", &flag);
        delta++;
   }while(flag);
    printf("o ciclo encerrou...\n");
    return 0;
}

void alocaMatriz(Tab *tabuleiro){
    int i;

    tabuleiro->m = (char**) malloc(tabuleiro->dim1 * sizeof(char*));

    for(i=0; i < tabuleiro->dim1; i++){
        tabuleiro->m[i] = (char*) malloc(tabuleiro->dim2);
    }

}

void desalocaMatriz(Tab *tabuleiro){
    int i;
    for(i=0; i < tabuleiro->dim1; i++){
        free(tabuleiro->m[i]);
    }
    free(tabuleiro->m);
}


void imprimeMatriz(Tab *tabuleiro){
    int i, j;

    for(i=0; i < tabuleiro->dim1; i++){
        for(j=0; j < tabuleiro->dim2; j++){
            printf("%c", tabuleiro->m[i][j]);
        }
        printf("\n");
    }
}

void limpaMatriz(Tab *tabuleiro){
    int i, j;
    for(i=0; i < tabuleiro->dim1; i++){
        for(j=0; j < tabuleiro->dim2; j++){
            tabuleiro->m[i][j] = VAZ;
        }
    }
}

void copiaMatriz(Tab *Atabuleiro, Tab *tabuleiro){
    int i, j;
    for(i=0; i < Atabuleiro->dim1; i++){
        for(j=0; j < Atabuleiro->dim2; j++){
            Atabuleiro->m[i][j] = tabuleiro->m[i][j];
        }
    }
}
// le e coloca as posicoes do .csv na matriz de forma "aleatoria".
void iniciaMatriz(Tab *tabuleiro){
    FILE *fp;
    int i, n, r, r2, j=0;
    char line[LINESIZE];
    
    fp = fopen(tabuleiro->nomeJogo, "r");

    if(fp == NULL){
        printf("ERROR\n");
        exit(1);
    }
    
    limpaMatriz(tabuleiro);
        
    srand(time(NULL));
    r = rand()%(tabuleiro->dim1/2);
    r2 = rand()%(tabuleiro->dim2/2);

    while(fgets(line, LINESIZE, fp) != NULL){
        for(i=0; line[i] != '\0'; i++){
            if(!((int)line[i] == 44 || line[i+1] == '\0')){
                n = atoi(&line[i])-1;
                tabuleiro->m[j+r][n+r2] = ORG;
            }
        }
        j++;
    }
    fclose(fp);
}

// coloca todas as posiçoes de forma "aleatoria".
void matrizAleatoria(Tab *tabuleiro){
    int i, j, r;
    srand(time(NULL));
    limpaMatriz(tabuleiro);
    for(i=0; i < tabuleiro->dim1; i++){
        for(j=0; j < tabuleiro->dim2; j++){
            r = rand()%2;
            if(r){
                tabuleiro->m[i][j] = ORG;
            }else{
                tabuleiro->m[i][j] = VAZ;
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
                strcpy(tabuleiro->nomeJogo, "bloco.csv");
                iniciaMatriz(tabuleiro);
            }else if(opt == 'b'){
                strcpy(tabuleiro->nomeJogo, "et.csv");
                iniciaMatriz(tabuleiro);
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(tabuleiro);
            }
            break;
        case 2:
            printf("a) blinker\nb) sapo\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                strcpy(tabuleiro->nomeJogo, "blinker.csv");
                iniciaMatriz(tabuleiro);
            }else if(opt == 'b'){
                strcpy(tabuleiro->nomeJogo, "sapo.csv");
                iniciaMatriz(tabuleiro);
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(tabuleiro);
            }
            break;
        case 3:
            printf("a) glider\nb) lightweight spaceship\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                strcpy(tabuleiro->nomeJogo, "glider.csv");
                iniciaMatriz(tabuleiro);
            }else if(opt == 'b'){
                strcpy(tabuleiro->nomeJogo, "lwss.csv");
                iniciaMatriz(tabuleiro);
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(tabuleiro);
            }
            break;
        case 4:
            printf("nome do arquivo: ");
            scanf("%s", &fileName);
            strcpy(tabuleiro->nomeJogo, fileName);
            iniciaMatriz(tabuleiro);
            break;
        default:
            printf("opcao invalida!, carregando a default\n");
            matrizAleatoria(tabuleiro);
            break;
    }
    imprimeMatriz(tabuleiro);
    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo..."); 
    while(getchar() !='\n'); getchar();
}

void novaMatriz(Tab *Atabuleiro, Tab *tabuleiro){
    int i, j, s, k, u;
    
    for(i=0; i < Atabuleiro->dim1; i++){
        for(j=0; j < Atabuleiro->dim2; j++){
            s=0;
            // variando de -1 até 1 tirando o 0 que é o próprio elemento.
            for(u=-1; u < 2; u++){
                for(k=-1; k < 2; k++){
                   if(u == 0 && k == 0){
                       continue;
                   }
                   // para eliminar a tentativa de ler espaços fora da alocação da matriz.
                   if(i-u < 0 || i-u == Atabuleiro->dim1 || j-k < 0 || j-k == Atabuleiro->dim2){
                       continue;
                   }
                   // verificando se está vivo um vizinho.
                   if(Atabuleiro->m[i-u][j-k] == ORG){
                       s++;
                    }
                }
            }
            
            if(Atabuleiro->m[i][j] == ORG && s < 2){
                tabuleiro->m[i][j] = VAZ;
            }else if(Atabuleiro->m[i][j] == ORG && s > 3){
                tabuleiro->m[i][j] = VAZ;   
            }else if(Atabuleiro->m[i][j] == ORG && (s == 3 || s == 2)){ 
                tabuleiro->m[i][j] = ORG;
            }else if(Atabuleiro->m[i][j] == VAZ && s == 3){
                tabuleiro->m[i][j] = ORG;
            }else if(Atabuleiro->m[i][j] != RUST){
                tabuleiro->m[i][j] = VAZ;
            }
        }
    }
}

// cria pontos de "ferrugem" no tabuleiro.
void rust(Tab *tabuleiro, int seed){
    int c, l;
    srand(time(NULL));

    c = seed+rand()%(tabuleiro->dim2-seed);
    l = seed+rand()%(tabuleiro->dim1-seed);
 
    tabuleiro->m[l][c] = RUST;
}

void jogaJogoVida(Tab *tabuleiro, int *delta){
    Tab aux;
    int i, k, j;

    printf("numero de ciclos: ");
    scanf("%d", &k);
    aux.dim1 = tabuleiro->dim1;
    aux.dim2 = tabuleiro->dim2;
    tabuleiro->ciclosVida = k;
    alocaMatriz(&aux);
    
    for(i=0; i < tabuleiro->ciclosVida; i++){
        // windows ou qualquer outro sistema unix like...
        #ifdef OS_windows
            Sleep(W_time);
            system("cls");
        #else
            usleep(U_time);
            system("clear");
        #endif

        novaMatriz(tabuleiro, &aux);

        // a partir da metade dos ciclos de vida menos a variação inicia o processo de rust
        if(i >= tabuleiro->ciclosVida/2-*delta){

            // a cada ciclo é gerado pontos de "ferrugem" equivalente ao número da variação
            for(j=0; (j < *delta) && (*delta < tabuleiro->dim1 && *delta < tabuleiro->dim2); j++){
                rust(&aux, j);
            }
        }
        
        copiaMatriz(tabuleiro, &aux);

        imprimeMatriz(tabuleiro);
    }    

    desalocaMatriz(&aux);
}
