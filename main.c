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

#define N_APARTAMENTOS 6

struct apartamento{ //total de 6 apartamentos, 2 por andar
    int consumo_gas;
    int consumo_agua;
    int consumo_energia;
};

void inicializa_apartamentos(struct apartamento a[]){
    for(int i=0;i<N_APARTAMENTOS;i++){
        a[i].consumo_gas=0;
        a[i].consumo_agua=0;
        a[i].consumo_energia=0;
    }
    
}

// os apartamentos produzem dados
void *produtor_consumo_gas(struct apartamento a[]){
    int valor_consumo=0;
    while(1){
        sleep(1)
        for(int i=0;i<N_APARTAMENTOS;i++){
            valor_consumo = rand()%10;
            a[i].consumo_gas=valor_consumo;
        }
    }
}

void *produtor_consumo_agua(struct apartamento a[]){
    while(1){
        int valor_consumo=0;
            while(1){
            sleep(0.5)
            for(int i=0;i<N_APARTAMENTOS;i++){
                valor_consumo = rand()%10;
                a[i].consumo_agua=valor_consumo;
            }
    }
}

void *produtor_consumo_energia(struct apartamento a[]){
    while(1){
        int valor_consumo=0;
        while(1){
            sleep(0.1)
            for(int i=0;i<N_APARTAMENTOS;i++){
                valor_consumo = rand()%10;
                a[i].consumo_energia=valor_consumo; 
            }
    }
}



void *consumidor(){// a empresa consome dados
    
}

int main(){
    srand(time(NULL));
    struct apartamento a[N_APARTAMENTOS];
    inicializa_apartamentos(a);
}