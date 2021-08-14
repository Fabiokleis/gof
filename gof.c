#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef  __unix__
    #include <unistd.h>
#elif defined(_WIN32)
    #define OS_Windows
    #include <windowns.h>
#endif

#define ORG '*'
#define VAZ '.'
#define TAM 101
#define LINESIZE 1024

void alocaMatriz(char ***mt, int nL, int nC);
void limpaMatriz(char **m, int nL, int nC);
void imprimeMatriz(char **mat, int nL, int nC);
void copiaMatriz(char **mat, char **mc, int nL, int nC);
void desalocaMatriz(char ***mt, int nL);
void iniciaMatriz(char ***mt, int nL, int nC, char fileName[]);
void menuInicJogo(char **mat, int nL, int nC);
void jogaJogoVida(char **m, int nL, int nC);
void novaMatriz(char **mA, char ***mC, int nL, int nC);
void matrizAleatoria(char ***mat, int nL, int nC);

typedef struct tab{
    char nomeJogo[TAM];
    int ciclosVida;
    int dim1, dim2;
    char **m;
} Tab;

int main(){
    char **mat; 
    int nl, nc, i;

    printf("nl e nc: ");
    scanf("%d %d", &nl, &nc);

    alocaMatriz(&mat, nl, nc);

    menuInicJogo(mat, nl, nc);

    jogaJogoVida(mat, nl, nc);

    desalocaMatriz(&mat, nl);

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


void imprimeMatriz(char **mat, int nL, int nC){
    int i, j;

    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            printf("%c", mat[i][j]);
        }
        printf("\n");
    }
}

void limpaMatriz(char **m, int nL, int nC){
    int i,j;
    for(i=0;i<nL;i++){
        for(j=0;j<nC;j++){
            m[i][j]=VAZ;
        }
    }
}

void copiaMatriz(char **mat, char **mc, int nL, int nC){
    int i, j;
    for(i=0; i < nL; i++){
        for(j=0; j < nC; j++){
            mat[i][j] = mc[i][j];
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
    
    limpaMatriz(*mt, nL, nC);
        
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
    limpaMatriz(*mat, nL, nC);
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

void menuInicJogo(char **mat, int nL, int nC){
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
                iniciaMatriz(&mat, nL, nC, "bloco.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&mat, nL, nC, "et.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&mat, nL, nC);
            }
            break;
        case 2:
            printf("a) blinker\nb) sapo\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                iniciaMatriz(&mat, nL, nC, "blinker.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&mat, nL, nC, "sapo.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&mat, nL, nC);
            }
            break;
        case 3:
            printf("a) glider\nb) lightweight spaceship\n");
            printf("opcao: ");
            scanf(" %c", &opt);
            if(opt == 'a'){
                iniciaMatriz(&mat, nL, nC, "glider.csv");
            }else if(opt == 'b'){
                iniciaMatriz(&mat, nL, nC, "lwss.csv");
            }else{
                printf("opcao invalida!, carregando a default\n");
                matrizAleatoria(&mat, nL, nC);
            }
            break;
        case 4:
            printf("nome do arquivo: ");
            scanf("%s", &fileName);
            iniciaMatriz(&mat, nL, nC, fileName);
            break;
        default:
            printf("opcao invalida!, carregando a default\n");
            matrizAleatoria(&mat, nL, nC);
            break;
    }
    imprimeMatriz(mat, nL, nC);
    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo..."); 
    while(getchar() !='\n'); getchar();


}

void novaMatriz(char **mA, char ***mC, int nL, int nC){
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
                   if(mA[i-u][j-k] == ORG){
                       s++;
                    }
                }
            }
            if(mA[i][j] == ORG && s < 2){
                (*mC)[i][j] = VAZ;
            }else if(mA[i][j] == ORG && s > 3){
                (*mC)[i][j] = VAZ;
            }else if(mA[i][j] == ORG && (s == 3 || s == 2)){
                (*mC)[i][j] = ORG;
            }else if(mA[i][j] == VAZ && s == 3){
                (*mC)[i][j] = ORG;
            }else{
                (*mC)[i][j] = VAZ;
            }
        }
    }
}


void jogaJogoVida(char **m, int nL, int nC){
    char **aux;
    int k;
    int i;

    printf("numero de ciclos: ");
    scanf("%d", &k);
    alocaMatriz(&aux, nL, nC);

    for(i=0; i < k; i++){
        novaMatriz(m, &aux, nL, nC);
        copiaMatriz(m, aux, nL, nC);
        imprimeMatriz(m, nL, nC);

        // windows ou qualquer outro sistema unix...
        #ifdef OS_windows
            Sleep(200);
            system("cls");
        #else
            usleep(200000);
            system("clear");
        #endif
    }

    desalocaMatriz(&aux, nL);
}

