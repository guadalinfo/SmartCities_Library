/*
Descripcion

Recuperar pronostico meteo

http://api.wunderground.com/api/6764fe6f32eb379c/conditions/q/Spain/Granada.json


Ejemplo: https://gist.github.com/acturcato/8352621
Conexiones


 */


/* ==== Includes - Librerias==== */

/* ====  END Includes ==== */

/* ==== Defines - Constantes ==== */
/* ==== END Defines ==== */

/* ==== Variables Globales ==== */

/* ==== END Global Variables ==== */


/* ==== Funciones a usar - Prototipos ==== */

/* ==== END Prototypes ==== */

/* ==== Setup - Configuracion==== */
void setup() {

}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {
}
/* ==== End Loop ==== */

/* ==== Funciones ==== */
void analiza(){
  Serial.print("Weather: ");
 Serial.println(getValuesFromKey(responseString, "weather"));
 Serial.println();

 Serial.print("Current Temperature: ");
 Serial.print(getValuesFromKey(responseString, "temp_c"));
 Serial.println(" oC\n");

 Serial.print("Relativy Humidity: ");
 Serial.println(getValuesFromKey(responseString, "relative_humidity"));
 Serial.println();

 Serial.print("Wind: ");
 Serial.println(getValuesFromKey(responseString, "wind_string"));
 Serial.println();

 Serial.print("Feels like: ");
 Serial.print(getValuesFromKey(responseString, "feelslike_c"));
 Serial.println(" oC\n");
}

String getValuesFromKey(const String response, const String sKey)
{
  String sKey_ = sKey;

  sKey_ = "\"" + sKey + "\":";

  char key[sKey_.length()];

  sKey_.toCharArray(key, sizeof(key));

  int keySize = sizeof(key)-1;

  String result = NULL;

  int n = response.length();

  for(int i=0; i < (n-keySize-1); i++)
  {
    char c[keySize];

    for(int k=0; k<keySize; k++)
    {
      c[k] = response.charAt(i+k);
    }

    boolean isEqual = true;

    for(int k=0; k<keySize; k++)
    {
      if(!(c[k] == key[k]))
      {
        isEqual = false;
        break;
      }
    }

    if(isEqual)
    {
      int j= i + keySize + 1;
      while(!(response.charAt(j) == ','))
      {
        result += response.charAt(j);
        j++;
      }
      
/* ==== END Functions ==== */
