/*
Interruptor crepuscular

Medimos la luz y si el valor esta por debajo de lo marcado por un potenciometro
activamos la iluminacion

Conexiones
Poenciometro  A0
LDR           A2

Luz           13

 */


/* ==== Includes - Librerias==== */

/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */

#define PIN_LDR A2
#define PIN_POTENCIOMETRO A0

#define PIN_ILUMNINACION 13
#define UMBRAL 20

#define ESPERA 300
/* ==== END Defines ==== */

/* ==== Variables Globales ==== */

/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup_iluminacion(){
  pinMode(PIN_ILUMNINACION,OUTPUT);
}


void setup_Serial(){
  Serial.begin(9600);
}

void setup() {
   setup_Serial();
   setup_iluminacion();
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {
  int iPotenciometro=analogRead(PIN_POTENCIOMETRO);
  int iLuz=analogRead(PIN_LDR);
  Serial.print(iLuz);
  Serial.print(" - ");
  Serial.println(iPotenciometro);

  if(abs(iPotenciometro-iLuz)<UMBRAL){
      enciendeLuz();
  } else {
    apagaLuz();
  }
  delay(espera)
}
/* ==== End Loop ==== */

/* ==== Funciones ==== */
void enciendeLuz(){
  digitalWrite(PIN_ILUMNINACION,HIGH);
  Serial.println("iluminacion encendida");
}

void apagaLuz(){
  digitalWrite(PIN_ILUMNINACION,LOW);
  Serial.println("iluminacion apagada");
}
/* ==== END Functions ==== */
