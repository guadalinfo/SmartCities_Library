/*
Hello Led

Encendemos un led conectado al pin 13

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
  pinMode(PIN_LED,OUTPUT);     // Activamos le pin como salida
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {
  digitalWrite(PIN_LED,HIGH);  // Encendemos el LED
  delay(espera);               // Esperamos un tiempo
  digitalWrite(PIN_LED,HIGH);  // Apagamos el LED
  delay(espera);               // Esperamos un tiempo

}
/* ==== End Loop ==== */

/* ==== Funciones ==== */

/* ==== END Functions ==== */
