#include "CONFIGURACION.h"
#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000

#define IN PORTBbits.RB0 //Originalemnte PA Y PP VAN EN UN MISMO PIN    
#define EJEX PORTBbits.RB1
//#define FCIZ PORTBbits.RB2
//#define CEXT PORTBbits.RB3
#define IZQ PORTBbits.RB4
#define DER PORTBbits.RB5
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

#define IZQUIERDA LATDbits.LATD0
#define DERECHA LATDbits.LATD1
#define SUBIR LATDbits.LATD2
#define BAJAR LATDbits.LATD3

char METAL=0, BLANCA=0, NEGRA=0;

void init_ports(void){
    ADCON1bits.PCFG=0x0F;// salida Digital
    TRISB=0XFF;
    TRISA=0XFF;
    TRISD=0X00;
    TRISE=0X00;
    TRISC=0XFF;
    TRISCbits.RC0=0;
    TRISCbits.RC6=0;
     
    LATA=0X00;
    LATB=0X00;
    LATC=0X00;
    LATD=0X00;
    LATE=0X00;
    return;
}

void init_int(void){
    RCONbits.IPEN= 1;                //HABILITAS LAS PRIORIDADES
    T0CON = 0B01111000;              //TMR0 CONFIGURADO COMO CONTADOR A PARTIR DE LOS PULSOS DESCENDENTES EN EL PIN TOCKI
    INTCON = 0B10101000;             //CONFIGURA LAS INTERRUPCIONES PARA PODER INTERRUMPIR POR OVERFLOW DEL TMR0 O CAMBIO EN EL PORTB
    INTCON2bits.TMR0IP = 1;          //PRIORIDAD ALTA PARA TMR0
    return;
}

void reposo(void){
	if(FCX1==1){
		BAJAR=1;
    	while(FCY1==1);
    	BAJAR=0;	
	}	
	
    if(FCY1==1){
    	IZQUIERDA=1;
    	while(FCX1==1);
    	IZQUIERDA=0;	
	}
    return;
}

void detectar(void){                //DESBORDE EN 256 DEL CONTADOR, ALTURA INICIAL 0
	if(PZA0==0 && PZA1==1){
		METAL=1;
		TMR0L= 250;                 //PRECARGA EL CONTADOR PARA DESBORDARSE EN 4+2 PULSOS
        T0CONbits.TMR0ON = 1;
	}
	else if(PZA0==1 && PZA1==0){
		BLANCA=1;
        TMR0L= 233;                 //PRECARGA EL CONTADOR PARA DESBORDARSE EN 21+2 PULSOS
        T0CONbits.TMR0ON = 1;
	}
	else if(PZA0==0 && PZA1==0){
		NEGRA=1;
        TMR0L= 222;                 //PRECARGA EL CONTADOR PARA DESBORDARSE EN 32+2 PULSOS 
        T0CONbits.TMR0ON = 1;
	}
    return;
}

void __interrupt(high_priority) Stop(void){
INTCONbits.GIEH = 0;
T0CONbits.TMR0ON = 0;
SUBIR=0;
BAJAR=0;
INTCONbits.TMR0IF = 0;
INTCONbits.GIEH = 1;
}

void dejar(){ //PRIMERO MOVER A LA DERECHA Y LUEGO HACIA ARRIBA, PARA MOVER A LA DERECHA ES NECESARIO CONSULTAR LA MEMORIA
	//METER PARA DESPLAZAR A LA DERECHA EN BASE A LA INFORMACION EN LA MEMORIA
    DERECHA=1
    DERECHA=0;
    
    SUBIR= 1;
    while(SUBIR==1);               //ESPERA A QUE HAYA SUBIDO PARA CONTINUAR
    __delay_ms(500);
    EXT=1;
    __delay_ms(250);
    TMR0L= 253;                    //PRECARGA EL CONTADOR PARA DESBORDARSE EN 3 PULSOS 
    T0CONbits.TMR0ON = 1;
    BAJAR=1;
    while(BAJAR==1);               //ESPERA A QUE HAYA BAJADO PARA CONTINUAR
    __delay_ms(500);
    EXT=0;
    __delay_ms(200);
    return;
}

void main(void) {
    init_ports();
    init_int();
    __delay_ms(200);
    
    while(1){
    while(IN==1){
		if(FCX1==1 || FCY1==1){ //ASEGURAR BUENA LOGICA DE ACTIVACION DE LA FUNCI0N!!
    		reposo();  			     //SE LLEVA A LA POSICION INICIAL, ESTO SEIMPRE QUE SE ESPERE POR INGRESAR OTRA PIEZA AL ALMACEN
		}
    };	
    detectar();	  			     //SE DETECTA LA PIEZA EN BASE A COMO COLOQUEMOS LOS SWITCHES PZA0 Y PZA1}
    if(METAL==1 || BLANCA==1 || NEGRA==1){  //REALMENTE ESTE IF SE PUEDE OBVIAR DE ALGUNA FORMA PROGRAMANDO MEJOR
    dejar();    
     }
    else {
        //ESCRIBIR EN PANTALLA ALGO COMO NO HAY PIEZA O VACIO
    }
	};
	return;
}

