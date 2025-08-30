#include <avr/interrupt.h>
#include <avr/io.h>
#define clk_speed 16000000
#define baud 9600
#define my_ubrr (clk_speed/16/baud-1)
const uint8_t LED_PORTB[6]={PB5,PB4,PB3,PB2,PB1,PB0};
const uint8_t LED_PORTD[6]={PD7,PD6,PD5,PD4,PD3,PD2};
//we use volatile bc of ISR
volatile uint8_t counter=0,point=0;
volatile uint8_t state[4][3]={{PB5,PB4,PB3}  //traffic 1
                            , {PB2,PB1,PB0}  //traffic 2
                            , {PD7,PD6,PD5}  //traffic 3
                            , {PD4,PD3,PD2}};//traffic 4

void setup() {
  for(int i=0;i<(sizeof(state)/sizeof(state[0][0]));i++){
    for(int j=0;j<(sizeof(state)/sizeof(state[0][0]));j++){
      if(state[i][j]==)
    }
  }
  
  
  ////////////////////////////////////////////
  TCCR1A=0;
  TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
  OCR1A=15624;
  TIMSK1|=(1<<OCIE1A);
  /////////////////////////////////
  sei();
}
ISR(TIMER1_COMPA_vect){
  switch(point){
    case 0:
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
      else{
        counter++;
      }
      point=1;
      break;
  }
}

void loop() {
    

  if(point==0){
  switch(state){
    case 0:
      PORTB|=(1<<LED_PORTB[0]);
      PORTB&=~((LED_PORTB[5]<<4));
      PORTD&=~((LED_PORTD[5]<<5));
      break;
    case 1:
      PORTB|=(1<<LED_PORTB[1])|(1<<PB0)|(1<<);
      PORTB&=~((1<<LED_PORTB[0])|(1<<LED_PORTB[2])|(1<<LED_PORTB[3])|(1<<LED_PORTB[4])|(1<<LED_PORTB[5]));

      break;
    case 2:
      PORTB|=(1<<LED_PORTB[2]);
      PORTB&=~((1<<LED_PORTB[1])|(1<<LED_PORTB[0]));

      break;
  }
  }
}
