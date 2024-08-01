#include <WiFi.h>
#include <PubSubClient.h>
#include <config.h>

//wifi
extern WiFiClient    espClient;
extern const char*   ssid;
extern const char*   password;


extern PubSubClient  client; // creates a pub sub client 
extern const char    *mqtt_broker;

void connect_broker()
{
  client.setServer(mqtt_broker, mqtt_port);

  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public EMQX MQTT broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
}

void connect_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int status = WL_IDLE_STATUS;
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println(get_wifi_status(status));
    delay(500);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print(" WiFi network (gateway) IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("[+] Subnet Mask : ");
  Serial.println(WiFi.subnetMask());
  Serial.println((String)"[+] RSSI : " + WiFi.RSSI() + " dB");
}

String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
    return String("no status case");
}

