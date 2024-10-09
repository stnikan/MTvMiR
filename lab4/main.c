#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

#include "sourse\ADC.h"

#include "sourse\SHIM3.h"

int16_t angle = 0;
ISR(INT0_vect)
{
    if ((PIND & (1 << 0)) != 0)
    {
        EICRA = (1 << ISC01)  | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            angle+=5;
        else
            angle-=5;
    }
    else
    {
        EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            angle-=5;
        else
            angle+=5;
    }
    if (angle<0){angle = 0;}
    
}

ISR(INT2_vect){angle=0;}

int main(void)
{
    

    DDRE = (1 << 3) | (1 << 4) | (1 << 5);

    sei();
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT2);
    EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);
    
    init_ADC(1,ADEN);
    /* Инициализация таймера №3. 8-ми битная быстрая
    ШИМ, преддедитель на 8 */
    //TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31);
    //TCCR3B = (1 << CS30)| (1 << WGM32);
    init_shim_3(5,COM3A1,COM3B1,COM3C1,WGM30,WGM31);
    //init_shim_3(2,CS30,WGM32);
    
    
    

    // включение ацп
    

    while (1)
    {
        
        OCR3AH = read_adc_10(3)>>8;
        OCR3AL = read_adc_10(3);
        
       
    }
}