/*	Universidade Federal de São Carlos, Campus Sorocaba
 *	Mergesort Paralelo (MPI)
 *  Autor:
 *			Arthur Pessoa de Souza
 *			Jonathan André Varella Gangi
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

int first_i(int worker,int total_workers, int vsize);
int last_i(int worker,int total_workers, int vsize);


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

	
	/*-------------------------*
	*		 Coordenador
	*--------------------------*/
	if(world_rank == COORDENADOR){ //coordenador
		//Lê as entradas
		do{
			scanf("%d",&I); //lê a quantidade de ordenações
			if(I){ //caso haja ordenações a fazer, continue...
				scanf("%d",&N); //lê o tamanho do vetor
				if(N){ //caso haja um vetor a ordenar, continue...
					Vetor = (int*)malloc(N*sizeof(int)); //aloca espaço pro vetor
					
					int i,j;
					for(i=0;i<N;++i) scanf("%d",&Vetor[i]); //lê o vetor	

					for(i=0;i<I;i++){
						MPI_Bcast(&I, 1, MPI_INT, 0, MPI_COMM_WORLD); //mando em broadcast
						//Vetores auxiliares
						int **subvetor;
						int * tam_subvetor;
						tam_subvetor=(int*)malloc(world_size*sizeof(int));
						subvetor = (int**)malloc(world_size*sizeof(int*));


						//copiar o vetor auxiliar do coordenador
						int first = first_i(COORDENADOR,world_size, N);
						int last = last_i(COORDENADOR,world_size, N);
						int vsize = last - first+1;

						subvetor[COORDENADOR]= malloc(vsize*sizeof(int));
						tam_subvetor[COORDENADOR]=vsize;


						int k=0;
						for(j=first;j<=last;j++){
							subvetor[COORDENADOR][k]=Vetor[j];
							k++;
						}

						for(j=0;j<world_size;++j)
						{
							if(j!=COORDENADOR){
								first = first_i(j,world_size, N);
								last = last_i(j,world_size, N);
								vsize = last - first+1;

								subvetor[j]= malloc(vsize*sizeof(int));
								tam_subvetor[j]=vsize;
							}
						}

						start = clock();

						//----------------------MERGESORT PARALELO -------------------------
						
						//envio um pedaço do vetor pra cada processo						
						for(j=0;j<world_size;++j)
						{
							if(j!=COORDENADOR){
								
								first = first_i(j,world_size, N);
								MPI_Send(&tam_subvetor[j], 1, MPI_INT, j, 0, MPI_COMM_WORLD);
								MPI_Send(Vetor+first, tam_subvetor[j], MPI_INT, j, 1, MPI_COMM_WORLD);
							}
						}						
						msort(subvetor[COORDENADOR],0,tam_subvetor[COORDENADOR]-1); //enquanto isso o mestre ordena um pedaço também	
						//Recebo os pedaços ordenados
						for(j=0;j<world_size;++j)
						{
							if(j!=COORDENADOR){								
								MPI_Recv(subvetor[j], tam_subvetor[j], MPI_INT, j, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
							} //DEADLOCK (?)
						}
						//junto os pedaços usando a ultima etapa do mergesort modificada
						multimerge(subvetor, tam_subvetor, world_size, Vetor);

						//------------------FIM MERGESORT----------------------------------
						stop = clock();
			
						
						if(!i){ //caso seja a primeira ordenação, exibir o vetor ordenado
							for(j=0;j<N;++j)printf("%d ",Vetor[j]);
							printf("\n");
						}
						printf("%f\n",(double)(stop-start)/CLOCKS_PER_SEC );//imprime o tempo de execução

						//desalocar os vetores auxiliares
						free(tam_subvetor);
						for(j=0;j<world_size;j++)free(subvetor[j]);
					}
					
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

			int RID = 1; //resp. ID
			MPI_Bcast(&I, 1, MPI_INT, COORDENADOR, MPI_COMM_WORLD);
			if(I){
				 MPI_Recv(&N, 1, MPI_INT, COORDENADOR, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);				 
						 
				 int *sVetor=(int*)malloc(N*sizeof(int));
	
				 MPI_Recv(sVetor, N, MPI_INT, COORDENADOR, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				 msort(sVetor,0,N-1);

				 MPI_Send(sVetor, N, MPI_INT, COORDENADOR, RID, MPI_COMM_WORLD); //retorna o vetor ordenado pro coordenador

				 free(sVetor);				 
			}
		}while(I);//se não tem mais trabalhos, vcs podem morrer gentem (China Feelings)
		
	}
	MPI_Finalize(); //termina o MPI
 	return 0;
}

int first_i(int worker,int total_workers, int vsize)
{
	int first = floor((vsize/total_workers))*(worker);

	if(worker<(vsize%total_workers)){
		first+=worker;
	}else
		first+=vsize%total_workers;
	return first;

}
int last_i(int worker,int total_workers, int vsize)
{
	int last = floor(vsize/total_workers)*(worker+1);

	if(worker<(vsize%total_workers)){
		last+=worker;
	}else
		last+=vsize%total_workers-1;
	return last;
}


