/*
 ========================================================================================
 Name        : Mini_project2.c
 Author      : Ahmed Saeed
 Version     : 1.0.0
 Description : Stop Watch Project
 Date        : 17/09/2022
 ========================================================================================
 */

/* ***************************** Includes Section Start ***************************** */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
/* ***************************** Includes Section End   ***************************** */

/* *************************** Global Variables Section Start *********************** */
/* global array contains 7 segments count value */
unsigned char Selected_7Segment[6] = {0};

/* global variable contains the ticks count of the timer */
unsigned int  tick = 0;

/* global variable selects if the stop watch will increment or to be printed */
unsigned char flag = 0;
/* *************************** Global Variables Section End ************************* */

/* ************************ functions Definition Section Start ********************** */
void reset(void)
{
	/* start counting from beginning */
	Selected_7Segment[0] = 0;
	Selected_7Segment[1] = 0;
	Selected_7Segment[2] = 0;
	Selected_7Segment[3] = 0;
	Selected_7Segment[4] = 0;
	Selected_7Segment[5] = 0;
	tick = 0;
}

void set_7segment_values(void)
{
	tick++;    /* Increment the number of ticks */

	Selected_7Segment[0]++;    /* increment the number shown on the first 7 segment */

	if ((tick % 10) == 0)
	{
		/* overflow after 10 seconds */
		Selected_7Segment[0] = 0;
		Selected_7Segment[1]++;
	}

	if ((tick % 60) == 0)
	{
		/* overflow after 60 seconds */
		Selected_7Segment[0] = 0;
		Selected_7Segment[1] = 0;
		Selected_7Segment[2]++;
	}

	if ((tick % 600) == 0)
	{
		/* overflow after 10 minutes */
		Selected_7Segment[0] = 0;
		Selected_7Segment[1] = 0;
		Selected_7Segment[2] = 0;
		Selected_7Segment[3]++;
	}

	if ((tick % 3600) == 0)
	{
		/* overflow after 1 hour */
		Selected_7Segment[0] = 0;
		Selected_7Segment[1] = 0;
		Selected_7Segment[2] = 0;
		Selected_7Segment[3] = 0;
		Selected_7Segment[4]++;
	}

	if ((tick % 36000) == 0)
	{
		/* overflow after 10 hours */
		Selected_7Segment[0] = 0;
		Selected_7Segment[1] = 0;
		Selected_7Segment[2] = 0;
		Selected_7Segment[3] = 0;
		Selected_7Segment[4] = 0;
		Selected_7Segment[5]++;
	}

	if ((Selected_7Segment[5] == 2) && (Selected_7Segment[4] == 4))
	{
		reset();   /* reset all 7 segments after reaching 24 hours */
	}
}

void print(void)
{
	int counter;
	for(counter = 0; counter<6; counter++)
	{
		PORTA = (PORTA & 0xC0) | ((0x01<<counter) & 0x3F);    /* enable the selected 7 segment only and disable the rest */
		PORTC = (PORTC & 0xF0) | (Selected_7Segment[counter] & 0x0F);  /* show the value of the selected 7 segment only */
		_delay_ms(2);    /* delay for 2 ms only,  due to the persistence of vision, it appears as a normal display */
	}
}

void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));  // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2);     // Activate the internal pull up resistor at PD2
	// Trigger INT0 with the falling edge
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR  |= (1<<INT0);    // Enable external interrupt pin INT0
}

void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin
	MCUCR |= (1<<ISC11) | (1<<ISC10);  // Trigger INT1 with the rising edge
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
}

void INT2_Init(void)
{
	DDRB   &= (~(1<<PB2));    // Configure INT2/PB2 as input pin
	PORTB |= (1<<PB2);        // Activate the internal pull up resistor at PB2
	MCUCSR &= (~(1<<ISC2));   // Trigger INT2 with the falling edge
	GICR   |= (1<<INT2);	  // Enable external interrupt pin INT2
}

/* Description:
 * For System Clock = 1Mhz and prescaler F_CPU/1024.
 * Timer frequency will be around 1Khz, Ttimer = 1.024ms
 * For compare value equals to 977 the timer will generate compare match interrupt every 1s.
 */
void Timer1_CTC_Init(void)
{
	TCNT1 = 0;		/* Set timer1 initial count to zero */

	OCR1A = 977;    /* Set the Compare value to 977 */

	TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. Select non-PWM mode FOC1A=1 FOC1B=1
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Prescaler = F_CPU/1024 CS10=1 CS11=0 CS12=1
	 */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
}

/* ************************* functions Definition Section End *********************** */

/* ******************************** ISR Section Start ******************************* */
/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	reset();      /* reset all 7 segments */
	TCNT1 = 0;    /* return the initial value of timer to zero to avoid showing 1 at the first 7 segment after reset*/
}

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	TCCR1B &= (~(1<<CS12)) & (~(1<<CS10));    /* turn off the clock source of the timer to pause the stop watch*/
}

/* External INT2 Interrupt Service Routine */
ISR(INT2_vect)
{
	/* turn on the clock source of the timer again with prescaler 1024 to let the stop watch resume counting*/
	TCCR1B |= (1<<CS12) | (1<<CS10);
}


/* Interrupt Service Routine for timer1 compare mode */
ISR(TIMER1_COMPA_vect)
{
	flag = 1;   /* set the interrupt flag to indicate that ISR for timer1 compare mode occurs */
}
/* ******************************** ISR Section End ********************************* */


/* ****************************** Main Function Start ******************************* */
int main(void)
{
	/********** Initialization Code **********/
	SREG |= (1<<7);       // Enable global interrupts by setting I-bit
	DDRA |= 0x3F;         // Configure the first six pins in PORTA as output pins.
	PORTA &= 0xC0;        // Set the first six pins in PORTA with value 0 to disable the BJT NPN transistor (7 segments).

	DDRC |= 0x0F;         // Configure the first four pins in PORTC as output pins.
	PORTC &= 0xF0;        // 7-segments display zero at the beginning.

	Timer1_CTC_Init();    // Start the timer.
	INT0_Init();          // Enable external INT0
	INT1_Init();          // Enable external INT1
	INT2_Init();          // Enable external INT2

	/********** Application Code **********/
	while(1)
	{
		if(flag == 0)
		{
			print();          // Keep showing time at all 7 segments
		}
		else
		{
			set_7segment_values();   // increment the stop watch
			flag = 0;    // reset the interrupt event flag to continue displaying the 7 segments after executing the required code
		}
	}
}
/* ******************************* Main Function End ******************************** */
