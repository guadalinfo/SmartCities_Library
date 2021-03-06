/*

This example show the interaction between the Ciao Library and the Thingspeak Cloud.
To run the example you need to register an account on thingspeak.com and create a
new channel by clicking "Channels" section in the website (Channels -> My Channels -> New Channel).
In the new channel you need to add two fields. The first one refers to the humidity data and the second one to the temperature value.
After that, replace the "XXXXXXXXX" value of APIKEY_THINGSPEAK with "Write API key" value reported in the API Keys section of the channel.

*/
#include <Wire.h>
#include <UnoWiFiDevEd.h>
#include <BME280I2C.h>

#define CONNECTOR     "rest"
#define SERVER_ADDR   "api.thingspeak.com"
#define intervalo 30000

#define APIKEY_THINGSPEAK  "2D9Z8DWIMI16HF7R" //Insert your API Key

BME280I2C bme;

#define metric true


long ultima_medida=millis();


void setup() {

   while(!bme.begin()){

    delay(1000);
  }
  Ciao.begin(); // CIAO INIT
}

void loop() {

  if((millis()-ultima_medida)>intervalo)    {
   float temp(NAN), hum(NAN), pres(NAN);

   
   BME280::TempUnit tempUnit(BME280::TempUnit_Celcius);
   BME280::PresUnit presUnit(BME280::PresUnit_atm);
   bme.read(pres, temp, hum,tempUnit, presUnit );

    String uri = "/update?api_key=";
    uri += APIKEY_THINGSPEAK;
    uri += "&field1=";
    uri += String(hum);
    uri += "&field2=";
    uri += String(temp);
    uri += "&field3=";
    uri += String(pres);


    Ciao.println("Send data on ThingSpeak Channel");
    Ciao.println(uri);
    CiaoData data = Ciao.write(CONNECTOR, SERVER_ADDR, uri);

    if (!data.isEmpty()){
      Ciao.println( "State: " + String (data.get(1)) );
      Ciao.println( "Response: " + String (data.get(2)) );
    }
    else{
      Ciao.println("Write Error");
    }


    ultima_medida=millis();
   }

}
