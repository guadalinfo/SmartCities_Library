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
#define intervalo 5000

#define APIKEY_THINGSPEAK  "2D9Z8DWIMI16HF7R" //Insert your API Key 

BME280I2C bme;

bool metric = true;

float temp(NAN), hum(NAN), pres(NAN);

long ultima_medida=millis();

 
void setup() {
   Serial.begin(9600);
   while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  Ciao.begin(); // CIAO INIT
}
 
void loop() {

  if((millis()-ultima_medida)>intervalo)    {
    
    printBME280Data(&Serial);
    printBME280CalculatedData(&Serial);
    
      
    String uri = "/update?api_key=";
    uri += APIKEY_THINGSPEAK;
    uri += "&field1=";
    uri += String(hum);
    uri += "&field2=";
    uri += String(temp);
    uri += "&field3=";
    uri += String(pres);
    
 
    Ciao.println("Send data on ThingSpeak Channel"); 
      
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

/* ==== Functions ==== */
void printBME280Data(Stream* client){

   uint8_t pressureUnit(3);                                           // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
   bme.read(pres, temp, hum, metric, pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
  /* Alternatives to ReadData():
    float temp(bool celsius = false);
    float pres(uint8_t unit = 0);
    float hum();

    Keep in mind the temperature is used for humidity and
    pressure calculations. So it is more effcient to read
    temperature, humidity and pressure all together.
   */
  client->print("Temp: ");
  client->print(temp);
  client->print("°"+ String(metric ? 'C' :'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->print(" atm");

}
void printBME280CalculatedData(Stream* client){
  float altitude = bme.alt(metric);
  float dewPoint = bme.dew(metric);
  client->print("A: ");
  client->print(altitude);
  client->print((metric ? "m" : "ft"));
  client->print("\t\tDew point: ");
  client->print(dewPoint);
  client->println("°"+ String(metric ? 'C' :'F'));

}

/* ==== END Functions ==== */
