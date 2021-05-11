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
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "HLinezNzj6FIqqjY96NTQF9Y3T-9DF0J";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Fritt WiFi";
char pass[] = "";

#define SIGNAL_PIN 14
#define T_MS_BLYNK_TIMER 3000L //!< [ms]

typedef struct {
  uint32_t t0;                //!< [ms]
  uint32_t pulses;            //!< [-] Total pulse count
  uint32_t tSampleTimestamp;  //!< [ms] Timestamp for sample
  uint32_t dtSampleTimestamp; //!< [ms] Delta time since last timestamp
  uint32_t tLastDebounce;     //!< [ms] Last debounce time
  uint32_t tDebounceDelay;    //!< [ms] Debounce delay time
} watermeter_t;

volatile watermeter_t watermeter;


// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  Serial.println(pinValue); 
}

void myTimerEvent()
{
  Blynk.virtualWrite(V1, watermeter.pulses);
  Blynk.virtualWrite(V2, watermeter.tSampleTimestamp);
  Blynk.virtualWrite(V3, watermeter.dtSampleTimestamp);
  Serial.println(watermeter.pulses);
}

                                                                                                      
void IRAM_ATTR pinInterrupt(){
  uint32_t tNow = millis();
  if ((tNow - watermeter.tLastDebounce) > watermeter.tDebounceDelay) {
    ++watermeter.pulses;
    watermeter.dtSampleTimestamp = tNow-watermeter.tSampleTimestamp;
    watermeter.tSampleTimestamp = tNow;
    watermeter.tLastDebounce = tNow;
  }
}



void setup()
{
                                                                                                                                      
    pinMode(SIGNAL_PIN, INPUT_PULLUP);

    attachInterrupt(SIGNAL_PIN, pinInterrupt, FALLING);

  watermeter.tDebounceDelay = 50; // [ms]

  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every T_MS_BLYNK_TIMER ms
  timer.setInterval(T_MS_BLYNK_TIMER, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
