#include <WiFiNINA.h>   // Include WiFiNINA library for WiFi connectivity
#include <Wire.h>       // Include Wire library for I2C communication
#include <BH1750.h>     // Include BH1750 library for the light sensor

// WiFi credentials
char wifiSSID[] = "Skater";      // SSID of the WiFi network
char wifiPassword[] = "Uv@235888";  // Password for the WiFi network

WiFiClient wifiClient;  // Create a WiFiClient object to handle communication with the server

BH1750 lightSensor;  // Create a BH1750 object for the light sensor

// IFTTT Webhook settings
char iftttHostName[] = "maker.ifttt.com"; // Hostname for IFTTT server
String iftttPathName = "https://maker.ifttt.com/trigger/TASK 3.1p/with/key/hivzkc5dKbyk9GhIhXJwPZdItvJwFTHfbyaVFJcBkd1"; // Full URL for the IFTTT webhook

void setup() 
{
  WiFi.begin(wifiSSID, wifiPassword);  // Start the WiFi connection
  Serial.begin(9600);                   // Initialize serial communication for debugging

  Wire.begin();                         // Initialize I2C communication
  lightSensor.begin();                  // Initialize the light sensor

  Serial.println(F("BH1750 Test begin")); // Print message indicating test has started

  // Attempt to connect to WiFi
  connectToWiFi();
}

void loop() 
{
  float lightLevel = lightSensor.readLightLevel();  // Read light level from the sensor
  Serial.print("lightLevel: ");                       // Print light level to the serial monitor
  Serial.println(lightLevel);
  
  // Trigger IFTTT webhook based on light level thresholds
  if (lightLevel >= 260) 
  {
    triggerIFTTTWebhook("Solar Light");  // Trigger webhook for bright light condition
  }
  else if (lightLevel < 60) 
  {
    triggerIFTTTWebhook("Nightfall");     // Trigger webhook for low light condition
  }
  
  delay(30000); // Wait for 30 seconds before the next reading
}

void connectToWiFi() 
{
  Serial.println("Attempting to connect to WiFi..."); // Print connection attempt message
  WiFi.begin(wifiSSID, wifiPassword);  // Start WiFi connection
  
  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);  // Delay for 0.5 seconds
    Serial.print("."); // Print a dot for each connection attempt
  }
  
  Serial.println("WiFi connected!"); // Print message upon successful connection
}

void triggerIFTTTWebhook(String eventName) 
{
  // Check if connected to WiFi, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectToWiFi(); 
  }
  
  // Connect to the IFTTT server and send a GET request
  if (wifiClient.connect("maker.ifttt.com", 80)) 
  {
    wifiClient.println("GET " + iftttPathName + " HTTP/1.1"); // Create HTTP GET request
    wifiClient.println("Host: " + String(iftttHostName));       // Specify the host
    wifiClient.println("Connection: close");                    // Close the connection after the response
    wifiClient.println();                                       // Blank line indicating end of headers
    delay(500);  // Small delay to allow response to be sent
    wifiClient.stop(); // Stop the client connection
    Serial.println("Data Transfer Successful"); // Indicate successful data transfer
  } 
  else 
  {
    Serial.println("Connection to Server Failed"); // Print error message if connection fails
  }
}