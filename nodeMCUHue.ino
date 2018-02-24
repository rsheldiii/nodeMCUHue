#include <WiFiClient.h>
#include <RestClient.h>
#include "credentials.h"

// Global variables
#define BUILTIN_LED 5
#define KEYSWITCH 0
#define LIGHT_GROUP 0

// Clients
WiFiClient client;
RestClient hue(HUE_HOST,HUE_PORT);

const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";

bool buttonPressed = false;

void setup() 
{
  // Start the Serial communication
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(SSID);

  // set LEDs are outputs
  pinMode(KEYSWITCH, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  // signify that we're starting to connect to WiFi
  digitalWrite(BUILTIN_LED, HIGH);

  // connect to WiFi
  WiFi.begin(SSID, PASSWORD);

  // wait until we're connected
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      //Serial.print(".");
  }

  // print connection status and IP address
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connected to WiFi; turn off red LED
  digitalWrite(BUILTIN_LED, LOW);

  // for testing
  //toggleLights();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    reconnectWifi();
  } else {
    // when button is pressed...
    if (digitalRead(KEYSWITCH) == LOW) {
      if (buttonPressed == false) {
        buttonPressed = true;
        toggleLights();
        delay(100);
      }
    } else {
      buttonPressed = false;
    }
  }
}

void toggleLights() {
  bool lightsAreOn = areAnyLightsOn(LIGHT_GROUP);
  String url = "/api/" + String(HUE_API_KEY) + "/groups/" + String(LIGHT_GROUP) + "/action";

  Serial.println(lightsAreOn);
  
  hue.put(url.c_str(),lightsAreOn ? LIGHTS_OFF : LIGHTS_ON);
}

bool areAnyLightsOn(int group) {
  String response = "";
  String url = "/api/" + String(HUE_API_KEY) + "/groups/" + String(group);

  hue.get(url.c_str(), &response); // Turn on light group

  return response.indexOf("\"any_on\":true") > 0;
}

void reconnectWifi() {
  // red LED on
  digitalWrite(BUILTIN_LED, HIGH);
  
  int numberOfTries = 0;
  
  while (WiFi.begin(SSID, PASSWORD) != WL_CONNECTED)
  {
      // unsuccessful, retry in 5 seconds
      //Serial.print("failed ... ");
      delay(5000);
      //Serial.print("retrying ... ");
  
      // how many times we've tried to reconnect
      numberOfTries++;
      // after failing for a minute
      if (numberOfTries > 20)
      {
          // reset it
          ESP.reset();
      }
  }
  
  // red LED off
  digitalWrite(BUILTIN_LED, LOW);
}
