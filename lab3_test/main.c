// /*
//  * test3.c
//  *
//  * Created: 20.09.2023 22:15:18
//  * Author : stnikan
//  */

#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

#include ".\..\sourse\ADC.h"

int main(void)
{
    // sei();
    //  lcdInit();

    // прерывания для энкодера
    /*EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT2);
    EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);*/

    DDRE = (1 << 3) | (1 << 4) | (1 << 5);
    /* Инициализация таймера №3. 8-ми битная быстрая
    ШИМ, преддедитель на 8 */
    TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30) | (1 << WGM31);
    TCCR3B = (1 << CS30)| (1 << WGM32);

    // OCR3AH = 3; // сброс по совпадению, это ограничение
    // OCR3AL = 255;

    // включение ацп
    init_ADC(1,ADEN);

    while (1)
    {
        
        OCR3AH = read_adc_10(3)>>8;
        OCR3AL = read_adc_10(3);
       
    }
}