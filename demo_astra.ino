#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "LeNhat";         // your network SSID (name)
char password[] = "038201026519"; // your network key
WiFiClientSecure client;

#define TEST_HOST "32832bcd-af04-4a6c-b9a5-e21ac3383784-asia-south1.apps.astra.datastax.com"
//#define TEST_HOST "www.facebook.com"
//#define TEST_HOST_FINGERPRINT "DF 4B 86 87 00 01 B0 17 0E 96 B9 43 F2 F2 45 A5 B5 46 81 25"
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
  if (!client.connect(TEST_HOST, 443)) {
    Serial.println(F("Connection failed"));
  } 
  String pathstr = "/api/rest/v2/keyspaces/demo_with_arduino/rest_example_products";
  const char* path = pathstr.c_str();
  client.print("POST ");
  client.print(
      path + String(" HTTP/1.1\r\n") + "Host: " + TEST_HOST + "\r\n"
          + "Content-Type: application/json\r\n"
          + "X-Cassandra-Token: " + "AstraCS:kLwmEZnlbiYXKAHMCEfqbmQv:3ae394b4191498b60436fa0a16b3c5199f018e7378a506584b74e35f277ba2e3" + "\r\n"
          + "Connection: close\r\n");

  int id = (rand() % (1000 - 1 + 1)) + 1;
  String bodystr = "{\"id\": \"";
  bodystr.concat(String(id));
  bodystr.concat("\",\"productname\":\"");
  bodystr.concat("test");
  bodystr.concat("\"}");
  //String bodystr = "{\"id\":\"test911\",\"productname\":\"BME280\"}";
  const char* body = bodystr.c_str();
  if (body != NULL) {
    Serial.println("body");
    client.print("Content-Length: ");
    client.println(strlen(body)); // number of bytes in body
    client.println(); // we need an empty line between headers and body here
    client.print(body);
  }
  client.println();

  int code = 0;


  while (client.connected()) {
    String line = client.readStringUntil('\n');
    // get http response code
    if (code == 0 && line.startsWith("HTTP/1.1 ")) {
      String codeStr = line.substring(9);
      codeStr = codeStr.substring(0, 3);
      code = atoi(codeStr.c_str());
    }
    if (line == "\r") {
      // done reading headers
      break;
    }
  }
  String response = client.readStringUntil('\n');
  Serial.println(response);
  client.stop();

  delay(20000);
}

void loop() {
 makeHTTPRequest();

}
