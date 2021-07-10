
#include <SoftwareSerial.h>    //Libraries required for the Serial communication and DHT
#include "DHT.h"
String Data_SMS;            //String that we're going to send via SMS
float t,h;                  //Stores the Temperature and humidity

SoftwareSerial sim800l(10, 11); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted
DHT dht (2, DHT11);

void setup()
{

  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  Serial.begin(9600); 
  dht.begin();           //Start the DHT
  delay(1000);
}
 
void loop()
{
   h = dht.readHumidity();            //Read the humidity and temperature in % and C
   t = dht.readTemperature();
  sim800l.print("AT+CMGF=1\r");          // Set the shield to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+21626018607\"\r");  //Your phone number don't forget to include your country code example +212xxxxxxxxx"
  delay(500);
  Data_SMS = "Temperature = "+String(t,1)+" C" + " \nHumidity ="+String(h,1)+ " %";   //A string to regroup the whole message as it's composed of Strings and Float --> to a single string,
                                                                                      //Example: Temperature 23.1 C
                                                                                      //         Humidity 40 %
  sim800l.print(Data_SMS);  //This string is sent as SMS
  delay(500);
  sim800l.print((char)26);//(required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Data Sent.");
  delay(500);
  Serialcom();         //If no SMS is being sent we constantly call this function, it permits the communication between you and the module via the serial monitor
                       //You can try AT commands and check the feedback, also sending message is an AT command
  
}


void Serialcom()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}
 
