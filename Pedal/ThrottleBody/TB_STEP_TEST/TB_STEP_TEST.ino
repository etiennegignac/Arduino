/*//////////////////////////////////////////////////////////////////////////////
Name:  TB_STEP_TEST
CPU:   UNO
Date:  7 Dec 25
Description:  Use this to test moving the TB in steps until it reaches a target

**********  WIRING  **********
ARDUINO:
Pin No      Function        Description
A0          TB_TSP1         Analog read of voltage of throttle position sensor #1 of the throttle body (not the pedal!)

H-BRIDGE:
Pin No      Function        Description
1           RPWM            Forward Level or PWM signal, Active High
2           LPWM            Reverse Level or PWM signal, Active High --> WE DONT USE THIS
3           R_EN            Forward Drive Enable Input, Active High/ Low Disable --> ALWAYS ENABLE
4           L_EN            Reverse Drive Enable Input, Active High/Low Disable --> ALWAYS ENABLE
5           R_IS            Forward Drive, Side current alarm output
6           L_IS            Reverse Drive, Side current alarm output
7           Vcc             +5V Power Supply microcontroller --> Also works on 3.3v according to datasheet, to 3.3V of Teensy
8           Gnd             Ground Power Supply microcontroller --> to ground of Teensy

////////////////////////////////////////////////////////////////////////////////*/

#define FWD 9         //Output PWM to H-Bridge (motor forward (LPWM in datasheet))
#define TB_TPS1 A0    //Analog value of throttle body position sensor #1
#define PWM_HOLD 18   //Value sent to analogWrite() that best holds the TB in a position against the internal return spring
#define PWM_SLOW 60   //Use this value to slowly open the TB (for testing purposes)

int target = 250;     //Target value for TB_TPS1

//One time setup
void setup()
{
  Serial.begin(9600);         //Console for debug

  //Setup pins
  pinMode(FWD, OUTPUT);       //Output high when going increasing throttle

  //Make sure we are stopped
  analogWrite(FWD, LOW);
  delay(1000); //1 sec delay to let the return spring do its thing
}

void loop()
{
  //Use this block of code to stop the TB while developing
  while(1);

  //Start opening up the TB
  analogWrite(FWD, PWM_SLOW);

  int nb_tries = 0; //Number of iterations in the while loop below

  //Keep opening until we reach target
  while(analogRead(A0) > target)
  {
    nb_tries++; //increase number of tries
    delay(50);
  }

  //By here we should have reached the target (or exceeded)
  //Lower number on analogRead = TB more open
  //Higher number on analogRead = TB closed

  Serial.print("Target: ");
  Serial.print(target);
  Serial.print(", Value: ");
  Serial.print(analogRead(TB_TPS1));
  Serial.print(", tries: ");
  Serial.println(nb_tries);

  //Hold TB
  analogWrite(FWD, PWM_HOLD);

  nb_tries = 0;

  delay(1000);

  //Close TB again
  analogWrite(FWD, LOW);
  delay(1000); //1 sec delay to let the return spring do its thing


  //analogRead is between 0 and 1023
}
