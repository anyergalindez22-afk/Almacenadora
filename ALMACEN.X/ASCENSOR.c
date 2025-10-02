#include "CONFIGURACION.h"
#include <xc.h>
#include <stdio.h>
#include "lcd.h"
#include "eeprom.h"
#define _XTAL_FREQ 4000000

#define IN PORTBbits.RB0 //Originalemnte PA Y PP VAN EN UN MISMO PIN    
#define EJEX PORTBbits.RB1
#define CLEAR PORTBbits.RB2
#define LLENAR PORTBbits.RB3
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
const char figura_1[8] = {0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1B, 0x1F};
const char figura_2[8] = {0x1B, 0x11, 0x1F, 0x1F, 0x1F, 0x11, 0x1B, 0x00};
char METAL=0, BLANCA=0, NEGRA=0;
char Fila1[4];
char Fila2[4];
char Fila3[4];
char f=0, c=0;

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
    INTCONbits.RBIE=1;               //HABILITAMOS INTERRUPCIONES DE RB
    INTCONbits.RBIF=0;               //BAJAMOS LA BANDERA
    INTCON2bits.RBIP=0;              //BAJA PRIORIDAD PARA RB
    T0CON = 0B01111000;              //TMR0 CONFIGURADO COMO CONTADOR A PARTIR DE LOS PULSOS DESCENDENTES EN EL PIN TOCKI
    INTCON = 0B10101000;             //CONFIGURA LAS INTERRUPCIONES PARA PODER INTERRUMPIR POR OVERFLOW DEL TMR0 O CAMBIO EN EL PORTB
    INTCON2bits.TMR0IP = 1;          //PRIORIDAD ALTA PARA TMR0
    return;
}

void reposo(void){
	if(FCY1==1){
		BAJAR=1;
    	while(FCY1==1);
    	BAJAR=0;	
	}	
	
    if(FCX1==1){
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

void Actualizar_Matriz(){
    Lcd_Set_Cursor(1,12);
    for(char i=0;i<=3;i++){
        if (Fila1[i]==0){
            Lcd_Write_Char(0);
        }
        else
        {
            Lcd_Write_Char(1);
        }
    }
    Lcd_Set_Cursor(2,12);
    for(char i=0;i<=3;i++){
        if (Fila2[i]==0){
            Lcd_Write_Char(0);
        }
        else
        {
            Lcd_Write_Char(1);
        }
    }
    Lcd_Set_Cursor(3,12);
    for(char i=0;i<=3;i++){
        if (Fila3[i]==0){
            Lcd_Write_Char(0);
        }
        else
        {
            Lcd_Write_Char(1);
        }
    }
    Lcd_Set_Cursor(1,12);
    f=1;
    c=12;
}

void guardar(void){
    for (char i=0; i<=3; i++){
        EEPROM_Write(i,Fila1[i]);
    }
    for (char i=0; i<=3; i++){
        EEPROM_Write(i+4,Fila2[i]);
    }
    for (char i=0; i<=3; i++){
        EEPROM_Write(i+8,Fila3[i]);
    }
}

void estado(void){
    char g;
    for (char i=0; i<=3; i++){
        Fila1[i]=EEPROM_Read(i);
    }
    for (char i=0; i<=3; i++){
        Fila2[i]=EEPROM_Read(i+4);
    }
    for (char i=0; i<=3; i++){
        Fila3[i]=EEPROM_Read(i+8);
    }
}

void __interrupt(low_priority) LowISR(void){
    Lcd_Blink();
    if (INTCONbits.RBIF){
       volatile unsigned char dummy = PORTB;
       INTCONbits.RBIF=0;
      
       if (ARRIBA==1 && f>1){      
           f--;
       }
       if (ABAJO==1 && f<3){
           f++;
       }     
       if (IZQ==1 && c>12){
          c--; 
       }
       if (DER==1 && c< 15){
           c++;
       }
       
       Lcd_Set_Cursor(f,c);
    }
}

void __interrupt(high_priority) Stop(void){
INTCONbits.GIEH = 0;
T0CONbits.TMR0ON = 0;
SUBIR=0;
BAJAR=0;
INTCONbits.TMR0IF = 0;
INTCONbits.GIEH = 1;
}

void Modificar(){
    if (LLENAR==1){
        Lcd_Write_Char(1);
        //AQUI DEBERIA MODIFICAR LA MEMORIA, Y EL VECTOR DONDE SE GUARDO
        c++;
        while(LLENAR==1);
    }
    if  (CLEAR==1){
        Lcd_Write_Char(0);
        //AQUI DEBERIA MODIFICAR LA MEMORIA, Y EL VECTOR DONDE SE GUARDO
        c++;
         while(CLEAR==1);
    }
    Lcd_NoBlink();
    
}

void dejar(){ //PRIMERO MOVER A LA DERECHA Y LUEGO HACIA ARRIBA, PARA MOVER A LA DERECHA ES NECESARIO CONSULTAR LA MEMORIA
	//METER PARA DESPLAZAR A LA DERECHA EN BASE A LA INFORMACION EN LA MEMORIA
    DERECHA=1;
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
    guardar();
    EXT=0;
    __delay_ms(200);
    return;
}

void main(void) {
    init_ports();
    init_int();
    Lcd_Init();
    
    Lcd_CGRAM_Init();
    Lcd_CGRAM_CreateChar(0,figura_1);   //GUARDAMOS EL CARACTER ESPECIAL PARA MOSTRAR VACIO
    Lcd_CGRAM_CreateChar(1,figura_2);   //GUARDAMOS EL CARACTER ESPECIAL PARA MOSTRAR LLENO
    Lcd_CGRAM_Close();
    
    estado();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("NEGRAS");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("BLANCAS");
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("METAL");
    __delay_ms(200);
    
    while(1){
        Actualizar_Matriz();
        INTCONbits.GIEL=1;
    while(IN==0){
		if(FCX1==1 || FCY1==1){ //ASEGURAR BUENA LOGICA DE ACTIVACION DE LA FUNCI0N!!
    		reposo();  			     //SE LLEVA A LA POSICION INICIAL, ESTO SEIMPRE QUE SE ESPERE POR INGRESAR OTRA PIEZA AL ALMACEN
		}
        if( CLEAR==1 || LLENAR==1){
            Modificar();
        }
    };	
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(1);
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

