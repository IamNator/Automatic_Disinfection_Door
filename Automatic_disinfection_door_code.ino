//Includes
#include <Wire.h>
#include <Adafruit_MLX90614.h> //for Temperature
#include "DS1307.h" //for RTC 
#include <U8x8lib.h> //128X64 LCD display
#include <U8g2lib.h>

//Pin Declearation

//OUTPUT 1|0 PINS
#define PUMP 24
#define BUZZER 23
#define UV 25

/*//OUTPUT CMPLX PINS
#define RD 28 //E - en - SCK
#define WR 27 //RW - rw - MOSI
#define VO 26 //D/I - id - CS 
*/

//INPUT 1|0 PINS
#define PIR 14
//INPUT I2C PINS

//#define I2C_SDA 21
//#define I2C_SCL 22


unsigned int isPerson = 0;
Adafruit_MLX90614 temp = Adafruit_MLX90614(); //for temperature (temperature object)
//U8G2_ST7920_128X64_1X u8g2(RD, WR, VO);  // SPI Com: SCK = en = 28, MOSI = rw = 27, CS = di = 26
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 28, /* data=*/ 27, /* CS=*/ 26, /* reset=*/ 29);//U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8

//INTERRUPTS ########################################################################################################
void isPersonISR(){
  isPerson = 1;
}


float takeTemperature(){
  float bodyTemperature = temp.readObjectTempC();
  //Serial.print("Body Temperature = "); Serial.print(bodyTemperature); Serial.println("*C"); //For Debugging
  //To be Implimented later: Give green lights then return true
  return bodyTemperature;
}





//DISPLAY ##############################################################################################################

void draw(void){
  // graphic commands to redraw the complete screen should be placed here
  u8g2.setFont(u8g_font_unifont); //set font
  u8g2.drawStr( 0, 22, "Little Tech!");
}

void buildPage(){
  u8g2.firstPage();
  do {
    draw();
  } while ( u8g2.nextPage() ); //u8g.nextPage == 1 when catch is full
}


void setup() {
  //OUTPUT PINS
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(UV, OUTPUT);

  //INPUT PINS
  pinMode(PIR, INPUT);

  //INTERRUPT PIN
  attachInterrupt(PIR, isPersonISR, RISING);
  Serial.begin(9600);//for serial monitor debugging
  DS1307 clock;
  clock.begin();
  clock.getTime();

}


void loop() {

  while (isPerson) {
    float body_temperature = takeTemperature();
    if (body_temperature > 30 && body_temperature <= 38.1) {
      digitalWrite(PUMP, HIGH);
      digitalWrite(UV, HIGH);
      delay(10 * 1000); //delay for 15secs
      digitalWrite(PUMP, LOW);
      digitalWrite(UV, LOW);
    }
    else if (body_temperature > 38.1) {
      digitalWrite(BUZZER, HIGH);
      delay(2500); //delays for 2.5secs
      digitalWrite(BUZZER, LOW);
    }
    isPerson = 0;
  }
  buildPage();
}

/*COMMENTED FUNCTIONS
 * 
 * 
//void setTimeOnce()
//{
//    clock.begin();
//    clock.fillByYMD(2020,07,11);//Jan 19,2013
//    clock.fillByHMS(15,28,30);//15:28 30"
//    clock.fillDayOfWeek(FRI);//Saturday
//    clock.setTime();//write time to the RTC chip
//}
//
*/
