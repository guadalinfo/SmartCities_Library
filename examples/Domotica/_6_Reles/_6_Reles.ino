/*

Lectura de datos atmosferico que se muestran en el LCD
Lectura de un sensor de humedad de suelo
Publicamos los datos en la web
Control de riego en funcion de la humedad del suelo

Acceso wifi a
http://192.168.1.<ID>/arduino/webserver

https://github.com/javacasm/SmartCities_Domotica#reles-de-riego

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

#define PIN_HUMEDAD_SUELO A0

#define PIN_RELE          12

#define MSG_RIEGO_ON F("ON")
#define MSG_RIEGO_OFF F("OFF")
#define MSG_RIEGO_MANUAL F("MAN")

#define pre F("<input type=button onClick=\"var w=window.open('/arduino/dig/")
#define post F("','_parent');w.close();\" value='")


/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);


BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

float temp(NAN), hum(NAN), pres(NAN);

int iHumedadSuelo; // Nivel de humedad del suelo

int iUmbral=20; // Nivel en el que se arranca el riego

int iRiego=0;  // Indica si el riego esta on o no
boolean bForzado=false; // Guarda si el usuario ha activado el riego en modo manual
// Control via Wifi

int iPines[]={PIN_RELE};
String iNombres[]={"Riego"};

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
  setup_Rele();
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

  if(bForzado==false){
    if(iUmbral>iHumedadSuelo){
      enciendeRiego();
    }else{
      apagaRiego();
    }
   } else {
      riegoManual();
   }

   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void setup_Rele(){
  pinMode(PIN_RELE,OUTPUT);
  apagaRiego();

}
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

  if (command == "dig") {
    digitalCommand(client);
  }

}
void WebServer(WifiData client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println(F("Refresh: 5"));  // refresh the page automatically every  sec
  client.println();
  client.println(F("<html>"));
  client.println(F("<head> <title>Riego</title> </head>"));
  client.print(F("<body>"));

 String pre=F("<input type=button onClick=\"var w=window.open('/arduino/dig/");
  String post=F("','_parent');w.close();\" value='");
  for(int i=0;i<1;i++)
  {
    String s=String(iNombres[i]);
    String sId=String(iPines[i]);
    String sTatus=digitalRead(iPines[i])?"/1":"/0";
    String sNombre=digitalRead(iPines[i])?"ON":"OFF";
    client.print(s+pre+sId+sTatus+post+sNombre+"'><br>");
  }

  client.print(F("T:"));
  client.print(temp);
  client.print(F(" C<br/>"));

  client.print(F("H:"));
  client.print(hum);
  client.print(F(" % <br/>"));

  client.print(F("P:"));
  client.print(pres);
  client.print(F(" atm <br/>"));

  client.print(F("HS:"));
  client.print(iHumedadSuelo);
  client.print(F("<br/>"));

  client.print(F("</body>"));
  client.println(F("</html>"));
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

    digitalWrite(pin, !digitalRead(pin));
    //bForzado=!digitalRead(pin);
    bForzado=!bForzado;
    Serial.print("\nForzado: ");
    Serial.println(bForzado);
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

void enciendeRiego(){
  digitalWrite(PIN_RELE,LOW);
  lcd.setCursor(14,1);
  lcd.print(MSG_RIEGO_ON);
  Serial.println(MSG_RIEGO_ON);
}

void apagaRiego(){
  digitalWrite(PIN_RELE,HIGH);
  lcd.setCursor(14,1);
  lcd.print(MSG_RIEGO_OFF);
  Serial.println(MSG_RIEGO_OFF);

}

void riegoManual(){
  lcd.setCursor(14,1);
  lcd.print(MSG_RIEGO_MANUAL);
  Serial.println(MSG_RIEGO_MANUAL);
  //bForzado=!bForzado;
}



/* ==== END Functions ==== */
