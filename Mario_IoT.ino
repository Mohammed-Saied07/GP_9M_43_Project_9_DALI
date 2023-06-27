#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h> // uart

SoftwareSerial s(1, 3); // uart

// WiFi information
const char WIFI_SSID[] = "realme";
const char WIFI_PSK[] = "Ahmed1234";

// Remote site information
const char http_site[] = "test55.freetzi.com";
const int http_port = 80;

// Pin definitions
const int LED_PIN = 5;

// Global variables
WiFiClient client;

void setup() {
  
  // Set up serial console to read web page
  s.begin(115200);
  Serial.begin(115200); // uart
  Serial.print("Thing GET Example");
  
  // Set up LED for debugging
  pinMode(LED_PIN, OUTPUT);
  
  // Connect to WiFi
  connectWiFi();
  
  // Attempt to connect to website
  if ( !getPage() ) {
    Serial.println("GET request failed");
  }
}
//------------------------------------------------------------------
void loop() {
  
  // If there are incoming bytes, print them
  if ( client.available() ) {
  unsigned char getS = '0'; // uart
  //Serial.write(getS);
  getS = s.read();  // uart
  //Serial.println(getS);
  
  if(getS == 'A')
  {
    s.write(client.read()); // uart
    //Serial.println("");
  }
 /* if(getS == 65)
  {
    getS = 0;
    byte index = 0;
    char c;
    char buff[50] = {0};
    do{
      c = client.read();
      //Serial.print(c);
      //Serial.write(c);
      buff[index] = c;
      Serial.write(buff[index]);
      delay(1000);
      index++;
    }
    while(c != '\n');
    //Serial.write(buff, index);
    //Serial.print('\n');
  }*/
  }
  
  // If the server has disconnected, stop the client and WiFi
  /*if ( !client.connected() ) {
    Serial.println();
    
    // Close socket and wait for disconnect from WiFi
    client.stop();
    if ( WiFi.status() != WL_DISCONNECTED ) {
      WiFi.disconnect();
    }
    
    // Turn off LED
    digitalWrite(LED_PIN, LOW);
    
    // Do nothing
    Serial.println("Finished Thing GET test");
    while(true){
      delay(1000);
    }
  }*/
}
//------------------------------------------------------------------
// Attempt to connect to WiFi
void connectWiFi() {
  
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(LED_PIN, led_status);
    led_status ^= 0x01;
    delay(100);
  }
  
  // Turn LED on when we are connected
  digitalWrite(LED_PIN, HIGH);
}

// Perform an HTTP GET request to a remote page
bool getPage() {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.println("GET /device.txt HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}