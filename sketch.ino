#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>

#define SSID "Wokwi-GUEST"
#define PASS ""
String HOME_URL = "https://api.thingspeak.com/update?api_key=6OQLKT6UGBZRH2Q9&field1=";


DHTesp dhtsensor;
TempAndHumidity data;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("WiFI bắt đầu kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFI đã kết nối");

  dhtsensor.setup(15, DHTesp::DHT22);
}

void loop() {
  data = dhtsensor.getTempAndHumidity();
  Serial.println("humidity: " + String(data.humidity));
  Serial.println("temperature: " + String(data.temperature));


  HTTPClient http;

  // Specify the target URL
  http.begin(HOME_URL + String(data.temperature) + "&field2=" + String(data.humidity));

  // Send the GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
  delay(10000);
}
