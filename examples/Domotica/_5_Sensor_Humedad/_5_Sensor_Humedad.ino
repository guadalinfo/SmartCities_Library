/*

Lectura de datos atmosferico que se muestran en el LCD
Lectura de un sensor de humedad de suelo
Publicamos los datos en la web

Acceso wifi a
http://192.168.1.<ID>/arduino/webserver

https://github.com/javacasm/SmartCities_Domotica#sensor-de-humedad

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
#include <UnoWiFiDevEd.h>       // Wifi
#include <BME280I2C.h>          // Sensor meteo
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD

/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 9600

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
  setup_wifi();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   lee_datos();
   serial_datos();
   lcd_datos();

   while(Wifi.available()){ // Procesamos las peticiones wifi
      process(Wifi);
   }

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
    Serial.println(F("BME280 sensor mal conectado!"));
    delay(1000);
  }
}

void setup_wifi(){
  Wifi.begin();
  Wifi.println(F("Web Server is up"));
}

void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}

void lee_humedadSuelo(){
  iHumedadSuelo=1023-analogRead(PIN_HUMEDAD_SUELO); // Para hacerlo mas visual
}

void lee_datos(){
   BME280::TempUnit tempUnit(BME280::TempUnit_Celcius);
   BME280::PresUnit presUnit(BME280::PresUnit_atm);
   bme.read(pres, temp, hum,tempUnit,  presUnit);                   // Parameters: (float& pressure, float& temp, float& humidity,uint8_t pressureUnit = 0x0)
   lee_humedadSuelo();
}

void serial_datos(){
  Serial.print(F("Temp: "));
  Serial.print(temp);
  Serial.print(F("°C"));


  Serial.print(F(" Humedad: "));
  Serial.print(hum);
  Serial.print(F("% "));


  Serial.print(F("\t\tPressure: "));
  Serial.print(pres);
  Serial.println(F(" atm"));

   Serial.print(F("Humedad suelo: "));
   Serial.println(iHumedadSuelo);

}
void lcd_datos(){

  lcd.setCursor(0,0);
  lcd.print(F("T:"));
  lcd.print(temp);

  lcd.setCursor(8,0);
  lcd.print(F("H:"));
  lcd.print(hum);
  lcd.print(F("%"));


  lcd.setCursor(0,1);
  lcd.print(F("P:"));
  lcd.print(pres);
  lcd.print(F("atm"));

   lcd.setCursor(8,1);
   lcd.print(F("HS:"));
   lcd.print(iHumedadSuelo);
   lcd.print(F(" "));

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
  client.println(F("<head> <title>Datos Meteo</title> </head>"));
  client.print(F("<body>"));

  client.print(F("Temperatura:"));
  client.print(temp);
  client.print(F(" C<br/>"));

  client.print(F("Humedad:"));
  client.print(hum);
  client.print(F(" % <br/>"));

  client.print(F("Presion:"));
  client.print(pres);
  client.print(F(" atm <br/>"));

  client.print(F("Humedad Suelo:"));
  client.print(iHumedadSuelo);
  client.print(F("<br/>"));

  client.print(F("</body>"));
  client.println(F("</html>"));
  client.print(DELIMITER); // very important to end the communication !!!
}


/* ==== END Functions ==== */
