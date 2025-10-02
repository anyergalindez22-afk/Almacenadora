#include <xc.h>
#define _XTAL_FREQ 4000000

//#define USE_RW_16
//#define USE_RW_32
//#define USE_RW_FLOAT

void EEPROM_Write(unsigned char dir, unsigned char data);
unsigned char EEPROM_Read(unsigned char dir);
void EEPROM_Clear(void);

#ifdef USE_RW_16
void EEPROM_Write_16(unsigned char dir, int data);
int EEPROM_Read_16(unsigned char dir);
#endif

#ifdef USE_RW_32
void EEPROM_Write_32(unsigned char dir, long data);
long EEPROM_Read_32(unsigned char dir);
#endif

#ifdef USE_RW_FLOAT
void EEPROM_Write_Float(unsigned char dir, float data);
float EEPROM_Read_Float(unsigned char dir);
#endif