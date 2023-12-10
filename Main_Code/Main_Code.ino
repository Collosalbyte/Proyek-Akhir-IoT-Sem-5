// ============================================================================
// LIBRARY LIBRARY LIBRARY LIBRARY LIBRARY LIBRARY LIBRARY LIBRARY LIBRARY
// ============================================================================
#include "HX711.h"

#include <ESP32Servo.h>

#include <WiFi.h>

#include <WebServer.h>

#include "DFRobotDFPlayerMini.h"

// ============================================================================
// KONSTANTA KONSTANTA  KONSTANTA KONSTANTA KONSTANTA  KONSTANTA KONSTANTA 
// ============================================================================
// Pin konfigurasi untuk button
#define BUTTON_PIN 15

// Pin konfigurasi untuk buzzer
#define BUZZER_PIN 22

// Pin konfigurasi untuk modul HX711
// GND = kuning, DT = Otrn, SCK = Merah, VCC = Coklat
#define LOADCELL_DOUT_PIN 32
#define LOADCELL_SCK_PIN 33

// Pin GPIO untuk servo
#define SERVO_PIN 13

#define RED_PIN 12 // Red channel
#define GREEN_PIN 14 // Green channel
#define BLUE_PIN 27 // Blue channel

// ============================================================================
// SENSOR SENSOR SENSOR SENSOR SENSOR SENSOR SENSOR SENSOR SENSOR SENSOR 
// ============================================================================
// Definisi sesnor timbangan modul HX711
HX711 scale;
// Definisi player mp3
DFRobotDFPlayerMini mp3;
// Object servo untuk kontrol servo
Servo myservo;

// ============================================================================
// Definisi Timer Definisi Timer Definisi Timer Definisi Timer Definisi Timer 
// ============================================================================
// Deklarasi timer handle untuk menutup pintu otomatis
TimerHandle_t autoCloseTimer;

// ============================================================================
// WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi WiFi 
// ============================================================================
// Kredensial WiFi
const char * ssid = "jembatan";
const char * password = "@kHSMR888";

// ============================================================================
// WebServer WebServer WebServer WebServer WebServer WebServer WebServer
// ============================================================================
// Menjalankan web server pada port 80
WebServer server(80);

// ============================================================================
// Functions Functions Functions Functions Functions Functions Functions
// ============================================================================
// Task
void webServer(void * pvParameters);
void buzzerButton(void * pvParameters);
void checkConnections(void * pvParameters);

// Functions
void autoCloseTimerCallback(TimerHandle_t xTimer);
void handleRoot();
void handleOpenDoor();
void handleCloseDoor();
void handleButtonStatus();
void handleWeight();
void startMP3();
void buttonBuzzer();
float getWeight();

void yellowColor();
void greenColor();
void redColor();
void blueColor();
void turnOff();

// ============================================================================
// Global Variable Global Variable Global Variable Global Variable
// ============================================================================
// Untuk button 
bool lastButtonState = HIGH; // kondisi awal tombol (tidak tertekan)
unsigned long lastDebounceTime = 0; // waktu terakhir tombol berubah status
unsigned long debounceDelay = 100; // waktu debounce dalam milidetik
volatile bool buttonPressed = false;

// Untuk status pintu
volatile bool isDoorOpen = false;

// ============================================================================
// Semaphore Semaphore Semaphore Semaphore Semaphore Semaphore Semaphore
// ============================================================================
SemaphoreHandle_t doorMutex;
SemaphoreHandle_t coreOneMutex;

// ============================================================================
// Setup Setup Setup Setup Setup Setup Setup Setup Setup Setup Setup Setup
// ============================================================================
void setup() {
    // SERIAL MONITOR
    // Memulai serial communication untuk melihat di serial monitor
    Serial.begin(115200);

    // RGB LED
    // Konfigurasi pin
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // SERVO
    // Memulai servo dengan posisi 0
    myservo.attach(SERVO_PIN);
    myservo.write(90);

    // WIFI
    WiFi.begin(ssid, password);

    // Menyambung ke WiFi
    while (WiFi.status() != WL_CONNECTED) {
        turnOff();
        delay(500);
        Serial.print(".");
        redColor();
        delay(500);
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    blueColor();

    // INISIALISASI TIMBANGAN
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
     //scale.set_scale(400.0588); // Menggunakan faktor skala dari hasil kalibrasi
    //scale.set_scale(400.0588); // Menggunakan faktor skala dari hasil kalibrasi
    scale.tare(); // Reset skala menjadi 0

    // WEB SERVER
    // Mendefinisikan endpoint
    server.on("/", handleRoot);
    server.on("/open", handleOpenDoor);
    server.on("/close", handleCloseDoor);
    server.on("/button-status", HTTP_GET, handleButtonStatus); // Menetapkan handler untuk route /button-status untuk notifikasi
    server.on("/weight", HTTP_GET, handleWeight); // Menetapkan handler untuk route mendapatkan nilai dari timbangan

    // Memulai server
    server.begin();
    Serial.println("HTTP server started");

    // MP3
    //Mulai MP3
    startMP3();

    // MUTEX
    doorMutex = xSemaphoreCreateMutex(); // Inisialisasi mutex
    coreOneMutex = xSemaphoreCreateMutex();

    // BUTTON DAN BUZZER
    // Konfigurasi pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH);
    //pinMode(BUZZER_PIN, INPUT_PULLDOWN);

    // MEMBUAT TASK
    // Task untuk menghandle client server
    xTaskCreatePinnedToCore(
        webServer, 
        "webServer", 
        10000, 
        NULL, 
        0, 
        NULL,
        0); 

    // Task untuk menghandle buzzer
    xTaskCreatePinnedToCore(
        buzzerButton, 
        "buzzerButton", 
        4096, 
        NULL, 
        2, 
        NULL,
        1); 

    // Task untuk melakukan koneksi ulang jika ada yang terputus
    xTaskCreatePinnedToCore(
        checkConnections, 
        "checkConnections", 
        10000, 
        NULL, 
        1, 
        NULL,
        1); 
}

void loop() {
    // Tidak digunakan karena menggunakan task FreeRTOS
}

// ============================================================================
// TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK
// ============================================================================
// Task untuk melakukan koneksi ulang jika ada yang terputus
void checkConnections(void * pvParameters) {
    Serial.println("Core 1: Checking connections");

    while (true) {
        if (xSemaphoreTake(coreOneMutex, 225)) {
            // Cek status koneksi DFPlayer
            //if (!isDFPlayerConnected()) {
            // Kode untuk menghubungkan ulang DFPlayer
            //startMP3();
            //}

            // Cek status koneksi WiFi
            if (WiFi.status() != WL_CONNECTED) {
                WiFi.begin(ssid, password);
                while (WiFi.status() != WL_CONNECTED) {
                    turnOff();
                    delay(500);
                    Serial.print(".");
                    redColor();
                    delay(500);
                }
            }

            xSemaphoreGive(coreOneMutex);
        }
        // Tunda task untuk beberapa waktu
        vTaskDelay(pdMS_TO_TICKS(10000)); // Tunda selama 10 detik
    }
}

// Task untuk menghandle client server
void webServer(void * pvParameters) {
    Serial.println("Core 1");
    while (true) {
        server.handleClient();
    }
}

// Task untuk menghandle tombol buzzer
void buzzerButton(void * pvParameters) {
    Serial.println("Core 2");
    while (true) {
        if (xSemaphoreTake(coreOneMutex, 5)) {
            buttonBuzzer();
            xSemaphoreGive(coreOneMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Tunda task untuk beberapa waktu
    }
}

// ============================================================================
// Functions Functions Functions Functions Functions Functions Functions
// ============================================================================
// Task untuk membaca tombol dan mengaktifkan buzzer
void buttonBuzzer() {
    // baca status tombol
    int reading = digitalRead(BUTTON_PIN);
    // jika status berubah, reset timer debounce
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // jika status tombol stabil selama waktu debounce, proses perubahan status
        if (reading == LOW) {
            Serial.println("Button Pressed! Buzzer On!");
            buttonPressed = true;
            digitalWrite(BUZZER_PIN, LOW);
            //digitalWrite(BUZZER_PIN, HIGH);
            delay(100);
            //digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(BUZZER_PIN, HIGH);
        } else {
            buttonPressed = false;
            //digitalWrite(BUZZER_PIN, HIGH);
            //digitalWrite(BUZZER_PIN, LOW);
        }
    }

    // simpan status untuk iterasi berikutnya
    lastButtonState = reading;
}

// Mendapatkan nilai pembacaan sensor timbangan HX711
float getWeight() {
    float weight = 0;
    if (scale.is_ready()) {
        weight = scale.get_units(20); // Mengambil rata-rata 20 pembacaan
        Serial.print("Berat: ");
        Serial.print(weight, 2); // Menampilkan 2 angka di belakang koma
        Serial.println(" gram");
    } else {
        Serial.println("HX711 not found.");
    }
    return weight;
}

// Functions untuk RGB LED
// RGB berwarna kuning
void yellowColor() {
    analogWrite(RED_PIN, 255); // Set RED to its maximum value
    analogWrite(GREEN_PIN, 255); // Set GREEN to its maximum value
    analogWrite(BLUE_PIN, 0); // Set BLUE to 0
}

// RGB berwarna hijau
void greenColor() {
    analogWrite(RED_PIN, 0); // Set RED to 0
    analogWrite(GREEN_PIN, 255); // Set GREEN to its maximum value
    analogWrite(BLUE_PIN, 0); // Set BLUE to 0
}

// RGB berwarna merah
void redColor() {
    analogWrite(RED_PIN, 255); // Set RED to 0
    analogWrite(GREEN_PIN, 0); // Set GREEN to its maximum value
    analogWrite(BLUE_PIN, 0); // Set BLUE to 0
}

// RGB berwarna biru
void blueColor() {
    analogWrite(RED_PIN, 0); // Set RED to 0
    analogWrite(GREEN_PIN, 0); // Set GREEN to its maximum value
    analogWrite(BLUE_PIN, 255); // Set BLUE to 0
}

// LED mati
void turnOff() {
    analogWrite(RED_PIN, 0); // Set RED to 0
    analogWrite(GREEN_PIN, 0); // Set GREEN to 0
    analogWrite(BLUE_PIN, 0); // Set BLUE to 0
}

// Fungsi callback timer
void autoCloseTimerCallback(TimerHandle_t xTimer) {
    handleCloseDoor();
}

// Mengecek apakah mp3 tersambung atau tidak
bool isDFPlayerConnected() {
    mp3.volume(30); // Kirim perintah set volume

    if (mp3.available()) {
      Serial.println("MP3 Terhubung");
        // Jika DFPlayer mengirimkan respons, maka itu terhubung
        return true;
    }
    Serial.println("MP3 terputus");
    // Jika tidak ada respons, kembalikan false
    return false;
}

// Memulai MP3
void startMP3() {
    Serial2.begin(9600);
    delay(100);
    Serial.println();
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

    if (!mp3.begin(Serial2)) { //Use softwareSerial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
    } else {
        Serial.println(F("DFPlayer Mini online."));

        mp3.setTimeOut(500); //Set serial communictaion time out 500ms

        //----Set volume----
        mp3.volume(30); //Set volume value (0~30).

        //----Set different EQ----
        mp3.EQ(DFPLAYER_EQ_NORMAL);                                               

        //----Set device we use SD as default----
        mp3.outputDevice(DFPLAYER_DEVICE_SD);
    }
}

// ============================================================================
// ENDPOINT ENDPOINT ENDPOINTENDPOINT ENDPOINT ENDPOINT ENDPOINT ENDPOINT
// ============================================================================
// Mendapatkan pembacaan sensor timbangan HX711
void handleWeight() {
    float sendWeight = getWeight(); // Fungsi untuk membaca berat dari HX711
    server.send(200, "text/plain", String(sendWeight)); // Mengirim data sebagai teks
}

// Mendapatkan status pembacaan tombol buzzer
void handleButtonStatus() {
    if (buttonPressed) {
        server.send(200, "text/plain", "Pressed");
    }
}

// Halaman utama
void handleRoot() {
    server.send(200, "text/plain", "ESP32 Server is running");
}

// Membuka pintu
void handleOpenDoor() {
    xSemaphoreTake(doorMutex, portMAX_DELAY); // Ambil mutex
    mp3.play(1);  //Play the first mp3
    delay(2000);
    myservo.write(0);
    isDoorOpen = true;
    xSemaphoreGive(doorMutex); // Lepas mutex
    // Mulai timer untuk menutup pintu secara otomatis setelah 10 detik
    if (autoCloseTimer == NULL) {
        autoCloseTimer = xTimerCreate("AutoCloseTimer", pdMS_TO_TICKS(10000), pdFALSE, (void * ) 0, autoCloseTimerCallback);
    }
    xTimerStart(autoCloseTimer, 0);
    Serial.println("open");
    server.send(200, "text/plain", "Door Opened");
}

// Menutup pintu
void handleCloseDoor() {
    xSemaphoreTake(doorMutex, portMAX_DELAY); // Ambil mutex
    // Perintahkan servo untuk berputar dengan kecepatan tertentu
    myservo.write(90); // Angka ini bisa diubah untuk menyesuaikan kecepatan putar
    delay(100); // Berikan waktu untuk servo berputar sebelum menghentikannya
    isDoorOpen = false;
    xSemaphoreGive(doorMutex); // Lepas mutex
    // Stop timer karena pintu sudah ditutup
    if (autoCloseTimer != NULL) {
        xTimerStop(autoCloseTimer, 0);
    }
    Serial.println("close");
    server.send(200, "text/plain", "Door Closed");
}