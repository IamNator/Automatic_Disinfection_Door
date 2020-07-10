//Includes
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "DS1307.h" //for Temperature
#include "U8glib.h" //128X64 LCD display

 
//Pin Declearation

//OUTPUT 1|0 PINS
#define PUMP 24
#define BUZZER 23
#define UV 25

//OUTPUT CMPLX PINS
#define RD 28
#define WR 27
#define VO 26


//INPUT 1|0 PINS
#define PIR 14
//INPUT I2C PINS
//
//#define I2C_SDA 21
//#define I2C_SCL 22


unsigned int isPerson = 0;
Adafruit_MLX90614 temp = Adafruit_MLX90614(); //for temperature (temperature object)
U8GLIB_ST7920_128X64_1X u8g(28, 27, 26);  // SPI Com: SCK = en = 28, MOSI = rw = 27, CS = di = 26

void isPersonISR(){
  isPerson = 1;
}

float takeTemperature(){
  float bodyTemperature = temp.readObjectTempC();
  //Serial.print("Body Temperature = "); Serial.print(bodyTemperature); Serial.println("*C"); //For Debugging
  //To be Implimented later: Give green lights then return true
  return bodyTemperature;
}

//void setTimeOnce()
//{
//    clock.begin();
//    clock.fillByYMD(2020,07,11);//Jan 19,2013
//    clock.fillByHMS(15,28,30);//15:28 30"
//    clock.fillDayOfWeek(FRI);//Saturday
//    clock.setTime();//write time to the RTC chip
//}
//

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont); //set font
  u8g.drawStr( 0, 22, "Little Tech!");
}

void buidPage(){
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() ); //u8g.nextPage == 1 when catch is full 
}


void setup() {
  //OUTPUT PINS
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(UV, OUTPUT);

  //INPUT PINS
  pinMode(PIR, INPUT);

  //INTERRUPT PIN
  attachInterrupt(PIR,isPersonISR,RISING);
  Serial.begin(9600);//for serial monitor debugging
  clock.getTime();
  
}


void loop() {

  while(isPerson){
     float body_temperature = takeTemperature();
     if (body_temperature > 30 && body_temperature <= 38.1){
      digitalWrite(PUMP, HIGH);
      digitalWrite(UV,HIGH);
      delay(10*1000); //delay for 15secs
      digitalWrite(PUMP,LOW);
      digitalWrite(UV,LOW);
     }
     else if(body_temperature > 38.1){
      digitalWrite(BUZZER, HIGH);
      delay(2500); //delays for 2.5secs
      digitalWrite(BUZZER,LOW);
     }
     isPerson = 0;
  }

}
