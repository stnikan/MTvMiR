#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>


void init_shim_3(uint8_t numA, uint8_t numB, uint8_t arg, ...){
    TCCR3A = 0;
    TCCR3B = 0;
    uint8_t *p = &arg;
    //uint8_t bit_A[] = {COM3A1,COM3A0,COM3B1,COM3B0, COM3C1,COM3C0, WGM31,WGM30};
    //uint8_t bit_B[] = {ICNC3,ICES3,WGM33, WGM32, CS32,CS31,CS30};
    while (numA--)
    {
        TCCR3A|=(1<<*p);        
        p++;
    }
    while (numB--)
    {
        TCCR3B|=(1<<*p);        
        p++;
    }
    //TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31);
    //TCCR3B = (1 << CS30)| (1 << WGM32);

    //if (TCCR3A==(1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31)){
    //    OCR3CH = 3;
    //    OCR3CL=200;}
    //if (TCCR3B == (1 << CS30)| (1 << WGM32)){OCR3BH = 3;
    //    OCR3BL=200;}
    
    
}