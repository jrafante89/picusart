/* 
 * File:   LCD8.h
 * Author: junior
 *
 * Created on 20 de Abril de 2015, 19:35
 * 
 * Para importar essa biblioteca é necessario definir e configurar como saidas os pinos EN RS e o port porta
 * Funções:
 * init_lcd() Inicia o lcd
 * escreve_str_lcd(x,y,"string") escreve string na posição x e y do display
 * Comando_lcd(comando) manda um comando para o lcd que pode ser:
 * limpa_lcd
 * inicio
 * desliga_lcd
 * liga_lcd
 * desliga_cursor
 * liga_cursor
 * pisca_cursor
 */

#ifndef LCD8_H
#define	LCD8_H

#define limpa_lcd 0b00000001
#define inicio 0b00000010
#define desliga_lcd 0b00001000
#define liga_lcd 0b00001100
#define desliga_cursor 0b00001100
#define liga_cursor 0b00001110
#define pisca_cursor 0b00001111

void delay_ms_x10(int tempo);
void dados(char chr, int rw);
void init_lcd(void);
void escreve_str_lcd(char x, char y, const unsigned char *str);
void comando_lcd(char comando);


void delay_ms_x10(int tempo)
 {
     int t = 0;
     while(t<tempo)
     {
         __delay_ms(10);
         t++;
     }
     t=0;
 }
void dados( unsigned char chr, int rw)
 {
     EN = 0;
     RS = rw;
     
     porta=chr;
          
     EN = 1;
     __delay_ms(5);
     EN = 0;
     RS =0;
     
 } 
void init_lcd(void)
{
     delay_ms_x10(3);
     
     dados(0b00111000, 0);
     dados(0b00001110, 0);
     __delay_ms(5);
     dados(0b00000110, 0);
     
     dados(0b00110000, 0);
     dados(0b00110000, 0);
     dados(0b00110000, 0);
     dados(0b00111000, 0);
     dados(0b00000001, 0);
     dados(0b00000110, 0);
     
}
void escreve_str_lcd(char x, char y, const unsigned char *str)
{
    char addressy, addressx ;

    if (y==1)
    {
        addressy = 0x80;        
    }
    else if (y==2)
    {
        addressy = 0xc0;
    }
    else if (y==3)
    {
        addressy = 0x90;
    }
    else if (y==4)
    {
        addressy = 0xd0;
    }
    
    
    addressx =addressy + x-1;
    
    dados(addressx, 0);
    
    while(*str!='\0')
    {
        
        dados(*str,1);
        str++;
    }
}

void comando_lcd (char comando)
{
    dados(comando,0);
}


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LCD8_H */

