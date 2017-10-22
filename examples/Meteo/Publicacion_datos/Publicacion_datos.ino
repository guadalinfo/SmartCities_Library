/*
BME280 I2C Test.ino
This code shows how to record data from the BME280 environmental sensor
using I2C interface. This file is an example file, part of the Arduino
BME280 library.
Copyright (C) 2016  Tyler Glenn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Written: Dec 30 2015.
Last Updated: Sep 19 2016.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* ==== Includes ==== */
#include <UnoWiFiDevEd.h>
#include <BME280I2C.h>
#include <Wire.h>             // Needed for legacy versions of Arduino.
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
#define CONNECTOR "mqtt"
#define TOPIC "topic/test"
#define intervalo 5000
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
bool metric = true;

float temp(NAN), hum(NAN), pres(NAN);

long ultima_medida=millis();


/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* === Print a message to stream with the temp, humidity and pressure. === */
void printBME280Data(Stream * client);
/* === Print a message to stream with the altitude, and dew point. === */
void printBME280CalculatedData(Stream* client);
/* ==== END Prototypes ==== */

/* ==== Setup ==== */
void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  lcd.init();
  lcd.backlight();


  Wifi.begin();
  Wifi.println("Web Server is up");

  Ciao.begin();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {
   if((millis()-ultima_medida)>intervalo)    {

    printBME280Data(&Serial);
    printBME280CalculatedData(&Serial);

    Ciao.write(CONNECTOR, TOPIC, String(22.6)); // pushes data into a channel called PIN_A1

    ultima_medida=millis();
   }
    while(Wifi.available()){
      process(Wifi);
    }

   delay(50);
}
/* ==== End Loop ==== */


/* ==== Functions ==== */
void printBME280Data(Stream* client){

                                          // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
   bme.read(pres, temp, hum, metric, pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
  /* Alternatives to ReadData():
    float temp(bool celsius = false);
    float pres(uint8_t unit = 0);
    float hum();

    Keep in mind the temperature is used for humidity and
    pressure calculations. So it is more effcient to read
    temperature, humidity and pressure all together.
   */
  client->print("Temp: ");
  client->print(temp);
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  //lcd.print("c");
  client->print("°"+ String(metric ? 'C' :'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  lcd.setCursor(8,0);
  lcd.print("H:");
  lcd.print(hum);
  lcd.print("%");
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->print(" atm");
  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(pres);
  lcd.print("atm");

}
void printBME280CalculatedData(Stream* client){
  float altitude = bme.alt(metric);
  float dewPoint = bme.dew(metric);
  client->print("A: ");
  client->print(altitude);
  client->print((metric ? "m" : "ft"));
  client->print("\t\tDew point: ");
  client->print(dewPoint);
  client->println("°"+ String(metric ? 'C' :'F'));

  /*lcd.setCursor(8,1);
  lcd.print("A ");
  lcd.print(altitude);
  lcd.print((metric ? "m" : "ft"));
*/
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  if (command == "webserver") {
    WebServer(client);
  }
}
void WebServer(WifiData client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("Refresh: 5");  // refresh the page automatically every  sec
  client.println();
  client.println("<html>");
  client.println("<head> <title>UNO WIFI Example</title> </head>");
  client.print("<body>");

  client.print("Temperatura:");
  client.print(temp);
  client.print("<br/>");

  client.print("Humedad:");
  client.print(hum);
  client.print("<br/>");

  client.print("Presion:");
  client.print(pres);
  client.print("<br/>");


  client.print("</body>");
  client.println("</html>");
  client.print(DELIMITER); // very important to end the communication !!!
}

/* ==== END Functions ==== */
