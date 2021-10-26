#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//os valores estão iguais ao enunciado, porém podem ser mudados para facilitar os testes
#define N_APARTAMENTOS 6 //número de apartamentos participantes na simulação
#define TAM_AMOSTRA 50 //tamanho do somatório para o cálculo da média
#define MAXIMO_CONSUMO 1000 //a range aleatória de consumo de um apartamento vai de 0-MAXIMO_CONSUMO

#define VELOCIDADE_GAS 1000000 //tempo em microsegundos entre uma medida e outra de gas 
#define VELOCIDADE_AGUA 500000 //tempo em microsegundos entre uma medida e outra de agua
#define VELOCIDADE_ENERGIA 100000 //tempo em microsegundos entre uma medida e outra de energia

//cores para o terminal
#define KBLU "\x1B[34m"
#define KYEL "\x1B[33m"
#define KGRN "\x1B[32m"
#define RESET "\x1B[0m"

sem_t mutex_gas;
sem_t mutex_agua;
sem_t mutex_energia;

struct apartamento{ //total de 6 apartamentos, 2 por andar
    int consumo_gas;
    int consumo_agua;
    int consumo_energia;
    int amostra_gas; //as variaveis de amostra são o somatório dos ultimos TAM_AMOSTRA valores
    int amostra_agua;
    int amostra_energia;
};

struct apartamento a[N_APARTAMENTOS]; //struct global para compartilhar valores entre as threads

void inicializa_apartamentos(){
    for(int i=0;i<N_APARTAMENTOS;i++){
        a[i].consumo_gas=0;
        a[i].consumo_agua=0;
        a[i].consumo_energia=0;
        a[i].amostra_gas=0; 
        a[i].amostra_agua=0;
        a[i].amostra_energia=0;
    }
}

// os apartamentos produzem dados
void *produtor_gas(){
    printf(KGRN"GAS Gerada thread do produtor\n"RESET);
    int valor_consumo=0;
    while(1){  
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%MAXIMO_CONSUMO;
            a[i].consumo_gas=valor_consumo;
        }
        usleep(VELOCIDADE_GAS);
        sem_post(&mutex_gas);
    }
}

// a empresa consome os dados
void *consumidor_gas(){// a empresa consome dados
    FILE *f = fopen("gas.log", "w");//ARMAZENA OS LOGS da operação
    printf(KGRN"GAS Gerada thread do consumidor\n"RESET);
    int contador_amostra=0;
    int media=0;
    while(1){
        sem_wait(&mutex_gas);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_gas=a[i].consumo_gas+a[i].amostra_gas;
            fprintf(f, "GAS consumo apartamento %d, %d\n", i, a[i].consumo_gas);
            //printf(KGRN"GAS consumo apartamento %d, %d\n"RESET, i, a[i].consumo_gas);
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_gas/TAM_AMOSTRA;
                printf(KGRN"GAS Media de consumo apartamento %d, %d\n"RESET,i,media);
                a[i].amostra_gas=0;
            }
            contador_amostra=0;
        }
    }
}

// os apartamentos produzem dados
void *produtor_agua(){
    printf(KBLU"AGUA Gerada thread do produtor\n"RESET);
    int valor_consumo=0;
    while(1){
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%MAXIMO_CONSUMO;
            a[i].consumo_agua=valor_consumo;
        }
        usleep(VELOCIDADE_AGUA);
        sem_post(&mutex_agua);
    }
}

// a empresa consome os dados
void *consumidor_agua(){// a empresa consome dados
    FILE *f = fopen("agua.log", "w");//ARMAZENA OS LOGS da operação
    printf(KBLU"AGUA Gerada thread do consumidor\n"RESET);
    int contador_amostra=0;
    int media=0;
    while(1){
        sem_wait(&mutex_agua);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_agua=a[i].consumo_agua+a[i].amostra_agua;
            fprintf(f, "AGUA consumo apartamento %d, %d\n", i, a[i].consumo_agua);
            //printf(KBLU"AGUA consumo apartamento %d, %d\n"RESET, i, a[i].consumo_agua);
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_agua/TAM_AMOSTRA;
                printf(KBLU"AGUA Media de consumo apartamento %d, %d\n"RESET,i,media);
                a[i].amostra_agua=0;
            }
            contador_amostra=0;
        }    
    }
}

// os apartamentos produzem dados
void *produtor_energia(){
    printf(KYEL"ENERGIA Gerada thread do produtor\n"RESET);
    int valor_consumo=0;
    while(1){ 
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%MAXIMO_CONSUMO;
            a[i].consumo_energia=valor_consumo;
        }
        usleep(100000);
        sem_post(&mutex_energia);
    }
}

// a empresa consome os dados
void *consumidor_energia(){// a empresa consome dados
    FILE *f = fopen("energia.log", "w");//ARMAZENA OS LOGS da operação
    printf(KYEL"ENERGIA Gerada thread do consumidor\n"RESET);
    int contador_amostra=0;
    int media=0;
    while(1){       
        sem_wait(&mutex_energia);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_energia=a[i].consumo_energia+a[i].amostra_energia;
            //printf(KYEL"ENERGIA consumo apartamento %d, %d\n"RESET, i, a[i].consumo_agua);
            fprintf(f, "ENERGIA consumo apartamento %d, %d\n", i, a[i].consumo_agua);
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_energia/TAM_AMOSTRA;
                printf(KYEL"ENERGIA Media de consumo apartamento %d, %d\n"RESET,i,media);
                a[i].amostra_energia=0;
            }
            contador_amostra=0;
        }
    }
}

int main(){
    sem_init(&mutex_gas, 1, 0);
    sem_init(&mutex_agua, 1, 0);
    sem_init(&mutex_energia, 1, 0);
    
    srand(time(NULL));
    
    inicializa_apartamentos(a);//Inicializa os valores da struct com 0

    pthread_t tp1,tp2,tp3;//(0)gas(1)agua(2)energia
    pthread_t tc1,tc2,tc3;//(0)gas(1)agua(2)energia
    
    pthread_create(&tc1,NULL,consumidor_gas,NULL);
    pthread_create(&tp1,NULL,produtor_gas,NULL); 

    pthread_create(&tc2,NULL,consumidor_agua,NULL);
    pthread_create(&tp2,NULL,produtor_agua,NULL); 

    pthread_create(&tc3,NULL,consumidor_energia,NULL);
    pthread_create(&tp3,NULL,produtor_energia,NULL); 
    
    pthread_join(tp1,NULL);
    pthread_join(tc1,NULL);
    pthread_join(tp2,NULL);
    pthread_join(tc2,NULL);
    pthread_join(tp3,NULL);
    pthread_join(tc3,NULL);
    
    return 0;
}