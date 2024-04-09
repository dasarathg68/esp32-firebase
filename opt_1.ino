#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define API_KEY ""
#define DATABASE_URL ""

int LED_PIN_1 = 2;  // GPIO pin connected to the LED

String nodeNumber ="1";
String clusterID = "Cluster-A"; // Unique identifier for the Cluster
String nodeID_1 = "Node-"+nodeNumber;  // Unique identifier for the first ESP32 node
String ledStatusPath = "/"+nodeID_1+"/LEDStatus";

FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

FirebaseData firebaseData;


void setup() {

  pinMode(LED_PIN_1, OUTPUT);
  
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  while (!Firebase.signUp(&config, &auth, "", "")) {
      Serial.print(".");
      delay(300);
  } 
   Serial.println("SignUp OK");
    signupOK = true;
  

  //  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setMaxErrorQueue(firebaseData, 30);


  if(!Firebase.beginStream(firebaseData, "/Cluster-A"))
  {
    Serial.println(firebaseData.errorReason());
  }
  Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);

  delay(300);
}

void loop() {
    if (Firebase.isTokenExpired()){
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  }
}
void streamCallback(StreamData data)
{
  Serial.println("Stream Data.....");
  Serial.println(data.streamPath());
  Serial.println(data.dataPath());
  Serial.println(data.dataType());
  Serial.println(ledStatusPath);
  if (strcmp(data.dataPath().c_str(),"/")==0) // to read initial status set in the firebase console
      {
        if (data.dataTypeEnum() == fb_esp_rtdb_data_type_json){
             FirebaseJson json = data.to<FirebaseJson>();
             size_t count = json.iteratorBegin();
            
                FirebaseJson::IteratorValue value = json.valueAt(nodeNumber.toInt());
                Serial.print(value.value);
                Serial.println(strcmp("ON",value.value.c_str()));
                
                
            if(strcmp("\"ON\"", value.value.c_str()) == 0) {
                       digitalWrite(LED_PIN_1, 1);
                        }
              else{
                  Serial.println("OFF");
                digitalWrite(LED_PIN_1,0);
                   }
                
            }
      }
  if (strcmp(data.dataPath().c_str(),ledStatusPath.c_str())==0)
  {
    if(strcmp("ON",data.stringData().c_str())==0)
      {
        Serial.println("ON");
        digitalWrite(LED_PIN_1, 1);
        }
    else{
        Serial.println("OFF");
      digitalWrite(LED_PIN_1,0);
    }
  }



}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    Serial.println();
    Serial.println("Stream timeout, resume streaming...........");
    Serial.println();
  }
}
