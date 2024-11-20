#include <avr/io.h>
// #define F_CPU 11059200
#include <util/delay.h> //для использования пауз

#include <avr/interrupt.h>

#define _BV(b) (1 << (b))
#define MUX (_BV(REFS0))
#define BIT_IS_CLEAR(Reg, b) ((Reg & _BV(b)) == 0)

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

    // my_lcdInit();

    // Инициализация портов ввода/вывода
    DDRB = _BV(5);
    DDRG = _BV(3);
    // Инициализация АЦП
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // Инициализация таймера 1. Быстрая ШИМ 10 бит
    TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(CS10);

    DDRE = (1 << 3) | (1 << 4) | (1 << 5);
    TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | _BV(WGM30) | _BV(WGM31);
    TCCR3B = _BV(WGM32) | _BV(CS10);
    /* переменные магнитного поля, ошибки регулирования и
    управления (здесь будут ещё переменные, например,
    значение ошибки регулирования на предыдущем шаге)*/
    float field, error = 0, pre_err = 0, ref;
    float control;
    int16_t out;
    float p = 0, i = 0, d = 0;
    int16_t help = 0;
    PORTG &= ~(1 << 3); // притягивание при 255
    // PORTG = (1 << 3);
    // OCR1A = 255;
    while (1)
    {
        field = (readAdc(1) + readAdc(1) + readAdc(1)) / 3;
        help = ((readAdc(3) + readAdc(3) + readAdc(3)) / 3)*80/255  ; // выравнивание макс управления
        ref = 70;
        pre_err = error;
        error = ref - field;
        

        p = error;
        d = error - pre_err;
        i += error;


        control = (p * 20) + (i * 0.0012) + (d * 1500.00);
        
        
        if (field > 90){
            control = -100;
        }

       
        if (control >= 0)
        {   
            
            if (control >= 512)
            {
                out = 1023;
                OCR3A = 0;
                OCR3C = 100;
            }
            else
            {
                out = 512 + control;
                OCR3A = 0;
                OCR3C = 100;
            }
        }
        else
        {   
            
            if (control <= -512)
            {
                out = 0;
                OCR3A = 100;
                OCR3C = 0;
            }
            else
            {   
                OCR3A = 0;
                OCR3C = 0;
                out = 512 + control;
            }
        }
        OCR1A = out;
        
        //my_out(field, help, error, out);
    }
}