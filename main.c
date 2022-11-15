/* 
 * REPRESENTACAO DE GRAFOS - Versao 2022/1 
 * - Completado com fun��es desenvolvidas em aula
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

// Definindo as cores dos vértices como se fossem números
#define WHITE 11
#define GRAY 22
#define BLACK 33
#define SIZE 40 // Define o tamanho máximo da fila

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct aresta{ /* Celula de uma lista de arestas */
	int    nome;
	int    custo;
	struct aresta *prox;
}Aresta;

typedef struct vertice{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele, um atributo para armazenar a cor do vértice */
	int nome;
	int comp;
    int color; // Cores representadas por WHITE (11), GRAY (22) e BLACK (33)
	int pred; // Predecessores
	int dist; // Distancia
	int d; // Tempo de descoberta
	int f; // Tempo final, representado por u.f no algoritmo do Cormen

	Aresta *prim;
}Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int  acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int custo);
void imprimeGrafo(Vertice G[], int ordem);

int calculaGrauDeVertice(Vertice G[], int ordem, int v);
int calculaGrauMinimo(Vertice G[], int ordem);
int calculaGrauMaximo(Vertice G[], int ordem);
int calculaTamanho(Vertice G[], int ordem);
int eConexo(Vertice G[], int ordem);


// *******   FUNÇÕES PARA MANIPULAR O GRAFO   *******

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vertice **G, int ordem){
	int i;
	*G= (Vertice*) malloc(sizeof(Vertice)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenua aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int c){
    Aresta * A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Aresta *) malloc(sizeof(Aresta));
	A1->nome= v2;
	A1->custo= c;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;

	if (v1 == v2) return 1; /* Aresta e? um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Aresta *) malloc(sizeof(Aresta));
	A2->nome= v1;
	A2->custo= c;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	
	return 1;
}

/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia (custos das arestas entre parenteses):\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d(%d): ", i, G[i].color);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			// printf("  v%d(%d)", aux->nome, aux->custo);
			printf("  v%d(%d)", aux->nome, G[aux->nome].color);

	}
	printf("\n\n");
}

/*
 * Retorna o grau de um vertice v de um grafo G.
 */
int calculaGrauDeVertice(Vertice G[], int ordem, int v){
	int     i;
	Aresta* aux;
	
	if (v<0 || v>= ordem)
	   return(-1);
	aux= G[v].prim;
	/* Percorre a lista de arestas que incidem em v, contando qtd. */
	for(i=0; aux!=NULL; aux= aux->prox, i++)
	    if(aux->nome == v) i++;  /* Se for laco, conto 2 vezes */
	
	return(i);
}

/*
 * Retorna o grau minimo de um grafo G.
 */
int calculaGrauMinimo(Vertice G[], int ordem){
	int i,j;
	int grauMin= INT_MAX; /* Inicializacao com valor maximo */
	/* Percorro lista de vertices, calculando o grau de cada um */
	for(i=0; i<ordem; i++){
		j= calculaGrauDeVertice(G,ordem,i);
		if (j<grauMin) grauMin=j; /* Encontrando menor, atualizo minimo */
	}
	return(grauMin);
}

/*
 * Retorna o grau maximo de um grafo G.
 */
int calculaGrauMaximo(Vertice G[], int ordem){
	int i,j;
	int grauMax= -1; /* Inicializacao com valor minimo */
	/* Percorro lista de vertices, calculando o grau de cada um */
	for(i=0; i<ordem; i++){
		j= calculaGrauDeVertice(G,ordem,i);
		if (j>grauMax) grauMax=j;/* Encontrando maior, atualizo minimo */
	}
	return(grauMax);
}

/*
 * Retorna o tamanho de um grafo G (no. de vertices + no. de arestas).
 */
int calculaTamanho(Vertice G[], int ordem){
	int i;
	int total= 0;
	/* Qtd. de arestas � igual a metade da soma dos graus dos v�rtices */
	for(i=0; i<ordem; i++){
		total += calculaGrauDeVertice(G,ordem,i);
	}
	return(total/2+ordem);	
}

/*
 * Retorna 1 se um grafo G for conexo; ou 0, caso contrario.
 */
int eConexo(Vertice G[], int ordem){
	int i;
	int achou;
	Aresta *aux;
	
	if (ordem == 0) return(0); /* Grafo vazio */
	G[0].comp= 1; /* Anoto um "vertice inicial" com 1 */
	
	for(i=1; i<ordem; i++){
		G[i].comp= 0;  /* Todos os demais sao marcados com 0 */
	}
	do { /* Repito enquanto encontrar vertice nao anotado adjacente a anotado */
		achou = 0;
		for(i=0; i<ordem; i++){
			if(G[i].comp == 1){ /* Vertice i esta �anotado */
				aux= G[i].prim;
				for( ; aux!=NULL; aux= aux->prox){
					if(G[aux->nome].comp == 0){/* Achei nao anotado adjacente a anotado */
						G[aux->nome].comp= 1;  /* Anoto novo vertice*/
						achou= 1;              /* Indico que anotei um novo vertice */
					}
				}
			}
		}
	} while(achou); /* SE anotei um novo vertice, repito */
	
	for(i=0; i<ordem; i++){ /* SE encontrar vertice nao anotado, grafo e� desconexo */
		if(G[i].comp==0) return(0);
	}
	return(1);
}

/*
* Declarações de funções da busca em largura (BFS)
*/
int bfs(Vertice G[], int s, int ordem);


/*
* Declarações de funções da busca em profundidade (DFS)
*/

int dfs(Vertice G[], int ordem);
int DFS_Visit(Vertice G[], int u, int ordem, int time);

/*
* Declarações de funções para manipulação de filas do tipo FIFO (First-In-First-Out) 
*/

struct Queue {
    int items[40];
    int front;
    int rear;
};

struct Queue* createQueue();
void enqueue(struct Queue* q, int vertice);
int dequeue(struct Queue* q);
void display(struct Queue* q);
int isEmpty(struct Queue* q);
void printQueue(struct Queue* q);

// Cria uma fila
struct Queue* createQueue() {
    struct Queue* Q = malloc(sizeof(struct Queue));
    Q->front = -1;
    Q->rear = -1;
    return Q;
}

// Verifica se a fila está vazia
int isEmpty(struct Queue* q) {
    if (q->rear == -1)
        return 1;
    else return 0;
}

// Adiciona vertices na fila
void enqueue(struct Queue* q, int name) {
    if (q->rear == SIZE - 1) {
        printf("\nFila esta cheia!\n"); // DEBUG
        return ;
    } else {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = name;
    }
}

// Remove vertices da fila
int dequeue(struct Queue* q) {
    int vertice;
    
    if (isEmpty(q)) {
        printf("\nFila esta vazia!\n");
        vertice = -1;
    } else {
        vertice = q->items[q->front];
        q->front++;

        if (q->front > q->rear) {
            printf("\nResetando a queue\n");
            q->front = q->rear = -1;
        }
    }
    return vertice;
}

// Imprime o conteudo da fila
void printQueue(struct Queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d ", q->items[i]);
    }
  }
}

// Busca em largura
int bfs(Vertice G[], int s, int ordem) {
	printf("inside BSF\n\n");
	
	for (int u = 0; u < ordem; u++) {
		if (u == s) continue; // pula o vertice "s"
		G[u].color = WHITE; // WHITE representa o numero 11 que é a cor branca
		G[u].dist = -1;
		G[u].pred = NULL; // predecessor, representado no algoritmo do Cormen por pi 
		printf("G[%d].color = %d | dist = %d\n", u, G[u].color, G[u].dist);
	}

	G[s].color = GRAY;
	G[s].dist = 0;
	G[s].pred = NULL;
	printf("\n\nG[%d].color = %d | dist = %d\n\n", s, G[s].color, G[s].dist);
	struct Queue* q = createQueue();
	imprimeGrafo(G, ordem);
	enqueue(q, s);
	while (!isEmpty(q)){
		printf("\n\ninside while\n\n");
		int u = dequeue(q);
		printQueue(q);
		Aresta *v= G[u].prim;
		printf("\n");
		for( ; v != NULL; v= v->prox) {

			printf("  v%d, color: %d", G[v->nome].nome, G[v->nome].color);
			if (G[v->nome].color == WHITE) {
				G[v->nome].color = GRAY;
				G[v->nome].dist = G[u].dist+1;
				G[v->nome].pred = G[u].nome;
				enqueue(q, G[v->nome].nome);
				printf("\nenqueuing: %d\n\n", G[v->nome].nome);
				printQueue(q);
				imprimeGrafo(G, ordem);
			}
		}

		G[u].color = BLACK;
		printf("  v%d, color: %d", G[u].nome, G[u].color);
		imprimeGrafo(G, ordem);
	}
	printf("\n\nEND\n\n");
	imprimeGrafo(G, ordem);

	// Após fazer a busca em largura passando por todos os vértices, 
	// verifica se algum vértice não possui a cor preta.
	// Se todos possuírem a cor preta, é um grafo conexo, caso contrário é desconexo

	// Aresta *aux;

	for (int i=0; i<ordem; i++){
		printf("\n    v%d(%d): ", i, G[i].color);
		if (G[i].color != BLACK) {
			printf("nao conexo\n\n");
			return 0;
		}
	}
	return 1;
}


int dfs(Vertice G[], int ordem) {
	for (int u = 0; u < ordem; u++) {
		G[u].color = WHITE; // WHITE representa o numero 11 que é a cor branca
		G[u].pred = NULL; // predecessor, representado no algoritmo do Cormen por pi
	}

	int time = 0;

	for (int u = 0; u < ordem; u++) {
		if (G[u].color == WHITE) {
			DFS_Visit(G, u, ordem, time);
		}
	}
}

int DFS_Visit(Vertice G[], int u, int ordem, int time) {
	int tempo = time + 1;
	G[u].d = tempo;
	G[u].color = GRAY; // GRAY representa o numero 22 que é a cor cinza

	Aresta *aux;
	aux = G[u].prim;
	printf("\n");
	for (; aux!= NULL; aux=aux->prox) {// Explora as arestas incidentes nesse vertice
		printf("  v%d(%d)", aux->nome, G[aux->nome].color);
		if (G[aux->nome].color == WHITE) {
			G[aux->nome].pred = u;
			DFS_Visit(G, aux->nome, ordem, tempo);
		}
	}

	G[u].color = BLACK;
	tempo = tempo+1;
	G[u].f = tempo;
}

/*
 * Programinha simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
    int i,v;
	Vertice *G;
	int ordemG= 6; 
		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G,ordemG,2,3,1);
	acrescentaAresta(G,ordemG,0,1,1);
	acrescentaAresta(G,ordemG,1,1,1);
	acrescentaAresta(G,ordemG,0,2,1);
	acrescentaAresta(G,ordemG,0,3,1);
	acrescentaAresta(G,ordemG,3,4,1);
	acrescentaAresta(G,ordemG,4,5,1);

	
	for(v=0; v<ordemG;v++){
	   i= calculaGrauDeVertice(G,ordemG,v);
	   printf("O grau do vertice %d e %d\n", v, i);
    }
    
    i= calculaGrauMaximo(G,ordemG);
    printf("O grau maximo do grafo e %d\n", i);
    
    i= calculaTamanho(G,ordemG);
    printf("O tamanho do grafo e %d\n", i);
    
    // if(eConexo(G,ordemG))
    //    printf("O grafo e conexo\n");
    // else
    //    printf("O grafo nao e conexo\n");
	// imprimeGrafo(G, ordemG);

	// int isConexo = bfs(G, 0, ordemG);
	
	// if (isConexo) {
	// 	printf("\nO grafo e conexo\n\n");
	// } else {
	// 	printf("\nO grafo nao e conexo\n\n");
	// }
	
	int isConexoDFS = dfs(G, ordemG);

	if (isConexoDFS) {
		printf("\nO grafo e conexo DFS\n\n");
	} else {
		printf("\nO grafo nao e conexo DFS \n\n");
	}

	imprimeGrafo(G, ordemG);

	destroiGrafo(&G, ordemG);
    // system("PAUSE");

	
	// printQueue(q);
	// enqueue(q, 1);
	// enqueue(q, 2);
	// enqueue(q, 3);
	// printQueue(q);
	// dequeue(q);
	// printQueue(q);
	return 0;
}
