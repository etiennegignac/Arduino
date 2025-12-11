/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>

//#define PIN_INPUT 0
#define PIN_INPUT A0
//#define PIN_OUTPUT 3
#define PIN_OUTPUT 9
#define REV 10

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1;
double Kp=0.75,Ki=4, Kd=0;//.04;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M,DIRECT);

void setup()
{
  Serial.begin(250000);         //Console for debug

  pinMode(PIN_OUTPUT, OUTPUT);       //Output high when going increasing throttle body
  pinMode(REV, OUTPUT);       //Output high when decresing or closing throttle body
  
  analogWrite(PIN_OUTPUT, 0);
  analogWrite(REV, 0);

  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 300;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  Input = analogRead(PIN_INPUT);

  Serial.print("Current value: ");
  Serial.print(Input);
  Serial.print("          Output: ");

  myPID.Compute();

  Serial.println(Output);
  analogWrite(PIN_OUTPUT, Output);
}


