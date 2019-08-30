#include <Servo.h>
#include <SimpleTimer.h>

SimpleTimer timer;
Servo myservo;

int conveyor_motor=6;
const int SigPin = 8;
float invcmCosnt = (2*1000000)/(100*344.8);
float rawTime, cmDist; 



void servo_function(byte raw_data)
{
  if (raw_data == 'a')
  {
      for (int i = 90; i >= 65; i--)
      {
        myservo.write(i);
        delay(20);
      }
      delay(3000);
      
      for (int i = 65; i <= 90; i++)
      {
        myservo.write(i);
        delay(20);
      }    
   }
   
   else if (raw_data == 'b')
  {
    myservo.write(90);
  }
  
    else if (raw_data == 'c')
      {
        for (int i = 90; i <= 120; i++)
        {
          myservo.write(i);
           delay(20);
        }

        delay(3000);
        for (int i = 120; i >= 90; i--)
        {
          myservo.write(i);
          delay(20);
        }
      }
    else
    {
      myservo.write(90);
    }
}

void ultra_check() 
{
  cmDist=analogRead(A0);
  
  if(cmDist<500)
 {
  delay(300);
  digitalWrite(conveyor_motor,LOW);
  Serial.print('z');
  delay(2000);
  analogWrite(conveyor_motor,100);
  delay(3000);
 }

 else
 {
  analogWrite(conveyor_motor,100);
  Serial.print('x');

 }
}

/*
void ultra_check()
{
  pinMode(SigPin, OUTPUT);
  digitalWrite(SigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(SigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(SigPin, LOW);


  pinMode(SigPin, INPUT);
  rawTime = pulseIn(SigPin, HIGH); //measured in u-seconds
  cmDist = rawTime/invcmCosnt;
  //Serial.println(cmDist);
  
 if(cmDist<14)
 {
  delay(300);
  digitalWrite(conveyor_motor,LOW);
  Serial.print('z');
  delay(2000);
  analogWrite(conveyor_motor,100);
  delay(3000);
 }

 else
 {
  analogWrite(conveyor_motor,100);
  Serial.print('x');

 }
}

*/

void setup(){
  
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(conveyor_motor,OUTPUT);
  
  analogWrite(conveyor_motor,100);
  
  timer.setInterval(100, ultra_check);
}



void loop(){
  
  timer.run();

  if(Serial.available()>0)
  {
    byte raw_data=Serial.read();
    servo_function(raw_data);
  }

}
