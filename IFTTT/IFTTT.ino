#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

// WiFi credentials
char wifiSSID[] = "naman";      // SSID
char wifiPassword[] = "naman123";  // PASSWORD

WiFiClient wifiClient; 

BH1750 lightSensor; 

char iftttHostName[] = "maker.ifttt.com"; // Hostname for IFTTT server
String iftttPathName = "https://maker.ifttt.com/trigger/LIGHT_DETECTED/with/key/peKGXMpeOgB8nqY9GWJCFB3JDrvZtuZrlhWmZ91zHVF"; // Webhook URL

void setup() 
{
  WiFi.begin(wifiSSID, wifiPassword); 
  Serial.begin(9600); 

  Wire.begin(); 
  lightSensor.begin(); 

  Serial.println(F("BH1750 Test begin"));

  // Attempt to connect to WiFi
  connectToWiFi();
}

void loop() 
{
  float lightLevel = lightSensor.readLightLevel(); 
  Serial.print("lightLevel: ");
  Serial.println(lightLevel);
  

  if (lightLevel >= 260) 
  {
    triggerIFTTTWebhook("Solar Light"); 
  }
  else if (lightLevel < 60) 
  {
    triggerIFTTTWebhook("Nightfall");
  }
  
  delay(30000); // delay 30 sec 
}

void connectToWiFi() 
{
  Serial.println("Attempting to connect to WiFi...");
  WiFi.begin(wifiSSID, wifiPassword);  // WiFi connection
  
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);  // delay 0.5 seconds
    Serial.print(".");
  }
  
  Serial.println("WiFi connected!"); // successful connection
}

void triggerIFTTTWebhook(String eventName) 
{

  if (WiFi.status() != WL_CONNECTED) 
  {
    connectToWiFi(); 
  }
  
  if (wifiClient.connect("maker.ifttt.com", 80)) 
  {
    wifiClient.println("GET " + iftttPathName + " HTTP/1.1");
    wifiClient.println("Host: " + String(iftttHostName));
    wifiClient.println("Connection: close");
    wifiClient.println();
    delay(500);
    wifiClient.stop(); 
    Serial.println("Data Transfer Successful");
  } 
  else 
  {
    Serial.println("Connection to Server Failed"); 
  }
}