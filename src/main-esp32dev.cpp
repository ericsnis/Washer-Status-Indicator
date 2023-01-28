/*
  Washing machine status for ESP32
  2021 EricSnis
  
  It connects to an MQTT server then:
  - checks washing machine status (topic sensors/washerstatus)
  - checks clothes dryer status (topic sensors/dryerstatus)
  - checks miscellaneous status (topic sensors/misc)
  - Indicates OFF with green LED (Ready)
  - Indicates ON with red LED (In Use)
  MQTT Topic can be changed to suit by changing TOPIC1, TOPIC2 & TOPIC3
  
  Assumes use of ESP32 based NodeMCU board, not similarly named ESP8266 NodeMCU.
  - Can be changed to other boards if pinout is changed
  - LED board would need to be changed if another board is used
  
  Uses the following libraries
  - Neopixel library by Adafruit released under LGPL 3.0 license
  - PubSubClient by Nick O'Leary released under MIT license
  - ESP32 core libraries by Espressif released under GNU Lesser General Public
  
  Code is provided as is without guarantees express or implied
  
  Code released under MIT License
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "WiFiSSID";           // name of your WiFi network
const char *password = "WiFiPassword";   // password of the WiFi network

int NeoPixelPin = 18; // Onboard NeoPixels pin

const char *ID = "ESP32-WasherStatus";      // Hostname of our device, must be unique
const char *TOPIC1 = "sensors/washerstatus"; // MQTT topic for washing machine status
const char *TOPIC2 = "sensors/dryerstatus";  // MQTT topic for clothes dryer status indication
const char *TOPIC3 = "sensors/misc";         // MQTT topic for misc status indication

// Set up the NeoPixels
Adafruit_NeoPixel indicator = Adafruit_NeoPixel(3, NeoPixelPin, NEO_GRB + NEO_KHZ800);

IPAddress broker(192, 168, 1, 11); // IP address of your MQTT on OpenHab server
WiFiClient wclient;

PubSubClient client(wclient); // Setup MQTT client

// Handle incomming messages from the broker
void callback(char *topic, byte *payload, unsigned int length)
{
  String response;

  for (int i = 0; i < length; i++)
  {
    response += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(response);

  if (strcmp(topic, TOPIC1) == 0)
  {
    if (response == "ON")
    {
      Serial.print("MQTT says washer is ON (In Use)");
      indicator.setPixelColor(0, 8, 0, 0); // Set washer NeoPixel to red
      indicator.show();                      // Push the NeoPixel update
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says washer is OFF (Ready)");
      indicator.setPixelColor(0, 0, 8, 0); // Set washer NeoPixel to green
      indicator.show();                      // Push the NeoPixel update
    }
  }
  if (strcmp(topic, TOPIC2) == 0)
  {
    if (response == "ON")
    {
      Serial.print("MQTT says dryer is ON (In Use)");
      indicator.setPixelColor(1, 8, 0, 0); // Set dryer NeoPixel to red
      indicator.show();                      // Push the NeoPixel update
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says dryer is OFF (Ready)");
      indicator.setPixelColor(1, 0, 8, 0); // Set dryer NeoPixel to green
      indicator.show();                      // Push the NeoPixel update
    }
  }
  if (strcmp(topic, TOPIC3) == 0)
  {
    if (response == "ON")
    {
      Serial.print("MQTT says misc is ON");
      indicator.setPixelColor(2, 8, 0, 0); // Set misc NeoPixel to red
      indicator.show();                      // Push the NeoPixel update
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says misc is OFF");
      indicator.setPixelColor(2, 0, 8, 0); // Set misc NeoPixel to green
      indicator.show();                      // Push the NeoPixel update
    }
  }
}

// Connect to WiFi network
void setup_wifi()
{
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{ // Reconnect to client
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID))
    {
      client.subscribe(TOPIC1);
      client.subscribe(TOPIC2);
      client.subscribe(TOPIC3);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC1);
      Serial.println('\n');
      Serial.println(TOPIC2);
      Serial.println('\n');
      Serial.println(TOPIC3);
      Serial.println('\n');
    }
    else
    {
      Serial.println(" try again in 5 seconds"); // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);         // Start serial communication at 115200 baud
  pinMode(NeoPixelPin, OUTPUT); // Configure Washer NeoPixel pin as an output
  // pinMode(WasherLEDPin, OUTPUT); // Configure Washer NeoPixel pin as an output
  // pinMode(DryerLEDPin, OUTPUT);  // Configure Dryer NeoPixel pin as an output
  // pinMode(MiscLEDPin, OUTPUT);   // Configure Misc indicator NeoPixel pin as an output
  indicator.begin();            // Initialize NeoPixels
  indicator.setBrightness(128); // Set NeoPixel brightness to 50%

  Serial.print("Testing NeoPixels");
  indicator.setPixelColor(0, 8, 0, 0); // Set washer NeoPixel to red
  indicator.setPixelColor(1, 8, 0, 0); // Set dryer NeoPixel to red
  indicator.setPixelColor(2, 8, 0, 0); // Set Misc NeoPixel to red
  indicator.show();                      // Push the NeoPixel update
  delay(500);
  indicator.setPixelColor(0, 0, 8, 0); // Set washer NeoPixel to green
  indicator.setPixelColor(1, 0, 8, 0); // Set dryer NeoPixel to green
  indicator.setPixelColor(2, 0, 8, 0); // Set Misc NeoPixel to green
  indicator.show();                      // Push the NeoPixel update
  delay(500);
  indicator.setPixelColor(0, 0, 0, 0); // Set washer NeoPixel to off
  indicator.setPixelColor(1, 0, 0, 0); // Set dryer NeoPixel to off
  indicator.setPixelColor(2, 0, 0, 0); // Set Misc NeoPixel to off
  indicator.show();                    // Push the NeoPixel update
  delay(100);

  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
  client.setCallback(callback); // Initialize the callback routine
}

void loop()
{
  if (!client.connected()) // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
}