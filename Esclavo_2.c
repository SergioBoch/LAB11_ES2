/* 
 * File:   Esclavo_2.c
 * Author: Sergio Boch 20887
 *
 * Created on May 12, 2022, 10:33 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 1000000     
#define FLAG_SPI 0xFF         

uint8_t cont_es;       
void setup(void);

void __interrupt() isr (void){
    if(INTCONbits.RBIF){              
        if(!PORTBbits.RB0){            
            cont_es++;                
        }
        else if(!PORTBbits.RB1){        
            cont_es--;                
        }
        INTCONbits.RBIF = 0;            
    }
    if (PIR1bits.SSPIF){           
        SSPBUF = cont_es;              
        PIR1bits.SSPIF = 0;           
    }
    return;
}

void main(void) {
    setup();
    while(1){        
    }
    return;
}

void setup(void){   
    TRISC = 0b00011000;        
    PORTCbits.RC5 = 0;
       
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    ANSEL = 0x00;
    ANSELH = 0x00;              // I/O digitales
        
    TRISA = 0b00100000;         // SS como entrada
    TRISB = 0b00000011;         // RB0 y RB1 como entradas
    TRISD = 0x00;               // PORTD como salida
    
    PORTA = 0x00;               // Limpieza del PORTA
    PORTB = 0x00;               // Limpieza del PORTB
    PORTC = 0x00;               // Limpieza del PORTC
    PORTD = 0x00;               // Limpieza del PORTD
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.RBIE = 1;        // Habilitación de interrupciones del PORTB
    INTCONbits.RBIF = 0;        // Limpieza bandera de interrupción del PORTB
    
    IOCBbits.IOCB0 = 1;         // Habilitación de interrupción por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitación de interrupción por cambio de estado para RB1
    OPTION_REGbits.nRBPU = 0;   // Habilitación de resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitación de resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitación de resistencia de pull-up de RB1

    SSPCONbits.SSPM = 0b0100;   
    SSPCONbits.CKP = 0;         
    SSPCONbits.SSPEN = 1;       
    SSPSTATbits.CKE = 1;      
    SSPSTATbits.SMP = 0;       

    PIR1bits.SSPIF = 0;        
    PIE1bits.SSPIE = 1;         // Habilitar interrupciones de SPI
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de periféricos
}

