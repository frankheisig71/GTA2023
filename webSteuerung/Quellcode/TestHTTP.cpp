#include <Arduino.h>
#include <WiFi.h>
#include "TestHTTP.h"

// Replace with your network credentials
const char* ssid = "ESP32_AccessPoint";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request String header;
String header;
// Declare the pins to which the LEDs are connected

#define LED_R0T    21
#define LED_GRUEN  20

String greenstate = "off";// state of green LED
String redstate = "off";// state of red LED
void setup() {
  Serial.begin(115200);
  Serial.print ("Application CPU is on core:");
  Serial.println (xPortGetCoreID());
 // Set the pinmode of the pins to which the LEDs are connected and turn them low to prevent flunctuations
  pinMode(LED_GRUEN, OUTPUT);
  pinMode(LED_R0T, OUTPUT);
  digitalWrite(LED_GRUEN, LOW);
  digitalWrite(LED_R0T, LOW);
  // Connect to Wi-Fi network with SSID and password
  // Remove the password parameter, if you want the AP (Access Point) to be open
  Serial.println("Setting AP (Access Point)");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  server.begin();
}
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    //Serial.println("request begin -->");
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            //Serial.println("<-- request end");
            // turns the GPIOs on and off
            if (header.indexOf("GET /") >= 0){
              for(int i=0; i<siteHeader_length; i++){ client.println(siteHeader[i]);}
            }
            if (header.indexOf("GET /green/on") >= 0) {
              Serial.println("green on");
              greenstate = "on";
              digitalWrite(LED_GRUEN, HIGH);
            } else if (header.indexOf("GET /green/off") >= 0) {
              Serial.println("green off");
              greenstate = "off";
              digitalWrite(LED_GRUEN, LOW);
            } else if (header.indexOf("GET /red/on") >= 0) {
              Serial.println("red on");
              redstate = "on";
              digitalWrite(LED_R0T, HIGH);
            } else if (header.indexOf("GET /red/off") >= 0) {
              Serial.println("red off");
              redstate = "off";
              digitalWrite(LED_R0T, LOW);
            }
            // Display current state, and ON/OFF buttons for GPIO 16
            client.println("<p>green - State " + greenstate + "</p>");
            // If the green LED is off, it displays the ON button
            if (greenstate == "off") {
              client.println("<p><a href=\"/green/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/green/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            // Display current state, and ON/OFF buttons for GPIO 17
            client.println("<p>red - State " + redstate + "</p>");
            // If the red LED is off, it displays the ON button
            if (redstate == "off") {
              client.println("<p><a href=\"/red/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/red/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
