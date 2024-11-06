#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int16_t angle = 0;

ISR(INT0_vect)
{
    if (PORTD2 == 0)
    {
        // EICRA = (1 << 1);
        if (PORTD7 != 0)
        {
            angle += 9;
        }
        else
        {
            angle -= 9;
        }
    }
    else
    {
        if (PORTD7 != 0)
        {
            angle -= 9;
        }
        else
        {
            angle += 9;
        }
    }
}

void spiInit(void)
{ // инициализация аппаратной части SPI
    // выводы SS (CS), MOSI, SCK – на выход
    DDRB |= (1 << 2) | (1 << 3) | (1 << 5);
    // включить аппаратный SPI, режим 0, предделитель на 128
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}
/*функция для выделения из трёхразрядного числа d разряда
m; например, digit(14, 3) = 4; или digit(591, 1) = 5 */
uint16_t raz(uint8_t k) // определяет разрядность числа
{
    uint16_t a = 1;
    for (uint16_t i = 0; i < k; i++)
    {
        a *= 10;
    }
    return a;
}

void showMe(int16_t spiData, _Bool znak)
{
    int8_t i;
    // разряды числа идут в массив dig[]
    uint8_t dig[] = {0, 0, 0}, j;
    // ‘0’,..., ‘9’, сегмент выключен, ‘-’
    uint8_t mas[] = {0x81, 0xF3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xF1, 0x01, 0x21, 0xFF, 0x7F};

    uint8_t k = 0;
    uint8_t flag = 1;
    uint16_t res;
    uint8_t sumb;
    if (znak)
    {
        sumb = 3;
        if (spiData < 0)
        {                   // Рисуем минус
            SPDR = mas[11]; // отрицательное число-рисуем «минус»
            while (!(SPSR & (1 << SPIF)))
                ;
            res = -spiData;
        }
        else
        {
            SPDR = mas[10]; // отрицательное число-рисуем «минус»
            while (!(SPSR & (1 << SPIF)))
                ;
            res = spiData;
        }
        if (res > 999)
            res = 999;
    }
    else
    {
        if (spiData > 9999)
            spiData = 9999;
        res = spiData;
        sumb = 4;
    }

    for (uint8_t i = 1; i <= sumb; i++)
    {                                   // бежим по всем разрядам
        k = (res / raz(sumb - i)) % 10; // вычисляем цифру в разряде

        if (k > 0)
        {
            flag = 0;
        }
        // подменям ноль на пустоту, если цифры уже были
        if (flag && i != sumb)
        {
            k = 10;
        }

        SPDR = mas[k];
        while (!(SPSR & (1 << SPIF)))
            ;
    }

    PORTB &= ~(1 << 2); // ”защелкиваем” выводимое число
    _delay_us(20);
    PORTB |= (1 << 2);
    _delay_us(20);
    PORTB &= ~(1 << 2);
}

int main(void)
{
    spiInit();

    EICRA = (1 << ISC00); //| 1 << 1);
    EIMSK = (1 << 0);
    sei();
    uint16_t test = 0;
    while (1)
    {
        showMe(angle, 1);
    }
}