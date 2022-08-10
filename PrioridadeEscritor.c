#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define LEITOR 10
#define ESCRITOR 10


pthread_mutex_t bancoDados;
pthread_mutex_t mutex1_controleLeitor_1;
pthread_mutex_t mutex2_controleLeitor_2;
pthread_mutex_t mutex3_controleEscritor;
pthread_mutex_t controlePrivilegio_escritor;


int leitor_lendo;
int quant_escritor;

int num;


void lerBancoDeDados(void);
void leitorSaindo(void);
void escritorPedindoAcesso(void);
void escreverBancoDeDados(void);

void * leitor(void*  quant)
{

    int * id = (int*) (quant) ;
    
    printf("O leitor %d foi criado com sucesso\n", *(id) );

    while(1)
    {
        pthread_mutex_lock(&mutex1_controleLeitor_1); // lock para garanitr esclusividade  ao lock e unlock do controlePrivilegio_escritor e que 
                            // o mutex1_controleLeitor_1 garante que so um leitor espere o unlock do controlePrivilegio_escritor
        pthread_mutex_lock(&controlePrivilegio_escritor);      // block para ver se algum escritor esta escrevendo
        pthread_mutex_lock(&mutex2_controleLeitor_2);  // lock para auterar variavel leitor_lendo
        leitor_lendo=leitor_lendo +1;

        if(leitor_lendo == 1 )
        {
            pthread_mutex_lock(&bancoDados);   // lock do primeiro leitor para area critica
        }

        pthread_mutex_unlock(&mutex2_controleLeitor_2);  // unlock variavel leitor_lendo
        pthread_mutex_unlock(&controlePrivilegio_escritor);      // unlock controlePrivilegio_escritor
        pthread_mutex_unlock(&mutex1_controleLeitor_1); // unlock para um proximo leitor mais ele vai ficar travado no sem_wait(r) por causa do escritor

        lerBancoDeDados();

        pthread_mutex_lock(&mutex2_controleLeitor_2);  // lock para auterar variavel leitor_lendo
        leitor_lendo=leitor_lendo -1;

        if(leitor_lendo == 0)
        {
            pthread_mutex_unlock(&bancoDados);  // unlock do ultimo leitor
        }

        pthread_mutex_unlock(&mutex2_controleLeitor_2);  // unlock variavel leitor_lendo

        leitorSaindo();

    }
}

void * escritor(void* quant)
{

    int * id = (int*) (quant) ;
    
    printf("O escritor %d foi criado com sucesso\n", *(id) );

    while (1)
    {
        escritorPedindoAcesso();
        pthread_mutex_lock(&mutex3_controleEscritor);  // lock para auterar variavel quant_escritor
        
        quant_escritor = quant_escritor +1;

        if(quant_escritor == 1)
        {
            pthread_mutex_lock(&controlePrivilegio_escritor); // lock para avisar que quer escrever bloquer os leitores
        }

        pthread_mutex_unlock(&mutex3_controleEscritor);  // unlock variavel quant_escritor
        pthread_mutex_lock(&bancoDados);  // lock para area critica
       
        escreverBancoDeDados();

        pthread_mutex_unlock(&bancoDados);  // unlock do escritor
        pthread_mutex_lock(&mutex3_controleEscritor);  // lock para auterar variavel quant_escritor

        quant_escritor = quant_escritor -1;

        if(quant_escritor ==0 )
        {
            pthread_mutex_unlock(&controlePrivilegio_escritor);  // unlock para avisar que podem ler ou escrever

        }
        
        pthread_mutex_unlock(&mutex3_controleEscritor);  // unlock variavel quant_escritor


        int tempo;
    tempo = rand() % 20;

    
   sleep(tempo);

    }
    
}







int main()
{
    pthread_t threadsdosescritores[ESCRITOR], threadsdosleitores[LEITOR];
    int i;


    pthread_mutex_init(&controlePrivilegio_escritor,NULL);
    pthread_mutex_init(&mutex1_controleLeitor_1,NULL);
    pthread_mutex_init(&mutex2_controleLeitor_2,NULL);
    pthread_mutex_init(&mutex3_controleEscritor,NULL);
    pthread_mutex_init(&bancoDados,NULL);
 

    for(i=0 ; i<ESCRITOR ; i++)
    {
        pthread_create(&threadsdosescritores[i],NULL,escritor ,(void*)&(i));
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
    printf("Escrevendo no banco de dados. \n ecrevendo %d\n\n",num);
   
}