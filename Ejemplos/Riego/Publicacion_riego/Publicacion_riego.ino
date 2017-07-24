/*
Ejemplo de publicacion de datos en la nube.
Usaremos la plataforma ThingSpeak

Conexiones


 */


/* ==== Includes - Librerias==== */
#include <Wire.h>
#include <UnoWiFiDevEd.h>

/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */
#define CONNECTOR     "rest"
#define SERVER_ADDR   "api.thingspeak.com"
#define intervalo     30000

#define APIKEY_THINGSPEAK  "2D9Z8DWIMI16HF7R" //Insert your API Key

#define SERIAL_BAUD     9600

#define MSG_WRITE_ERROR    T("Write Error")


/* ==== END Defines ==== */

/* ==== Variables Globales ==== */
long lIntervaloMedida=2000;  // Para un sistema real sería no menos de un minuto
long lUltimaMedida=0;
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

void setup() {
  setup_Serial();
  setup_nube();
}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */

void publica_datos(){
  String uri = "/update?api_key=";
  uri += APIKEY_THINGSPEAK;
  uri += "&field1=";
  uri += String(hum);
  uri += "&field2=";
  uri += String(temp);
  uri += "&field3=";
  uri += String(pres);
  Ciao.println(T("Send data on ThingSpeak Channel"));
  Ciao.println(uri);
  CiaoData data = Ciao.write(CONNECTOR, SERVER_ADDR, uri);

  Serial.print(uri);
  if (!data.isEmpty()){
    Ciao.println( "State: " + String (data.get(1)) );
    Ciao.println( "Response: " + String (data.get(2)) );
    Serial.println(data.get(1));
    Serial.println(data.get(2));
  }
  else{
    Ciao.println(MSG_WRITE_ERROR);
    Serial.println(MSG_WRITE_ERROR)
  }

}

void loop() {
  if(millis()-lUltimaMedida>lIntervaloMedida){ // Toca Medir
    lUltimaMedida=millis();

    dormir(lIntervaloMedida);
  }
}


void dormir(long espera){
  delay(espera);
}

/* ==== End Loop ==== */

/* ==== Funciones ==== */

/* ==== END Functions ==== */
