#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define LEITOR 10
#define ESCRITOR 5

pthread_mutex_t Base_dados;
pthread_mutex_t Quant_leitores;
int leitor_lendo;
int num;


void lerBancoDeDados(void);
void leitorSaindo(void);
void escritorPedindoAcesso(void);
void escreverBancoDeDados(void);

void * leitor(void*  quant)
{

    int * id = (int*) (quant);
    printf("O leitor %d foi criado com sucesso\n", *(id) );

    while(1)
    {
        pthread_mutex_lock(&Quant_leitores);
        leitor_lendo=leitor_lendo +1;

        if(leitor_lendo == 1 )
        {
            pthread_mutex_lock(&Base_dados);
        }

        pthread_mutex_unlock(&Quant_leitores);

        lerBancoDeDados();
      
        pthread_mutex_lock(&Quant_leitores);
        leitor_lendo=leitor_lendo -1;

        if(leitor_lendo == 0)
        {
            pthread_mutex_unlock(&Base_dados);
        }

        pthread_mutex_unlock(&Quant_leitores);

        leitorSaindo();

    }
}

void * escritor(void*)
{
    while (1)
    {
        escritorPedindoAcesso();
        pthread_mutex_lock(&Base_dados);
        escreverBancoDeDados();
        pthread_mutex_unlock(&Base_dados);

    }
    
}







int main()
{
    pthread_t threadsdosescritores[ESCRITOR], threadsdosleitores[LEITOR];
    int i;


    pthread_mutex_init(&Base_dados,NULL);
    pthread_mutex_init(&Quant_leitores,NULL);

    for(i=0 ; i<ESCRITOR ; i++)
    {
        pthread_create(&threadsdosescritores[i],NULL,escritor ,NULL);
    }

    for(i=0 ; i<LEITOR ; i++)
    {
        int calma;
        calma = rand() % 2;
        pthread_create(&threadsdosleitores[i],NULL,leitor ,(void*)&(i));
        sleep(calma);
    }

    for(i=0 ; i<ESCRITOR ; i++)
    {
        pthread_join(threadsdosescritores[i],NULL);
    }

    for(i=0 ; i<LEITOR ; i++)
    {
        pthread_join(threadsdosleitores[i],NULL);
    }

    return(0);

}



void lerBancoDeDados()
{
    int tempo_de_leitura;
    tempo_de_leitura = rand() % 5;

    printf("Lendo o Banco de dados. Um total de %d.\n\n Lendo %d\n\n",leitor_lendo,num);
    sleep(tempo_de_leitura);
}

void  leitorSaindo()
{
    
    int tempo_de_uso;
    tempo_de_uso= rand() % 5;

    printf("O leitor esta saindo.\n\n");
    sleep(tempo_de_uso);

}

void escritorPedindoAcesso()
{
    int tempo_para_pensar;
    tempo_para_pensar = rand() % 10;

    printf("Eu quero escrever.\n\n");
    sleep(tempo_para_pensar);
}

void escreverBancoDeDados()
{
    int tempo_de_escrita;
    
    num=num+1;
    printf("Alterando o valor para %d.\n\n",num);
    
}