#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>

#define SBIT_ADON     0
#define SBIT_CHS0     3
#define SBIT_ADFM     7

//define pinos referentes a interface com LCD
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#define _XTAL_FREQ 4000000
#include <lcd.h>

#pragma config FOSC = HS        // Oscillator Selection bits 
#pragma config WDTE = ON        // Watchdog Timer Enable bit 
#pragma config PWRTE = ON       // Power-up Timer Enable bit (reset ao ligar))
#pragma config BOREN = ON       // Brown-out Reset Enable bit (reset quando houverem quedas abruptas na fonte de energia) 
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void ADC_Init() {
   //define todas as entradas como analogicas
   ADCON1bits.PCFG0=0;
   ADCON1bits.PCFG1=0;
   ADCON1bits.PCFG2=0;
   ADCON1bits.PCFG3=0;
   
   //10 bits
   ADCON1bits.ADFM = 1;
    
   //define o clock de conversao
   ADCON0bits.ADCS0 = 0  ;   //confirmando default Fosc/2
   ADCON0bits.ADCS1 = 0  ;   //confirmando default Fosc/2
}

int ADC_Read(unsigned char adcChannel){   
    ADCON0bits.ADON=1;
    
    //inicializar valor analogico com 0
    ADRESL = 0x00;          
    ADRESH = 0x00; 
   
    ADCON0bits.CHS0=adcChannel;
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=0;
    
    __delay_ms(1000); //Acquisition Time(Wait for Charge Hold Capacitor to get charged )
    
    ADCON0bits.GO=1;
    __delay_us(10);
    return((ADRESH<<8) + ADRESL); //return right justified 10-bit result
}

void WDT_Init() {
    OPTION_REGbits.PSA = 1;  //define pre-escalar para WDT
    
    // configura a taxa de temporizacao do WatchDog Time(WDT)1:128 - 2304ms
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
     
    CLRWDT();    //reseta a contagem do WDT   
}

void Timer1_Init() {
    T1CONbits.TMR1CS = 0;   //Define timer 1 como temporizador (Fosc/4)
    T1CONbits.T1CKPS0 = 1;  //bit pra configurar pre-escaler, nesta caso 1:8
    T1CONbits.T1CKPS1 = 1;  //bit pra configurar pre-escaler, nesta caso 1:8
        
    TMR1L = 0xDC;          //carga do valor inicial no contador (65536-62500)
    TMR1H = 0x0B;          //3036. Quando estourar contou 62500, passou 0,5s   
   
    T1CONbits.TMR1ON = 1;   //liga o timer
}

void Pins_Init() {
    TRISA = 0xFF;   
    TRISB = 0x7F;   
    TRISC = 0x00;
    TRISD = 0x00;
    
    OPTION_REGbits.nRBPU = 0; //Ativa resistores de pull-ups
   
    //configurando interrupcoes
    INTCONbits.GIE=1;       //Habiliita a int global
    INTCONbits.INTE = 1; //interrupcao do rb0
    OPTION_REGbits.INTEDG = 0; //borda de descida
    INTCONbits.PEIE = 1;    //Habilita a int dos perif?ricos
    PIE1bits.TMR1IE = 1;    //Habilita int do timer 1
    
    PORTC = 0;
    PORTD = 0;
}

void Init() { // inicia todos os modulos
    Pins_Init();
    ADC_Init();
    Lcd_Init();
    Timer1_Init();
    WDT_Init();
}