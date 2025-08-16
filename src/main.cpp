#include <avr/interrupt.h>
#include <Arduino.h>
const uint8_t PORTB_pins[3]={4,3,2};
const uint8_t PORTD_btn[2]={PD7,PD6};//2==D2 and 3==D3

void setup() {
  for(int i=0;i<(sizeof(PORTB_pins)/sizeof(PORTB_pins[0]));i++){
  DDRB|=1<<PORTB_pins[i];
  }
  for(int i=0;i<(sizeof(PORTD_btn)/sizeof(PORTD_btn[0]));i++){
  DDRD&=~(1<<PORTD_btn[i]);
  PORTD|=1<<PORTD_btn[i];
  }
  DDRD|=(1<<PD7);
  DDRD|=(1<<PD6);
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //this part sets up external interrupts   INT1 INT0
  //we will configure both interrupts for falling edge 2 first bits are for INT0 and the other 2 are for INT1 and the other bits are unused go checkout the datasheet
  EICRA|=0b00001010;
  //then wee need to mess with this register to make both interrupts enable
  EIMSK|=0b00000011;
  /////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////this part is for the 16 bit timer
  TCCR1A=0;
  TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);//this is equivilent to 0b00001101 both are same to use and we dont use the TCCR1A register bc we dont want that we use CTC mode 
  //and we use 1024 prescaler so every one second technically its not exactly 1 second its 1.024s the register OCR1A will overflow and ISR will be activated
  //one more thing for these register which bitmask to give them or anything u have too have toooo look at the data sheet of AtmelAtmega328p
  OCR1A=15624; //there is a formula to calculate that number here which we got 15624 with prescaler 1024
  TIMSK1|=1<<OCIE1A;
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  sei();

}
volatile uint8_t counter=0,state=0;//state=0 RED,,state=1 GREEN,,state=2 YELLOW
////////////////////////////////////////////////////////////////////////////////////
//this is for the buttons
ISR(INT0_vect){
  PORTD ^= (1 << PD7); 

}
ISR(INT1_vect){
  PORTD ^= (1 << PD6); 
   
}
//////////////////////////////////////////////////////////////////////////////////////////
//and this is for the timer comparator
ISR(TIMER1_COMPA_vect){
  if(state==0&&counter>=5){
    state=1;
    counter=0;

  }
  else if(state==1&&counter>=5){
    state=2;
    counter=0;
  }
  else if(state==2&&counter>=2){
    state=0;
    counter=0;
  }
  else {
    counter++;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  switch(state){

  case 0:
    PORTB|=(1<<PORTB_pins[0]);
    PORTB&=~((1<<PORTB_pins[1])|(1<<PORTB_pins[2]));
    break;
  case 1:
    PORTB|=(1<<PORTB_pins[1]);
    PORTB&=~((1<<PORTB_pins[0])|(1<<PORTB_pins[2]));
    break;
  case 2:
    PORTB|=(1<<PORTB_pins[2]);
    PORTB&=~((1<<PORTB_pins[1])|(1<<PORTB_pins[0]));
    break;
  
  }

}
