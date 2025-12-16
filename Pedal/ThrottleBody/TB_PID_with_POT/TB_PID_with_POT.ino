/********************************************************
 * Throttle body PID test
 * with 10k potentiometer to simulate pedal sensor
 *
 * WIRING
 * Potentiometer from UNO (GRND, 5V and A1 input pin)
 ********************************************************/

#include <PID_v1.h>

#define PIN_INPUT A0 //Throttle body sensor analog input
#define PIN_PEDAL_INPUT A1 //Truck pedal sensor analog input
#define PIN_OUTPUT 9 //Output for PWM signal to throttle body (forward to open)
#define REV 10 //Output for PWM signal to throttle body (reverse to close, most likely always 0 since we use the return spring to close the throttle body)

#define PWM_HOLD 20 //PWM value used to hold the TB at a certain position
#define ERROR_RANGE 1 //Range of throttle body analog value (from analogRead()) use to hold the throttle body in a position instead of hunting for the perfect value

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//double Kp=1,Ki=1, Kd=0;
//PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

double Kp=5, Ki=1, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M, DIRECT);

int error = 0; //Error between Setpoint and Input



void setup()
{
  Serial.begin(250000);         //Console for debug

  pinMode(PIN_OUTPUT, OUTPUT);       //Output high when going increasing throttle body
  pinMode(REV, OUTPUT);       //Output high when decresing or closing throttle body
  
  analogWrite(PIN_OUTPUT, 0);
  analogWrite(REV, 0);

  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = readTarget();

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //Turn the PID off
  //myPID.SetMode(MANUAL);

}

/*
* Reads input pin to use as target (either pedal in truck or potentiometer on the bench)
* Input range on the bench with 10k pot (uses Ground and 5V supply from UNO) --> 0 to 1024.
* Limit range a bit to be safe, expect range from 5 to 1020
*/
int readTarget()
{
  int temp = analogRead(PIN_PEDAL_INPUT);

  if (temp < 5)
    temp = 5;
  else if (temp > 1020)
    temp = 1020;

  //Serial.print("Read target: ");
  //Serial.println(temp);
  return temp;
}

void loop()
{
  //while(1); //Stop if necessary

  //Read current throttle body position
  Input = analogRead(PIN_INPUT);

  //Read target value (pedal or pot)
  Setpoint = readTarget(); //expect value 5 - 1020

  //Decide here if we need to turn off the PID
  if(Setpoint < Input) // Target for TB is lower, therefore TB needs to close
  {
    error = abs(Setpoint - Input); //Difference between Setpoint and Input
    Serial.print("PID OFF with diff: ");
    Serial.println(error);

    //Turn off PID
    myPID.SetMode(MANUAL);

    if(error < ERROR_RANGE) //We are close to the value, hold
      analogWrite(PIN_OUTPUT, PWM_HOLD);
    else
      //We are further from the Setpoint, let spring do its thing
      //Write 0 to FWD drive
      analogWrite(PIN_OUTPUT, 0);
  }

  while(Setpoint <= Input) //Wait loop
  {
    //Update Input (TB position)
    Input = analogRead(PIN_INPUT);

    //Update Setpoint (pedal or pot)
    Setpoint = analogRead(PIN_PEDAL_INPUT);

    error = abs(Setpoint - Input); //Difference between Setpoint and Input

    if(error < ERROR_RANGE) //We are close to the value, hold
      analogWrite(PIN_OUTPUT, PWM_HOLD);
    else
      //We are further from the Setpoint, let spring do its thing
      //Write 0 to FWD drive
      analogWrite(PIN_OUTPUT, 0);
  }

  //At this point the pedal is far enough that we need to turn on the PID again
  Serial.println("PID ON");
  myPID.SetMode(AUTOMATIC);
  
  //Re-Read current throttle body position
  Input = analogRead(PIN_INPUT);

  //Re-Read target value (pedal or pot)
  Setpoint = readTarget(); //expect value 5 - 1020

  //Compute new value for output
  myPID.Compute();

  //Debug values
  Serial.print("Input : Setpoint : Output -->   ");
  Serial.print(Input);
  Serial.print(" : ");
  Serial.print(Setpoint);
  Serial.print(" : ");
  Serial.println(Output);
  
  //Put output on output pin
  analogWrite(PIN_OUTPUT, Output);

  //Slow down if necessary
  //delay(1000);
}


