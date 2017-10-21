/*
File: WebServerBlink.ino
This example creates a simple web server on your Arduino Uno WiFi. It serves a
simple web page with 2 buttons that can be used to switch on and off on-board led 13.
Please type on your browser http://<IP>/arduino/webserver/ or http://<hostname>.local/arduino/webserver/

Note: works only with Arduino Uno WiFi Developer Edition.

http://www.arduino.org/learning/tutorials/boards-tutorials/webserverblink
*/

#include <Wire.h>
#include <UnoWiFiDevEd.h>
#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8

void setup() {
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);
    Wifi.begin();
    Wifi.println("Web Server is up");
}
void loop() {

    while(Wifi.available()){
      process(Wifi);
    }
  delay(50);
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "webserver") {
    WebServer(client);
  }

  if (command == "dig") {
    digitalCommand(client);
  }
}

void WebServer(WifiData client) {

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html>");

  client.println("<head> </head>");
  client.print("<body>");
  String pre="<input type=button onClick=\"var w=window.open('/arduino/dig/";
  
  for(int i=8;i<12;i++)
  {
    String s=String(i);
    
    client.print(s+pre+s+"/1','_parent');w.close();\"value='ON'>");
    client.print(pre+s+"/0','_parent');w.close();\"value='Off'><br>");
  }
  client.print("</body>");
  client.println("</html>");
  client.print(DELIMITER); // very important to end the communication !!!
  
}

void digitalCommand(WifiData client) {
  int pin, value = 0;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/d/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, !value);
  }

  // Send feedback to client
  client.println("Status: 200 OK\n");
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.print(value);
  client.print(EOL);    //char terminator

}
