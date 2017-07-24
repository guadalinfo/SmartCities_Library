/*
Publicacion de datos en la nube.
Usaremos la plataforma ThingSpeak

Conexiones

Control de riego en funcion de la humedad del suelo
Se muestran los datos en una LCD

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


/* ==== Includes - Librerias==== */
#include <Wire.h>
#include <UnoWiFiDevEd.h>
#include <LiquidCrystal_I2C.h>  // Pantalla LCD
/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */
#define CONNECTOR     "rest"
#define SERVER_ADDR   "api.thingspeak.com"
#define intervalo     30000

#define APIKEY_THINGSPEAK  "UC3QL166QDZW9Q30" //Insert your API Key

#define SERIAL_BAUD     9600

#define PIN_SENSOR_SUELO  A1
#define PIN_SENSOR_LLUVIA A2
#define PIN_POTENCIOMETRO A3

#define PIN_RELE          12

#define MSG_RIEGO_ON F("RGO. ON")
#define MSG_RIEGO_OFF F("RGO. OFF")
#define MSG_WRITE_DATA  F("Send data on ThingSpeak Channel")
#define MSG_WRITE_ERROR    F("Write Error")


/* ==== END Defines ==== */

/* ==== Variables Globales ==== */
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
long lIntervaloMedida=2000;  // Para un sistema real sería no menos de un minuto
long lUltimaMedida=0;

int iUmbral=20;

int iHumedad;
int iPotenciometro;
int iLLuvia;
int iRiego=0;
/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */

void setup_Serial(){
  Serial.begin(SERIAL_BAUD);
}

void setup_nube(){
  Ciao.begin(); // CIAO INIT
}

void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}


void setup_Rele(){
  pinMode(PIN_RELE,OUTPUT);
  apagaRiego();

}

void setup() {
  setup_Serial();
  setup_nube();

  setup_Rele();
  setup_LCD();
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */



void loop() {
  if(millis()-lUltimaMedida>lIntervaloMedida){ // Toca Medir

    iHumedad=analogRead(PIN_SENSOR_SUELO);
    iLLuvia=analogRead(PIN_SENSOR_LLUVIA);
    iPotenciometro=analogRead(PIN_POTENCIOMETRO);
    showData();
    if(iPotenciometro-iHumedad>iUmbral){
      if(iLLuvia>512){
       enciendeRiego();
      }else{
        apagaRiego();
      }
     } else {
       apagaRiego();
     }

     publica_datos();

     lUltimaMedida=millis();

     dormir(lIntervaloMedida);
  }
}

/* ==== End Loop ==== */

/* ==== Funciones ==== */
void publica_datos(){
  String uri = String("/update?api_key=") + \
     APIKEY_THINGSPEAK  + \
         "&field1="  + \
         String(iHumedad/100)  + \
         "&field2="  + \
         String(iUmbral/100)    +
         "&field3="  + \
         String(iLLuvia/1000);  // + \
//         "&field4="  + \
//         String(iRiego/1000);

  Ciao.println(MSG_WRITE_DATA);
  Ciao.println(uri.length());
  Ciao.println(uri);
  CiaoData data = Ciao.write(CONNECTOR, SERVER_ADDR, uri);

  Serial.println(uri);
  if (!data.isEmpty()){
    Ciao.println( "State: " + String (data.get(1)) );
    Ciao.println( "Response: " + String (data.get(2)) );
    Serial.println(data.get(1));
    Serial.println(data.get(2));
  }
  else{
    Ciao.println(MSG_WRITE_ERROR);
    Serial.println(MSG_WRITE_ERROR);
  }

}

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
  iRiego=1;
  digitalWrite(PIN_RELE,LOW);
  lcd.setCursor(0,1);
  lcd.print(MSG_RIEGO_ON);
  Serial.println(MSG_RIEGO_ON);
}

void apagaRiego(){
  iRiego=0;
  digitalWrite(PIN_RELE,HIGH);
  lcd.setCursor(0,1);
  lcd.print(MSG_RIEGO_OFF);
  Serial.println(MSG_RIEGO_OFF);

}


void dormir(long espera){
  delay(espera);
}

/* ==== END Functions ==== */
