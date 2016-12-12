#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <DallasTemperature.h>

// Credentials reside in the Credentials.h file (which is not part of the github
// repository). Create one with the following fields:
// SSID, PASS, APIKEY, CHANNEL
#include <Credentials.h>

#define DEBUG 0
#define ONE_WIRE_BUS 2 //GPIO2

const char* host = "api.thingspeak.com";
int sleeptime = 2; // sleeptime in minutes

// De ADC pin
const int switchPin = 12;

// Thingspeak fields
int temperature_field = 1;
int adc_field = 2;

// De resistors for the ADC
float r1 = 100000;
float r2 =  15000;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

char temperatureString[6];
char voltageString[6];

void setup(void){
  Serial.begin(115200);

  // Connect to the WiFi network
  WiFi.begin(SSID, PASS);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // initialize the ds18b20
  DS18B20.begin();

  // set the ADC pullup to high
  pinMode(switchPin, OUTPUT);
  digitalWrite(switchPin, HIGH);

  // debugging info
  if ( DEBUG ) {
    Serial.println();
    Serial.print("ESP.getBootMode(); ");
    Serial.println(ESP.getBootMode());
    Serial.print("ESP.getSdkVersion(); ");
    Serial.println(ESP.getSdkVersion());
    Serial.print("ESP.getBootVersion(); ");
    Serial.println(ESP.getBootVersion());
    Serial.print("ESP.getChipId(); ");
    Serial.println(ESP.getChipId());
    Serial.print("ESP.getFlashChipSize(); ");
    Serial.println(ESP.getFlashChipSize());
    Serial.print("ESP.getFlashChipRealSize(); ");
    Serial.println(ESP.getFlashChipRealSize());
    Serial.print("ESP.getFlashChipSizeByChipId(); ");
    Serial.println(ESP.getFlashChipSizeByChipId());
    Serial.print("ESP.getFlashChipId(); ");
    Serial.println(ESP.getFlashChipId());
  }

}

float getTemperature() {
  float temp;
  int i = 0;

  do {
    DS18B20.requestTemperatures();
    temp = DS18B20.getTempCByIndex(0);

    // If we cannot find the temparture after 10 attempts, we default it to -20
    // degrees
    if ( i++ > 10 ) {
      temp = -20.0;
      break;
    }

    Serial.print("i="); Serial.println(i);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

float getVoltage() {
  float val = (float)analogRead(A0);
  Serial.print("ADC value: "); Serial.println(val);
  float voltage =  val * (( r1 + r2 ) / r2 ) / 1023;    // read the input pin nieuwe bord 1/1024 * 15k / 100k
  Serial.print("Voltage value: "); Serial.println(voltage);
  return voltage;
}

void loop() {
  char url[256];

  // Check if we can reach Thingspeak
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Thingspeak unreachable right now, going to deepsleep for 15 minutes");
    ESP.deepSleep(15 * 60 * 1000000);
  }

  // request temperatuur
  float temperature = getTemperature();
  dtostrf(temperature, 2, 1, temperatureString);

  // request remaining battery voltage
  float voltage = getVoltage();
  dtostrf(voltage, 2, 2, voltageString);

  // lower the pullup for the ADC
  digitalWrite(switchPin, LOW);

  // Construct the url
  sprintf(url, "http://%s/update?key=%s&field%d=%s&field%d=%s", host, apikey, temperaturefield, temperatureString, voltagefield, voltageString);
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n\r\n");

  // Go back to sleep for the sleeptime.
  // GPIO16 needs to be connected to reset, in order to wake from the deepsleep
  Serial.print("Going to sleep (in minutes): ");
  Serial.println(sleeptime);
  ESP.deepSleep(sleeptime * 60 * 1000000);

}
