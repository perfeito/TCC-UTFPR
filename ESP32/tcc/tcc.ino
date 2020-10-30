#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "BluetoothSerial.h" 

//wifi
const char* ssid       = "VIVOFIBRA-7479";
const char* password   = "7223067479";

//led
int LED_PIN_R = 26;
int LED_PIN_G = 27;
int LED_PIN_B = 25;

String BT_CALLBACK = "";
String BT_READ_LINE = "";

//const String base_url = "http://d5e6793ca93b.ngrok.io/api/verify_access";
const String base_url = "http://tcc-utfpr-perfeito.azurewebsites.net/api/verify_access";

BluetoothSerial SerialBT;

void httpGETRequest(String key) {
  if(WiFi.status()== WL_CONNECTED){

    SerialBT.println("WIFI IS CONNECTED");
    
    HTTPClient http;
    
    http.begin(base_url);
    
    int httpCode = http.POST(key);
    http.addHeader("Content-Type", "text/plain");
 
    if (httpCode > 0) {
 
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }

    if(httpCode == 200){
      SerialBT.println("SIM");
      Serial.println("SIM");

      digitalWrite(LED_PIN_G, HIGH);
      delay(1000);
      digitalWrite(LED_PIN_G, LOW);
      delay(1000);
    }
    else {
      SerialBT.println("NAO");
      Serial.println("NAO");
      Serial.println(httpCode);
      Serial.println("Error on HTTP request");

      digitalWrite(LED_PIN_R, HIGH);
      delay(1000);
      digitalWrite(LED_PIN_R, LOW);
      delay(1000);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
    delay(1000);

    digitalWrite(LED_PIN_R, HIGH);
    delay(1000);
    digitalWrite(LED_PIN_R, LOW);
    delay(1000);
  }
}

void connectToWiFi() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("WIFI CONNECTED");
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void initBluetooth() {
  
  SerialBT.begin("ESP32");

  SerialBT.register_callback(BTCallback);
  
  Serial.println("Bluetooth Device is Ready to Pair");

  digitalWrite(LED_PIN_R, HIGH);
  digitalWrite(LED_PIN_B, HIGH);
  delay(1000);
  digitalWrite(LED_PIN_R, LOW);
  digitalWrite(LED_PIN_B, LOW);
  delay(1000);
}

void BTCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    if(event == ESP_SPP_SRV_OPEN_EVT)
    {
      BT_CALLBACK = "BTonConnect";
      Serial.println("BT CONNECTION STABLISHED");

      digitalWrite(LED_PIN_B, HIGH);
      delay(1000);
      digitalWrite(LED_PIN_B, LOW);
      delay(1000);
    }
    else if(event == ESP_SPP_CLOSE_EVT)
    {
      ESP.restart();
      Serial.println("BT CONNECTION CLOSED");
    }
    else if(event == ESP_SPP_DATA_IND_EVT)
    {
      BT_CALLBACK = "BTonReceivedStart";
      Serial.println("BT DATA RECEIVED");
    }
    else if(event == ESP_SPP_WRITE_EVT)
    {
      BT_CALLBACK = "BTonWrite";
      Serial.println("BT DATA WRITED");
    }
}

void BTonReceivedStart()
{
    if(SerialBT.available())
    {
        byte r = SerialBT.read();
        BT_READ_LINE = BT_READ_LINE + char(r);
    }
    else
        BTonReceivedEnd();
}

void BTonReceivedEnd()
{
  Serial.println(BT_READ_LINE);
  const String key = BT_READ_LINE;
  httpGETRequest(key);

  BT_CALLBACK = "";
  BT_READ_LINE = "";
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);

  connectToWiFi();

  initBluetooth();
}

void loop() {
  if(BT_CALLBACK=="BTonReceivedStart")
  {
      BTonReceivedStart();
  }
}
