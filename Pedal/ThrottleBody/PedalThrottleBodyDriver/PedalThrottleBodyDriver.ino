/*//////////////////////////////////////////////////////////////////////////////
Name:  PedalThrottleBodyDriver
CPU:   UNO
Date:  17 August 2025


**********  WIRING  **********
TRUCK WIRING:
  - 12 volts to 3.3v regulator
  - Ground to 3.3v regulator
  - 12 volts to H-bridge
  - Ground to H-bridge

TRUCK PEDAL:
  - 5 volts from UNO
  - Ground from UNO
  - Signal back to UNO for analog read

UNO:
  - 5v from regulator
  - ground from regulator
  - 5v to H-bridge control
  - ground to H-bridge control

  - OUTPUT PWM to H-Bridge. 



H-BRIDGE:
Pin No      Function        Description
1           RPWM            Forward Level or PWM signal, Active High
2           LPWM            Reverse Level or PWM signal, Active High --> WE DONT USE THIS
3           R_EN            Forward Drive Enable Input, Active High/ Low Disable --> ENABLE
4           L_EN            Reverse Drive Enable Input, Active High/Low Disable --> DISABLE
5           R_IS            Forward Drive, Side current alarm output
6           L_IS            Reverse Drive, Side current alarm output
7           Vcc             +5V Power Supply microcontroller --> Also works on 3.3v according to datasheet, to 3.3V of Teensy
8           Gnd             Ground Power Supply microcontroller --> to ground of Teensy

////////////////////////////////////////////////////////////////////////////////*/
#define TB_IN A0      //Pin to read abalog voltage from the throttle body position sensor
#define PEDAL_IN A1   //Input voltage to be read from truck pedal sensor
#define FWD 9         //Output PWM to H-Bridge (motor forward (LPWM in datasheet))
#define REV 10        //Output PWM to H-Bridge (motor reverse (RPWM in datasheet))

//Global variables
int pedalValue = 0;   //Value read from truck pedal
int TBValue = 0;      //Value read from throttle body sensor
int TBOpen = 62;       //Throttle body sensor when fully open
int TBClosed = 850;     //Throttle body sensor when fully closed

//One time setup
void setup()
{
  Serial.begin(9600);         //Console for debug

  //Setup pins
  pinMode(TB_IN, INPUT);      //Analog voltage from throttle body sensor
  pinMode(PEDAL_IN, INPUT);   //Analog voltage from truck pedal
  pinMode(FWD, OUTPUT);       //Output high when going increasing throttle
  pinMode(REV, OUTPUT);       //Output high when decresing throttle

  //Make sure we are stopped
  digitalWrite(FWD, LOW);
  digitalWrite(REV, LOW);

  //calibrate();
}

//Moves throttle body at both extremes to check sensor values
void calibrate()
{
  //Move all the way open
  digitalWrite(FWD, 250);
  digitalWrite(REV, 0);
  delay(250);

  //TODO, read and save sensor value

  //Move all the way closed
  digitalWrite(FWD, 0);
  digitalWrite(REV, 250);
  delay(250);

  //TODO, read and save sensor value

  //Cal finished
}

void loop()
{
  while(abs(analogRead(TB_IN) - TBOpen) > 25)
  {
    Serial.print("Sensor value: ");
    Serial.println(analogRead(TB_IN));
    //Move all the way open
    analogWrite(FWD, 50);
    analogWrite(REV, 0);
    //delay(250);
  }

  analogWrite(FWD, 0);
  analogWrite(REV, 0);

  delay(1000);

  while(abs(analogRead(TB_IN) - TBClosed) > 25)
  {
    Serial.print("Sensor value: ");
    Serial.println(analogRead(TB_IN));
    //Move all the way open
    analogWrite(FWD, 0);
    analogWrite(REV, 50);
    //delay(250);
  }

  analogWrite(FWD, 0);
  analogWrite(REV, 0);

  delay(1000);
  
  //Test output TB position sensor value
  //Serial.print("Sensor value: ");
  //Serial.println(analogRead(TB_IN));
  //delay(250);
}
