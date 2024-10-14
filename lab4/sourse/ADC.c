#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

void init_ADC(uint8_t num, uint8_t arg, ...)
{
    ADCSRA = 0;
    uint8_t *p = &arg;
    while (num--)
    {
        /* code */

        ADCSRA |= (1 << *p);
        p++;
    }
}

uint16_t read_adc_10(uint8_t channel)
{
    // AVCC + левое выравнивание + выбор канала
    ADMUX = (1 << REFS0) | channel;
    ADCSRA |= (1 << ADSC); // Начало преобразования
    // Ждем окончания преобразования АЦП
    while (!(ADCSRA & (1 << ADIF)))
        ;
    // Сброс флага завершения преобразования
    ADCSRA |= (1 << ADIF);
    // uint8_t low = ADCL;
    uint16_t my_acp = ADCL | (ADCH << 8);
    return (my_acp); // возвращаем старшие 8 бит результата
}

uint8_t read_adc_8_H(uint8_t channel)
{
    // AVCC + левое выравнивание + выбор канала
    ADMUX = (1 << REFS0) | (1 << ADLAR) | channel;
    ADCSRA |= (1 << ADSC); // Начало преобразования
    // Ждем окончания преобразования АЦП
    while (!(ADCSRA & (1 << ADIF)))
        ;
    // Сброс флага завершения преобразования
    ADCSRA |= (1 << ADIF);
    // uint8_t low = ADCL;

    return (ADCH); // возвращаем старшие 8 бит результата
}