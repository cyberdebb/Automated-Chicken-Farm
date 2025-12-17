#include "configs.h"

//definicao de variaveis de saida
#define motor_agua_abrindo PORTCbits.RC0 
#define motor_agua_fechando PORTCbits.RC1 
#define motor_comida_abrindo PORTCbits.RC2 
#define motor_comida_fechando PORTCbits.RC3 
#define refrigeracao PORTCbits.RC4 
#define aquecimento PORTCbits.RC5
#define luzes PORTCbits.RC6
#define reposicao PORTCbits.RC7
#define buzzer PORTBbits.RB7

//definicao de variaveis de entrada
__bit agua_aberta, agua_fechada, comida_aberta, comida_fechada;
__bit agua, comida;
__bit sensor_noite;

//criacao de variaveis
char buffer[20];
int lux, adc;
double volt, temp;
int conta = 0;
int noite = 0;

void __interrupt() interrupcao() {    
    if (PIR1bits.TMR1IF) { //foi a interrupcao de estouro do timer1 que chamou a int?
        PIR1bits.TMR1IF = 0; //reseta o flag da interrupcao
        TMR1L = 0xDC;        //reinicia contagem com 3036
        TMR1H = 0x0B;        
        
        //comandos pra tratar a interrupcao
        conta++;
        if (conta==60){ //30 segundos
            reposicao=1;
            
            //atualiza entradas da agua e da comida
            agua = PORTBbits.RB5; 
            comida = PORTBbits.RB6;
            
            if(agua==1) { // se houver falta no peso da agua
                motor_agua_abrindo=1;
                
                //enquanto o motor nao tiver aberto o container de agua, atualiza a entrada
                agua_aberta = PORTBbits.RB2; 
                while (agua_aberta == 1) { 
                    CLRWDT();
                    agua_aberta = PORTBbits.RB2; 
                }
                motor_agua_abrindo=0;
                
                //enquanto o motor nao tiver fechado o container de agua, atualiza a entrada
                agua_aberta = PORTBbits.RB2;
                if (agua_aberta == 0) {
                    motor_agua_fechando = 1; 
                
                    agua_fechada = PORTBbits.RB1; 
                    while (agua_fechada == 1) {
                        CLRWDT();
                        agua_fechada = PORTBbits.RB1;
                    }
                    motor_agua_fechando=0;
                }
            }
            
            if(comida==1) { // se houver falta no peso da comida
                motor_comida_abrindo=1;
                
                //enquanto o motor nao tiver aberto o container de comida, atualiza a entrada
                comida_aberta = PORTBbits.RB4; 
                while (comida_aberta == 1) {
                    CLRWDT();
                    comida_aberta = PORTBbits.RB4; 
                }
                motor_comida_abrindo=0;
                
                //enquanto o motor nao tiver fechado o container de comida, atualiza a entrada
                comida_aberta = PORTBbits.RB4; 
                if (comida_aberta == 0) {
                    motor_comida_fechando = 1; 
                
                    comida_fechada = PORTBbits.RB3; 
                    while (comida_fechada == 1) {
                        CLRWDT();
                        comida_fechada = PORTBbits.RB3;
                    }
                    motor_comida_fechando=0;
                }
            }       
            conta=0;
            reposicao=0;
        }
    }
    
    if (INTCONbits.INTF) { //interrupcao do pino rb0
        luzes=0; //desliga as luzes
        noite=1; //atualiza a variavel responsavel por confirmar a noite
        T1CONbits.TMR1ON = 0; //desliga o timer1
        INTCONbits.INTF=0;
    } 
}

void main() {
    Init(); //inicia os modulos de configuracao

    while(1) {     
        CLRWDT();
                
        //le o valor analogico da temperatura e converte para o valor real (10 bits)
        adc = ADC_Read(0); 
        volt = adc*0.00488281;  
        temp = volt*100;
        
        //se a temperatura tiver saido do estado critico e o buzzer estiver ligado, desliga o buzzer
        if (buzzer==1 && (int)temp<45 && (int)temp>5) buzzer=0;
        
        CLRWDT();
        
        //mostra no display lcd a temperatura
        Lcd_Clear();    
        sprintf(buffer,"Temp: %.0f.0", temp);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(buffer);
        __delay_ms(1000);
        
        if ((int)temp>45 || (int)temp<5) {//se a temperatura estiver critica
            buzzer=1;
        }
        if ((int)temp>28) { //temperatura acima do ideal
            refrigeracao=1;
        }
        else if ((int)temp<18){ //temperatura abaixo do ideal
            aquecimento=1;
        }
        else{ //temperatura ideal (18-28 graus)
            refrigeracao=0;
            aquecimento=0;
        }
        
        CLRWDT();
        
        //le o valor analogico da iluminacao e o converte
        adc = ADC_Read(1); 
        adc = (adc/3) - 1;
        if (adc > 150) lux = 0;
        else lux = 1;
        
        CLRWDT();
        
        //mostra no display lcd a quantidade de iluminacao natural
        Lcd_Clear(); 
        sprintf(buffer,"Ilum Natural");
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(buffer);
        if (lux == 1) sprintf(buffer,"Alta");
        else sprintf(buffer,"Baixa");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(buffer);
        __delay_ms(1000);
        
        //se o sensor noite nao estiver acionado e houver falta de luz, liga as luzes da granja
        //se o sensor noite estiver acionado, desliga as luzes independentemente da falta de luz
        if (lux == 0 && noite == 0) luzes=1;
        else luzes=0;
        
        if (noite == 1) { // checa se o sensor da noite ainda está acionado
            sensor_noite = PORTBbits.RB0;
            if (sensor_noite == 1) {
                noite=0; //reinicia a variavel
                Timer1_Init(); //reinicia o timer1
            }
        }
    }
}