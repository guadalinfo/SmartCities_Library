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



/* ==== Includes ==== */
#include <BME280I2C.h>          // Sensor meteo
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD

/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 9600
#define pressureUnit 3
#define metric true
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);


BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

float temp(NAN), hum(NAN), pres(NAN);

/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* === Print a message to stream with the temp, humidity and pressure. === */
void printBME280Data(Stream * client);
/* === Print a message to stream with the altitude, and dew point. === */
void printBME280CalculatedData(Stream* client);
/* ==== END Prototypes ==== */

/* ==== Setup ==== */
void setup() {
  setup_Serial();
  setup_BME280();
  setup_LCD();



}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   lee_datos();
   serial_datos();
   lcd_datos();

   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */

void setup_Serial(){
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait

}

void setup_BME280(){
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

}

void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}

void lee_datos(){
   bme.read(pres, temp, hum, metric, pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
}

void serial_datos(){
   Serial.print("Temp: ");
   Serial.print(temp);
   Serial.print("°C");


   Serial.print(" Humedad: ");
   Serial.print(hum);
   Serial.print("% ");


   Serial.print("\t\tPressure: ");
   Serial.print(pres);
   Serial.println(" atm");

}
void lcd_datos(){

   lcd.setCursor(0,0);
   lcd.print("T:");
   lcd.print(temp);

   lcd.setCursor(8,0);
   lcd.print("H:");
   lcd.print(hum);
   lcd.print("%");


   lcd.setCursor(0,1);
   lcd.print("P:");
   lcd.print(pres);
   lcd.print("atm");

}
/* ==== END Functions ==== */
