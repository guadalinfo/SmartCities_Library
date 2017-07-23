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

#define PIN_SENSOR_CORRIENTE_1 A0
#define PIN_SENSOR_CORRIENTE_2 A3

#define pre F("<input type=button onClick=\"var w=window.open('/arduino/dig/")
#define post F("','_parent');w.close();\" value='")


// Sensibilidad del sensor en V/A
//float SENSIBILITY = 0.185;   // Modelo 5A
//float SENSIBILITY = 0.100; // Modelo 20A
float SENSIBILITY = 0.066; // Modelo 30A

int SAMPLESNUMBER = 100;

int iPines[]={IN1,IN2,IN3,IN4};

void setup() {
 Serial.begin(9600);
 for(int i=0;i<4;i++)   {
    pinMode(iPines[i],OUTPUT);
    digitalWrite(iPines[i],HIGH);
  }
  Wifi.begin();
  Wifi.println("Web Server is up");
}

float corriente1,corriente2;

void loop() {

    while(Wifi.available()){
      process(Wifi);
    }
    corriente1=getCorriente(SAMPLESNUMBER,PIN_SENSOR_CORRIENTE_1);
    Serial.print(corriente1);
    Serial.print(", ");
    corriente2=getCorriente(SAMPLESNUMBER,PIN_SENSOR_CORRIENTE_2);
    Serial.println(corriente2);
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

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();
  client.println(F("<html>" \
                  "<head> </head>" \
                  "<body>"));

  String pre=F("<input type=button onClick=\"var w=window.open('/arduino/dig/");
  String post=F("','_parent');w.close();\" value='");
  for(int i=0;i<4;i++)
  {
    String s=String(iPines[i]);
    String sTatus=digitalRead(iPines[i])?"/1":"/0";
    String sNombre=digitalRead(iPines[i])?"ON":"OFF";
    client.print(s+pre+s+sTatus+post+sNombre+"'><br>");
  }

  client.print(F("<p>Corrientes:"));
  client.print(corriente1);
  client.print(",");
  client.print(corriente2);
  client.println("</p>");

  client.print(F("</body>" \
                  "</html>"));

  client.print(DELIMITER); // very important to end the communication !!!

}


float getCorriente(int samplesNumber,int iPin)
{
   float voltage;
   float corrienteSum = 0;
   for (int i = 0; i < samplesNumber; i++)
   {
      voltage = analogRead(iPin) * 5.0 / 1023.0;
      corrienteSum += (voltage - 2.5) / SENSIBILITY;
   }
   return(corrienteSum / samplesNumber);
}

void digitalCommand(WifiData client) {
  int pin, value = 0;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/d/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    //digitalWrite(pin, !value);
    digitalWrite(pin, !digitalRead(pin));
  }


  // Send feedback to client
  client.println("Status: 200 OK\n");
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.print(value);
  client.print(EOL);    //char terminator

  WebServer(client);
}
