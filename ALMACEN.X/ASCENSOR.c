#include "CONFIGURACION.h"
#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000


#define IN PORTBbits.RB0 //Originalemnte PA Y PP VAN EN UN MISMO PIN    
#define EJEX PORTBbits.RB1
//#define FCIZ PORTBbits.RB2
//#define CEXT PORTBbits.RB3
#define IZQP PORTBbits.RB4
#define DERP PORTBbits.RB5
#define ARRIBA PORTBbits.RB6
#define ABAJO PORTBbits.RB7

#define EXT LATCbits.LC0
#define PZA0 PORTCbits.RC1
#define PZA1 PORTCbits.RC2
#define OUT PORTCbits.RC6

#define FCY1 PORTAbits.RA1
#define FCX1 PORTAbits.RA2
#define FCZ1 PORTAbits.RA3
#define EJEY PORTAbits.RA4
#define FCZ2 PORTAbits.RA5
//#define ROT LATAbits.LA6

#define IZQ LATDbits.LATD0
#define DER LATDbits.LATD1
#define SUB LATDbits.LATD2
#define BAJ LATDbits.LATD3

void init_ports(){
     ADCON1bits.PCFG=0x0F;// salida Digital
     TRISB=0XFF;
     TRISA=0XFF;
     TRISD=0X00;
     TRISE=0X00;
     LATA=0X00;
     LATB=0X00;
     LATC=0X00;
     LATD=0X00;
     LATE=0X00;
     TRISC=0XFF;
     TRISCbits.RC0=0;
     TRISCbits.RC6=0;
     return;
}
void main(void) {
    init_ports();
    __delay_ms(200);
    BAJ=1;
    while(FCY1==1);
    BAJ=0;
    IZQ=1;
    while(FCX1==1);
    IZQ=0;
    while(IN==0);
    __delay_ms(500);
    DER=1;
    __delay_ms(1000);
    DER=0;
    
    return;
}
