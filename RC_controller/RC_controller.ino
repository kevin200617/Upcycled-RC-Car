#include <SPI.h>  
#include "RF24.h"
#include "pitches.h" 


#define xCordIN 14 //x coordinate pot (Analog pin A0)
#define yCordIN 15 //y coordinate pot (Analog pin A1)
#define SpeedIN 16 //Speed pot (Analog pin A2)
#define varPotIN 17 //Extra pot (Analog pin A3)
#define Buzzer 5 //Indication Buzzer
RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
//The following are only applicable to Arduino Nano:
//Connect MOSI to pin 11 
//Connect MISO to pin 12
//Connect SCK to pin 13
//Connect IRQ to pin 2
struct package //data package that will be sent
{
  int Speed;
  int xCord;
  int yCord;
  int varPot;
};

package data;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
  digitalWrite(Buzzer, LOW);
}

void loop() {
readData();
sendData();
}

void readData(){
  data.xCord = analogRead(xCordIN); //subtract 512 from this later on
  data.yCord = analogRead(yCordIN);
  data.Speed = analogRead(SpeedIN);
  data.varPot = analogRead(varPotIN);
}
void sendData(){
  myRadio.write(&data, sizeof(data)); 
  delay(30); //slight delay
}
