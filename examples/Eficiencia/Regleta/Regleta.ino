/*
Control remoto de una

Conexion del LCD
-----------------------------
Vin (Voltage In)    ->  5V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

Relé                ->  pin 11

Sensor de corriente ->  pin A0

 */



/* ==== Includes ==== */
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD

/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 9600

#define PIN_SENSOR_CORRIENTE A0

#define PIN_RELE 11

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//LiquidCrystal_I2C lcd(0x3F, 16, 2);

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
}

void setup() {
  setup_Serial();
  setup_LCD();
  setup_Rele();

  enciende_Rele();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {
   int iValorSensorCorriente=analogRead(PIN_SENSOR_CORRIENTE);

   lcd.setCursor(0,1);
   lcd.print(iValorSensorCorriente);
   Serial.println(iValorSensorCorriente);
   delay(20);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void enciende_Rele(){
  digitalWrite(PIN_RELE,HIGH);
}

void apaga_Rele(){
  digitalWrite(PIN_RELE,LOW);
}


/* ==== END Functions ==== */
