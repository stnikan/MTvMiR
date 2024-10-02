// /*
//  * test3.c
//  *
//  * Created: 20.09.2023 22:15:18
//  * Author : stnikan
//  */

#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

uint16_t read_adc(uint8_t channel)
{
    // AVCC + левое выравнивание + выбор канала
    ADMUX = (1 << REFS0) | channel;
    ADCSRA |= (1 << ADSC); // Начало преобразования
    // Ждем окончания преобразования АЦП
    while (!(ADCSRA & (1 << ADIF)))
        ;
    // Сброс флага завершения преобразования
    ADCSRA |= (1 << ADIF);
    //uint8_t low = ADCL;
    uint16_t my_acp =ADCL| (ADCH<<8);
    return (my_acp); // возвращаем старшие 8 бит результата
}
uint8_t read_adcH(uint8_t channel) {
 // AVCC + левое выравнивание + выбор канала
 ADMUX = (1<<REFS0) | channel;
 ADCSRA |= (1<<ADSC); // Начало преобразования
 // Ждем окончания преобразования АЦП
 while(!(ADCSRA & (1<<ADIF)));
 // Сброс флага завершения преобразования
 ADCSRA |= (1<<ADIF);
 return(ADCH); // возвращаем старшие 8 бит результата
}
uint8_t read_adcL(uint8_t channel) {
 // AVCC + левое выравнивание + выбор канала
 ADMUX = (1<<REFS0) | channel;
 ADCSRA |= (1<<ADSC); // Начало преобразования
 // Ждем окончания преобразования АЦП
 while(!(ADCSRA & (1<<ADIF)));
 // Сброс флага завершения преобразования
 ADCSRA |= (1<<ADIF);
 return(ADCL); // возвращаем старшие 8 бит результата
}
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
    ADCSRA = (1 << ADEN);

    //uint16_t myColor = 0;
    uint8_t i = 0;

    while (1)
    {
        // char c[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        // myColor = read_adc(3);
        // ADMUX = (1 << REFS0) | 3;
        // ADCSRA |= (1 << ADSC); // Начало преобразования
        // // Ждем окончания преобразования АЦП
        // while(!(ADCSRA & (1 << ADIF)));
        // // Сброс флага завершения преобразования
        // ADCSRA |= (1 << ADIF);
        // uint16_t my_acp = (ADCH<<8) | (ADCL);
        
        //OCR3AH = ADCH;
        //OCR3AH = read_adc(3);
        //OCR3AH = read_adcH(3);
        //OCR3AL = 0;//read_adcL(3);
        OCR3AH = read_adc(3)>>8;
        OCR3AL = read_adc(3);
        //OCR3AL = read_adc(3);
        //_delay_ms(100);
    }
}