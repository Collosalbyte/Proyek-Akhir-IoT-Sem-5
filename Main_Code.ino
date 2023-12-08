#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini mp3;

// Ganti dengan SSID dan Password jaringan WiFi Anda
const char* ssid = "jembatan";
const char* password = "@kHSMR888";

WebServer server(80); // Server pada port 80

// Prototype fungsi
void timerTask(void *pvParameters);
void timerCallback(TimerHandle_t xTimer);

void handleRoot();
void handleOpenDoor();
void handleCloseDoor();

void startMP3();

void setup() {
  Serial.begin(115200);

  // Koneksi ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());



  // Mendefinisikan endpoint
  server.on("/", handleRoot);
  server.on("/open", handleOpenDoor);
  server.on("/close", handleCloseDoor);

  // Memulai server
  server.begin();
  startMP3();
  Serial.println("HTTP server started");

  
}

void loop() {
  // Menangani permintaan client
  server.handleClient();
}

void startMP3(){
  Serial2.begin(9600);
  delay(100);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!mp3.begin(Serial2)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  mp3.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  mp3.volume(30);  //Set volume value (0~30).

  //----Set different EQ----
  mp3.EQ(DFPLAYER_EQ_NORMAL);

  //----Set device we use SD as default----
  mp3.outputDevice(DFPLAYER_DEVICE_SD);
}

// Fungsi untuk menangani permintaan root atau halaman utama
void handleRoot() {
  server.send(200, "text/plain", "ESP32 Server is running");
}

// Fungsi untuk membuka pintu (contoh)
void handleOpenDoor() {
  // Logika untuk membuka pintu
  // ...
  //mp3.play(1);  //Play the first mp3
  //delay(1000);
    myServo.write(180);
  Serial.println("open");
  server.send(200, "text/plain", "Door Opened");
}

// Fungsi untuk menutup pintu (contoh)
void handleCloseDoor() {
  // Logika untuk menutup pintu
  // ...
   myServo.write(0); 
    Serial.println("close");
  server.send(200, "text/plain", "Door Closed");
}