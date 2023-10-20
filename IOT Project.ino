#include<Servo.h>
//IR SENSOR + Server Motor
Servo s1;
int sensor_pin=8;
int servo_pin=10;
int val;

//FLAME SENSOR + BUZZER
const int buzzerPin=2;
const int fireSensorPin=3;

//PIR SENSOR + LED
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 10000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 7;    //the digital pin connected to the PIR sensor's output
int ledPin1 = 11;
int ledPin2 = 12;
 //the digital pin connected to the LED output
void setup() 
{
  //IR SENSOR + Server Motor
  pinMode(sensor_pin,INPUT);
  s1.attach(servo_pin);//setvo_pin=10

  //FLAME SENSOR + BUZZER
   Serial.begin(9600);
   pinMode(buzzerPin,OUTPUT);
   pinMode(fireSensorPin,INPUT);

   //PIR SENSOR + LED
   Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  //pinMode(Buzzer, OUTPUT);
  digitalWrite(pirPin, LOW);
  
}

void loop()
 {
  //IR SENSOR + Server Motor
  val=digitalRead(sensor_pin);
  if(val==0)
   {
    s1.write(0);
   }
  if(val==1)
   {
     delay(500);
    s1.write(200);
   }

  //FLAME SENSOR + BUZZER
    int fireValue=digitalRead(fireSensorPin);
   // Serial.println(fireValue);
    if(fireValue==0)
     {
      digitalWrite(buzzerPin,HIGH);
     }
    else
     {
     digitalWrite(buzzerPin,LOW);
     }

    //PIR SENSOR + LED
    if(digitalRead(pirPin) == HIGH)
     {
       digitalWrite(ledPin1, HIGH); 
       digitalWrite(ledPin2, HIGH);
        
       delay(5); //the led visualizes the sensors output pin state
      
       if(lockLow)
        {  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         
         delay(50);
         }         
         takeLowTime = true;
      }

     if(digitalRead(pirPin) == LOW)
      {       
       digitalWrite(ledPin1, LOW);
       digitalWrite(ledPin2, LOW);
       
        //the led visualizes the sensors output pin state
       //noTone(Buzzer);
       if(takeLowTime)
       {
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause)
        {  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;
           
           delay(50);
        }
       }
}//78
