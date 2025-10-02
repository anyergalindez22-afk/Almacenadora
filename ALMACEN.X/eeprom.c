/* #include "eeprom.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/02/2021
* Electronica y Circuitos
*
*/

#include "eeprom.h"

void EEPROM_Write(unsigned char dir, unsigned char data)
{
    EEADR = dir;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    while(!PIR2bits.EEIF);
    PIR2bits.EEIF = 0;
    EECON1bits.WREN = 0;
}

unsigned char EEPROM_Read(unsigned char dir)
{
    EEADR = dir;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

void EEPROM_Clear(void)
{
    for(int i=0; i<256; i++)
    {
        EEPROM_Write(i, 0xFF);
    }
}

#ifdef USE_RW_16
void EEPROM_Write_16(unsigned char dir, int data)
{
    for(short i=0; i<=1; i++)
    {
        EEPROM_Write(dir+i,*((char*)(&data)+i)); 
    }
}

int EEPROM_Read_16(unsigned char dir)
{
    int data;
    for(short i=0; i<=1; i++)
    {
        *((char*)(&data)+i) = EEPROM_Read(dir+i);
    }
    return(data);
}
#endif

#ifdef USE_RW_32
void EEPROM_Write_32(unsigned char dir, long data)
{
    for(short i=0; i<=3; i++)
    {
        EEPROM_Write(dir+i,*((char*)(&data)+i)); 
    }
}

long EEPROM_Read_32(unsigned char dir)
{
    long data;
    for(short i=0; i<=3; i++)
    {
        *((char*)(&data)+i) = EEPROM_Read(dir+i);
    }
    return(data);
}
#endif

#ifdef USE_RW_FLOAT
void EEPROM_Write_Float(unsigned char dir, float data)
{
    for(short i=0; i<=3; i++)
    {
        EEPROM_Write(dir+i,*((char*)(&data)+i)); 
    }
}

float EEPROM_Read_Float(unsigned char dir)
{
    float data;
    for(short i=0; i<=3; i++)
    {
        *((char*)(&data)+i) = EEPROM_Read(dir+i);
    }
    return(data);
}
#endif