/*

Lectura de datos atmosferico que se muestran en el LCD
Lectura de un sensor de humedad de suelo

https://github.com/javacasm/SmartCities_Domotica#sensor-bme280

-----------------------------
Vin (Voltage In)    ->  Vcc
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4
SCK (Serial Clock)  ->  A5

Sensor de Humedad de suelo
A0    -> A0
Vcc   -> V
GND   -> GND

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
#define PIN_HUMEDAD_SUELO A0
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);


BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

float temp(NAN), hum(NAN), pres(NAN);

int iHumedadSuelo;
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
  Wire.begin();
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  BME280::TempUnit tempUnit(BME280::TempUnit_Celcius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

}

void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}

void lee_humedadSuelo(){
  iHumedadSuelo=analogRead(PIN_HUMEDAD_SUELO);
}

void lee_datos(){
   bme.read(pres, temp, hum,  pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity,uint8_t pressureUnit = 0x0)
   lee_humedadSuelo();
}

void serial_datos(){
   Serial.print("Temp: ");
   Serial.print(temp);
   Serial.print("°C");


   Serial.print(" Humedad: ");
   Serial.print(hum);
   Serial.print("% ");


   Serial.print("\t\tPresion: ");
   Serial.print(pres);
   Serial.print(" atm");

   Serial.print("Humedad suelo: ");
   Serial.println(iHumedadSuelo);

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

   lcd.setCursor(8,1);
   lcd.print("HS:");
   lcd.print(iHumedadSuelo);
   lcd.print(" ");

}
/* ==== END Functions ==== */
