//Define pins to L293D chip
#define ENABLE1 3 // Connect to pin 1 of chip (Needs to be PWM pin)
#define DIRA1 4 // Connect to pin 2 on L293D (Can be any I/O pin)
#define DIRB1 5 // Connect to pin 7 on L293D (Can be any I/O pin)

//Define pins for other motor
#define ENABLE2 6 // Connect to pin 9 on L293D (Needs to be PWM pin)
#define DIRA2 7 // Connect to pin 10 on L293D (Can be any I/O pin)
#define DIRB2 8 // Connect to pin 15 on L293D (Can be any I/O pin)

#define Buzzer 14 //Buzzer to analog pin 0
//For NRF module (radio):
#include <SPI.h>  //Import SPI library
#include "RF24.h" //Import RF24 library

RF24 myRadio (9, 10); //These can be any I/O pins. Connect to (CE, CSN) respectively 

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

int power;
int Delta;
int Theta;

int Speed;
int Turn;
int Direction;
int varPot;
  
void setup() {
  Serial.begin(9600);
  delay(1000);
  //Define pinmode:
  pinMode(ENABLE1, OUTPUT);
  pinMode(DIRA1,OUTPUT);
  pinMode(DIRB1,OUTPUT);
  pinMode(ENABLE2, OUTPUT);
  pinMode(DIRA2,OUTPUT);
  pinMode(DIRB2,OUTPUT);
  pinMode(Buzzer, OUTPUT);
  //Setup radio
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX); //Set NRF24L01+ to max current draw
  myRadio.setDataRate( RF24_250KBPS ) ; //Set data rate to 250KBPS (for longest range)
  myRadio.maskIRQ(1, 1, 0); //set NRF24L01+ to inturuppt only when data is recieved
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  
  attachInterrupt(0,ReadRadio,FALLING); //Interrupt to read radio when NRF24L01 is recieving data. 0 is for pin 2
  
}

void loop() {
  delay(10);
  
  //Serial.println(map(abs(Speed), 0, 1024, 0, 255)
  
  if (Speed > 75){
    power = map(abs(Speed), 0, 1024, 100, 255); //The motor starts to rotate after a certain voltage threshold is hit. Therefore, when speed =! 0, we start at the voltage threshold value
    //Serial.println(power);
    Forward(power);
  }
  else{
    Stop();
  }   
  
    if (Turn < -3){ //Sometimes there is noise when reading the analog joysticks. Thats why we start turning when the Turn value is above +-3
        Delta = map(abs(Turn), 0, 499, 120, 255);
        Left(Delta);  
       /* Serial.print("\n");
        Serial.println("Turning Left:");
        Serial.println(Delta); */
    }
      else if (Turn > 3){
        Theta = map(abs(Turn), 0, 524, 120, 255);
        Right(Theta);
        /*Serial.print("\n");
        Serial.println("Turning Right:");
        //Serial.println(Theta); */
    } 
      else{
         NoTurn();
    }
    
  
  
}


void ReadRadio(){
  if ( myRadio.available()) //Check if radio is available 
  {
      myRadio.read(&data, sizeof(data));
      //For testing reasons:
   /* Serial.print("\nPackage:");
    Serial.print("x-Cord:");
    Serial.print(data.xCord);
    Serial.print("\n");
    Serial.print("y-Cord:");
    Serial.print(data.yCord);
    Serial.print("\n");
    Serial.print("Speed:");
    Serial.print(data.Speed);
    Serial.print("\n");
    Serial.print("Var Pot:");
    Serial.print(data.varPot);
    Serial.print("\n"); */
  }
  else {
    Serial.print("Radio was not avaliable");
    Stop();
    NoTurn();
    return;
  }
  /*Serial.println(Direction);
  Serial.println(Turn);
  Serial.println(Speed);*/
  
  Direction = data.yCord - 508;
  Turn = data.xCord - 499;
  Speed = data.Speed;//allows for very fine control on speed
}

/* NOTE: Never turn on DIRA and DIRB at the same time. This will short the motor driver and it will go up in flames!*/

void Forward(int Speed){ //pass in the speed
    analogWrite(ENABLE2, Speed); //go forward at this speed
    digitalWrite(DIRA2,LOW); 
    digitalWrite(DIRB2,HIGH);
    delay(1); //slight delay is needed
  }
  
void Backward(int Speed){
    analogWrite(ENABLE2, Speed); 
    digitalWrite(DIRA2,HIGH); 
    digitalWrite(DIRB2,LOW);
    delay(1);
}

void Stop(){
    digitalWrite(ENABLE2, LOW); 
    digitalWrite(DIRA2,LOW); 
    digitalWrite(DIRB2,LOW);
    delay(1);
}
void NoTurn(){
   digitalWrite(ENABLE1, LOW); //Reset steering 
   digitalWrite(DIRA1,LOW); 
   digitalWrite(DIRB1,LOW);
   delay(1);
}

void Right(int tRadius){
    analogWrite(ENABLE1, tRadius); //Turn right 
    digitalWrite(DIRA1,LOW); 
    digitalWrite(DIRB1,HIGH);
    delay(1);
}

void Left(int tRadius){
    analogWrite(ENABLE1, tRadius); //Turn left
    digitalWrite(DIRA1,HIGH); 
    digitalWrite(DIRB1,LOW);
    delay(1);
}




 
