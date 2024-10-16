#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

#include "sourse\ADC.h"

#include "sourse\SHIM.h"

int16_t angle = 0;
ISR(INT0_vect)
{
    if ((PIND & (1 << 0)) != 0)
    {
        EICRA = (1 << ISC01) | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            angle += 50;
        else
            angle -= 50;
    }
    else
    {
        EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            angle -= 50;
        else
            angle += 50;
    }
    if (angle < 0)
    {
        angle = 0;
    }
    if (angle > 1200)
    {
        angle = 1200;
    }
}
ISR(INT2_vect) { angle = 0; }

int main(void)
{

    // DDRE = (1 << 3) | (1 << 4) | (1 << 5);

    // Прерывания для энкодера
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT2);
    EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);

    // включение ацп
    init_ADC(1, ADEN);

    // шим светодиодов
    //  TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31);
    //   TCCR3B = (1 << CS30)| (1 << WGM32);
    // init_shim_3(5, 2, COM3A1, COM3B1, COM3C1, WGM30, WGM31, CS30, WGM32); // число настроек для TCCR3A и для TCCR3B, далее названия регистров
    DDRB |= (1 << 5) | (1 << 6);
    init_shim_1(3, 3, WGM11, COM1B1,  COM1A1, 
    CS11, WGM13, WGM12);
    ICR1 = 27648;
    // шим для серво
    //  uint8_t bit_A[] = {COM3A1,COM3A0,COM3B1,COM3B0, COM3C1,COM3C0, WGM31,WGM30};
    //  uint8_t bit_B[] = {ICNC3,ICES3,WGM33, WGM32, CS32,CS31,CS30};
    //  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1) | (1 << WGM30) | (1 << WGM31);
    //  TCCR1B = (1 << CS30)| (1 << WGM32);

    sei();

    OCR1B = 1400;
    OCR1A = 1400;
    _delay_ms(1000);
    while (1)
    {
        // меняем шим
        //  OCR1BH = read_adc_10(3) >> 8;
        //  OCR1BL = read_adc_10(3);
        //  OCR1CH = read_adc_10(3) >> 8;
        //  OCR1CL = read_adc_10(3);
        OCR1A = 1400+angle;
        OCR1B = 1400+read_adc_8_H(3);
        
        
        // OCR1B = read_adc_10(3);
    }
}