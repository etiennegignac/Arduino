/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "gyOLQqL94BcPKjtSaNAZgvp0dIpMyv3q";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OverlandFummins";
char pass[] = "Potato123!";

long randomNumber = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, IPAddress(192,168,6,10), 8080);

  randomSeed(analogRead(0)); //Seed for random generation
}

void generateRandom()
{
  randomNumber = random(255);
}

BLYNK_WRITE(V0)
{
  int value = param.asInt(); //Check command, 0 = generate

  Serial.println(value);

  if(value == 0)
  {
    //generate new random number
    generateRandom();
    //Send it to blynk
    Blynk.virtualWrite(V1, randomNumber);

  }
}

void loop()
{
  Blynk.run();
}

