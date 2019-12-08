/*
***** Project Two *****
Members: Kylie Outlaw, Maria Abassi, Miles O'Brien, Marco Carmack

Notes: On our current Simon board, LED3 and LED4 are not working. We believe they are shorted / "blew a fuse". 

*/

/* defines and includes */
#define F_CPU 16000000UL // 16mhZ
#define BAUD 9600

#include <avr/io.h>							 
#include <avr/interrupt.h>
#include <string.h>

/* function prototypes */
void USART_Init(unsigned long);// USART initialize function
char USART_RxChar();// Data receiving function
void USART_TxChar(char);/// Data transmitting function
void menu();
void transmit(char arr[]);
void sound(int Note, int length);
void delay(int Note);
void tune1();
void tune2();
void pause(int length);

unsigned char mode = 0; //mode 0 is keyboard, mode 1 is songs, mode 2 is kylie's stuff

/*  list of notes and delay values used  */
int G4Shp = 18, C4Shp = 28, B4 = 16, A4 = 18, G4 = 20, F4 = 22, E4 = 24, D4 = 27, C4 = 30;
int main(void)
{
	/*  port initializations  */
    DDRA = 0x00; //port A input mode
	PORTA = 0xFF; //enable pull up resistors on port A
    
	DDRD = 0xFF;
	DDRD &=~(1<<PORTD2); //portD bit 2 input, bit 3 output
    PORTD = 0x00; //disable all LEDs (active low)
	
    DDRE |= ((1<<PORTE4) | (1<<PORTE5)); // PE4 output speaker
    PORTE = 0x00;

    unsigned char value;  // used if using polling for receiving data
	
	/*  usart initializations */
    USART_Init(BAUD); //initialize usart to the baud rate
    sei(); //global interrupts enabled


    menu();
	while(1) /* infinite loop */
	{
		/* polling on receiver */
        value = USART_RxChar(); //get value input from 
		switch (value)
		{
			case '1': //play tune 1
                if( mode != 1 )
                    break;
                tune1();
                break;
                
            case '2': //play tune 2
                if ( mode != 1 )
                    break;
                tune2();
                break;    

			case '3': //keyboard mode
                mode = 0;
                menu();
                break;		  
            
            case '4': //tunes mode
                mode = 1;
                menu();
                break;

            case '5': //secret mode, kylie's deal
                mode = 2;
                menu();
                break;
            
            default:
                break;  
		}

        if( mode == 0 ) // KEYBOARD MODE
        {
            if( (~(PINA) & (1<<PINA0) ) == (1<<PINA0) ) //if sw1 is pushed
            {
                PORTD &= ~(1<<PORTD0);
                while( (~(PINA) & (1<<PINA0)) == (1<<PINA0) )
                {
                    sound(A4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINA) & (1<<PINA1) ) == (1<<PINA1) ) //if sw2 is pushed
            {
                PORTD &= ~(1<<PORTD1);
                while( (~(PINA) & (1<<PINA1)) == (1<<PINA1) )
                {
                    sound(B4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINA) & (1<<PINA2) ) == (1<<PINA2) ) //if sw3 is pushed
            {
                PORTD &= ~(1<<PORTD2);
                while( (~(PINA) & (1<<PINA2)) == (1<<PINA2) )
                {
                    sound(C4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINA) & (1<<PINA3) ) == (1<<PINA3) ) //if sw4 is pushed
            {
                PORTD &= ~(1<<PORTD3);
                while( (~(PINA) & (1<<PINA3)) == (1<<PINA3) )
                {
                    sound(D4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINE) & (1<<PINE6) ) == (1<<PINE6) ) //if sw5 is pushed
            {
                PORTE &= ~(1<<PORTE5);
                while( (~(PINE) & (1<<PINE6) ) == (1<<PINE6) )
                {
                    sound(E4, 10);
                }
                PORTE = 0xFF;
            }

            if( (~(PINA) & (1<<PINA4) ) == (1<<PINA4) ) //if sw6 is pushed
            {
                PORTD &= ~(1<<PORTD4);
                while( (~(PINA) & (1<<PINA4)) == (1<<PINA4) )
                {
                    sound(F4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINA) & (1<<PINA5) ) == (1<<PINA5) ) //if sw7 is pushed
            {
                PORTD &= ~(1<<PORTD5);
                while( (~(PINA) & (1<<PINA5)) == (1<<PINA5) )
                {
                    sound(G4, 100);
                }
                PORTD = 0xFF;
            }

            if( (~(PINA) & (1<<PINA6) ) == (1<<PINA6) ) //if sw8 is pushed
            {
                while( (~(PINA) & (1<<PINA6)) == (1<<PINA6) )
                {
                  // enable LED
                  // sound (higher octave)
                }
            }

            if( (~(PINA) & (1<<PINA7) ) == (1<<PINA7) ) //if sw9 is pushed
            {
                while( (~(PINA) & (1<<PINA7)) == (1<<PINA7) )
                {
                  // enable LED
                  // sound (lower octave)
                }
            }
        }
        
        else if (mode == 1) //TUNES MODE
        {} // all code done within the switch case

        else if ( mode == 2 )
        {
            if( (~(PINA) & (1<<PINA0) ) == (1<<PINA0) ) //if sw1 is pushed
            {
                transmit("An apple a day keeps the doctor away!");
                while( (~(PINA) & (1<<PINA0)) == (1<<PINA0) );
            }

            if( (~(PINA) & (1<<PINA1) ) == (1<<PINA1) ) //if sw2 is pushed
            {
                transmit("Live, laugh, love - Karen");
                while( (~(PINA) & (1<<PINA1)) == (1<<PINA1) );
            }

            if( (~(PINA) & (1<<PINA2) ) == (1<<PINA2) ) //if sw3 is pushed
            {
                transmit("Do your best!");
                while( (~(PINA) & (1<<PINA2)) == (1<<PINA2) );
            }

            if( (~(PINA) & (1<<PINA3) ) == (1<<PINA3) ) //if sw4 is pushed
            {
                transmit("Microcontrollers are fun.");
                while( (~(PINA) & (1<<PINA3)) == (1<<PINA3) );
            }

            if( (~(PINA) & (1<<PINA4) ) == (1<<PINA4) ) //if sw6 is pushed
            {
                transmit("Merry Christmas and Happy New Year!");
                while( (~(PINA) & (1<<PINA4)) == (1<<PINA4) );
            }

            if( (~(PINA) & (1<<PINA5) ) == (1<<PINA5) ) //if sw7 is pushed
            {
                transmit("I hope your car lasts with your daughter driving it.");
                while( (~(PINA) & (1<<PINA5)) == (1<<PINA5) );
            }

            if( (~(PINA) & (1<<PINA6) ) == (1<<PINA6) ) //if sw8 is pushed
            {
                transmit("Aloha.");
                while( (~(PINA) & (1<<PINA6)) == (1<<PINA6) );
            }

            if( (~(PINA) & (1<<PINA7) ) == (1<<PINA7) ) //if sw9 is pushed
            {
                transmit("I don't know what else to put here.");
                while( (~(PINA) & (1<<PINA7)) == (1<<PINA7) );
            }
        }
        else{}
	}
    return 0;
}


/* *** FUNCTION DEFINITIONS *** */
void USART_Init(unsigned long BAUDRATE) //USART initialize function
{
    UCSR1B = (1<<TXEN) | (1<<RXEN); //enable transmitter and receiver
    UCSR1C |= (1<<UCSZ1) | (1<<UCSZ0); //8 bit data
	//one stop bit by UCSR1C USBS bit being 0

	int scalar = ((F_CPU/(16*BAUDRATE))-1); //define prescalar value 
    UBRR1H = (unsigned char)(scalar>>8); //set UBRR1 to baud rate
	UBRR1L = (unsigned char)(scalar);
}

char USART_RxChar() //receive data
{
    while ( !(UCSR1A & (1<<RXC)) ) //data to be received?
    {
        return 0;  //if no data received, return 0
    }
    return UDR1;//return data being received
}

void USART_TxChar(char data) //transmit data
{
	UDR1 = data; //set your transmit value to the data to be transmitted
    while ( !(UCSR1A & (1<<UDRE)) ); //wait until buffer is empty (data fully transmitted)
}

void transmit(char arr[])
{
    USART_TxChar('\r');
    for(int i = 0; i < (strlen(arr)); i++)
    {
        USART_TxChar(arr[i]);
    }
}

void menu() //transmit menu options
{
    transmit("\r");
    transmit(" ********** MENU ********** ");
    transmit("Current Mode: ");
    if( mode == 0 )
    {
        transmit("Keyboard. Play some keys!");
        transmit("or choose an option:");
        transmit("4. Mode: Tunes");
        transmit("5. Mode: Kylie");    
    }
    
    else if ( mode == 1 )
    {
        transmit("Tunes. Enter 1 or 2 to listen!");
        transmit("or choose an option:");
        transmit("1. Tune 1");
        transmit("2. Tune 2");
        transmit("3. Mode: Keyboard");
        transmit("5. Mode: Kylie");
    }
    else // kylie's mode
    {
        transmit(" Welcome to Kylie's mode! Press some buttons to see the magic.");
        transmit("or choose an option:");
        transmit("3. Mode: Keyboard");
        transmit("4. Mode: Tunes");
    }
}

void sound(int Note, int length)
{
    for(int i = 0; i < length; i++)
    {
        PORTE ^= (1<<PORTE4);
        delay(Note);
    }   
}

void delay(int Note)
{
    TCNT1 = (0xFFFF - Note);
    TCCR1A = 0x00;
    TCCR1B = 0b00000101; //1024 prescalar
    while((TIFR1 & (1<<TOV1)) == 0){} //preforming bitmasking to determine TOV1 bit
    TIFR1 |= (1<<TOV1); //clear to 0 by writing a 1
    TCCR1B = 0x00; //turn off timer counter module
}

void tune1()
{
    sound(E4, 100);
    pause(10000);
    sound(G4Shp, 300);
}

void tune2()
{
    sound(E4, 100);
    pause(1000);
    sound(G4Shp, 300);
}

void pause(int length)
{
    for( int i = 0; i < length; i++){}
}