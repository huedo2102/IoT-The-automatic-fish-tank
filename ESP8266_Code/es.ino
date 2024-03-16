/*
 * This is the code for the WiFi Module 
 *  
 * The job of the Wifi Module (ESP8266-01) is to receive HTTP requests over the local Wifi network
 *  and then then forward that request to the Arduino over the serial connection. The Arduino then 
 *  processes the request and generates a response to the request(if required) and send it to
 *  the Wi-Fi module over serial. After which the Wi-Fi module responds back to the requester over 
 *  the local network.
 * 
 * 
 * Before uploading this code, ensure that you assgin variables "ssid" and "password" variables
 *  with your wifi credentials. 
 *  [There is no framework to assign a wifi network via a user interface]
 *  [Everytime you change your WiFi network, you will have to change the 
 *    mentioned variables, recompile and reupload the code.]
 */


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "VIETTEL_CUjv"
#define WIFI_PASSWORD "huedo50984"
#define API_KEY "AIzaSyAijfIusyAeyua4YZDckEAmR2gSXHaAdNA"
#define DATABASE_URL "https://smart-aquaeium-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "hue86219@st.vimaru.edu.vn"
#define USER_PASSWORD "huedo50984"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

 
//GPIO 
const int ledPin = D2; // GPIO2 of ESP8266 - LED used to indicate the status of the module



const char* ssid = "VIETTEL_CUjv";//type your ssid
const char* password = "huedo50984";//type your password


int i=0;

//String rec;//remove this if not used
unsigned long previousMillis = 0;
const long interval = 10000; // 1 phút


/**
 * setup() function
 * Called once when the WiFi module boots up
 * 
*/
void setup() {

  //LED: Initialize LED.
  pinMode(ledPin, OUTPUT);


  //LED: Turn LED ON when below initializing process begins
  digitalWrite(ledPin, HIGH);

  //Begin serial for communication with arduino
  Serial.begin(9600);
  delay(500);

  //Begin WiFi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(500);
  while (WiFi.status() != WL_CONNECTED)//Connect to Wifi
  {
    delay(500); //wait till the WiFi is connected.
  }
  delay(500);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  
  Firebase.begin(&config, &auth);
  
  //LED: Turn LED OFF when initializing is done.
  digitalWrite(ledPin, LOW);
  Firebase.getString(fbdo, "/CRTC");
  Serial.println(fbdo.stringData());
  Serial.flush();
 
  
  
  
}


/**
 * loop() function
 * This is function is called continuously by the arduino
 * Statements in here get executed like they are in an inifinite loop; 
*/
void loop() 
{
  if (Serial.available()) { // Kiểm tra nếu có dữ liệu nhận được từ Arduino
    String arduinoRequest = Serial.readStringUntil('\n'); // Đọc dữ liệu từ cổng Serial cho đến khi gặp ký tự xuống dòng '\n'
    arduinoRequest.trim(); 
    if (arduinoRequest.indexOf("T:") != -1){
      Firebase.setString(fbdo, "/T",arduinoRequest);
    }
  }

  /////////////////////////////////////////////////////////
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Đoạn code bạn muốn thực hiện sau mỗi khoảng thời gian

    Firebase.getString(fbdo, "/CA");
    Serial.println(fbdo.stringData());
    Serial.flush();
  }

  

  if(i==50000){
    i=0;
    Firebase.getString(fbdo, "/Led");
    Serial.println(fbdo.stringData());
    Serial.flush();
    Firebase.getString(fbdo, "/Servo");
    Serial.println(fbdo.stringData());
    Serial.flush();
    

  }
  i++;
  
  


}
