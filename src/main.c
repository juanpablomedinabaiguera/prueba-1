#include "ciclismo.h"
#define FUEGOS_ARTIFICIALES 0   //salida PA0
#define PARTICIPANTE_1 1        //entrada PB1
#define PARTICIPANTE_2 2        //entrada PB2
#define PARTICIPANTE_3 3        //entrada PB3
#define PREMIACION 8            //exti PB8

int btn[3]={PARTICIPANTE_1, PARTICIPANTE_2, PARTICIPANTE_3};

int tiempo_p1=0, tiempo_p2=0, tiempo_p3=0;

freddy_t sistema;

int main(){
    ciclismo(&sistema);
    sistema.d7s_puerto=GPIOC;
    sistema.d7s_pines[0]=4;
    sistema.d7s_pines[1]=5;
    sistema.d7s_pines[2]=6;
    sistema.d7s_pines[3]=7;
    sistema.d7s_pines[4]=8;
    sistema.d7s_pines[5]=9;
    sistema.d7s_pines[6]=10;
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPAEN|RCC_APB2ENR_AFIOEN;
    
    for(int i=0;i<3;i++){
        GPIOB->CRL&=~(0xF<<btn[i]*4);
        GPIOB->CRL&=~(0x8<<btn[i]*4);
        GPIOB->BSRR|=(1<<btn[i]);
    }
    GPIOA->CRL&=~(0xF<<FUEGOS_ARTIFICIALES);
    GPIOA->CRL&=~(0x1<<FUEGOS_ARTIFICIALES);

    GPIOB->CRH&=~(0xF<<((PREMIACION)%8)*4);
    GPIOB->CRH|=(0x8<<((PREMIACION)%8)*4);
    GPIOB->BSRR|=(1<<PREMIACION+16);
    AFIO->EXTICR[2]|=(0x1<<(PREMIACION%4)*4);
    EXTI -> IMR|=(1<<PREMIACION);
    EXTI -> RTSR|=(1<<PREMIACION);
    EXTI -> FTSR&=~(1<<PREMIACION);
    NVIC_EnableIRQ


    while(1){

    }

    return 0;
}

void EXTI9_5IRQHandler(){
    if(EXTI->PR)
}