#ifndef CICLISMO_H
#define CICLISMO_H
#include "stm32f103xb.h"

#define SS 1
#define SL 2

typedef struct{

    //sensores
    GPIO_TypeDef* puerto_sensores = GPIOA;
    int sensores[2] = {SS,SL};

    //tiempo
    GPIO_TypeDef* tiempoL_puerto = GPIOC;
    int t_l_pines[4] = {0,1,2,3};

    GPIO_TypeDef* tiempoH_puerto = GPIOB;
    int t_h_pines[4] = {12,13,14,15};    

    //d7s
    GPIO_TypeDef* d7s_puerto;
    int d7s_pines[7];
    int d7s_num;
    int tf;             //tiempo final
} freddy_t;

void ciclismo_init(freddy_t*);
void d7s(freddy_t*);
void carrera(freddy_t*);
int tiempo;

#endif