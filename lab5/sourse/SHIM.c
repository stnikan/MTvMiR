#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>


void init_shim_3(uint8_t numA, uint8_t numB, uint8_t arg, ...){
    TCCR3A = 0;
    TCCR3B = 0;
    uint16_t *p = &arg;
    
    while (numA--)
    {
        TCCR3A|=(1<<*p);        
        p++;
    }
    while (numB--)
    {
        TCCR3B|=(1<<*p);        
        p++;
    }}

void init_shim_1(uint8_t numA, uint8_t numB, uint8_t arg, ...){
    TCCR1A = 0;
    TCCR1B = 0;
    uint16_t *p = &arg;
    
    while (numA--)
    {
        TCCR1A|=(1<<*p);        
        p++;
    }
    while (numB--)
    {
        TCCR1B|=(1<<*p);        
        p++;
    }}


   

// void init_shim_3(uint8_t numA, uint8_t numB, uint8_t arg, ...){
//     TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31);
//     TCCR3B = (1 << CS30)| (1 << WGM32);

//     if (TCCR3A==(1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31)){
//         OCR3CH = 3;
//         OCR3CL=200;
//         _delay_ms(200);
//         OCR3CH = 0;
//         OCR3CL=0;}
//     if (TCCR3B == (1 << CS30)| (1 << WGM32)){
//         OCR3BH = 3;
//         OCR3BL=200;
//         _delay_ms(500);
//         OCR3BH = 0;
//         OCR3BL=0;}
    
    
// }