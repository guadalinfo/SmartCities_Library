/*
Hello Led & PC

https://github.com/javacasm/SmartCities_Domotica#hello-led-y-pc

Encendemos un led conectado al pin 13
Enviamos al PC un aviso de que se ha encendido

Conexiones
Led:
* Negativo - patilla corta - GND
* Positivo - patilla larga - 13

 */


/* ==== Includes - Librerias==== */
/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */
#define PIN_LED 13
/* ==== END Defines ==== */

/* ==== Variables Globales ==== */
int espera=1000; // 1000 milisegundos = 1 Segundo
/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup() {
  pinMode(PIN_LED,OUTPUT); // Activamos le pin como salida

  Serial.begin(9600);      // Inicializamos la conexion con el PC a la velocidad de 9600
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {
  digitalWrite(PIN_LED,HIGH);  // Encendemos el LED
  Serial.println("Led encendido");
  delay(espera);               // Esperamos un tiempo
  digitalWrite(PIN_LED,HIGH);  // Apagamos el LED
  Serial.println("Led apagado");
  delay(espera);               // Esperamos un tiempo

}
/* ==== End Loop ==== */

/* ==== Funciones ==== */

/* ==== END Functions ==== */
