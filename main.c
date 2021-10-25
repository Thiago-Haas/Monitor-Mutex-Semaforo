//consumo de energia elétrica,  consumo de gás e consumo de água.
//automação de 3 andares de prédio de  teste, sendo monitorado os 2 apartamentos que há por andar. 
//Os sensores de água devem ser lidos a cada 500 ms;
//Os sensores de energia elétrica devem ser lidos a cada 100 ms;
//Os sensores de gás devem ser lidos a cada 1 s;
//A cada 50 medições de cada unidade (água, eletricidade, gás) deve ser feito uma média
//de consumo por andar e armazenado em uma variável que pode ser lida posteriormente. 
//O   sistema   deverá   capturar   os   dados   e   deverá   estar   também   disponível   para   enviar   os
//dados   lidos   (valores   dos   apartamentos   e   médias   por   andar)   quando   requisitado   pela
//central da empresa.
//Deve ser proposto uma fuVELOCIDADE_AGUAnção de ativação de aumento ou diminuição de gás, água ou
//eletricidade a medida que há mais ou menos demanda.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//os valores estão iguais ao enunciado, porém podem ser mudados para facilitar os testes
#define N_APARTAMENTOS 6 //número de apartamentos participantes na simulação
#define TAM_AMOSTRA 50 //tamanho do somatório para o cálculo da média
#define MAXIMO_CONSUMO 1000 //a range aleatória de consumo de um apartamento vai de 0-MAXIMO_CONSUMO

#define VELOCIDADE_GAS 1 //tempo entre uma medida e outra de gas
#define VELOCIDADE_AGUA 0.5 //tempo entre uma medida e outra de agua
#define VELOCIDADE_ENERGIA 0.1 //tempo entre uma medida e outra de energia

sem_t mutex_gas;
sem_t mutex_agua;
sem_t mutex_energia;

struct apartamento{ //total de 6 apartamentos, 2 por andar
    int consumo_gas;
    int consumo_agua;
    int consumo_energia;
    int amostra_gas;
    int amostra_agua;
    int amostra_energia;
};

struct apartamento a[N_APARTAMENTOS];

void inicializa_apartamentos(){
    for(int i=0;i<N_APARTAMENTOS;i++){
        a[i].consumo_gas=0;
        a[i].consumo_agua=0;
        a[i].consumo_energia=0;
        a[i].amostra_gas=0; //as variaveis de amostra são o somatório dos ultimos TAM_AMOSTRA valores
        a[i].amostra_agua=0;
        a[i].amostra_energia=0;
    }
    
}

// os apartamentos produzem dados
void *produtor_gas(){
    printf("GAS Gerada thread do produtor\n");
    int valor_consumo=0;
    while(1){  
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%MAXIMO_CONSUMO;
            a[i].consumo_gas=valor_consumo;
        }
        usleep(1000000);
        sem_post(&mutex_gas);
    }
}

// a empresa consome os dados
void *consumidor_gas(){// a empresa consome dados
    printf("GAS Gerada thread do consumidor\n");
    int contador_amostra=0;
    int media=0;
    while(1){
        sem_wait(&mutex_gas);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_gas=a[i].consumo_gas+a[i].amostra_gas;
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_gas/TAM_AMOSTRA;
                printf("GAS Media de consumo apartamento %d, %d\n",i,media);
                a[i].amostra_gas=0;
            }
            contador_amostra=0;
        }
    }
}

// os apartamentos produzem dados
void *produtor_agua(){
    printf("AGUA Gerada thread do produtor\n");
    int valor_consumo=0;
    while(1){
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%MAXIMO_CONSUMO;
            a[i].consumo_agua=valor_consumo;
        }
        usleep(500000);
        sem_post(&mutex_agua);
    }
}

// a empresa consome os dados
void *consumidor_agua(){// a empresa consome dados
    printf("AGUA Gerada thread do consumidor\n");
    int contador_amostra=0;
    int media=0;
    while(1){
        sem_wait(&mutex_agua);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_agua=a[i].consumo_agua+a[i].amostra_agua;
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_agua/TAM_AMOSTRA;
                printf("AGUA Media de consumo apartamento %d, %d\n",i,media);
                a[i].amostra_agua=0;
            }
            contador_amostra=0;
        }    
    }
}

// os apartamentos produzem dados
void *produtor_energia(){
    printf("ENERGIA Gerada thread do produtor\n");
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
    printf("ENERGIA Gerada thread do consumidor\n");
    int contador_amostra=0;
    int media=0;
    while(1){       
        sem_wait(&mutex_energia);
        for(int i=0;i<N_APARTAMENTOS;i++){
            a[i].amostra_energia=a[i].consumo_energia+a[i].amostra_energia;
        }
        contador_amostra++;
        if(contador_amostra>TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_energia/TAM_AMOSTRA;
                printf("ENERGIA Media de consumo apartamento %d, %d\n",i,media);
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
    //
    pthread_create(&tc2,NULL,consumidor_agua,NULL);
    pthread_create(&tp2,NULL,produtor_agua,NULL); 
    //
    pthread_create(&tc3,NULL,consumidor_energia,NULL);
    pthread_create(&tp3,NULL,produtor_energia,NULL); 
    

    pthread_join(tp1,NULL);
    pthread_join(tc1,NULL);
    pthread_join(tp2,NULL);
    pthread_join(tc2,NULL);
    pthread_join(tp3,NULL);
    pthread_join(tc3,NULL);
    //return 0;
}