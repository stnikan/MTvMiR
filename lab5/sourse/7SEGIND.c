#include <avr/io.h>
// #define F_CPU 11059200
#include <util/delay.h>

uint8_t my_znak[] = {0b00111111, 0b00000110, 0b01011011,
                     0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,
                     0b01111111, 0b01101111, 0b00000000}; // рисунки цифр

uint16_t raz(uint8_t k) // определяет разрядность числа
{
    uint16_t a = 1;
    for (uint16_t i = 0; i < k; i++)
    {
        a *= 10;
    }
    return a;
}

void MyPrintINT(int16_t a)
{
    DDRA = 0xFF; // теперь ножки это выход
    DDRC = 0xFF;
    uint8_t k = 0;
    uint8_t flag = 1;
    if (a < 0)
    { // Рисуем минус
        PORTC = 0b01000000;
        PORTA = 1 << (1);
        _delay_us(10);
        PORTA = 0x00;
        a = -a / 10;
    }
    else
    {
        PORTC = 0b00000000; // не рисуем минус
        PORTA = 1 << (1);
        _delay_us(3);
        PORTA = 0x00;
        a = a / 10;
    }

    for (uint8_t i = 2; i < 6; i++)
    {                              // бежим по всем разрядам
        k = (a / raz(5 - i)) % 10; // вычисляем цифру в разряде
        if (k > 0)
        {
            flag = 0;
        }
        // подменям ноль на пустоту, если цифры уже были
        if (flag && i != 5)
        {
            k = 10;
        }
        PORTC = my_znak[k]; // вгружаем в по порту нашу цифру
        _delay_ms(1);
        PORTC = my_znak[10];
        // выбираем разряд в который будем рисовать
        PORTA = 1 << (i);
        _delay_us(10);
        PORTA = 0x00;
    }
}

void MyPrintUint(uint16_t a)
{
    DDRA = 0b00111110; // теперь ножки это выход
    DDRC = 0xFF;
    
    uint8_t k = 0;
    uint8_t flag = 1;

    for (uint8_t i = 1; i < 6; i++)
    {                              // бежим по всем разрядам
        k = (a / raz(5 - i)) % 10; // вычисляем цифру в разряде
        if (k > 0)
        {
            flag = 0;
        }
        // подменям ноль на пустоту, если цифры уже были
        if (flag && (i != 5))
        {
            k = 10;
        }
        PORTC = my_znak[k]; // вгружаем в по порту нашу цифру
        // выбираем разряд в который будем рисовать
        PORTA = 1 << (i);
        _delay_us(15);
        PORTA = 0x00;
    }
}