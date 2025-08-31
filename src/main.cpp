#include <avr/interrupt.h>
#include <avr/io.h>
//#include <MyUSART.h>
#define clk_speed 16000000
#define baud 9600
#define my_ubrr (clk_speed/16/baud-1)
//we use volatile bc of ISR
volatile uint8_t counter=0,state=0;
//volatile uint8_t traffic_pos=0;



volatile const uint8_t traffic[4][3]={{PB5,PB4,PB3}  //traffic 1-SOUTH
                            ,         {PB2,PB1,PB0}  //traffic 2-EAST
                            ,         {PD7,PD6,PD5}  //traffic 3-NORTH
                            ,         {PD4,PD3,PD2}};//traffic 4-WEST

void setup() {
  DDRB|=0b00111111;//output from PB0->PB5
  DDRD|=0b11111100;//OUTPUT FROM PD3->PD7
  cli();
  ////////////////////////////////////////////
  TCCR1A=0;
  TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
  OCR1A=15625;//one second
  TIMSK1|=(1<<OCIE1A);
  /////////////////////////////////
  sei();
}
ISR(TIMER1_COMPA_vect){
  counter++;
  switch(state){
    case 0://in first state NORTH AND SOUTH are green and EAST,WEST ARE RED
    
      if(counter>=5){
        state=1;//yellow state of N-S
        counter=0;
      
      }
      break;
    case 1://N-S are gonna be yellow and E-W are still red
      if(counter>=2){
        state=2;//red for N-S GREEN FOR EAST WEST
        counter=0;
      }
      break;
    case 2://third state both N-S are gonna be red and E-W are gonna be green
      if(counter>=5){
        state=3;
        counter=0;
      }
      break;
    case 3://fourth and final state N-S still red and E-W gonna be yellow and the loop gonna restart from the first case
      if(counter>=2){
        state=0;
        counter=0;

      }
    break;
  }

  
  
}
void turnoff_leds(void){
  PORTB&=~0b00111111;
  PORTD&=~0b11111100;
}
void loop() {
  turnoff_leds();
  switch(state){
    case 0:
      PORTB|=(1<<traffic[0][2])|(1<<traffic[1][0]);
      PORTD|=(1<<traffic[2][2])|(1<<traffic[3][0]);
      break;
   
    case 1:
      PORTB|=(1<<traffic[0][1])|(1<<traffic[1][0]);
      PORTD|=(1<<traffic[2][1])|(1<<traffic[3][0]);
      break;
    case 2:
      PORTB|=(1<<traffic[0][0])|(1<<traffic[1][2]);
      PORTD|=(1<<traffic[2][0])|(1<<traffic[3][2]);
      break;
    case 3:
      PORTB|=(1<<traffic[0][0])|(1<<traffic[1][1]);
      PORTD|=(1<<traffic[2][0])|(1<<traffic[3][1]);
      break;
    
  }
}

  

