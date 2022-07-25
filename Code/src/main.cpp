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
  
  Code released under MIT License
*/

#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "YourWiFiName";           // name of your WiFi network
const char *password = "YourWiFiPassword"; // password of the WiFi network

int WasherOffLEDPin = 12; // Washer OFF status light (Green)
int WasherOnLEDPin = 14;  // Washer ON status light (Red)
int DryerOffLEDPin = 33;  // Dryer OFF status light (Green)
int DryerOnLEDPin = 25;   // Dryer ON status light (Red)
int Misc1LEDPin = 26;  // Misc indicator light 1 (Green)
int Misc2LEDPin = 27;  // Misc indicator light 2 (Red)

const char *ID = "ESP32-WasherStatus";      // Hostname of our device, must be unique
const char *TOPIC1 = "sensors/washerstatus"; // MQTT topic for washing machine status
const char *TOPIC2 = "sensors/dryerstatus"; // MQTT topic for clothes dryer status indication
const char *TOPIC3 = "sensors/misc"; // MQTT topic for misc status indication

IPAddress broker(192, 168, 1, 1); // IP address of your MQTT on OpenHab server
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
      Serial.print("MQTT says washer is ON");
      digitalWrite(WasherOnLEDPin, HIGH); // Turn on washer ON (Red) LED
      digitalWrite(WasherOffLEDPin, LOW); // Turn off washer OFF (Green) LED
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says washer is OFF");
      digitalWrite(WasherOnLEDPin, LOW);   // Turn off washer ON (Red) LED
      digitalWrite(WasherOffLEDPin, HIGH); // Turn on washer OFF (Green) LED
    }
  }
  if (strcmp(topic, TOPIC2) == 0)
  {
    if (response == "ON")
    {
      Serial.print("MQTT says dryer is ON");
      digitalWrite(DryerOnLEDPin, HIGH); // Turn on dryer ON (Red) LED
      digitalWrite(DryerOffLEDPin, LOW); // Turn off dryer OFF (Green) LED
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says dryer is OFF");
      digitalWrite(DryerOnLEDPin, LOW);   // Turn off dryer ON (Red) LED
      digitalWrite(DryerOffLEDPin, HIGH); // Turn on dryer OFF (Green) LED
    }
  }
  if (strcmp(topic, TOPIC3) == 0)
  {
    if (response == "ON")
    {
      Serial.print("MQTT says misc is ON");
      digitalWrite(Misc1LEDPin, HIGH); // Turn on Misc 1 (Green) LED
      digitalWrite(Misc2LEDPin, LOW); // Turn off Misc 2 (Red) LED
    }
    else if (response == "OFF")
    {
      Serial.print("MQTT says misc is OFF");
      digitalWrite(Misc1LEDPin, LOW);   // Turn off Misc 1 (Green) LED
      digitalWrite(Misc2LEDPin, HIGH); // Turn on Misc 2 (Red) LED
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
  Serial.begin(9600);               // Start serial communication at 9600 baud
  pinMode(WasherOnLEDPin, OUTPUT);  // Configure Washer On pin as an output
  pinMode(WasherOffLEDPin, OUTPUT); // Configure Washer Off pin as an output
  pinMode(DryerOnLEDPin, OUTPUT); // Configure Dryer On pin as an output
  pinMode(DryerOffLEDPin, OUTPUT); // Configure Dryer Off pin as an output
  pinMode(Misc1LEDPin, OUTPUT); // Configure Misc indicator 1 pin as an output
  pinMode(Misc2LEDPin, OUTPUT); // Configure Misc indicator 2 pin as an output

  Serial.print("Testing LED bulbs");
  digitalWrite(WasherOnLEDPin, HIGH); // Turn on washer OFF (Green) LED
  delay(250);
  digitalWrite(WasherOnLEDPin, LOW); // Turn off washer OFF (Green) LED
  delay(250);
  digitalWrite(WasherOffLEDPin, HIGH); // Turn on washer ON (Red) LED
  delay(250);
  digitalWrite(WasherOffLEDPin, LOW); // Turn off washer ON (Red) LED
  delay(250);
  digitalWrite(DryerOnLEDPin, HIGH); // Turn on dryer OFF (Green) LED
  delay(250);
  digitalWrite(DryerOnLEDPin, LOW); // Turn off dryer OFF (Green) LED
  delay(250);
  digitalWrite(DryerOffLEDPin, HIGH); // Turn on dryer ON (Red) LED
  delay(250);
  digitalWrite(DryerOffLEDPin, LOW); // Turn off dryer ON (Red) LED
  delay(250);
  digitalWrite(Misc1LEDPin, HIGH); // Turn ON Misc LED 1 (Green) LED
  delay(250);
  digitalWrite(Misc1LEDPin, LOW); // Turn OFF Misc LED 1 (Green) LED
  delay(250);
  digitalWrite(Misc2LEDPin, HIGH); // Turn ON Misc LED 2 (Red) LED
  delay(250);
  digitalWrite(Misc2LEDPin, LOW); // Turn OFF Misc LED 2 (Red) LED
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