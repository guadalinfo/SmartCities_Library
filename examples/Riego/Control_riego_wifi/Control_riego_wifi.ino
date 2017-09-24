/*
Control de riego en funcion de la humedad del suelo
Se muestran los datos en una LCD

Se puede controlar remotamente via wifi
http://192.168.1.202/arduino/webserver

Conexion del LCD
-----------------------------
Vin (Voltage In)    ->  5V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

Sensor Humedad Suelo    A1
Sensor LLuvia           A2
Potenciometro           A3

Rele de bomba de riego  12

 */



/* ==== Includes ==== */
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD


// Librerias Wifi
#include <UnoWiFiDevEd.h>
/* ====  END Includes ==== */

/* ==== Defines ==== */

#define SERIAL_BAUD 9600

#define PIN_SENSOR_SUELO  A1
#define PIN_SENSOR_LLUVIA A2
#define PIN_POTENCIOMETRO A3

#define PIN_RELE          12

#define MSG_RIEGO_ON F("RGO. ON")
#define MSG_RIEGO_OFF F("RGO. OFF")
#define MSG_RIEGO_MANUAL F("-MANUAL-")

#define pre F("<input type=button onClick=\"var w=window.open('/arduino/dig/")
#define post F("','_parent');w.close();\" value='")

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

long lIntervaloMedida=1000;  // Para un sistema real sería no menos de un minuto
long lUltimaMedida=0;
int iUmbral=20;

int iHumedad;        // Nivel de humedad del suelo
int iPotenciometro;  // Umbral de activacion del riego
int iLLuvia;   // Nivel de lluvia (o de rocio en el sensor de lluvia)
int iRiego=0;  // Indica si el riego esta on o no
boolean bForzado=false;

// Control via Wifi

int iPines[]={PIN_RELE};
String iNombres[]={"Riego"};

/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* ==== END Prototypes ==== */

/* ==== Setup ==== */


void setup_Serial(){
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait

}


void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}


void setup_Rele(){
  pinMode(PIN_RELE,OUTPUT);
  apagaRiego();

}

void setup_Wifi(){
  Wifi.begin();
  Wifi.println("Web Server is up");
}

void setup() {
  setup_Serial();
  setup_Rele();
  setup_LCD();
  setup_Wifi();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */


void loop() {

  // Procesamos el wifi
  while(Wifi.available()){
      process(Wifi);
  }

  if(millis()-lUltimaMedida>lIntervaloMedida){ // Toca Medir

    iHumedad=1023-analogRead(PIN_SENSOR_SUELO);
    iLLuvia=1023-analogRead(PIN_SENSOR_LLUVIA);
    iPotenciometro=analogRead(PIN_POTENCIOMETRO);
    showData();
    if(bForzado==false){
        if(iPotenciometro-iUmbral>iHumedad){
          if(iLLuvia<512){
           enciendeRiego();
          }else{
            apagaRiego();
          }
         } else {
           apagaRiego();
         }
     } else {
      riegoManual();
     }
     
     lUltimaMedida=millis();
     dormir(lIntervaloMedida);
  }

}
/* ==== End Loop ==== */

/* ==== Functions ==== */

void showData(){
  lcd.setCursor(0,0);
  lcd.print("Hum:");
  lcd.print(iHumedad);
  lcd.print("  ");

  Serial.print("Hum:");
  Serial.print(iHumedad);


  lcd.setCursor(8,0);
  lcd.print("Umb:");
  lcd.print(iPotenciometro);
  lcd.print("  ");

  Serial.print(" Umbral:");
  Serial.println(iPotenciometro);

  lcd.setCursor(8,1);
  lcd.print("Llv:");
  lcd.print(iLLuvia);
  lcd.print("  ");
}

void enciendeRiego(){
  digitalWrite(PIN_RELE,LOW);
  lcd.setCursor(0,1);
  lcd.print(MSG_RIEGO_ON);
  Serial.println(MSG_RIEGO_ON);
}

void apagaRiego(){
  digitalWrite(PIN_RELE,HIGH);
  lcd.setCursor(0,1);
  lcd.print(MSG_RIEGO_OFF);
  Serial.println(MSG_RIEGO_OFF);

}

void riegoManual(){
  lcd.setCursor(0,1);
  lcd.print(MSG_RIEGO_MANUAL);
  Serial.println(MSG_RIEGO_MANUAL); 
}

void dormir(long espera){
  delay(espera);
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
  client.println(F("Connection: close"));
  client.println(F("Refresh: 1"));
  client.println();
  client.println(F("<html>" \
                  "<head> </head>" \
                  "<body>"));

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
  client.print("<br/>");
  client.print("Humedad:");
  client.print(iHumedad);
  client.print("  ");

  Serial.print("Hum:");
  Serial.print(iHumedad);


  client.print("<br/>");
  client.print("Umbral:");
  client.print(iPotenciometro);
  client.print("  ");

  Serial.print(" Umbral:");
  Serial.println(iPotenciometro);

  client.print("<br/>");
  client.print("Lluvia:");
  client.print(iLLuvia);
  client.print("  ");
  
  client.println("</p>");

  client.print(F("</body>" \
                  "</html>"));

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
    //digitalWrite(pin, !value);
   // bForzado=digitalRead(pin);
   // Serial.print("\nForzado: ");
   // Serial.println(bForzado);
    digitalWrite(pin, !digitalRead(pin));
    bForzado=!digitalRead(pin);
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

/* ==== END Functions ==== */
