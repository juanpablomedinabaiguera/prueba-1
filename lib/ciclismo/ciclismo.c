#include "ciclismo.h"

volatile int t_inicial=0, t_final=0,vueltas=0;
freddy_t s;

int d7s_e[4][7]={
                {0,0,0,0,0,0,1},
                {1,1,0,0,0,0,0},
                {1,1,0,1,1,0,1},
                {1,1,1,1,0,0,1}
};

void ciclismo_init(freddy_t* s){
    //7 seg
    if(s->d7s_puerto==GPIOA) RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    if(s->d7s_puerto==GPIOB) RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
    if(s->d7s_puerto==GPIOC) RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
    if(s->d7s_puerto==GPIOD) RCC->APB2ENR|=RCC_APB2ENR_IOPDEN;
    if(s->d7s_puerto==GPIOE) RCC->APB2ENR|=RCC_APB2ENR_IOPEEN;

    for(int i=0;i<7;i++){
        if(s->d7s_pines<8){
            s->d7s_puerto -> CRL &=~ (0xF<<(s->d7s_pines[i]*4));
            s->d7s_puerto -> CRL &=~ (0x1<<(s->d7s_pines[i]*4));
        }else{
            s->d7s_puerto -> CRH &=~ (0xF<<((s->d7s_pines[i]%8)*4));
            s->d7s_puerto -> CRH &=~ (0x1<<((s->d7s_pines[i]%8)*4));
        }
    }
//sensores
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    for(int i=0;i<2;i++){
        s->puerto_sensores -> CRL &=~ (0xF<<(s->sensores[i]*4));
        s->puerto_sensores -> CRL &=~ (0x4<<(s->sensores[i]*4));
        AFIO -> EXTICR[0]|=(0x0<<((s->sensores[i]%4)*4));
        EXTI -> IMR     |= (1<<s->sensores[i]);
        EXTI -> FTSR    |= (1<<s->sensores[i]);
        EXTI -> RTSR    &=~(1<<s->sensores[i]);
    }

    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_SetPriority(EXTI1_IRQn,1);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_SetPriority(EXTI2_IRQn,1);
    //tiempo
    RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
    for(int i=0;i<4;i++){
        s->tiempoL_puerto -> CRL &=~ (0xF<<(s->t_l_pines[i]*4));
        s->tiempoL_puerto -> CRL |= (0x4<<(s->t_l_pines[i]*4));
        s->tiempoH_puerto -> CRH &=~ (0xF<<(s->t_l_pines[i]*4));


    }
}
void d7s(freddy_t*s){
    for(int i=0;i<7;i++){
        if(d7s_e[s->d7s_num][i]) s->d7s_puerto -> BSRR |= (1<<s->d7s_pines[i]);
        else s->d7s_puerto -> BSRR |= (1<<((s->d7s_pines[i])+16));
    }    
}

void tiempo(freddy_t*s){
    int t_low = s->tiempoL_puerto -> IDR & (0x4);
    int t_high = s->tiempoH_puerto -> IDR & (0x4<<12);
    int t_final = (t_low|(t_high<<4));
    return  t_final;
}

void carrera(freddy_t*s){
    int v[5], vueltas=0;
    while(vueltas<5){
        if(vuelta_terminada){
            v[vueltas]=t_final - t_inicial;
            vueltas++;
            vuelta_terminada=0;
        }
    }
    int tiempo_total;
    for(int i=0;i<5;i++){
        tiempo_total+=v[i];
    }
    return tiempo_total;



void EXTI1IRQHandler(){
    if(EXTI->PR&(1<<SS)){
        EXTI->PR|=(1<<SS);
        t_inicial=tiempo();
    }
}

void EXTI2IRQHandler(){
    if(EXTI->PR&(1<<SL)){
        EXTI->PR|=(1<<SL);
        t_final=tiempo(&s);
        vueltas++;
    }
}