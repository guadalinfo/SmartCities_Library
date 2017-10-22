/*
Usamos un RTC para mantener la fecha y hora
Mostramos hora y fecha en el LCD

Conexiones

RTC
======
VCC      5V
Gnd     GND
SCK_PIN  4
IO_PIN  5  //También conocido como Data
CE_PIN  6  //También conocido como RST

LCD
======

Vin (Voltage In)    ->  5V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro


DHT22
======

Conexiones DHT22
* Pin +    a Vcc
* Pin -    a GND
* Pin out  a S PIN 12


 */


/* ==== Includes - Librerias==== */

#include <Wire.h>          // Need for NewLiquidCrystal library
#include <LiquidCrystal_I2C.h>  // Pantalla LCD
#include <DS1302RTC.h>
#include <Time.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */

#define SCK_PIN 4
#define IO_PIN  5
#define CE_PIN  6

#define intervalo 500

#define SERIAL_BAUD 9600

#define DHT_PIN            12         

//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)



/* ==== END Defines ==== */

/* ==== Variables Globales ==== */
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Init the DS1302
// Set pins:  CE, IO,CLK
DS1302RTC RTC(CE_PIN, IO_PIN, SCK_PIN);


long ultima_medida=millis();

DHT_Unified dht(DHT1_PIN, DHTTYPE);

/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */

void setup_DHT(){


void setup_DHT(){
  dht.begin();
  
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;

  Serial.println("Sensor");
  dht.temperature().getSensor(&sensor);
  Serial.println("Temperatura");
  showSensorData(sensor);

  dht.humidity().getSensor(&sensor);
  Serial.println("Humedad");
  showSensorData(sensor);

}

void setup_Serial(){
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait

}


void setup_LCD(){
  lcd.init();         // Inicializamos el LCD
  lcd.backlight();    // Encendemos la luz
}

void setup_RTC(){
  lcd.print("RTC activado");

  delay(500);

  lcd.clear();
  if (RTC.haltRTC()) {
    lcd.print(F("RTC parado!"));
  }
  else
    lcd.print(F("RTC Funcionando"));

  lcd.setCursor(0,1);
  if (RTC.writeEN())
    lcd.print(F("Escritura Permitida"));
  else
    lcd.print(F("Escritura Bloqueada"));

  delay ( 2000 );

  // Setup time library
  lcd.clear();
  lcd.print(F("RTC Sincronizando"));
  setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if(timeStatus() == timeSet)
    lcd.print(F(" Ok!"));
  else
    lcd.print(F(" Error!"));

  delay ( 2000 );

  lcd.clear();

}

void setup() {
  setup_Serial();
  setup_LCD();

  setup_RTC();
  setup_DHT();
  
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {
  if((millis()-ultima_medida)>intervalo)    {


  showTime();
  ultima_medida=millis();
  }

  if (Serial.available() >= 12){
    setTime();
  }

}
/* ==== End Loop ==== */

/* ==== Funciones ==== */
void showTime(){
  static int sday = 0; // Saved day number for change check

  // Display time centered on the upper line
  lcd.setCursor(3, 0);
  print2digits(hour());
  lcd.print("  ");
  print2digits(minute());
  lcd.print("  ");
  print2digits(second());

  // Update in 00:00:00 hour only
  if(sday != day()) {
    // Display abbreviated Day-of-Week in the lower left corner
    lcd.setCursor(0, 1);
    lcd.print(dayShortStr(weekday()));

    // Display date in the lower right corner
    lcd.setCursor(5, 1);
    lcd.print(" ");
    print2digits(day());
    lcd.print(F("/"));
    print2digits(month());
    lcd.print(F("/"));
    lcd.print(year());
  }
  // Warning!
  if(timeStatus() != timeSet) {
    lcd.setCursor(0, 1);
    lcd.print(F("RTC ERROR: SYNC!"));
  }

  // Save day number
  sday = day();

  // Wait small time before repeating :)
  delay (100);

}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}


void setTime(){

    //check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
     {
        tmElements_t tm;
        time_t t;
        //note that the tmElements_t Year member is an offset from 1970,
        //but the RTC wants the last two digits of the calendar year.
        //use the convenience macros from Time.h to do the conversions.
        int y = Serial.parseInt();
        if (y >= 100 && y < 1000)
            lcd.println(F("Error: Year must be two digits or four digits!"));
        else {
            if (y >= 1000)
                tm.Year = CalendarYrToTm(y);
            else    //(y < 100)
                tm.Year = y2kYearToTm(y);
            tm.Month = Serial.parseInt();
            tm.Day = Serial.parseInt();
            tm.Hour = Serial.parseInt();
            tm.Minute = Serial.parseInt();
            tm.Second = Serial.parseInt();
            t = makeTime(tm);
      //use the time_t value to ensure correct weekday is set
            if(RTC.set(t) == 0) { // Success
              setTime(t);
              lcd.clear();
              lcd.print(F("RTC set!"));
      }       else  {
              lcd.clear();
              lcd.print(F("RTC not set!"));
      }
            //dump any extraneous input
            while (Serial.available() > 0) Serial.read();
        }
    }

}

/* ==== END Functions ==== */
