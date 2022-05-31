#include "DHT.h"
#include <WiFiClient.h>
#include <WiFi.h>
#include <SPI.h>
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
float humidityData;
float temperatureData;
int percentageHumidity;
int sensorVal;
const char* ssid = "Abby-PC-WIFI";    
const char* password = "00000000"; 
const int dry=3351;
const int wet=1072;
//WiFiClient client;
char server[] = "192.168.137.128";
WiFiClient client;    
void setup()
{
 Serial.begin(115200);
  delay(10);
  dht.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 }
void loop()
{ 
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature(); 
  Sending_To_phpmyadmindatabase(); 
  sensorVal=analogRead(A0);
  percentageHumidity=map(sensorVal,wet,dry,100,0);
 
  delay(30000); // interval
 }

 void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    //Serial.print("GET /project/insert.php?temperature=50&humidity=55.5&soilmoisture=79project/insert.php?temperature=,humidity=");
    client.print("GET /project/insert.php?");     //YOUR URL
    client.print("temperature=");
    Serial.print("temperature=");
    client.print(temperatureData);
    Serial.print(temperatureData);
    client.print("&humidity=");
    Serial.print("&humidity=");
    client.print(humidityData);
    Serial.print(humidityData);
    client.print("&soilmoisture=");
    Serial.print("&soilmoisture=");
    client.print(percentageHumidity);
    Serial.print(percentageHumidity);
    Serial.print("%");
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.137.128"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
