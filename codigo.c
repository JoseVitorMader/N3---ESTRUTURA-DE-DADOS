/* INTEGRANTES: Nykolas, Jose, Carla, Luis, Isa */
#include <stdio.h>
#include <string.h>

#define N 5      /* numero de predios */
#define INF 9999 /* "infinito": ainda nao ha caminho */

/* nomes dos predios: a posicao no vetor e o numero do predio */
char nomes[N][20] = {"Bloco A", "Bloco B", "Laboratorio", "Refeitorio", "Biblioteca"};

int grafo[N][N];   /* matriz de adjacencia (distancias; 0 = sem ligacao direta) */
int alcanca[N][N]; /* matriz de alcancabilidade (1 = existe caminho, 0 = nao) */



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

/* ================= MAIN (pronta) ================= */
int main(void)
{
    char texto[50];
    int opcao;

    criarGrafo();      /* monta o grafo */
    floydWarshall();   /* calcula a alcancabilidade uma vez so */

    do {
        printf("\n=======================================\n");
        printf("       SISTEMA DE ROTAS DO CAMPUS\n");
        printf("=======================================\n\n");
        printf("1 - Exibir mapa do campus\n");
        printf("2 - Encontrar menor rota entre dois predios\n");
        printf("3 - Verificar conexao entre dois predios\n");
        printf("4 - Exibir matriz de alcancabilidade\n");
        printf("0 - Sair\n\n");
        printf("Escolha uma opcao: ");

        fgets(texto, 50, stdin);
        opcao = texto[0] - '0';            /* primeiro caractere vira numero */

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

/* Le um predio: o usuario digita o numero (1 a 5) ou o nome exato.
   Retorna o indice do predio (0 a 4) 
   qualqur coisa, pede pra mim no zap q eu te explico melhor carlinha, mas acho q com os comentario ja vai dar de ter
   uma noção doq fazer*/
int lerPredio(char *mensagem)
{
    char texto[50];
    int i;

    while (1) {
        printf("%s", mensagem);
        fgets(texto, 50, stdin);                  /* le a linha digitada */
        texto[strcspn(texto, "\n")] = '\0';       /* remove o ENTER do final */

        if (texto[0] >= '1' && texto[0] <= '5' && texto[1] == '\0')
            return texto[0] - '1';                /* '1' vira 0, '2' vira 1... */

        for (i = 0; i < N; i++)
            if (strcmp(texto, nomes[i]) == 0)     /* digitou o nome? */
                return i;

        printf("Predio invalido! Tente de novo.\n");
    }
}

void exibirMapa(void)
{
    /* nykolas aqui */
}



/* DIJKSTRA: calcula a menor distancia da origem ate todos os predios
ent se alguem quiser entender, depois eu documento melhor como fazer esse caba doidera.
   dist[v]     = menor distancia conhecida origem -> v
   anterior[v] = predio de onde viemos para chegar em v (-1 = nenhum) */
void dijkstra(int origem, int dist[], int anterior[])
{
    int visitado[N];  /* 1 = distancia do predio ja e definitiva */
    int i, k, u, v, menor;

    /* 1) inicializa: ninguem foi alcancado ainda */
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
            break;                         /* o que sobrou e inalcancavel */

        visitado[u] = 1;                   /* fixa o predio escolhido */

        /* relaxamento: passar por u melhora o caminho dos vizinhos? */
        for (v = 0; v < N; v++)
            if (grafo[u][v] > 0 && dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];   /* nova distancia menor */
                anterior[v] = u;                   /* chegamos em v vindo de u */
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

    dijkstra(origem, dist, anterior);                  /* calcula as menores rotas */
    exibirRota(origem, destino, dist, anterior);       /* RF05 - funcao da Isa */
}


void verificarConexao(void)
{
    /* carla aqui */
    /* dica: usa a funcao lerPredio() para pegar os dois predios e consulta a matriz alcanca[][] q fai ser mel na chupetinha */
}

void floydWarshall(void)
{
    /* luis aqui */
    /* tem que preencher a matriz global alcanca[][] com 1 ou 0, reocmento dar uma olhada no yt que tem bastante forma de fazer
    n vai inventar moda de fazer diferente, jaq tem q ser o algoritimo dele em especifico
    da de só pegar o código dele pronto e adaptar*/

}

void exibirMatriz(void)
{
    /* luis aqui */
    /*achou q ia ser easy né luis? zoeira, so pega e faz o printf da função*/ 
}

/* ================= RF05 - ISA ================= */
/* Recebe o resultado do Dijkstra do meu (jose):
   dist[destino] == INF  -> nao existe caminho
   anterior[]            -> permite montar a rota voltando do destino ate a origem (-1 = fim) */
void exibirRota(int origem, int destino, int dist[], int anterior[])
{
    /* isa aqui */
}