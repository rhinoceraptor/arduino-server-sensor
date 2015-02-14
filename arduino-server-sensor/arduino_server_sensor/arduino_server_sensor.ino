/*
 * Original code written by ladyada, released into the public domain.
 * https://github.com/adafruit/TinyDHT/blob/master/examples/TinyDHT_TestUno.ino
 * This code relies on the Adafruit DHT library
 */
 
#include "DHT.h"
#include <SPI.h>
#include <EtherCard.h>

/* Define the digital input pins for the sensors */
#define DHT_ONE 2
#define DHT_TWO 3
#define DHT_THREE 4
#define DHT_FOUR 5
/* we are using DHT11 sensors */
#define DHT_TYPE DHT11

DHT dht_one(DHT_ONE, DHT_TYPE);
DHT dht_two(DHT_TWO, DHT_TYPE);
DHT dht_three(DHT_THREE, DHT_TYPE);
DHT dht_four(DHT_FOUR, DHT_TYPE);

static byte mac[] = {0x00, 0x19, 0xC5, 0xE5, 0x80, 0x1F};
static byte ip[] = {192, 168, 1, 37};

byte Ethernet::buffer[500];
BufferFiller bfill;

float dht_val_one[] = {0.0, 0.0, 0.0};
float dht_val_two[] = {0.0, 0.0, 0.0};
float dht_val_three[] = {0.0, 0.0, 0.0};
float dht_val_four[] = {0.0, 0.0, 0.0};

unsigned long last_data = 0;
unsigned long data_interval = 3000;

void setup()
{
  ether.begin(sizeof Ethernet::buffer, mac);
  ether.staticSetup(ip);
 
  dht_one.begin();
  dht_two.begin();
  dht_three.begin();
  dht_four.begin();
}

/* General function for printing temp data for the given dht */
void get_data(DHT dht, float values[])
{
  /* 
   * index 0: humidity
   * index 1: Calcius
   * index 2: Fahrenheight
   */
  values[0] = dht.readHumidity();
  values[1] = dht.readTemperature();
  values[2] = dht.readTemperature(true);

  if (isnan(values[0]) || isnan(values[1]) || isnan(values[2])) 
  {
    return;
  }
  return;
}

/* Only read data every couple of seconds */
void try_get_data()
{
  // The cast helps since millis() overflows every 49 days
  if ((unsigned long)(millis() - last_data) >= data_interval)
  {
    last_data = millis();
    get_data(dht_one, dht_val_one);
    get_data(dht_two, dht_val_two);
    get_data(dht_three, dht_val_three);
    get_data(dht_four, dht_val_four);
  
    return;
  }
  else
  {
    return;
  }
}

static word show_temps()
{
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "{\r\n"
    "  \"sensor one\": {\r\n"
    "    \"humidity\": \"$T\",\r\n"
    "    \"celcius\": \"$T\",\r\n"
    "    \"fahrenheit\": \"$T\"\r\n"
    "  },\r\n"
    "  \"sensor two\": {\r\n"
    "    \"humidity\": \"$T\",\r\n"
    "    \"celcius\": \"$T\",\r\n"
    "    \"fahrenheit\": \"$T\"\r\n"
    "  },\r\n"
    "  \"sensor three\": {\r\n"
    "    \"humidity\": \"$T\",\r\n"
    "    \"celcius\": \"$T\",\r\n"
    "    \"fahrenheit\": \"$T\"\r\n"
    "  },\r\n"
    "  \"sensor four\": {\r\n"
    "    \"humidity\": \"$T\",\r\n"
    "    \"celcius\": \"$T\",\r\n"
    "    \"fahrenheit\": \"$T\"\r\n"
    "  }\r\n"
    "}\r\n"
    ), dht_val_one[0], dht_val_one[1], dht_val_one[2],
       dht_val_two[0], dht_val_two[1], dht_val_two[2],
       dht_val_three[0], dht_val_three[1], dht_val_three[2],
       dht_val_four[0], dht_val_four[1], dht_val_four[2]);
  return bfill.position();
}

void loop() 
{
  try_get_data();

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)
    ether.httpServerReply(show_temps());

}
