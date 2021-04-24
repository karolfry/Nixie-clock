#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

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

const char* ssid     = "multimedia_karol";
const char* password = "V9CgmmkeuyMi6E3";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 7200;
//const int   daylightOffset_sec = 0;

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

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();

  Display_set_time(timeinfo.tm_hour,timeinfo.tm_min);

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
  Serial.begin(115200);
  Serial.print("Hello esp");

  //WiFi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  //Display test
  for(uint8_t i=0; i<2;i++)
  {
    for(uint8_t x =0;x<10;x++)
    {
    Display_set_time (x*11,x*11);
    delay(300);
    }
  }

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();


}

void loop() {
 // put your main code here, to run repeatedly:
  // digitalWrite(LED_PIN,HIGH);

  //delay(80);
  //digitalWrite(LED_PIN,LOW);

  //delay(80);
  //digitalWrite(LED_PIN,HIGH);

  printLocalTime();
  delay(900);

  

// for(uint8_t x =10;x<20;x++)
  // {
    // for(uint8_t y=0;y<100;y++)
    // {
      //  Display_set_time (x,y);
      //  delay(100);
    // }
  // }

// for(uint8_t x =0;x<10;x++)
// {
// Display_set_time (x*11,x*11);
// delay(300);
// }
// 
 // Display_set_time (77,77);
}