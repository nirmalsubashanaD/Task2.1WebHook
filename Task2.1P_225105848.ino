//KALANCHIGE NIRMAL SUBASHANA
//225105848

#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Home WiFi";   
char pass[] = "akith12345";
WiFiClient client;
unsigned long channelID = 2877304;     
const char* apiKey = "SGKLHHPUYV66IEXC";  

void setup() {
  Serial.begin(115200);
  
  unsigned long startTime = millis();
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime > 30000) {
      Serial.println("Failed to connect to WiFi");
      return;
    }
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor, retrying...");
    delay(2000);
    return; 
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println("%");

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);

  int response = ThingSpeak.writeFields(channelID, apiKey);
  
  if (response == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.print("Error writing to ThingSpeak. Response code: ");
    Serial.println(response);
  }

  delay(60000);  
}
