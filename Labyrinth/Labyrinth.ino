/*
		Artur Ferreira Moreira
		11 of november of 2015
		Instituto Federal de Educação, Ciência e Tecnologia (IFCE) - Cedro
*/


#include <Ultrasonic.h>


//////////////////////////////////////////////////////////////////////////////////////////////
#define TRIGGER_PIN  12
#define ECHO_PIN     13

int Trigger[3] = {2, 4, 6};
int Echo[3] = {3, 5, 7};

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
Ultrasonic SensorL(Trigger[0], Echo[0]), SensorM(Trigger[1], Echo[1]), SensorR(Trigger[2], Echo[2]);

float DirThresh = 10.0f;
/////////////////////////////////////////////////////////////////////////////////////////////


int IN1 = 30;
int IN2 = 31;
int IN3 = 32;
int IN4 = 34;

int ENA = 8;
int ENB = 10;

void MotorControl(int index, int Dir, int Vel)
{
  if(index == 0)
  {
      if(Dir == 0)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 0);
      }
      else if(Dir == 1)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, Vel);
      }
      else if(Dir == 2)
      {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, Vel);
      }
  }
  else
  {
    if(Dir == 0)
      {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, 0);
      }
      else if(Dir == 1)
      {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, Vel);
      }
      else if(Dir == 2)
      {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, Vel);
      }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  int k = 0;
  float cmMsec[3], inMsec[3];
  long microsec[3] = {SensorL.timing(), SensorM.timing(), SensorR.timing()};

  cmMsec[0] = SensorL.convert(microsec[0], Ultrasonic::CM);
  inMsec[0] = SensorL.convert(microsec[0], Ultrasonic::IN);

  cmMsec[1] = SensorM.convert(microsec[1], Ultrasonic::CM);
  inMsec[1] = SensorM.convert(microsec[1], Ultrasonic::IN);

  cmMsec[2] = SensorR.convert(microsec[2], Ultrasonic::CM);
  inMsec[2] = SensorR.convert(microsec[2], Ultrasonic::IN);


  if(cmMsec[1] < DirThresh) //Then Try Right
  {
    if(cmMsec[2] < DirThresh) // Then try Left
    {
      if(cmMsec[0] < DirThresh) //Then try to find the next best right
      {
        MotorControl(0, 2, 150);
        MotorControl(1, 1, 150);
      }
      else
      {
        MotorControl(0, 1, 150);
        MotorControl(1, 2, 150);
      }
    }
    else
    {
      MotorControl(0, 2, 150);
      MotorControl(1, 1, 150);
    }
  }
  else
  {
    MotorControl(0, 1, 150);
    MotorControl(1, 1, 150);
  }
  
  for(k = 0; k < 3; ++k)
  {
    Serial.print("MS[");
    Serial.print(k);
    Serial.print("]: ");
    Serial.print(microsec[k]);
    Serial.print(", CM: ");
    Serial.print(cmMsec[k]);
    Serial.print(", IN: ");
    Serial.println(inMsec[k]);
  }
  Serial.println("----------------------------------------------------------");
  delay(200);
}
