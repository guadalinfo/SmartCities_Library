/*
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



/* ==== Includes ==== */
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD

/* ====  END Includes ==== */

/* ==== Defines ==== */

#define SERIAL_BAUD 9600

#define PIN_SENSOR_SUELO  A1
#define PIN_SENSOR_LLUVIA A2
#define PIN_POTENCIOMETRO A3

#define PIN_RELE          12

#define MSG_RIEGO_ON F("RGO. ON")
#define MSG_RIEGO_OFF F("RGO. OFF")

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

long lIntervaloMedida=100;
long lUltimaMedida=0;
int iUmbral=20;

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

void setup() {
  setup_Serial();
  setup_Rele();
  setup_LCD();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
int iHumedad;
int iPotenciometro;
int iLLuvia;

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
     lUltimaMedida=millis();
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


/* ==== END Functions ==== */
