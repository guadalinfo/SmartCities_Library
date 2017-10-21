/*


Medida de temperatura y humedad con 2 sensores DHT22

Conexiones
* Pin +    a Vcc
* Pin -    a GND
* Pin out  a S

Un sensor DHT22 al pin 4 y otro al 6

Interruptor crepuscular

Medimos la luz y si el valor esta por debajo de lo marcado por un potenciometro
activamos la iluminacion

Conexiones
Potenciometro  A0
LDR           A2
Luz           13

Control remoto de 4 reles
http://<IP>/arduino/webserver/

Pines 9-12

Alarma de presencia PIR
Alarma  Pin 2
Luz alarma 7

 */


/* ==== Includes - Librerias==== */
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Servo.h>

#include <Wire.h>
#include <UnoWiFiDevEd.h>

/* ====  END Includes ==== */
#define DHT1_PIN            6
#define DHT2_PIN            4
#define SERVO_PIN           8

#define ALARMA_PIN          2
#define LED_ALARMA_PIN      7

// Pines de control de los Reles

#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9

/* ==== Defines - Constantes ==== */

//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)


#define PIN_LDR A2
#define PIN_POTENCIOMETRO A0

#define PIN_ILUMNINACION 13
#define UMBRAL 20

#define ESPERA 300

/* ==== END Defines ==== */


/* ==== Variables Globales ==== */

DHT_Unified dht1(DHT1_PIN, DHTTYPE);
DHT_Unified dht2(DHT2_PIN, DHTTYPE);

uint32_t delayMS;

float temperatura_interior;
float temperatura_exterior;

Servo servo;

int reles[]={IN1,IN2,IN3,IN4};
boolean bAlarma=false;
boolean bLuz=false;

/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup_Servo(){
  servo.attach(SERVO_PIN);
}

void setup_iluminacion(){
  pinMode(PIN_ILUMNINACION,OUTPUT);
}

void setup_alarma(){
  pinMode(ALARMA_PIN,INPUT);
  pinMode(LED_ALARMA_PIN,OUTPUT);
}
void setup_reles(){
  for(int i=0;i<sizeof(reles);i++){
    pinMode(reles[i],OUTPUT);
    digitalWrite(reles[i],HIGH);
  }
}

void setup_wifi(){
    Wifi.begin();
    Wifi.println(F("Web Server On"));
}

void setup_DHT(){
  dht1.begin();
  dht2.begin();

  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;

  Serial.println(F("Sensor 1"));
  dht1.temperature().getSensor(&sensor);
  Serial.println(F("Temperatura"));
  showSensorData(sensor);

  dht1.humidity().getSensor(&sensor);
  Serial.println(F("Humedad"));
  showSensorData(sensor);

  Serial.println(F("------------------------------------"));

  Serial.println(F("Sensor 2"));
  dht2.temperature().getSensor(&sensor);
  Serial.println(F("Temperatura"));
  showSensorData(sensor);

  dht2.humidity().getSensor(&sensor);
  Serial.println(F("Humedad"));
  showSensorData(sensor);

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void setup_Serial(){
  Serial.begin(9600);
}

void setup() {
  setup_Serial();
  setup_Servo();
  setup_DHT();
  setup_iluminacion();
  setup_alarma();
  setup_reles();
  setup_wifi();
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {

  // Control de temperatura
  getData();
  Serial.print(temperatura_interior);
  Serial.print(" ");
  Serial.println(temperatura_exterior);
  delay(delayMS);

  if (temperatura_interior<temperatura_exterior){
    cierraVentanas();
  }
  else{
    abreVentanas();
  }


  // Control de iluminacion
  int iPotenciometro=analogRead(PIN_POTENCIOMETRO);
  int iLuz=analogRead(PIN_LDR);
  Serial.print(iLuz);
  Serial.print(" - ");
  Serial.println(iPotenciometro);

  if(iLuz+UMBRAL>iPotenciometro){
      enciendeLuz();
  } else {
    apagaLuz();
  }

  if(digitalRead(ALARMA_PIN)==HIGH){
    enciendeAlarma();
  } else {
    apagaAlarma();
  }

  while(Wifi.available()){
     process(Wifi);
   }
}
/* ==== End Loop ==== */

/* ==== Funciones ==== */
void showSensorData(sensor_t sensor){
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" *C"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" *C"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" *C"));
  Serial.println("------------------------------------");
}

void getData() {

  // Get temperature event and print its value.
  sensors_event_t event;

  dht1.temperature().getEvent(&event);
  temperatura_exterior=event.temperature;

  dht2.temperature().getEvent(&event);
  temperatura_interior=event.temperature;

}

void cierraVentanas(){
  servo.write(20);
  Serial.println(F("Cerrando Ventanas"));
}

void abreVentanas(){
  servo.write(160);
  Serial.println(F("Abriendo Ventanas"));
}

void enciendeAlarma(){
  digitalWrite(LED_ALARMA_PIN,HIGH);
  bAlarma=true;
  Serial.println(F("Alarma encendida"));
}

void apagaAlarma(){
  digitalWrite(LED_ALARMA_PIN,LOW);
  bAlarma=false;
  Serial.println(F("Alarma apagada"));
}


void enciendeLuz(){
  digitalWrite(PIN_ILUMNINACION,HIGH);
  bLuz=true;
  Serial.println(F("Iluminacion encendida"));
}

void apagaLuz(){
  digitalWrite(PIN_ILUMNINACION,LOW);
  bLuz=false;
  Serial.println(F("Iluminacion apagada"));
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == F("webserver")) {
    WebServer(client);
  }

  if (command == F("dig")) {
    digitalCommand(client);
  }
}

void WebServer(WifiData client) {

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();
  client.println(F("<html>"));

  client.println(F("<head> </head>"));
  client.print(F("<body>"));
  String pre=F("<input type=button onClick=\"var w=window.open('/arduino/dig/");

  for(int i=0;i<sizeof(reles);i++)
  {
    String s=String(reles[i]);

    client.print(s+pre+s+F("/1','_parent');w.close();\"value='ON'>"));
    client.print(pre+s+F("/0','_parent');w.close();\"value='Off'><br>"));
  }
  client.print(F("<br/> Temp ext.: "));
  client.print(temperatura_exterior);
  client.print(F("<br/> Temp int.: "));
  client.print(temperatura_interior);
  if(bAlarma==true){
    client.print(F("<br/> Alarma ON"));
  } else {
    client.print(F("<br/> Alarma OFF"));
  }

  if(bLuz==true){
    client.print(F("<br/> Luz ON"));
  } else {
    client.print(F("<br/> Luz OFF"));
  }

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
    digitalWrite(pin, !value);
  }

  // Send feedback to client
  client.println(F("Status: 200 OK\n"));
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.print(value);
  client.print(EOL);    //char terminator

}

/* ==== END Functions ==== */
