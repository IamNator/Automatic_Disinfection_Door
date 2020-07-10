//Includes
#include <Wire.h>
#include <Adafruit_MLX90614.h>
include 'DS1307.h'
 
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
Adafruit_MLX90614 temp = Adafruit_MLX90614();

void isPersonISR(){
  isPerson = 1;
}

int takeTemperature(){
  int bodyTemperature = temp.readObjectTempC();
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
     int body_temperature = takeTemperature();
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
