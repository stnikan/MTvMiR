#include <avr/io.h>
// #define F_CPU 11059200
#include <util/delay.h> //для использования пауз

#include <avr/interrupt.h>

#define _BV(b) (1 << (b))
#define MUX (_BV(REFS0))
#define BIT_IS_CLEAR(Reg, b) ((Reg & _BV(b)) == 0)

#define E 6
#define RS 7

void lcdCmd(uint8_t cmd)
{                                   // посыл команды на экран
    DDRC = 0xFF;                    // все разряды PORTC на выход
    DDRD |= ((1 << E) | (1 << RS)); // разряды PORTD на выход
    PORTD &= ~(1 << RS);            // выбор регистра команд RS=0
    PORTC = cmd;                    // записать команду в порт PORTC
    PORTD |= (1 << E);              // \ сформировать на
    _delay_us(5);                   // | выводе E строб 1-0
    PORTD &= ~(1 << E);             // / передачи команды
    _delay_ms(10);                  // задержка для завершения записи
}
void lcdInit(void)
{                                   // инициализация (ВКЛ) экрана
    DDRC = 0xFF;                    // все разряды PORTC на выход
    DDRD |= ((1 << E) | (1 << RS)); // разряды PORTD на выход
    _delay_ms(100);                 // задержка для установления питания
    lcdCmd(0x30);                   // \ вывод
    lcdCmd(0x30);                   // | трех
    lcdCmd(0x30);                   // / команд 0x30
    lcdCmd(0x38);                   // 8 разр.шина, 2 строки, 5 × 7 точек
    lcdCmd(0x0C);                   // включить ЖКИ
    lcdCmd(0x06);                   // инкремент курсора, без сдвига экрана
    lcdCmd(0x01);                   // очистить экран, курсор в начало
}
void lcdData(uint8_t data)
{ // посыл данных на экран
    DDRC = 0xFF;
    DDRD |= ((1 << E) | (1 << RS));
    PORTD |= (1 << RS);
    PORTC = data;
    PORTD |= (1 << E);
    _delay_us(5);
    PORTD &= ~(1 << E);
    _delay_ms(1);
}
void my_lcdInit()
{
    // uint8_t hello[] = {'fiel:000 ref:000 '}
}
void my_out(uint16_t a, uint16_t b, int16_t d)
{
    char c[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    lcdCmd((1 << 7) | 1);
    lcdData(c[a / 1000]);
    lcdData(c[(a / 100) % 10]);
    lcdData(c[(a / 10) % 10]);
    lcdData(c[a % 10]);
    lcdCmd((1 << 7) | 7);
    lcdData(c[b / 1000]);
    lcdData(c[(b / 100) % 10]);
    lcdData(c[(b / 10) % 10]);
    lcdData(c[b % 10]);

    lcdCmd((1 << 7) | 67);
    if (d < 0)
    {
        lcdData('-');
        d = -d;
    }
    lcdData(c[d / 10000]);
    lcdData(c[(d / 1000) % 10]);
    lcdData(c[(d / 100) % 10]);
    lcdData(c[(d / 10) % 10]);
    lcdData(c[d % 10]);
}

uint8_t readAdc(uint8_t channel)
{
    ADMUX = MUX | (1 << ADLAR) | channel;
    _delay_us(10);
    ADCSRA |= _BV(ADSC);
    while (BIT_IS_CLEAR(ADCSRA, ADIF))
        ;
    ADCSRA |= _BV(ADIF);
    return ADCH;
}

int main(void)
{
    lcdInit();
    // my_lcdInit();

    // Инициализация портов ввода/вывода
    DDRB = _BV(5);
    DDRG = _BV(3);
    // Инициализация АЦП
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // Инициализация таймера 1. Быстрая ШИМ 10 бит
    TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(CS10);
    /* переменные магнитного поля, ошибки регулирования и
    управления (здесь будут ещё переменные, например,
    значение ошибки регулирования на предыдущем шаге)*/
    int16_t field, error = 0, preerr = 0, ref, control;
    int16_t p = 0, i = 0, d = 0;
    int16_t help = 0;
    // PORTG = (1 << 3);
    // OCR1A = 255;
    while (1)
    {
        field = (readAdc(1) + readAdc(1) + readAdc(1)) / 3;
        help = ((readAdc(3) + readAdc(3) + readAdc(3)) / 3) * 90 / 255;
        ref = 50;
        error = ref - field;

        p = error;
        d = error - preerr;
        i += error;
        control = p * help/10 + i * 0 + d * 0;
        my_out(field, ref, control);
        if (field > 95)
        {
            PORTG |= (1 << 3);
            OCR1A = 100;
            
        }

        if (control >= 0)
        {   
            if (control > 1023){control = 1023;}
            PORTG &= ~(1 << 3); // притягивание при 255
            OCR1A = control;
        }
        else
        {   
            control = -control;
            if (control > 1023){control = 1023;}
            PORTG |= (1 << 3); // отталкивание
            OCR1A = control;
        }

        _delay_ms(10);
    }
}