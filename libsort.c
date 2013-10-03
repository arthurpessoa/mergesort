#include "libsort.h"

/*  Função:
 *      multimerge(int **subvetores, int  *tam_subvetores, int num_subvetores, int *vetorfinal)
 *  Descrição:
 *      Modificação da etapa final do mergesort, onde o número de vetores a 
 *      serem mesclados pode ser maior que dois.
 *  Entradas:
 *      int **subvetores    : Um vetor de subvetores ordenados que deverão ser mesclados
 *      int *tam_subvetores : Vetor que contém o tamanho de cada um dos subvetores
 *      int num_subvetores  : Número de subvetores que deverão ser mesclados
 *      int *vetorfinal     : Apontador para o vetor final, que deverá conter todos os subvetores ordenados mesclados
 */

void multimerge(int **subvetores, int  *tam_subvetores, int num_subvetores, int *vetorfinal){
    int i = 0; //apontador que guarda os indices pro vetor final
    int j = 0; //indice do mínimo da rodada
    int min; // valor mínimo da rodada
    int posicao_min;// posição do mínimo do subvetor

    int *apontador =(int*) malloc(num_subvetores*sizeof(int));// apontador para os vetores que estão divididos
    int k; //inicializo os apontadores com 0 (posição inicial dos subvetores)
    for(k=0;k<num_subvetores;++k)apontador[k]=0;

    while(1){
        min = INT_MAX; //jogo um valorzão pra poder fazer a comparação do mínimo
        posicao_min = -1; //flag de posição inválida
        // Vou de posição em posição dos subvetores e pego o mínimo
        for(j = 0; j < num_subvetores; ++j){
            if(apontador[j] < tam_subvetores[j] &&subvetores[j][apontador[j]] < min){  //vejo se o apontador tá ok, e se o elemento é o menor
                min = subvetores[j][apontador[j]];  //guardo o valor
                posicao_min = j;             //guardo a posição do menor valor
            }
        }
        //se não achei o mínimo, então a posição é inválida
        if(posicao_min == -1)break;
        // atualizo o vetor final, com o mínimo           
        vetorfinal[i++] = min; 
        apontador[posicao_min]++; //incremento a posição do subvetor pra não usar mais aquele valor
    }
    free(apontador); //libero o vetor de apontadores pros subvetores, Lol tá ficando confuso! D:
}


/*  Função: 
 *      msort(int *A,int e, int d)
 *  Descrição:
 *      Implementação Clássica do mergesort com algumas otimizações.
 *  Entradas: 
 *      int* A : Apontador do vetor que será ordenado
 *      int  e : Posição inicial a ser ordenada (default 0)  
 *          int  d : Posição final a ser ordenada (default VET_TAM)
 */
void msort(int *A,int e, int d){
   int m;
   int *B;//veotr auxiliar
   int i;
   int j;
   int k;
   int inverso;
   
   B = (int*) malloc((d+1)* sizeof(int));
   if(d > e ){      
        m = (e + d)/2;//calculando o meio do vetor
        msort(A,e,m);//metade esquerda
        msort(A,m+1,d);//metade direita      
       //intercalando
        for(i = e; i <= m; i++)
                B[i] = A[i]; 
        for(j = m+1;j <= d;j++){
                inverso = d + m + 1 - j ;
                B[inverso] = A[j];//preenchendo inversamente                        
        }      
        i = e;//inicio do vetor
        j = d;//fim do vetor
        for(k = e; k <= d;k++){
            if(B[i] < B[j]){
                A[k] = B[i];
                i++;                
            }else{
                A[k] = B[j];
                   j--;                       
            }                 
        }
    }        
    free(B);//liberando B
}
