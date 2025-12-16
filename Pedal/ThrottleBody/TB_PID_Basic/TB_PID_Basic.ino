/********************************************************
 * Throttle body PID test - Basic
 * 
 ********************************************************/

#include <PID_v1.h>

#define PIN_INPUT A0
#define PIN_OUTPUT 9
#define REV 10

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

int target = 0;
int nb_iterations = 1000;
int current_iterations = 0;

//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1;
double Kp=0.5,Ki=4, Kd=0;//.04;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M, DIRECT);
//PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  Serial.begin(250000);         //Console for debug

  pinMode(PIN_OUTPUT, OUTPUT);       //Output high when going increasing throttle body
  pinMode(REV, OUTPUT);       //Output high when decresing or closing throttle body
  
  analogWrite(PIN_OUTPUT, 0);
  analogWrite(REV, 0);

  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = random(175, 700);;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //Turn the PID off
  //myPID.SetMode(MANUAL);

}

int readTarget()
{
  current_iterations++;

  if(current_iterations == nb_iterations)
  {
    current_iterations = 0;
    //generate new random
    Setpoint = random(175, 700); //between 175 and 800
  }
  return Setpoint;
}

void loop()
{
  /*while(target < Input) //If throttle body is opened too much
  {
    myPID.SetMode(MANUAL); //Turn off PID

    //Drive TB closed
    analogWrite(PIN_OUTPUT, 0); //No output forward
    //analogWrite(REV, 10); //Small output in reverse (return spring is helping)

    //Update position
    Input = analogRead(PIN_INPUT);
  }
  //*/

  Input = analogRead(PIN_INPUT);
  target = readTarget();

  Serial.print("Current: ");
  Serial.print(Input);
  Serial.print("          Target: ");
  Serial.print(target);
  Serial.print("          Output: ");

  //Here we should have target > Input which means we need to open up the TB

  //Stop driving the TB closed
  //analogWrite(REV, 0);

  //Turn on PID
  //myPID.SetMode(AUTOMATIC);

  //Comput new value for output
  myPID.Compute();

  //Debut output value
  Serial.println(Output);
  
  //Put output on output pin
  analogWrite(PIN_OUTPUT, Output);
}


