#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP_Mail_Client.h>
#include <EEPROM.h>

String ssid = "";
String password = "";

ESP8266WebServer server(80);
String receivedEmail;  // Stores the email entered by the user

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "mail@gmail.com"     // Your email
#define AUTHOR_PASSWORD "**** **** **** ****"  // Use an App Password

#define ssid_index 0
#define pass_index 32


SMTPSession smtp;

bool softapon = false;
bool credentials_exist = false;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  smtp.debug(1);

  load_credentials_EEPROM();
  Serial.println("Loaded ssid: " + ssid + "password: " + password);
  delay(1000);
  if (ssid == "")
    accesspointmode();
  else
    wifimode();

  Serial.println("HTTP server started.");

  
}

void loop() {
  server.handleClient();
}

void accesspointmode(){
  softapon = true;
  credentials_exist = false;
  server.close();
  setupserver();
  server.begin();
  WiFi.softAP("HTML_Sender_Inialize");
  Serial.println("Enter network");
  Serial.println(WiFi.softAPIP());
}

void wifimode(){
  if(softapon)
    {
      WiFi.softAPdisconnect(true);
      softapon = false;
    }
  WiFi.begin(ssid,password);
  Serial.print("Connecting to wifi");
  int count_fail = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count_fail = count_fail +1;
    if (count_fail >= 30)
    {
      accesspointmode();
      Serial.println("Failed to connect to wifi, entered access mode");
      return;
    }  
  }
  credentials_exist = true;
  server.close();
  setupserver();
  server.begin();
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


}

