#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>   //äëÿ èñïîëüçîâàíèÿ ïðåðûâàíèé

#define DEV_ADDR 0b1100000 // адрес модуля по умолчанию
#define W 0                // признак записи данных в матрицу
#define R 1                // признак чтения данных из матрицы
// регистр состояния диодов столбца
#define COLUMN_DATA_REG 0x01
// регистр обновления состояния диодов столбца
#define UPDATE_COLUMN_REG 0x0C

uint8_t pict1[] = {             0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00011000,
                        0b00011000,
                        0b00000000,
                        0b00000000,
                        0b00000000};

uint8_t pict2[] = {             0b00000000,
                        0b00000000,
                        0b00011000,
                        0b00111100,
                        0b00111100,
                        0b00011000,
                        0b00000000,
                        0b00000000};

uint8_t pict3[] = {             0b00000000,
                        0b00011000,
                        0b00111100,
                        0b01111110,
                        0b01111110,
                        0b00111100,
                        0b00011000,
                        0b00000000};

uint8_t pict4[] = {             0b00011000,
                        0b00111100,
                        0b01111110,
                        0b11111111,
                        0b11111111,
                        0b01111110,
                        0b00111100,
                        0b00011000};

uint8_t pict5[] = {             0b00111100,
                        0b01111110,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b01111110,
                        0b00111100};

uint8_t pict6[] = {             0b01111110,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111,
                        0b01111110};

uint8_t pict_p[] = {    0b00100100,
                        0b00100100,
                        0b11111111,
                        0b00100100,
                        0b00100100,
                        0b11111111,
                        0b00100100,
                        0b00100100,};

void twiStart();
void twiStop();
void twiWrite(uint8_t data);
void matrDrawPict(uint8_t devAddr, uint8_t *dataBuf);
void matrInit(uint8_t devAddr);
int main(void)
{
    uint8_t pict[] = {  0b01000010,
                        0b01000110,
                        0b01001100,
                        0b01110000,
                        0b01110000,
                        0b01001100,
                        0b01000110,
                        0b01000001}; // рисунок – значок копирайта ©
    matrInit(DEV_ADDR);
    matrDrawPict(DEV_ADDR, pict_p);
    uint8_t i = 0;
    while (1){

            matrDrawPict(DEV_ADDR, pict1);
            _delay_ms(100);
            matrDrawPict(DEV_ADDR, pict2);
            _delay_ms(100);
            matrDrawPict(DEV_ADDR, pict3);
            _delay_ms(100);
            matrDrawPict(DEV_ADDR, pict4);
            _delay_ms(100);
            matrDrawPict(DEV_ADDR, pict5);
            _delay_ms(100);
            matrDrawPict(DEV_ADDR, pict6);
            _delay_ms(1000);
            
            


        }

}


void twiStart(void)
{ // состояние СТАРТ
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}

void twiStop(void)
{ // состояние СТОП
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiWrite(uint8_t data)
{ // передача данных в матрицу
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}
// передача изображения по столбцам на матрицу с МК

void matrDrawPict(uint8_t devAddr, uint8_t *dataBuf)
{
    uint8_t i;

    twiStart();
    twiWrite((DEV_ADDR << 1) | W);
    for (i = 0; i <= 7; i++)
    {
        // очередную часть рисунка кладём в очередной регистр
        twiWrite(COLUMN_DATA_REG + i);
        twiWrite(dataBuf[i]);
    }
    // чтобы рисунок был выведен, обновляем регистр 0x0C
    twiWrite(UPDATE_COLUMN_REG);
    twiWrite(0xFF);
    twiStop();
}

void matrInit(uint8_t devAddr)
{
    TWBR = 32; // 200 кГц частота обмена
    /* настройки по умолчанию (матрица 8х8, без
    эквалайзера, 40мА тока) нам подходят, поэтому ничего
    не меняем и не перенастраиваем */
}