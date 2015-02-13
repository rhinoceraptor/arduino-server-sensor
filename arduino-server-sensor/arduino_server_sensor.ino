#include "DHT.h"

#define DHT_ONE 2
#define DHT_TWO 3
#define DHT_THREE 4
#define DHT_FOUR 5

#define DHT_TYPE DHT11

DHT dht_one(DHT_ONE, DHT_TYPE);
DHT dht_two(DHT_TWO, DHT_TYPE);
DHT dht_three(DHT_THREE, DHT_TYPE);
DHT dht_four(DHT_FOUR, DHT_TYPE);

void setup()
{
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
 
  dht_one.begin();
  dht_two.begin();
  dht_three.begin();
  dht_four.begin();
}

void get_print_data(DHT dht, String num)
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Sensor: ");
  Serial.print(num);
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
}

void loop() 
{
  // Wait a few seconds between measurements.
  delay(2000);
  get_print_data(dht_one, "one");
  get_print_data(dht_two, "two");
  get_print_data(dht_three, "three");
  get_print_data(dht_four, "four");
 
}
