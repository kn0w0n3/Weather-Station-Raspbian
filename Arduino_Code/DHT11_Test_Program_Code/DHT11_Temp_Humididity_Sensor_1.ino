#include <dht11.h>

#define DHT11PIN 4
dht11 DHT11;

void setup()
{
  Serial.begin(9600);
 
}

void loop()
{
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);

  Serial.print("Temperature (F): ");
  Serial.println((float)(DHT11.temperature*1.8)+32, 2);

  //Photo Sensor Seems to not be working properly at the moment
  //int value = analogRead(A0);
  //Serial.println("Analog value : ");
  //Serial.println(value);

  delay(2500);

}
