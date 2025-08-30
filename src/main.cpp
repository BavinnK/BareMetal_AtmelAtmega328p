#include <avr/interrupt.h>
#include <avr/io.h>
#define clk_speed 16000000
#define baud 9600
#define my_ubrr (clk_speed/16/baud-1)
//we use volatile bc of ISR
volatile uint8_t counter[4]={0,0,0,0};
volatile uint8_t state[4]={0,0,0,0};



volatile const uint8_t traffic[4][3]={{PB5,PB4,PB3}  //traffic 1
                            ,         {PB2,PB1,PB0}  //traffic 2
                            ,         {PD7,PD6,PD5}  //traffic 3
                            ,         {PD4,PD3,PD2}};//traffic 4

void setup() {
  DDRB|=0b00111111;//output from PB0->PB5
  DDRD|=0b11111100;//OUTPUT FROM PD3->PD7
  cli();
  ////////////////////////////////////////////
  TCCR1A=0;
  TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
  OCR1A=15624;
  TIMSK1|=(1<<OCIE1A);
  /////////////////////////////////
  sei();
}
ISR(TIMER1_COMPA_vect){
  for (int i=0; i<4; i++) {
    switch(state[i]){
      case 0: 
        if(counter[i]>=5){//red
          state[i]=1;
          counter[i]=0;
        }
        else{
          counter[i]++;
        }
        break;
      case 1:
         if(counter[i]>=5){//green
          state[i]=2;
          counter[i]=0;
        }
        else{
          counter[i]++;
        }
        break;
      case 2:
       if(counter[i]>=2){//yellow
          state[i]=0;
          counter[i]=0;
        }
        else{
          counter[i]++;
        }
        break;
    }
  }
  
}

void loop() {
    for(int i=0;i<=4;i++){
      switch(state[i]){
        case 0:
          DDRB=|(1<<traffic[i][0])|
      }
    }

  
}
