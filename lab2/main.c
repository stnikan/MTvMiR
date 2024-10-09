// /*
//  * test3.c
//  *
//  * Created: 20.09.2023 22:15:18
//  * Author : stnikan
//  */

#include <avr/io.h>
// #define F_CPU 11059200
#include <util/delay.h> //для использования пауз

#include <avr/interrupt.h>

#define E 6
#define RS 7

uint8_t my_color = 0;
uint8_t myColorRed = 0;
uint8_t myColorGreen = 0;
uint8_t myColorBlue = 0;

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

ISR(INT0_vect) // прерывания по инкодеру
{
    int8_t delta = 0;
    if ((PIND & (1 << 0)) != 0)
    {
        EICRA = (1 << ISC01) | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            delta = 5;
        else
            delta = -5;
    }
    else
    {
        EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);
        if ((PIND & (1 << 1)) != 0)
            delta = -5;
        else
            delta = 5;
    }
    switch (my_color)
    {
    case 0:
        myColorRed += delta;
        break;
    case 1:
        myColorGreen += delta;
        break;
    case 2:
        myColorBlue += delta;
        break;

    default:
        break;
    }
}
ISR(INT2_vect) { my_color = (my_color + 1) % 3; }

uint8_t TabCon[] = {0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45, 0xA3, 0xA4,
                    0xA5, 0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50, 0x43, 0x54, 0xA9,
                    0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE, 0x62, 0xAF, 0xB0, 0xB1,
                    0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB,
                    0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63, 0xBF, 0x79, 0x5C, 0x78, 0xE5, 0xC0,
                    0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7};
uint8_t Code(uint8_t symb)
{
    //[]------------------------------------------------[]
    //| Назначение: перекодировка символов кириллицы |
    //| Входные параметры: symb – символ ASCII |
    //| Функция возвращает код отображения символа |
    //[]------------------------------------------------[]
    uint8_t a = symb >= 192 ? TabCon[symb - 192] : symb;
    return a;
}

void my_print(void)
{
    char c[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    lcdCmd((1 << 7) | 2);
    lcdData(c[myColorRed / 100]);
    lcdData(c[(myColorRed / 10) % 10]);
    lcdData(c[myColorRed % 10]);
    lcdCmd((1 << 7) | 7);
    lcdData(c[myColorGreen / 100]);
    lcdData(c[(myColorGreen / 10) % 10]);
    lcdData(c[myColorGreen % 10]);
    lcdCmd((1 << 7) | 12);
    lcdData(c[myColorBlue / 100]);
    lcdData(c[(myColorBlue / 10) % 10]);
    lcdData(c[myColorBlue % 10]);
}

int main(void)
{
    sei();
    lcdInit();

    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT2);
    EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC21);

    DDRE = (1 << 3) | (1 << 4) | (1 << 5);
    /* Инициализация таймера №3. 8-ми битная быстрая
    ШИМ, преддедитель на 8 */
    TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30);
    TCCR3B = (1 << WGM32) | (1 << CS31);
    OCR3AH = 0;
    OCR3AL = 128;

    char Start_str[] = {'R', ':', ' ', ' ', ' ', 'G', ':', ' ', ' ', ' ', 'B', ':'};
    lcdCmd((1 << 7) | 0);
    for (uint8_t i = 0; i < 12; i++)
    {
        lcdData(Start_str[i]);
    }

    while (1)
    {
        my_print();
        OCR3A = myColorBlue;
        OCR3B = myColorGreen;
        OCR3C = myColorRed;
        _delay_us(100);
    }
}