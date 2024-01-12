/*******************************************************
Project : Send and receive two-way radio message(NAVA)
Version : 0.01
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

#include <mega16.h> //Header marbot be ATmega
#include <alcd.h>   //header marbot LCD
#include <stdio.h> // Header marbot be dastorat Sprintf , Scanf va ...
#include <delay.h> // baray estefade az delay

//Paye hay KeyPad
//Pin Keypad

#define   ROW1   PORTB.3
#define   ROW2   PORTB.4
#define   ROW3   PORTB.5
#define   ROW4   PORTB.6
#define   C1     PINB.0
#define   C2     PINB.1
#define   C3     PINB.2  

unsigned char str[16]; //Persian; tarif array STR  // ENG ; Define STR
int num  , number ; //Persian; tarif motegayer "num","number" // ENG ; Define "num","number"
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
        sprintf(str,"number = %4d",number);
        //Persian ; Megdar "number" ra dar "str" beriz //ENG ;Put the value of "number" in "str" 
        lcd_puts(str);
        //Persian; Meghdar "str"ra namyesh bede  // ENG ; Show the value of "str"          
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
         
//Persian; Baray pak kardan LCD //ENG ; For Clear LCD



interrupt [EXT_INT0] void ext_int0_isr(void) // clear mode 
{
        number = 0 ; 
        lcd_clear();
                        //PERSIAN ; Sefr kardan Number , pak kardan LCD
                        ///ENG ; Zeroing Number , Clear the LCD
                       
                        
        putchar('&');
                        //PERSIAN; Ersal &
                        //ENG;Send &
                       //ba didan &,LCD pak mishe (Khat =196 )
                      //By seeing & and the LCD is cleared (Line =196 )  
        lcd_gotoxy(0,0);
        sprintf(str,"number = %4d",number);
        //Persian ; Megdar "number" ra dar "str" beriz //ENG ;Put the value of "number" in "str" 
        lcd_puts(str);
        //Persian; Meghdar "str"ra namyesh bede  // ENG ; Show the value of "str"
        
}
            //Persian ; Interrupt Khareji 1
            //ENG ; External Interrupt 1
            //Ersal Payam
            //Send Massage
interrupt [EXT_INT1] void ext_int1_isr(void)
{
        puts(str) ; 
        //araye marbot be STR ra ba in dastor ersal mikonim (Khat =  , tabe Show)
        //Send STR (Line =  , Function Show)
       // putchar(13); // go to new line,Vercual Terminal 
                      
        PORTA.1 = 1 ;
        // LED ghermaz roshan shavad
        // LED RED ON
        delay_ms(1000);
        PORTA.1 = 0 ;
        // LED ghermaz khamosh shavad
        // LED RED OFF      
        
}


#define DATA_REGISTER_EMPTY (1<<UDRE)
// DATA_REGISTER_EMPTY tarif mishe
//register UDRE 1 mikone
//Eng ; register UDRE 1
//UDRE vaghti 1 bashe,Flag Bit 0 ast
//amaade daryaft etelaat jadid ast
//Eng ; Ready to receive new messages
#define RX_COMPLETE (1<<RXC)
//RX_COMPLETE tarif mishe
//RXC 1  mishe
//etelaat be UDR reside
//amade daryaft etelaate
#define FRAMING_ERROR (1<<FE)
//zamani ke 1 bashe etelaat amade nashode
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
//1 bashe etelaat hanoz neveshte nashode
#define RX_BUFFER_SIZE 15

char rx_buffer[RX_BUFFER_SIZE];
//Persian ; rx_buffer ba tol araye RX_BUFFER_SIZE tarif mishe
//Eng ; The presentation of rx_bufferA is defined by the length of RX_BUFFER_SIZE

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
// Marbor be interrupt daryaft payam
interrupt [USART_RXC] void usart_rx_isr(void)
{
    char status,data;
    //  tarif "status","data"
    //Eng ; define "status","data"
    status=UCSRA;
    //UCSRA register bit parcham ersal, daryaft payam //Flag Bit
    //Chera "status"? chon vaziyat register ra chek mikonim
    data=UDR;
    //UDR register daryaf, ersal etelaat      
    
    if(data == '&')
    // dar khat=112 gofter shode ke darsorat faal shdans Interrupt Khareji 0 '&' ersal shavad
    // ba daryaft "&" LCD Girande clear mishavad
    //Eng ; In line= 112, it was said that it will be sent to '&', it will be received here, and the LCD will be cleared when it is seen
    
    {
     lcd_clear();
     number = 0 ;

     }
    
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
       {
           rx_buffer[rx_wr_index++]=data;
           //Ta zamani ke shart bala barghara bashad
           //data ra dar rx_buffer mirizad va har bar tol
           //array ra afzayesh mkdahad
        #if RX_BUFFER_SIZE == 256
           // special case for receiver buffer size=256
           if (++rx_counter == 0) rx_buffer_overflow=1;
        #else
           if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
        if (++rx_counter2 == RX_BUFFER_SIZE)
          {
          rx_counter=0;
          rx_buffer_overflow=1; 
                              
          lcd_clear(); 
          lcd_gotoxy(0,1);
          lcd_puts("=>");
          lcd_puts(rx_buffer);
          //Data be "rx_buffer" dade shode ast
          //DATA namayesh dade mishavad 
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

DDRB = 0XF8 ; // 4tay paeni khoroji bashand //Keypad
PORTB = 0X07 ;// 0b00000111 // pulap kon 3 tay aval ro //Keypad

PORTD.2 = 1 ; // INT 0 AS PULLUP // CLEAR
PORTD.3 = 1 ; // INT 1 AS PULLUP // Send

DDRA.0 = 1 ;// LED BLUE OUTPUT
DDRA.1 = 1 ;// LED BLUE OUTPUT


// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Rising Edge
// INT1: On
// INT1 Mode: Rising Edge
// INT2: Off
GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
// GICR marbot be faal kardan INT hast
// 7 // 6 //
// INT1 // INT0  // 
MCUCR=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
//MCUCR tanzim labe bala va payen ravande
//tanzim baray payen ravande
MCUCSR=(0<<ISC2);

GIFR=(1<<INTF1) | (1<<INTF0) | (0<<INTF2);

//bit parcham vaghfe



// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600 (Double Speed Mode)
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (1<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

//(1<<RXCIE) baray daryaft payam tanzim mishavad
//(1<<TXEN) bar ersal etelaat ast

UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);

// tanzim sancron va asncron // asancron tanzim shode

UBRRH=0x00; 
// 0b00000000
UBRRL=0x0C;
// 0b00001100

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
