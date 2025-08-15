#include <Arduino.h>
#include <avr/interrupt.h>
const uint8_t PORTB_pins[3]={5,4,3};
const uint8_t PORTD_btn[2]={2,3};//2==D2 and 3==D3
ISR(INT0_vect){
  PORTD ^= (1 << PD7); 

}
ISR(INT1_vect){
  PORTD ^= (1 << PD6); 
   
}
void setup() {
  Serial.begin(115200);
  // this forloop is for the led outputs
  for(int i=0;i<(sizeof(PORTB_pins)/sizeof(PORTB_pins[0]));i++){
  DDRB|=1<<PORTB_pins[i];
  }
  //this for loop is for the inputs 
  //when the DDRD is 1 it means its an OUTPUT pin but when its 0 its an INPUT pin and automatically the DDRD has 0b00000000 all the pins are input but we have to give it a zero even if its autamatically has zero
  //and for the PORTD when the DDRB is input for a specific pin if PORTD is 0 then we have to add an external pullup so it doesnt float but why would we if the MCU has an integrated pullup
  //if we want to use that pullup we need to make PORTB to 1 for the pin that we want to
  for(int i=0;i<(sizeof(PORTD_btn)/sizeof(PORTD_btn[0]));i++){
  DDRD&=~(1<<PORTD_btn[i]);
  PORTD|=1<<PORTD_btn[i];
  }
   DDRD|=(1<<PD7);
  //PORTD|=(1<<PD7);
  DDRD|=(1<<PD6);
  //PORTD|=(1<<PD6);
  //we will configure both interrupts for falling edge 2 first bits are for INT0 and the other 2 are for INT1 and the other bits are unused go checkout the datasheet
  EICRA|=0b00001010;
  //then wee need to mess with this register to make both interrupts enable
  EIMSK|=0b00000011;
  sei();
}
char buff[100];
void loop() {

  #if 1
  for(int i=0;i<(sizeof(PORTB_pins)/sizeof(PORTB_pins[0]));i++){
    if(PORTB_pins[i]==4){
    PORTB|=1<<PORTB_pins[i];
  delay(1000);
  PORTB&=~(1<<PORTB_pins[i]);
  delay(100);
  }
  else{
  PORTB|=1<<PORTB_pins[i];
  delay(3000);
  PORTB&=~(1<<PORTB_pins[i]);
  delay(100);}
  
  //Serial.println(PB5);
  }
  #endif
 
}
