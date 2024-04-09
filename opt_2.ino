#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define API_KEY ""
#define DATABASE_URL ""


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

int LED_PIN_1 = 2;  // GPIO pin connected to the LED

String clusterID = "Cluster-A"; // Unique identifier for the Cluster
String nodeID_1 = "Node-1";  // Unique identifier for the first ESP32 node


int counter;
unsigned long sendDataPrevMillis = 0;

bool ledStatus;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_1, OUTPUT);

  connectFirebase();
}

void connectFirebase() {
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
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void controlLED(int pin, const String &cluster, const String &nodeID) {
  if (Firebase.ready() && signupOK && (millis() -sendDataPrevMillis > 500 || sendDataPrevMillis ==0)) {
    sendDataPrevMillis = millis();
    String nodePath = "/" + cluster + "/" + nodeID + "/LEDStatus";
    
    if (Firebase.RTDB.getBool(&fbdo, nodePath.c_str())) 
    {
      // Serial.println("check");
      if (fbdo.dataType() == "string") 
      {
        Serial.println(fbdo.stringData());
        Serial.println("boolean?");
        if(strcmp("ON",fbdo.stringData().c_str())==0){
        ledStatus = 1;
        digitalWrite(pin, ledStatus);

        }
        else if(strcmp("OFF",fbdo.stringData().c_str())==0){
          ledStatus = 0;
        digitalWrite(pin, ledStatus);
        }
       
        } 
    }
    else digitalWrite(pin,LOW);
}
}
   
void loop() {
  controlLED(LED_PIN_1, clusterID, nodeID_1);// Control LED for Node 1
}
