/*******************************************************
Project : Send and receive two-way radio message(NAVA)
Version : 1.01
Date    : 12/25/2023
Author  : Arshia Madadi
Company : SCU - University
Comments: 


Chip type               : ATmega16A
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>

//Paye hay KeyPad
//Pin Keypad

#define   ROW1   PORTB.3
#define   ROW2   PORTB.4
#define   ROW3   PORTB.5
#define   ROW4   PORTB.6
#define   C1     PINB.0
#define   C2     PINB.1
#define   C3     PINB.2  

unsigned char str[16]; 
int num  , number ; 
bit p ; //  as pressed 

    //Code Keypad
        
void show()
{
    if(p == 1)
    {
        p = 0 ; 
        number = (number * 10) + num ; 
        lcd_clear();
        lcd_gotoxy(0,0);
        sprintf(str,"number = %4d",number);   //DAST BE NAMBER NAZAN KHARAB MISHE
        lcd_puts(str);            
    }   
}
void keyboard()
    {
    ROW1 = 0 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
        if(C1==0) num = 1 ,  p = 1  ;   // P AS PRESSED
        if(C2==0) num = 2  , p = 1;
        if(C3==0) num = 3  , p = 1  ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
    ROW1 = 1 ; 
            
    show();            
                        
    ROW2 = 0 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
        if(C1==0) num = 4  , p = 1 ;
        if(C2==0) num = 5  , p = 1 ;
        if(C3==0) num = 6  , p = 1 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
    ROW2 = 1 ; 

    show();                        

    ROW3 = 0 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
        if(C1==0) num = 7 , p = 1 ;
        if(C2==0) num = 8 , p = 1; 
        if(C3==0) num = 9 , p = 1  ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
    ROW3 = 1 ; 
           
    show();             
                       
    ROW4 = 0 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
        if(C2==0) num = 0  , p = 1 ;
    delay_ms(50);   // in normal we use 2ms but proteus can not detect
    ROW4 = 1 ; 
            
    show();            

    }
            
  
// Interrupt Khareji 0
//External Interrupt 0
         
//  Baray pak kardan LCD
//For Clear LCD


interrupt [EXT_INT0] void ext_int0_isr(void) // clear mode 
{
        number = 0 ; 
        lcd_clear();
                        //Sefr kardan Number
                        //pak kardan LCD
                        //Zeroing the number and clearing the LCD
        putchar('&');
                        //Ersal &
                        //Send &
                       //ba didan &,LCD pak mishe (Khat = )
                      //By seeing & and the LCD is cleared (Line = )  
        lcd_gotoxy(0,0);
        sprintf(str,"number = %4d",number);
        lcd_puts(str); 
        
}
            //Interrupt Khareji 1
            //External Interrupt 1
            //Ersal Payam
            //Send Massage
interrupt [EXT_INT1] void ext_int1_isr(void)
{
        puts(str); 
        //araye marbot be STR ra ba in dastor ersal mikonim (Khat =  , tabe Show)
        //Send STR (Line =  , Function Show)
        putchar(13); // go to new line,Vercual Terminal 
                      
        PORTA.1 = 1 ; // LED RED 
        delay_ms(1000);
        PORTA.1 = 0 ;      
        
}


#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
#define RX_BUFFER_SIZE 15
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_rd_index=0 , rx_wr_index = 0 ;
#else
unsigned int rx_rd_index=0 , rx_wr_index = 0 ;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

bit rx_buffer_overflow;

// USART Receiver interrupt service routine

interrupt [USART_RXC] void usart_rx_isr(void)
{
    char status,data;
    status=UCSRA;
    data=UDR;      
    
    if(data == '&')
    {
     lcd_clear();
     number = 0 ; 
     }
    
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
       {
           rx_buffer[rx_wr_index++]=data;
        #if RX_BUFFER_SIZE == 256
           // special case for receiver buffer size=256
           if (++rx_counter == 0) rx_buffer_overflow=1;
        #else
           if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
        if (++rx_counter == RX_BUFFER_SIZE)
          {
          rx_counter=0;
          rx_buffer_overflow=1; 
                              
          lcd_clear(); 
          lcd_gotoxy(0,1);
          lcd_puts("=>");
          lcd_puts(rx_buffer); 
          lcd_gotoxy(0,2);
          lcd_puts("                   "); 
          number =0 ;
          
          PORTA.0 = 1 ; // BLUE LED on
          delay_ms(1000);
          PORTA.0 = 0 ; // BLUE LED off 
            
                      
          }
    #endif
       }         
       
       
       
           
}



void main(void)
{

DDRB = 0XF8 ; 
PORTB = 0X07 ;// 0b00000111 

PORTD.2 = 1 ; // INT 0 AS PULLUP
PORTD.3 = 1 ; // INT 1 AS PULLUP

DDRA.0 = 1 ;// LED BLUE OUTPUT
DDRA.1 = 1 ;// LED BLUE OUTPUT


// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Rising Edge
// INT1: On
// INT1 Mode: Rising Edge
// INT2: Off
GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (0<<INTF2);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600 (Double Speed Mode)
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (1<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x0C;

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 16
lcd_init(16);

#asm("sei")
    
        
while (1)
      {     
        keyboard();
      }
}
