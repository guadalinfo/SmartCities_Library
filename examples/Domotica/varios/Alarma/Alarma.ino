/*
Descripcion: Alarma domestica. 
Un sensor de presencia se activa y enciende una luz de alarma

Conexiones

  Sensor PIR Pin 2 

  Led Rojo de alarma Pin 7


 */


/* ==== Includes - Librerias==== */

/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */

#define ALARMA_PIN          2
#define LED_ALARMA_PIN      7

#define ESPERA 100

/* ==== END Defines ==== */

/* ==== Variables Globales ==== */

/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup_Serial(){
  Serial.begin(9600);
}

void setup_alarma(){
  pinMode(ALARMA_PIN,INPUT);
  pinMode(LED_ALARMA_PIN,OUTPUT);
}


void setup() {
    setup_Serial();
   setup_alarma();

}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
  void loop() {
 if(digitalRead(ALARMA_PIN)==HIGH){
    enciendeAlarma();
  } else {
    apagaAlarma();
  }
 delay(ESPERA);
}
/* ==== End Loop ==== */

/* ==== Funciones ==== */

void enciendeAlarma(){
  digitalWrite(LED_ALARMA_PIN,HIGH);
  Serial.println("Alarma encendida");
}

void apagaAlarma(){
  digitalWrite(LED_ALARMA_PIN,LOW);
  Serial.println("alarma apagada");
}

/* ==== END Functions ==== */
