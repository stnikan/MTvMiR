#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

#include "sourse\ADC.h"
#include "sourse\7SEGIND.h"
#include "sourse\SHIM.h"

uint16_t timer_n = 0;
uint16_t dist = 0;
ISR(INT7_vect)
{

    // если фронт сигнала на ECHO, то сбрасываем таймер
    if ((PINE & (1 << 7)) != 0)
    {
        TCNT1H = 0;
        TCNT1L = 0;
    }
    else
    { // если срез сигнала на ECHO, то забираем значение
        timer_n = TCNT1L;
        timer_n |= (TCNT1H << 8);
        // dist = (1016*340*100)/(2*172800);//(dist*64*340*100)/(2*11059200);
        // 1016 primerno
    }
}

int main(void)
{

    // DDRE = (1 << 3) | (1 << 4) | (1 << 5);

    // Прерывания для энкодера
    EIMSK = (1 << INT7);
    EICRB = (1 << ISC70);
    // EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);

    init_shim_1(2, 2, WGM11, WGM10,                WGM12, CS12); // таймер для счета

    // LED ------------------------------------
    init_shim_3(8, 2, COM3A1, COM3B1, COM3C1, COM3B0, COM3A0, COM3C0, WGM30, WGM31, CS30, WGM32); // число настроек для TCCR3A и для TCCR3B, далее названия регистров
    DDRE = (1 << 3) | (1 << 4) | (1 << 5);
    DDRE |= (1 << 6);
    //---------------------------------------

    sei();
    OCR3B = 1023;
    OCR3A = 923;
    OCR3C = 1023;
    _delay_ms(300);
    OCR3B = 1023;
    OCR3A = 1023;
    OCR3C = 1023;
    uint16_t test = 40;
    while (1)
    {

        PORTE |= (1 << 6);
        _delay_us(25);
        PORTE &= ~(1 << 6);
        _delay_ms(200);

        dist = (timer_n * 40) / (100);
        MyPrintUint(dist);
        if (dist < 15)
        {
            OCR3B = 1023;
            OCR3A = 1023;//blue
            OCR3C = 923;
        }
        else if (dist < 25)
        {
            OCR3B = 923;
            OCR3A = 1023; //blue
            OCR3C = 923;
        }
        else
        {
            OCR3B = 923;// green
            OCR3A = 1023;//blue
            OCR3C = 1023;//red
        }


        // MyPrintUint(dist); //1016 primerno
    }
}