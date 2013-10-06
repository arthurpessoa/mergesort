/*	Universidade Federal de São Carlos, Campus Sorocaba
 *	Mergesort Paralelo (MPI)
 *  Autor:
 *			Arthur Pessoa de Souza
 *  Arquivo:
 *			libsort.h 
 *	Descrição:
 *			Arquivo que contém as funções de ordenação
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>


/*	Função:
 *			multimerge(int **subvetores, int  *tam_subvetores, int num_subvetores, int *vetorfinal)
 *	Descrição:
 *			Modificação da etapa final do mergesort, onde o número de vetores a 
 *			serem mesclados pode ser maior que dois.
 *	Entradas:
 *			int **subvetores    : Um vetor de subvetores ordenados que deverão ser mesclados
 *			int *tam_subvetores : Vetor que contém o tamanho de cada um dos subvetores
 *			int num_subvetores  : Número de subvetores que deverão ser mesclados
 *			int *vetorfinal     : Apontador para o vetor final, que deverá conter todos os subvetores ordenados mesclados
 */         
void multimerge(int **subvetores, int  *tam_subvetores, int num_subvetores, int *vetorfinal);


/*	Função: 
 *			msort(int *A,int e, int d)
 *	Descrição:
 *			Implementação Clássica do mergesort com algumas otimizações.
 *	Entradas: 
 *			int* A : Apontador do vetor que será ordenado
 *			int  e : Posição inicial a ser ordenada (default 0)  
 *          int  d : Posição final a ser ordenada (default VET_TAM)
 */
void msort(int *A,int e, int d);


