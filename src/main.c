#include "ciclismo.h"

#define FUEGOS_ARTIFICIALES 0 //salida PA0
#define PARTICIPANTE_1 1 //entrada PB1
#define PARTICIPANTE_2 2 //entrada PB2
#define PARTICIPANTE_3 3 //entrada PB3
#define PREMIACION 8 //exti PB8

int btn [3] = {PARTICIPANTE_1,PARTICIPANTE_2,PARTICIPANTE_3};

int tiempo_p1=0,tiempo_p2=0,tiempo_p3=0;

volatile int flag_premiacion=0;

freddy_t sistema;

int main(){
    sistema.d7s_puerto=GPIOC;
    sistema.d7s_pines[0]=4;
    sistema.d7s_pines[1]=5;
    sistema.d7s_pines[2]=6;
    sistema.d7s_pines[3]=7;
    sistema.d7s_pines[4]=8;
    sistema.d7s_pines[5]=9;
    sistema.d7s_pines[6]=10;
    ciclismo_init(&sistema);


    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_AFIOEN;
    //BOTONES PARTICIPANTES
    for(int i=0;i<3;i++){
        GPIOB->CRL&=~(0xF<<btn[i]*4);
        GPIOB->CRL|= (0x8<<btn[i]*4);
        GPIOB->BSRR|= (1<<btn[i]+16);
    }
    //FUEGOS ARTIFICIALES

    GPIOA->CRL&=~(0xF<<FUEGOS_ARTIFICIALES*4);
    GPIOA->CRL|= (0x1<<FUEGOS_ARTIFICIALES*4);
    //PREMIACION

    GPIOB->CRH&=~(0xF<<(PREMIACION%8)*4);
    GPIOB->CRH|= (0x8<<(PREMIACION%8)*4);
    GPIOB->BSRR|= (1<<PREMIACION+16);
    AFIO->EXTICR[2]|=(0x1<<(PREMIACION%4)*4);
    EXTI->IMR|=(1<<PREMIACION);
    EXTI->RTSR|=(1<<PREMIACION);
    EXTI->FTSR&=~(1<<PREMIACION);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn,2);
    while(1){
        if(flag_premiacion==0){
            int participante1=GPIOB->IDR&(1<<btn[0]);
            int participante2=GPIOB->IDR&(1<<btn[1]);
            int participante3=GPIOB->IDR&(1<<btn[2]);
            if((participante1==1)&&(participante2==0)&&(participante3==0)){
                tiempo_p1=(&sistema);
            }
            if((participante1==0)&&(participante2==1)&&(participante3==0)){
                tiempo_p2=(&sistema);
            }
            if((participante1==0)&&(participante2==0)&&(participante3==1)){
                tiempo_p3=(&sistema);
            }
            sistema.d7s_num=0;
            d7s(&sistema);
        }else{
            if((tiempo_p1<<tiempo_p2) && (tiempo_p1<<tiempo_p3)) sistema.d7s_num=1;
            else if(tiempo_p2<<tiempo_p3) sistema.d7s_num=2;
            else sistema.d7s_num=3;
            d7s(&sistema);
            GPIOA->BSRR|=(1<<FUEGOS_ARTIFICIALES);
            flag_premiacion=0;
        }

    }
    return 0;
}

void EXTI9_5IRQHandler(){
    if(EXTI->PR&(1<<PREMIACION)){
        EXTI->PR|=(1<<PREMIACION);
        flag_premiacion=1;
    }
}