/*
Control remoto de una regleta que mide el consumo y lo muestra por el LCD

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
// Librerias para la pantalla LCD
#include <Wire.h>               // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h>  // Pantalla LCD

// Libreria para filtrado
#include <Filters.h>


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

// Medida de corriente

float testFrequency = 60;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float intercept = -0.02;//-0.1129; // to be adjusted based on calibration testing
float slope = 0.066; // 0.0405; // to be adjusted based on calibration testing
float current_amps; // estimated actual current in amps

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading
unsigned long previousMillis = 0;


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

  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );

  while( true ) {
    sensorValue = analogRead(A0);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function

    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time



      // convert signal sigma value to current in amps
      current_amps = intercept + slope * inputStats.sigma();
      Serial.print( current_amps );
      Serial.print( "," );
      Serial.println( inputStats.sigma() );
      lcd.setCursor(0,0);
      lcd.print(current_amps);
    }
  }


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
