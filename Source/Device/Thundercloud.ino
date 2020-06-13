#include "Arduino.h"
#include "AZ3166WiFi.h"
#include "AzureIotHub.h"
#include "DevKitMQTTClient.h"
#include "RGB_LED.h"

static RGB_LED rgbLed;
static bool hasWifi = false;
static bool hasIoTHub = false;

void blinkLights()
{
    rgbLed.setColor(0, 0, 255);
    digitalWrite(PB_6,HIGH);
}

static void InitWifi()
{
  Screen.print(2, "Getting Wi-Fi");

  if (WiFi.begin() == WL_CONNECTED)
  {
    Screen.print(2, "Wi-Fi Connected");
    hasWifi = true;    
  }
  else
  {
    Screen.print(1, "No Wi-Fi");
    hasWifi = false;    
  }
}

static void InitIoTHub()
{
  Screen.print(2, "Getting IoT Hub");  
  if (DevKitMQTTClient_Init())
  {
    Screen.print(2, "IoT Hub Connected");
    hasIoTHub = true;    
  }
  else
  {
    Screen.print(1, "No IoT Hub");
    hasIoTHub = false;    
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  blinkLights();
  Screen.print(1, payLoad, true);
  delay(300);
}

void setup()
{
  digitalWrite(PB_6,LOW);
  Screen.init();
  Screen.print(0, "Thundercloud");
  Screen.print(1, "Initializing...");  
  Screen.print(2, "Connecting Wi-Fi");
  hasWifi = false;
  InitWifi();
  if (!hasWifi)
  {
    return;
  }
  Screen.print(2, "Connecting IoT Hub");
  hasIoTHub = false;
  InitIoTHub();
  if (!hasIoTHub)
  {
    return;
  }
  Screen.print(1, "Device ready."); 
  Screen.print(2, "Powered by Azure");
  DevKitMQTTClient_SetOption(OPTION_MINI_SOLUTION_NAME, "Thundercloud");
  DevKitMQTTClient_Init(true);
  DevKitMQTTClient_SetMessageCallback(MessageCallback);
}

void loop()
{
  digitalWrite(PB_6,LOW);
  rgbLed.turnOff();
  if (hasWifi)
  {    
      Screen.print(1, "Device Ready");
      DevKitMQTTClient_Check();
  }
}