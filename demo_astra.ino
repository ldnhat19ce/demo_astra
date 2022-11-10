#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

char ssid[] = "n";         // your network SSID (name)
char password[] = "888888888"; // your network key
WiFiClientSecure client;

#define TEST_HOST "32832bcd-af04-4a6c-b9a5-e21ac3383784-asia-south1.apps.astra.datastax.com"
#define TEST_HOST_FINGERPRINT "50 2A CA 9D 78 AD A0 A2 89 52 39 33 CC ED 8E DD 11 14 34 A6"

void setup() {

  Serial.begin(115200);

  // Connect to the WiFI
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  //--------

  // If you don't need to check the fingerprint
  // client.setInsecure();

  // If you want to check the fingerprint
  client.setFingerprint(TEST_HOST_FINGERPRINT);

  
}

void makeHTTPRequest() {
  HTTPClient http;
  if (!client.connect(TEST_HOST, 443)) {
    Serial.println(F("Connection failed"));
  } 
  
  String serverName = "https://32832bcd-af04-4a6c-b9a5-e21ac3383784-asia-south1.apps.astra.datastax.com/api/rest/v2/keyspaces/demo_with_arduino/rest_example_products";
  http.begin(client, serverName.c_str());
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Cassandra-Token", "AstraCS:kLwmEZnlbiYXKAHMCEfqbmQv:3ae394b4191498b60436fa0a16b3c5199f018e7378a506584b74e35f277ba2e3");

  /*Define body*/
  int id = (rand() % (1000 - 1 + 1)) + 1;
  String bodystr = "{\"id\": \"";
  bodystr.concat(String(id));
  bodystr.concat("\",\"productname\":\"");
  bodystr.concat("test");
  bodystr.concat("\"}");
  int httpResponseCode = http.POST(bodystr.c_str());
  if (httpResponseCode>0) {
        String payload = http.getString();
        Serial.println("payload: "+payload);
    }else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  /*end define body*/

  delay(20000);
}

void loop() {
 makeHTTPRequest();
}
