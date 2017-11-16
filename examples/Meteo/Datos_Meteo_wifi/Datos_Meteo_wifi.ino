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


/* ==== Includes ==== */
#include <UnoWiFiDevEd.h>
#include <BME280I2C.h>
#include <Wire.h>             // Needed for legacy versions of Arduino.
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 9600

#define PIN_CO2 A0
#define PIN_GAS A1

#define metric true
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

float temp(NAN), hum(NAN), pres(NAN);
int iGas,iCO2;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);


/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* ==== END Prototypes ==== */

/* ==== Setup ==== */

void setup_serial(){
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
}

void setup_lcd(){
    lcd.init();
  lcd.backlight();
}

void setup_sensor(){
  while(!bme.begin()){
    Serial.println(F("Could not find BME280 sensor!"));
    lcd.setCursor(0,0);
    lcd.print(F("Could not find BME280 sensor!"));
    delay(1000);
  }

}

void setup_wifi(){
  Wifi.begin();
  Wifi.println(F("Web Server is up"));

}

void setup() {
  setup_serial();
  setup_lcd();
  setup_sensor();
  setup_wifi();

}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {
   printBME280Data(&Serial);

   printGasData(&Serial);

   while(Wifi.available()){
      process(Wifi);
  }

   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void printGasData(Stream* client){
  iGas=analogRead(PIN_GAS);
  iCO2=analogRead(PIN_CO2);
  client->print(F("\tGas:"));
  client->print(iGas);
  client->print(F("Co2:"));
  client->println(iCO2);
  lcd.setCursor(0,1);
  lcd.print("Gas:");
  lcd.print(iGas);
  lcd.print(" CO2:");
  lcd.print(iCO2);
}

void printBME280Data(Stream* client){

   BME280::TempUnit tempUnit(BME280::TempUnit_Celcius);
   BME280::PresUnit presUnit(BME280::PresUnit_atm);

   bme.read(pres, temp, hum,tempUnit, presUnit );                    // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)


  client->print(F("T: "));
  client->print(temp);
  lcd.setCursor(0,0);
  lcd.print(F("T:"));
  lcd.print(int(temp));
  lcd.print(" ");
  client->print("°"+ String(metric ? 'C' :'F'));
  client->print(F("\tH: "));
  client->print(hum);
  lcd.setCursor(5,0);
  lcd.print("H:");
  lcd.print(int(hum));
  lcd.print("%");
  client->print(F("%\tP: "));
  client->print(pres);
  client->print(F(" a"));
  lcd.setCursor(10,0);
  lcd.print("P:");
  lcd.print(pres);
  lcd.print("atm");

}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  if (command == "webserver") {
    WebServer(client);
  }
}
void WebServer(WifiData client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println(F("Refresh: 5"));  // refresh the page automatically every  sec
  client.println();
  client.println(F("<html>"));
  client.println(F("<head> <title>UNO WIFI Example</title> </head>"));
  client.print(F("<body>"));

  client.print(F("Temperatura:"));
  client.print(temp);
  client.print(F("<br/>"));

  client.print(F("Humedad:"));
  client.print(hum);
  client.print(F("<br/>"));

  client.print(F("Presion:"));
  client.print(pres);
  client.print(F("<br/>"));

  client.print(F("Gas:"));
  client.print(iGas);
  client.print(F("<br/>"));
  client.print(F("Co2:"));
  client.print(iCO2);
  client.print(F("<br/>"));

  client.print(F("</body>"));
  client.println(F("</html>"));
  client.print(DELIMITER); // very important to end the communication !!!
}

/* ==== END Functions ==== */
