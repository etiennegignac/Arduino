/*//////////////////////////////////////////////////////////////////////////////
Name:  TB_PWM_TEST
CPU:   UNO
Date:  7 Dec 25
Description:  Use this to write a PWM value to the H-bridge and see which value can hold it in position against the internal return spring

**********  WIRING  **********
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
  //while(1); //Use this to stop the TB while developing
  
  
  /*Results:
  0: Nothing
  15: Not quite enough to hold against the internal return spring
  17: Not quite enough to hold against the internal return spring
  18: Works, current = 0.0-0.1 (on 12v supply)  <<--------------------- WINNER
  20: Works, current = 0.1 (on 12v supply)
  25: Works, current = 0.1-0.2A (on 12v supply)
  30: Good candidate, maybe moves a tiny bit at first
  40: Opens a little and stays
  50: Slow and notchy (not smooth)
  60: Slowish, controlled
  70: on the faster side, could be good for calibration
  100: Full
  */
  analogWrite(FWD, 100);
}
