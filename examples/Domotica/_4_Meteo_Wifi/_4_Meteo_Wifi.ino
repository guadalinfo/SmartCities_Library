/*

Lectura de datos atmosfericos que se muestran en el LCD
Publicamos los datos en la web

http://192.168.1.<ID>/arduino/webserver

https://github.com/javacasm/SmartCities_Domotica#sensor-bme280

-----------------------------
Vin (Voltage In)    ->  Vcc
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4
SCK (Serial Clock)  ->  A5

 */



/* ==== Includes ==== */
#include <UnoWiFiDevEd.h>       // Wifi
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
  setup_wifi();


}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   lee_datos();
   serial_datos();
   lcd_datos();
   
   while(Wifi.available()){
      process(Wifi);
   }
   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */

void setup_wifi(){
  Wifi.begin();
  Wifi.println("Web Server is up");
}

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

void lee_datos(){
   bme.read(pres, temp, hum, pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity,  uint8_t pressureUnit = 0x0)
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
  client.println("<head> <title>Datos Meteo</title> </head>");
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
