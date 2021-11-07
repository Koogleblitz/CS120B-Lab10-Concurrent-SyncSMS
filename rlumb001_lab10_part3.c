/*	Author: Richard Tobing, rlumb001@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding timer.h or example
 *	code, is my own original work.

	Demo link: https://www.youtube.com/watch?v=VFoPFuz6MZc
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//----------------------------------------timer.h----------------------------------------------//
volatile unsigned char TimerFlag = 0; 
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}
void TimerOff() {TCCR1B = 0x00;}
void TimerISR() {TimerFlag = 1;}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if(_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
////----------------------------------------/timer.h----------------------------------------------////


//-------------------------------------------------ADC------------------------------------------//
void ADC_init() {ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE); }
//-------------------------------------------------/ADC------------------------------------------//










enum threeLEDStates{first, second}threeLEDState;
enum blinkingLEDStates{on, off}blinkingLEDState;
enum speakerStates{hi, lo, wait}speakerState;
enum combineLEDStates{doSomething}combineLEDState;

unsigned char threeLED = 1;
unsigned char blinkingLED = 0;
unsigned char out;
unsigned short cnt = 0;
unsigned char A;
unsigned char speakerOut = 0;




void ThreeLEDSM(){
	switch(threeLEDState){
		case first: 
			threeLEDState = second;
		break;
		case second: 
			threeLEDState = first;
		break;
		default: threeLEDState = first;	break;
	}
	//-----------------------------------------------------
	switch(threeLEDState){
		case first: 
			if(threeLED == 0x04){threeLED = 0b00000001; }
			else{threeLED = threeLED << 1; }
		break;
		case second: 
			if(threeLED == 4){threeLED = 1; }
			else{threeLED = 0b00000010*threeLED; }
	}
	//PORTB = threeLED;
	
	
}


void BlinkingLEDSM(){
	switch(blinkingLEDState){
		case on: blinkingLEDState = off;	break;
		case off: blinkingLEDState = on;	break;
		default: blinkingLEDState = on;		break;
	}
	//-----------------------------------------------------
	switch(blinkingLEDState){
		case on: blinkingLED = 8;	break;
		case off: blinkingLED = 0;	break; 
	}
	//PORTB = blinkingLED;

}


void SpeakerSM(){

	A = (~PINA) & 7;

	//if(A == 4){


		switch(speakerState){
			case hi: speakerState = lo;		break;
			case lo: //if(A == 4){speakerState = hi; }	
				speakerState = 	hi;		break;
		default: speakerState = lo;			break;
		}
	//-----------------------------------------------------
		switch(speakerState){
			case hi: speakerOut = 16;	break;
			case lo: speakerOut = 0;	break; 
		}
	}

	
	

//}



void CombineLEDSM(){
	switch(combineLEDState){
		case doSomething: combineLEDState = doSomething;break;
		default: combineLEDState = doSomething;break;
	}
	//-----------------------------------------------------
	switch(combineLEDState){
		case doSomething: out = threeLED + blinkingLED + speakerOut;	break;
		default: out = threeLED + blinkingLED + speakerOut;	break; 
	}
	PORTB = out;
}





int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF;   
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerOn();
	TimerSet(2);
	ADC_init();

	//threeLEDState = first;
	
	

    while (1) 
    {		
		A = (~PINA) & 7;
		
		if(!(cnt%150)){ThreeLEDSM();	}
		if(!(cnt%500)){BlinkingLEDSM(); }
		if(A == 4){SpeakerSM();	}
		//SpeakerSM();
		CombineLEDSM();	
		
		
		
				
		while(!TimerFlag){}
		TimerFlag = 0;	
		cnt++;	
		
    }
}


