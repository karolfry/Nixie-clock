#include <Arduino.h>

#define HV_PIN 4
#define LED_PIN 22
#define OUT_UP_PIN 23
#define D_CLK_PIN 19
#define D_IN_PIN 18

#define SW_1_PIN 26
#define SW_2_PIN 27



//  Zmienna do zapisu sekwencji dla rejestrów przesównych
//
//          U1 U2 : U3 U4
//
//       U2     U1    U4    U3
//      0000   0000  0000  0000
uint16_t SR_tab = 0;

void Display_update(){

  for(uint8_t x=0 ;x<=15; x++)
  {
    if(SR_tab & (1<<x)) digitalWrite(D_IN_PIN,HIGH);
    else digitalWrite(D_IN_PIN,LOW);

    digitalWrite(D_CLK_PIN,LOW);
    delay(1);
    digitalWrite(D_CLK_PIN,HIGH);

  }
  digitalWrite(OUT_UP_PIN,LOW);
  delay(1);
  digitalWrite(OUT_UP_PIN,HIGH);
}

void Display_set_time(uint8_t hours, uint8_t minutes)
{
  SR_tab = 0;

  uint16_t d_hours, u_hours, d_minutes, u_minutes;
  d_hours = hours/10;
  u_hours = hours%10;
  d_minutes = minutes/10;
  u_minutes = minutes%10; 
  
  SR_tab |= (d_hours << 8) | (u_hours << 12) | d_minutes | (u_minutes<<4);

  Display_update();
}

void setup() {
   //Schift Reg gpio config 
   pinMode(LED_PIN,OUTPUT);
   pinMode(D_IN_PIN,OUTPUT);
   pinMode(OUT_UP_PIN,OUTPUT);
   pinMode(D_CLK_PIN,OUTPUT);

  // HV ON
  delay(3000);
  pinMode(HV_PIN,OUTPUT);
  digitalWrite(HV_PIN,LOW);

  //UART Config

}

void loop() {
 // put your main code here, to run repeatedly:
  // digitalWrite(LED_PIN,HIGH);

  delay(80);
  digitalWrite(LED_PIN,LOW);

  delay(80);
  digitalWrite(LED_PIN,HIGH);




// for(uint8_t x =10;x<20;x++)
  // {
    // for(uint8_t y=0;y<100;y++)
    // {
      //  Display_set_time (x,y);
      //  delay(100);
    // }
  // }


  Display_set_time (22,22);
}