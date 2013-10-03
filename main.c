
/*	Universidade Federal de São Carlos, Campus Sorocaba
 *	Mergesort Paralelo (MPI)
 *  Autores:
 *			Arthur Pessoa de Souza
 * 			Jonathan André Varella Gangi
 *  Arquivo:
 *			main.c
 *	Descrição:
 *			Arquivo que contém as funções principais, e implementa o paralelismo entre os processos
 */

#include "libsort.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


#define COORDENADOR 0 //Processo Mestre


void init_workers(int nWorkers, int root, int N, int I);
void finalize_workers();

/*	Função:
 *			main(void)
 *	Descrição:
 *			Função principal, onde é distribuída todas as tarefas para os processos
 */
int main(void) {
 	MPI_Init(NULL, NULL); //inicializa o MPI
	int world_size;   //pega o numero de processos
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;   //pega o ranking do processo
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	int I; //Quantidade de Testes
	int N; //Quantidade de elementos no vetor

	int *Vetor; //vetor a ser ordenado
	clock_t start, stop;

	int nWorkers=0; //número de trabalhadores ativos (default 0 single-core)

	/*-------------------------*
	*		 Coordenador
	*--------------------------*/
	if(world_rank == COORDENADOR){ //coordenador
		//Lê as entradas
		do{
			scanf("%d",&I); //lê a quantidade de ordenações
			if(I){ //caso haja ordenações a fazer, continue...
				MPI_Bcast(&I, 1, MPI_INT, 0, MPI_COMM_WORLD); //mando em broadcast
				scanf("%d",&N); //lê o tamanho do vetor
				MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD); //mando em broadcast N
				if(N){ //caso haja um vetor a ordenar, continue...
					Vetor = (int*)malloc(N*sizeof(int)); //aloca espaço pro vetor

					int i,j;
					for(i=0;i<N;++i) scanf("%d",&Vetor[i]); //lê o vetor


					nWorkers = 2; //TODO: Descobrir uma fórmula pra saber quando vale a pena adicionar vários
								//trabalhadores (tempo sequencial < (tempo_sequencial/nworkers) + overhead)




					if(nWorkers>world_size)nWorkers=world_size; //caso o melhor caso seja com muitos trabalhadores, limitar para o número de processos
					init_workers(nWorkers, COORDENADOR, N, I);

					for(i=0;i<I;i++){
						start = clock();
						//MERGESORT


						stop = clock();
						//imprime os resultados
						if(!i){ //caso seja a primeira ordenação, exibir o vetor ordenado
							for(j=0;j<N;++j)printf("%d ",Vetor[j]);
							printf("\n");
						}
						printf("%f\n",(double)(stop-start)/CLOCKS_PER_SEC );//imprime o tempo de execução
					}

					finalize_workers();
					free(Vetor);
				}
			}
		}while(I);
		MPI_Bcast(&I, 1, MPI_INT, 0, MPI_COMM_WORLD); //mando em broadcast o termino das tarefas
	}

	/*-------------------------*
	*		 Trabalhadores
	*--------------------------*/
	else{
		do{
			MPI_Bcast(&I, 1, MPI_INT, COORDENADOR, MPI_COMM_WORLD);
			if(I){
				do{
					MPI_Bcast(&N, 1, MPI_INT, COORDENADOR, MPI_COMM_WORLD); //recebo N
					if(N){
						Vetor = (int*) malloc(N*sizeof(int)); //pré-aloco a memória, whatever memóra não é problema
						int vsize;
						MPI_Recv(&vsize, 1, MPI_INT, COORDENADOR, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						MPI_Recv(Vetor, vsize, MPI_INT, COORDENADOR, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						int active;
						do{
							MPI_Recv(&active, 1, MPI_INT, COORDENADOR, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
							if(active){





							}
						}while(active);
					}
					free(Vetor);
				}while(N);
			}
		}while(I);//se não tem mais trabalhos, vcs podem morrer gentem (China Feelings)
	}
	MPI_Finalize(); //termina o MPI
 	return 0;
}


void init_workers(int nWorkers, int root, int N, int I)
{
	int i;
	int tam_subvetor = ceil(N/nWorkers);
	for (i = 0; i < nWorkers; ++i)
	{
		if(i!=root){
			MPI_Send(&I, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&tam_subvetor, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
	int active = 1;
	MPI_Send(&active, 1, MPI_INT, i, 2, MPI_COMM_WORLD);//indica que a sessão é a mesma
}


void finalize_workers(){
	int active = 0;
	MPI_Send(&active, 1, MPI_INT, i, 2, MPI_COMM_WORLD);//indica que a sessão é a mesma
}
