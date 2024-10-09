// /*
//  * test3.c
//  *
//  * Created: 20.09.2023 22:15:18
//  * Author : stnikan
//  */

#include <avr/io.h>
#include <util/delay.h> //для использования пауз
#include <avr/interrupt.h>

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


int main(void)
{
    // sei();
    lcdInit();

    char Start_str[] = {'A','D','C',':',' '};
    lcdCmd((1 << 7) | 0);
    for (uint8_t i = 0; i < 5; i++)
    {
        lcdData(Start_str[i]);
    }

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
    uint16_t my_red = 0;
    char c[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
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
        OCR3BH = read_adc(3)>>8;
        OCR3BL = read_adc(3);
        my_red = read_adc(3);
        
        
        lcdCmd((1 << 7) | 5);
        lcdData(c[my_red / 1000]);
        lcdData(c[(my_red / 100) % 10]);
        lcdData(c[(my_red / 10) % 10]);
        lcdData(c[my_red % 10]);
        //OCR3AL = read_adc(3);
        //_delay_ms(100);
    }
}