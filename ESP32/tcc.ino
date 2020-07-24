#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid       = "moto g7";
const char* password   = "4467bc157f6a";
//const char* ssid       = "Windows Phone3836";
//const char* password   = "5#5L80t4";

String code_id   = "8467287462376492#";

String jsonBuffer;

String base_url = "https://tcc-utfpr-perfeito.azurewebsites.net/api/";

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void finish(){
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

String httpGETRequest() {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(base_url + "verify_access?code_id=a8467287462376492");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void loop()
{
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      
      jsonBuffer = httpGETRequest();
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);

      JSONVar keys = myObject.keys();

      boolean result = false;

      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print("key: ");
        Serial.print(keys[i]);
        Serial.print(" - value: ");
        Serial.println(value);

        String data_json = JSON.stringify(value);
        
        if (data_json == "access granted") {
          result = true;
          break;
        }
      }

      if(result){
        Serial.println("Acessou");
      }
      else {
        Serial.println("Sem acesso");
      }
    }
    else {
      Serial.println("WiFi Disconnected");
      delay(1000);
    }
  }
