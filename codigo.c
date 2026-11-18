/* INTEGRANTES: Nykolas José Köpp, José Vitor Mader, Carla Canalle Mundel, Luís Carlos Martins Junior, Isadora L. Brandt*/
#include <stdio.h>
#include <string.h>

#define N 5      /* numero de predios */
#define INF 9999 /* "infinito": ainda nao ha caminho */


char nomes[N][20] = {"Bloco A", "Bloco B", "Laboratorio", "Refeitorio", "Biblioteca"};

int grafo[N][N];   /* matriz de adjacencia (distancias; 0 = sem ligacao direta) */
int alcanca[N][N]; /* matriz de alcancabilidade (1 = existe caminho, 0 = nao) */
int distancia[N][N];


void criarGrafo(void);
void listarPredios(void);
int lerPredio(char *mensagem);


void exibirMapa(void);                                                /* RF01 - Nykolas */
void dijkstra(int origem, int dist[], int anterior[]);                /* RF02 - Jose */
void menorRota(void);                                                 /* RF02 - Jose */
void verificarConexao(void);                                          /* RF03 - Carla */
void floydWarshall(void);                                             /* RF04 - Luis */
void exibirMatriz(void);                                              /* RF04 - Luis */
void exibirRota(int origem, int destino, int dist[], int anterior[]); /* RF05 - Isa */


int main(void)
{
    char texto[50];
    int opcao;

    criarGrafo();      
    floydWarshall();   

    do {
        printf("\n==========================================\n");
        printf("|          SISTEMA DE ROTAS DO CAMPUS       |\n");
        printf("============================================\n");
        printf("|1 - Exibir mapa do campus                  |\n");
        printf("|2 - Encontrar menor rota entre dois predios|\n");
        printf("|3 - Verificar conexao entre dois predios   |\n");
        printf("|4 - Exibir matriz de alcancabilidade       |\n");
        printf("|0 - Sair                                   |\n");
        printf("============================================\n");
        printf("Escolha uma opcao: ");

        fgets(texto, 50, stdin);
        opcao = texto[0] - '0';            

        if (opcao == 1) exibirMapa();
        else if (opcao == 2) menorRota();
        else if (opcao == 3) verificarConexao();
        else if (opcao == 4) exibirMatriz();
        else if (opcao != 0) printf("\nOpcao invalida!\n");
    } while (opcao != 0);

    printf("\nEncerrando...\n");
    return 0;
}


void criarGrafo(void)
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            grafo[i][j] = 0;               /* comeca sem nenhuma ligacao */

    grafo[1][2] = grafo[2][1] = 240;       /* Bloco B <-> Laboratorio */
    grafo[2][0] = grafo[0][2] = 210;       /* Laboratorio <-> Bloco A */
    grafo[0][3] = grafo[3][0] = 190;       /* Bloco A <-> Refeitorio */
    grafo[3][4] = grafo[4][3] = 280;       /* Refeitorio <-> Biblioteca */
    grafo[4][0] = grafo[0][4] = 160;       /* Biblioteca <-> Bloco A */
}


void listarPredios(void)
{
    int i;
    printf("\n");
    for (i = 0; i < N; i++)
        printf("%d - %s\n", i + 1, nomes[i]);
}


int lerPredio(char *mensagem)
{
    char texto[50];
    int i;

    while (1) {
        printf("%s", mensagem);
        fgets(texto, 50, stdin);                 
        texto[strcspn(texto, "\n")] = '\0';     

        if (texto[0] >= '1' && texto[0] <= '5' && texto[1] == '\0')
            return texto[0] - '1';               
        for (i = 0; i < N; i++)
            if (strcmp(texto, nomes[i]) == 0)     
                return i;

        printf("Predio invalido! Tente de novo.\n");
    }
}

void exibirMapa(void)
{
	
	printf("\n===== MAPA DO CAMPUS ===== \n\n");
	
    int i, j;
    for (i = 0; i < N; i++){
    	for (j = i+1; j < N; j++){
    		if (grafo[i][j] > 0){
    			printf("%s <-> %s: %dm\n", nomes[i], nomes[j], grafo[i][j]);	
			}
		}
	}
}




void dijkstra(int origem, int dist[], int anterior[])
{
    int visitado[N];  
    int i, k, u, v, menor;


    for (i = 0; i < N; i++) {
        dist[i] = INF;
        anterior[i] = -1;
        visitado[i] = 0;
    }
    dist[origem] = 0;                     

    /* 2) repete uma rodada para cada predio */
    for (k = 0; k < N; k++) {

        /* escolhe o predio NAO visitado com a menor distancia */
        u = -1;
        menor = INF;
        for (i = 0; i < N; i++)
            if (!visitado[i] && dist[i] < menor) {
                menor = dist[i];
                u = i;
            }

        if (u == -1)
            break;                        

        visitado[u] = 1;                  

      
        for (v = 0; v < N; v++)
            if (grafo[u][v] > 0 && dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];   
                anterior[v] = u;                   
            }
    }
}


void menorRota(void)
{
    int dist[N];
    int anterior[N];
    int origem, destino;

    listarPredios();
    origem = lerPredio("Origem (numero ou nome): ");
    destino = lerPredio("Destino (numero ou nome): ");

    dijkstra(origem, dist, anterior);                  
    exibirRota(origem, destino, dist, anterior);      
}


void verificarConexao(void)
{
    int inicio, fim;
    
    listarPredios();

    inicio = lerPredio("Primeiro predio (numero ou nome): ");
    fim = lerPredio("Segundo predio (numero ou nome): ");

    if (inicio < 0 || fim < 0) {
        printf("\nPredio invalido.\n");
        return;
    }

    if(alcanca [inicio][fim] == 1){
        printf("\nExiste caminho entre %s e %s.\n", nomes[inicio], nomes[fim]);
    }else{
        printf("\nNao existe caminho entre %s e %s. Ou está indo de si mesmo para si mesmo\n", nomes[inicio], nomes[fim]);
    }
}

void floydWarshall(void)
{
    int i, j, k;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == j)
                distancia[i][j] = 0;
            else if (grafo[i][j] > 0)
                distancia[i][j] = grafo[i][j];
            else
                distancia[i][j] = INF;
        }
    }

    for (k = 0; k < N; k++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (distancia[i][k] + distancia[k][j] < distancia[i][j]) {
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                }
            }
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == j)
                alcanca[i][j] = 0;
            else if (distancia[i][j] < INF)
                alcanca[i][j] = 1;
            else
                alcanca[i][j] = 0;
        }
    }
}

void exibirMatriz(void)
{
    int i, j;

    printf("\n=======================================\n");
    printf("       MATRIZ DE ALCANCABILIDADE\n");
    printf("=======================================\n\n");

    printf("      ");
    for (i = 0; i < N; i++)
        printf("%-4d", i + 1);
    printf("\n");

    printf("    --------------------------------\n");

    for (i = 0; i < N; i++) {
        printf("%-4d| ", i + 1);

        for (j = 0; j < N; j++)
            printf("%-4d", alcanca[i][j]);

        printf("\n");
    }
    printf("             LEGENDA\n");
    printf("---------------------------------------\n");

    printf("1 - Existe um caminho entre os dois predios.\n");
    printf("0 - Nao representa uma conexao entre predios diferentes.\n");
    printf("    Na diagonal principal, indica o proprio predio.\n\n");

    printf("             IDENTIFICACAO\n");
    printf("---------------------------------------\n");

    for (i = 0; i < N; i++)
        printf("%d - %s\n", i + 1, nomes[i]);

    printf("\n=======================================\n");
}



void exibirRota(int origem, int destino, int dist[], int anterior[])
{
    int caminho[N];
    int tam = 0;
    int atual;
    int i;


    if (origem < 0 || destino < 0 || origem >= N || destino >= N) {
       
        printf("\nPredio invalido. \n");
        return;
    }

    if (dist[destino] == INF) {

        printf("\nNao existe caminho entre %s e %s.\n", nomes[origem], nomes[destino]);
        return;
    }

    atual = destino;

    while (atual != -1) {
        caminho[tam] = atual;
        tam++;
        atual = anterior[atual];
    }

    printf("\n Rota: ");
    for ( i = tam -1; i>= 0; i--) {
        
        printf("%s", nomes[caminho[i]]);
        if (i > 0) {
            printf(" -> ");
        }
    }

    printf("\n Distancia total: %d metros\n", dist[destino]);
    printf("\n=======================================\n");
}
