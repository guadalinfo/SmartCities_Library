/*
 Web Client to consume Weather Underground web service

 This sketch connects to a website (http://api.wunderground.com)
 using an Arduino Ethernet shield and get data from site.

 Circuit:
 * Arduino MEGA 2560 R3 Board
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 07 Jan 2014
 by Afonso C. Turcato

 */

/* ==== Includes - Librerias==== */
#include <Wire.h>
#include <UnoWiFiDevEd.h>

/* ====  END Includes ==== */


/* ==== Defines - Constantes ==== */
#define CONNECTOR     "rest"
#define SERVER_ADDR   "api.wunderground.com"
#define intervalo     3000

#define myKey "6764fe6f32eb379c"
#define myFeatures "conditions"
#define myCountry  "Spain"
#define myCity   "Granada"

#define SERIAL_BAUD     9600


/* ==== END Defines ==== */

/* ==== Variables Globales ==== */



//Response from Server
String responseString;

boolean startCapture;

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

void loop()
{

    String uri = String("/api/") + myKey + "/" + myFeatures + "/q/" + myCountry + "/" + myCity + ".json";
    String html_cmd2 = "Host: " + (String)SERVER_ADDR;
    String html_cmd3 = "Connection: close";


    // Make a HTTP request:
    Ciao.print(SERVER_ADDR);
    Ciao.println(uri);
    Serial.print(SERVER_ADDR);
    Serial.println(uri);

    CiaoData data=Ciao.write(CONNECTOR, SERVER_ADDR, uri);
    delay(2000);
     Serial.println(uri);
    if (!data.isEmpty()){
      Ciao.println( "State: " + String (data.get(1)) );
      Ciao.println( "Response: " + String (data.get(2)) );
      Serial.println(data.get(1));
      Serial.println(data.get(2));
    }   else   {
      Ciao.println("No response");
  /*
      if(data.isError()){

        Ciao.print("Error:");
        String id = data.get(0);

        String error = data.get(2);
        Ciao.println(id);
        Ciao.println(error);
       } else {
        Ciao.print("No Error");
      } */
    }
  }
  /*
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("Received " + (String)responseString.length() + " bytes");
    Serial.println("Disconnecting.");
    client.stop();
    client.flush();

    Serial.println();

    //Now, some examples of how to use it!
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

    // do nothing forevermore:
    while(true);
  }*/
}

String getValuesFromKey(const String response, const String sKey)
{
  String sKey_ = sKey;

  sKey_ = "\"" + sKey + "\":";

  char key[sKey_.length()];

  sKey_.toCharArray(key, sizeof(key));

  int keySize = sizeof(key)-1;

  String result ="";

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

      //Remove char '"'
      result.replace("\"","");
      break;
    }
  }

  return result;
}
