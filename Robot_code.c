FIRE BIRD CODE
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char data;

void dc_config (void)
{
	
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
	DDRL  = DDRL | 0xC0;
	PORTL = PORTL & 0x3F;
	DDRL  = DDRL | 0x40;
	PORTL = PORTL & 0xBF;
}

void servo1_pin_config (void)
{
	DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
	PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

void port_init()
{
	servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
	dc_config();
}

void dc_off(void){
	unsigned char port_restore = 0;
	port_restore = PINL;
	port_restore = port_restore & 0x3F;
	PORTL = port_restore;
}


void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}

void uart0_init(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}


SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	data = UDR0; 				//making copy of data from UDR0 in 'data' variable

	UDR0 = data; 				//echo data back to PC
	unsigned char i =0;
	if(data == 0x31) //ASCII value of 1..DC motor left
	{
		PORTL = 0x40;
	}

	if(data == 0x33) //ASCII value of 3...DC motor right
	{
		PORTL = 0x80;
	}
	if(data == 0x35) //ASCII value of 5
	{
		
		dc_off();
		PORTA = 0x00;
		servo_1_free();
		
	}
	if(data == 0x38) //ASCII value of 8
	{
		init_devices();
		PORTA=0x06;  //forward
	}

	if(data == 0x32) //ASCII value of 2
	{
		init_devices();
		PORTA=0x09; //back
	}

	if(data == 0x34) //ASCII value of 4
	{
		init_devices();
		PORTA=0x05;  //left
	}

	if(data == 0x36) //ASCII value of 6
	{
		init_devices();
		PORTA=0x0A; //right
	}
	
	if(data == 0x39) //ASCII value of 7
	{
		while(i<140)
		{
			servo_1(i);
			i++;
			_delay_ms(25);
			
			
			
			
		}
	}

	if(data == 0x37) //ASCII value of 9
	{
		i=140;
		while(i>0){
			servo_1(i);
			_delay_ms(25);
			i--;
			
			
		}
	}


}

void servo_1(unsigned char degrees)
{
	float PositionPanServo = 0;
	PositionPanServo = ((float)degrees / 1.86) + 35.0;
	OCR1AH = 0x00;
	OCR1AL = (unsigned char) PositionPanServo;
}

void servo_1_free (void) //makes servo 1 free rotating
{
	OCR1AH = 0x03;
	OCR1AL = 0xFF; //Servo 1 off
}



void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	uart0_init();
	timer1_init();
	sei(); //Enables the global interrupts
}
int main(void)
{
	
	
	init_devices();
	while(1);	
	
	
	
}
