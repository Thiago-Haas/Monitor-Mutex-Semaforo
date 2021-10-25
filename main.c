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
//Deve ser proposto uma função de ativação de aumento ou diminuição de gás, água ou
//eletricidade a medida que há mais ou menos demanda.

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>


#define N_APARTAMENTOS 6
#define TAM_AMOSTRA 50

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
        a[i].amostra_gas=0;
        a[i].amostra_agua=0;
        a[i].amostra_energia=0;
    }
    
}

// os apartamentos produzem dados
void *produtor_consumo_gas(){
    int valor_consumo=0;
    while(1){
        sleep(1);
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%10;
            a[i].consumo_gas=valor_consumo;
        }
        //aqui precisa de um mutex para sincronizar com o consumidor
    }
}

void *produtor_consumo_agua(){
    int valor_consumo=0;
    while(1){
        sleep(0.5);
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%10;
            a[i].consumo_agua=valor_consumo; 
        }
        //aqui precisa de um mutex para sincronizar com o consumidor
    }
}

void *produtor_consumo_energia(){
    int valor_consumo=0;
    while(1){
        sleep(0.1);
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%10;
            a[i].consumo_energia=valor_consumo; 
        }
        //aqui precisa de um mutex para sincronizar com o consumidor
    }
}

// a empresa consome os dados
void *consumidor_registra_gas(){// a empresa consome dados
    int contador_amostra=0;
    int media=0;
    int valor_consumo=0; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
    while(1){
        //aqui precisa de um MUTEX para sincronizar com o produtor
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = a[i].consumo_gas; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
            a[i].amostra_gas=a[i].consumo_gas+a[i].amostra_gas;
            contador_amostra++; 
        }
        if(contador_amostra==TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_gas/TAM_AMOSTRA;
                printf("Media de consumo apartamento %d, %d",i,media);
            }
        }
    }
}

void *consumidor_registra_agua(){// a empresa consome dados
    int contador_amostra=0;
    int media=0;
    int valor_consumo=0; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
    while(1){
        //aqui precisa de um MUTEX para sincronizar com o produtor
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = a[i].consumo_agua; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
            a[i].amostra_agua=a[i].consumo_agua+a[i].amostra_agua;
            contador_amostra++; 
        }
        if(contador_amostra==TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_agua/TAM_AMOSTRA;
                printf("Media de consumo de gas apartamento %d, %d",i,media);
            }
        }
    }
}

void *consumidor_registra_energia(){// a empresa consome dados
    int contador_amostra=0;
    int media=0;
    int valor_consumo=0; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
    while(1){
        //aqui precisa de um MUTEX para sincronizar com o produtor
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = a[i].consumo_energia; // não sei se vou usar essa variável, provavelmente vai poluir muito o terminal
            a[i].amostra_energia=a[i].consumo_energia+a[i].amostra_energia;
            contador_amostra++; 
        }
        if(contador_amostra==TAM_AMOSTRA){
            for(int i=0;i<N_APARTAMENTOS;i++){
                media=a[i].amostra_energia/TAM_AMOSTRA;
                printf("Media de consumo de energia apartamento %d, %d",i,media);
            }
        }
    }
}

int main(){
    srand(time(NULL));
    inicializa_apartamentos(a);//Inicializa as structs com 0
    return 0;
}