

#include <xc.h>
#include <plib/usart.h>
#include <plib/pwm.h>
#include <stdlib.h>
#include <stdio.h>
#include <plib/adc.h>

#define _XTAL_FREQ 48000000  //frequencia do pic em 48Mhz
#define RS RD0               //define o pino RS do diplay no RD0
#define EN RD1               //define o pino EN do display no RD1
#define porta PORTB          //define o todos os pino do port b para a entrada de dados no diplay (display funcionando com 8 bits)
#define freqPWM 0b10111011   //config 4khz com t2 preescale 16

#include "LCD8.h"
char rx1[4],rx[3], *valorstr; //rx1 recebe bytes do serial, rx: variavel de debug 
    float a=0, ton=0, valor = 0, volt =0; 
    int status, flag = 0;
    
        
        
void Board_Init (void);
void init_uart(void);

void interrupt RS232(void)//vetor de interrupção
 {
     getsUSART(rx1,3 );
     flag = 1;      
     PIR1bits.RCIF = 0;//  limpa flag de interrupção de recepção
 }

void main(void)
{
   
    //char rx="testando";
    
    
    Board_Init();
    init_lcd();
    init_uart();
    OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_20_TAD, ADC_CH0 & ADC_INT_OFF & ADC_REF_VDD_VSS , ADC_1ANA );
    OpenPWM1(freqPWM);
    OpenPWM2(freqPWM);
    
    SetDCPWM1(ton);
    SetDCPWM2(ton);
    //*texto=rx;
    //escreve_str_lcd(1,1,texto);
    escreve_str_lcd(1,1,"teste PWM");
    escreve_str_lcd(1,2,"test prog Linux");
    rx1[0]=0;
    rx1[1]=0;
    rx1[2]=0;
    rx1[3]=0;
    rx1[4]=0;
    while(1)
    {
                   
        if (flag == 1)
        {
           escreve_str_lcd(1,2,"    ");
           escreve_str_lcd(1,3,"    ");
           escreve_str_lcd(1,4,"    ");
           delay_ms_x10(10);
           escreve_str_lcd(1,2,rx1);
           delay_ms_x10(10);
           
           a=atoi((char *)rx1);
           itoa(rx,a,10);
           escreve_str_lcd(1,3,rx);
           delay_ms_x10(10);
           ton=a*1023/100;
            
           SetDCPWM1(ton);
           SetDCPWM2(ton);
           itoa(rx,ton,10);
           escreve_str_lcd(1,4,rx);
           delay_ms_x10(10);
           
           
        
        rx1[0]=0;
        rx1[1]=0;
        rx1[2]=0;
        rx1[3]=0;
        rx1[4]=0;
        
        flag = 0;
        }
       /*
        if (DataRdyUSART()!=0)
        {
           
           getsUSART(rx1,3 );
           
           
                      
           escreve_str_lcd(1,2,"    ");
           escreve_str_lcd(1,3,"    ");
           escreve_str_lcd(1,4,"    ");
           delay_ms_x10(10);
           escreve_str_lcd(1,2,rx1);
           delay_ms_x10(10);
           
           a=atoi((char *)rx1);
           itoa(rx,a,10);
           escreve_str_lcd(1,3,rx);
           delay_ms_x10(10);
           ton=a*1023/100;
            
           SetDCPWM1(ton);
           SetDCPWM2(ton);
           itoa(rx,ton,10);
           escreve_str_lcd(1,4,rx);
           delay_ms_x10(10);
           
           
        
        rx1[0]=0;
        rx1[1]=0;
        rx1[2]=0;
        rx1[3]=0;
        rx1[4]=0;
           
        }
        */
        ConvertADC();         // Start conversion
        while( BusyADC() );   // Wait for completion
        valor = ReadADC();
        volt=valor*5/1023;
        valorstr=ftoa(volt,status);
        escreve_str_lcd(8,4,"      ");
        escreve_str_lcd(8,4,valorstr);
        delay_ms_x10(100);
        
        
        /*
        escreve_str_lcd(1,1,"LCD line 1");
        delay_ms_x10(100);
        escreve_str_lcd(1,2,"LCD line 2");
        delay_ms_x10(100);
        escreve_str_lcd(1,3,"LCD line 3");
        delay_ms_x10(100);
        escreve_str_lcd(1,4,"LCD line 4");
        delay_ms_x10(100);
        comando_lcd(desliga_lcd);
        delay_ms_x10(100);
        comando_lcd(liga_lcd);
        delay_ms_x10(100);
        comando_lcd(desliga_cursor);
        delay_ms_x10(100);
        comando_lcd(liga_cursor);
        delay_ms_x10(100);
        comando_lcd(pisca_cursor);
        delay_ms_x10(100);
        comando_lcd(inicio);
        delay_ms_x10(100);
        comando_lcd(limpa_lcd);
        delay_ms_x10(100);
        */
        
    }
   
}
void Board_Init (void)
{
        RCON = 0X80;        // LIMPA REGISTRO DE RESET
        ADCON1 = 0x0F;      // CONFIGURA TODAS AS PORTAS ANALOGICAS COMO I/O
        CMCON = 0x0F;       // DESABILITA COMPARADOR
        T0CON = 0b11000101; // timer ativo, 8 bits,clock interno, preescale 1:64
        T2CON = 0b00000111; // timer 2 ativo, preescale 1:16 para o pwm
        
        
        TRISA = 0b00000001;
        TRISB = 0;          // todos Saida
        TRISC = 0b10000000; //rc7 rx (entrada) rc6 tx (saida)
        TRISD = 0;
        TRISE = 0;
                      

        PORTAbits.RA1 = 0;          // nível 0 saída
        PORTAbits.RA2 = 0;
        PORTAbits.RA3 = 0;
        PORTAbits.RA4 = 0;
        PORTAbits.RA5 = 0;
        //PORTAbits.RA6 = 0;
        //PORTAbits.RA7 = 0;
        PORTB = 0;
        PORTCbits.RC0 = 0;
        PORTCbits.RC1 = 0;
        PORTCbits.RC2 = 0;
        PORTCbits.RC3 = 0;
        PORTCbits.RC4 = 0;
        PORTCbits.RC5 = 0;
        PORTCbits.RC6 = 0;
        PORTD = 0;
        PORTE = 0;
}
void init_uart()
{
    unsigned char config =0, spbrg =0, baudconfig=0;
    
    CloseUSART();
    
    INTCONbits.GIE     = 1;  /* Set GIE. Enables unmasked interrupts */
    INTCONbits.PEIE   = 1;  /* Set GIEL. Enables all unmasked peripheral interrupts */
    IPR1bits.RCIP      = 1; //EUSART Receive Interrupt Priority bit
    
    
    config = USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW;
    
    spbrg = 77;
    
    OpenUSART(config, spbrg);
   
    PIR1bits.RCIF      = 0; //0 = The EUSART receive buffer is empty
    
   //baudconfig = BAUD_IDLE_CLK_HIGH & BAUD_8_BIT_RATE & BAUD_AUTO_OFF ;
    
    //baudUSART (baudconfig);
    
    
    
}
