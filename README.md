# SafeDrop (Final Project IOT 5)

## Team Members
- Mohamad Varrel Bramasta 
- Roy Oswhalda
- Michael Winston
- Jeffri

## Overview
Proyek ini adalah sistem pemantauan pengiriman inovatif yang menggunakan Internet of Things (IoT) untuk meningkatkan efisiensi dan keamanan dalam proses pengiriman barang. Dengan memanfaatkan berbagai Hardware, proyek ini mengintegrasikan ESP32 CAM sebagai kamera untuk mendeteksi keberadaan kurir, servo untuk menggerakkan wooden box sebagai tempat penyimpanan barang, HX711 sebagai penimbang berat di dalam wooden box, serta speaker untuk memberikan pemberitahuan suara dari file MP3. Keseluruhan sistem dikendalikan oleh ESP32-WROOM yang berfungsi sebagai pusat kontrol dan penghubung IoT. Kamera ESP32 CAM digunakan untuk mendeteksi kurir, sementara servo menggerakkan wooden box dan HX711 memberikan informasi akurat tentang berat barang. Speaker digunakan untuk memberikan pemberitahuan suara yang dapat diprogram. Proyek ini memberikan solusi terpadu untuk pemantauan dan kontrol otomatis dalam pengiriman barang, meningkatkan efisiensi operasional dan keamanan dengan menggabungkan teknologi IoT yang canggih.

## Problem Statement

Dalam industri pengiriman barang, terdapat sejumlah tantangan yang dapat menghambat efisiensi dan keamanan proses pengiriman. Beberapa masalah yang dihadapi termasuk:

1. **Ketidakpastian Keberadaan Kurir:**
   - Sulit untuk memantau dan memastikan keberadaan kurir di titik pengambilan atau pengantaran barang.

2. **Ketidakakuratan Informasi Berat Barang:**
   - Kurangnya informasi akurat tentang berat barang dapat mengarah pada ketidakakuratan biaya pengiriman dan potensi masalah logistik.

3. **Ketidakefisienan Pemberian Pemberitahuan:**
   - Kurangnya sistem pemberitahuan yang efisien kepada penerima atau pengirim mengenai status pengiriman dapat menyebabkan ketidakjelasan dan ketidakpastian.

## Solution Overview

Untuk mengatasi tantangan-tantangan tersebut, kami mengusulkan pengembangan sistem pemantauan pengiriman inovatif berbasis Internet of Things (IoT) yang terintegrasi. Berikut adalah komponen dan solusi yang diusulkan:

1. **Deteksi Keberadaan Kurir:**
   - Menggunakan ESP32 CAM sebagai kamera untuk mendeteksi keberadaan kurir di titik pengambilan dan pengantaran barang.

2. **Akurasi Informasi Berat Barang:**
   - Integrasi HX711 sebagai penimbang berat di dalam wooden box.
   - Memastikan informasi berat barang yang akurat, yang dapat diakses secara real-time melalui jaringan IoT.

3. **Pemberitahuan Suara Terprogram:**
   - Menggunakan speaker untuk memberikan pemberitahuan suara yang dapat diprogram kepada penerima atau pengirim.
   - Pemberitahuan suara ini dapat memberikan informasi tentang status pengiriman, instruksi khusus, atau konfirmasi keberhasilan pengiriman.

4. **Pusat Kontrol dan IoT:**
   - ESP32-WROOM berfungsi sebagai pusat kontrol yang mengoordinasikan semua komponen dalam sistem.
   - Memungkinkan pengelolaan dan monitoring dari jarak jauh melalui platform IoT, meningkatkan efisiensi operasional dan memfasilitasi pengambilan keputusan yang cepat.

  
## Hardware Design

Berikut adalah gambaran desain hardware proyek ini:

<img src="https://github.com/Collosalbyte/Proyek-Akhir-IoT-Sem-5/raw/main/Hardware%20Design/Hardware%20Schematics.png" alt="Hardware Schematics" width="700">

1. **ESP32 CAM:**
   Digunakan sebagai kamera untuk mendeteksi keberadaan kurir.
   
2. **HX711:**
   Berfungsi sebagai penimbang berat di dalam wooden box.
   
3. **Servo:**
   Menggerakkan wooden box sebagai tempat penyimpanan barang.
   
4. **Speaker:**
   Memberikan pemberitahuan suara dari file MP3.
   
5. **ESP32-WROOM:**
   Berfungsi sebagai pusat kontrol dan penghubung IoT.
     
## Network Infrastructure

Proyek SafeDrop berbasis ESP32 yang memanfaatkan infrastruktur jaringan. Proyek ini memungkinkan kendali pintu jarak jauh melalui antarmuka web dan menyediakan fitur tambahan seperti pengukuran berat dan umpan balik audio.ESP32 terhubung ke jaringan Wi-Fi dengan kredensial berikut:

- SSID: jembatan
- Kata Sandi: @kHSMR888


## Software Implementation Details
### `Sequence Diagram`
<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Sequence%20Diagram/Sequence%20Diagram.png" alt="Sequence Diagram" width="700">

### 1. `webServer`

- **Deskripsi:** Fungsi ini berjalan pada inti prosesor kedua dan menangani interaksi klien dengan server web.
- **Invocation:** `xTaskCreatePinnedToCore(webServer, "webServer", 10000, NULL, 0, NULL, 0);`.

### 2. `buzzerButton`

- **Deskripsi:** Fungsi ini berjalan pada inti prosesor kedua dan menangani pembacaan tombol serta mengaktifkan buzzer sesuai kondisi tombol.
- **Invocation:** `xTaskCreatePinnedToCore(buzzerButton, "buzzerButton", 4096, NULL, 2, NULL, 1);`.

### 3. `checkConnections`

- **Deskripsi:** Fungsi ini berjalan pada inti prosesor kedua dan secara berkala memeriksa status WiFi dan DFPlayer, serta melakukan koneksi ulang jika diperlukan.
- **Invocation:** `xTaskCreatePinnedToCore(checkConnections, "checkConnections", 10000, NULL, 1, NULL, 1);`.

### 4. `autoCloseTimerCallback`

- **Deskripsi:** Fungsi panggilan balik dari timer untuk menutup pintu secara otomatis setelah periode tertentu.
- **Invocation:** Dipanggil oleh `autoCloseTimer` setelah periode tertentu.

### 5. `handleRoot`

- **Deskripsi:** Penangan untuk akar ("/") endpoint. Mengirim tanggapan berisi informasi bahwa Server ESP32 berjalan.
- **Invocation:** Dipicu ketika permintaan diterima pada akar endpoint.

### 6. `handleOpenDoor`

- **Deskripsi:** Penangan untuk membuka pintu. Memutar motor servo, memainkan MP3, dan menetapkan timer untuk menutup pintu secara otomatis.
- **Invocation:** Dipicu ketika permintaan diterima pada endpoint "/open".

### 7. `handleCloseDoor`

- **Deskripsi:** Penangan untuk menutup pintu. Memutar motor servo dan menghentikan timer otomatis.
- **Invocation:** Dipicu ketika permintaan diterima pada endpoint "/close".

### 8. `handleButtonStatus`

- **Deskripsi:** Penangan untuk mengirim status tombol (ditekan atau tidak) sebagai tanggapan.
- **Invocation:** Dipicu ketika permintaan diterima pada endpoint "/button-status".

### 9. `handleWeight`

- **Deskripsi:** Penangan untuk mengirim berat dari sensor timbangan sebagai tanggapan.
- **Invocation:** Dipicu ketika permintaan diterima pada endpoint "/weight".

### 10. `buttonBuzzer`

- **Deskripsi:** Membaca status tombol, mengaktifkan buzzer, dan memberikan umpan balik berdasarkan kondisi tombol.
- **Invocation:** Dipanggil dalam loop utama dari tugas `buzzerButton`.

### 11. `getWeight`

- **Deskripsi:** Membaca nilai dari sensor berat HX711 dan mengembalikan hasilnya.
- **Invocation:** Digunakan oleh penangan "/weight" untuk mengirim berat sebagai tanggapan.

### 12. `yellowColor`, `greenColor`, `turnOff`

- **Deskripsi:** Fungsi-fungsi ini mengontrol warna LED RGB untuk indikasi visual.
- **Invocation:** Dipanggil dalam berbagai kondisi untuk mengatur warna LED.

### 13. `autoCloseTimerCallback`

- **Deskripsi:** Fungsi panggilan balik dari timer untuk menutup pintu secara otomatis setelah periode tertentu.
- **Invocation:** Dipanggil ketika timer otomatis selesai.

### 14. `isDFPlayerConnected`

- **Deskripsi:** Memeriksa apakah DFPlayer Mini terhubung atau tidak.
- **Invocation:** Digunakan dalam fungsi `checkConnections` untuk menangani koneksi ulang jika diperlukan.

### 15. `startMP3`

- **Deskripsi:** Menginisiasi dan mengkonfigurasi DFPlayer Mini untuk pemutaran audio.
- **Invocation:** Dipanggil selama inisialisasi untuk memastikan DFPlayer Mini aktif.

### 16. `RTOS & Task Scheduling`

- **Penggunaan `xTaskCreatePinnedToCore`:**
  - Fungsi ini digunakan untuk membuat tugas yang berjalan pada inti prosesor tertentu.
  - Contoh Penggunaan: `xTaskCreatePinnedToCore(webServer, "webServer", 10000, NULL, 0, NULL, 0);`
  - Penjelasan: Membuat tugas `webServer` pada inti prosesor 0 dengan prioritas 0.

- **Dua Tugas menggunakan FreeRTOS Task Scheduler:**
  - `webServer` dan `buzzerButton` dijadwalkan menggunakan FreeRTOS task scheduler.
  - Contoh Penggunaan: `xTaskCreatePinnedToCore(buzzerButton, "buzzerButton", 4096, NULL, 2, NULL, 1);`
  - Penjelasan: Membuat tugas `buzzerButton` pada inti prosesor 1 dengan prioritas 2.

### 17. `Memory Management & Queue`

- **Penggunaan Variabel Global dan Lokal:**
  - Beberapa variabel global dan lokal digunakan, termasuk variabel yang diberi tanda volatile.
  - Contoh Penggunaan: `volatile int globalVariable;`
  - Penjelasan: Variabel `globalVariable` diberi tanda `volatile` untuk pembacaan yang aman.

- **Penggunaan `vTaskDelay`:**
  - Fungsi `vTaskDelay` digunakan untuk membuat keterlambatan dalam tugas.
  - Contoh Penggunaan: `vTaskDelay(1000 / portTICK_PERIOD_MS);`
  - Penjelasan: Membuat keterlambatan selama 1 detik.

### 18. `Mutex & Semaphore`

- **Penggunaan Mutex:**
  - Dua mutex (`doorMutex` dan `coreOneMutex`) digunakan untuk mengamankan bagian kritis kode.
  - Contoh Penggunaan: `xSemaphoreTake(doorMutex, portMAX_DELAY);`
  - Penjelasan: Menggunakan mutex sebelum memasuki bagian kritis kode.

### 19. `Software Timer & Hardware Interrupts`

- **Penggunaan TimerHandle_t dan Fungsi Callback:**
  - Timer perangkat lunak (`autoCloseTimer`) diimplementasikan dengan `TimerHandle_t` dan fungsi callback (`autoCloseTimerCallback`).
  - Contoh Penggunaan: `autoCloseTimer = xTimerCreate("AutoCloseTimer", AUTO_CLOSE_TIMER_PERIOD, pdFALSE, (void *) 0, autoCloseTimerCallback);`
  - Penjelasan: Membuat dan mengaktifkan timer perangkat lunak.

- **Penggunaan Fungsi `digitalRead` untuk Membaca Status Tombol:**
  - Fungsi `digitalRead` digunakan untuk membaca status tombol sebagai contoh penggunaan interrupt.
  - Contoh Penggunaan: `int buttonState = digitalRead(BUTTON_PIN);`
  - Penjelasan: Membaca status tombol pada pin tertentu.

### 20. `Priority Inversion & Multicore Systems`

- **Penggunaan `xTaskCreatePinnedToCore` untuk Multicore Systems:**
  - Fungsi ini digunakan untuk menjadwalkan tugas pada inti prosesor tertentu.
  - Contoh Penggunaan: `xTaskCreatePinnedToCore(webServer, "webServer", 10000, NULL, 0, NULL, 0);`
  - Penjelasan: Menjadwalkan tugas pada inti prosesor tertentu.

## Test Results and Performance Evaluation

Testing dilakukan dengan memeriksa berjalannya fungsionalitas seperti sensor berat HX711, servo pintu kotak, buzzer notifikasi, sambungan Wi-Fi, DFPlayer untuk speaker, dan web server untuk dihubungkan dengan aplikasi.

### Testing Sensor Berat HX711

Testing sensor berat dinilai dari output serial monitor yang menampilkan berat yang terdeteksi oleh sensor. Pada hasil berikut, sensor berhasil mengukur berat benda di atasnya.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/HX711.png" alt="Weight Sensor Output" width="700">

### Testing Servo Pintu Kotak

Testing servo dinilai dari terbuka atau tertutupnya kotak berdasarkan tombol yang ditekan pada aplikasi. Pada hasil berikut, kotak berhasil dibuka dan ditutup sesuai perintah.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/Servo_Open.jpg" alt="Servo Open Box" width="700">

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/Servo_Close.jpg" alt="Servo Close Box" width="700">

### Testing Buzzer

Testing buzzer dinilai dari output serial monitor yang menandakan tombol ditekan, dan akibatnya buzzer menyala. Pada hasil berikut, serial monitor mengindikasikan buzzer berhasil menyala.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/Buzzer.png" alt="Buzzer Serial Monitor Output" width="700">

### Testing Sambungan Wi-Fi

Testing sambungan Wi-Fi dinilai dari keberhasilan sambungan perangkat pada jaringan Wi-Fi beserta IP address yang dialokasikan. Pada hasil berikut, serial monitor menandakan perangkat berhasil tersambung dengan Wi-Fi serta mendapatkan IP address lokalnya.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/Wi-Fi.png" alt="Wi-Fi Connection Success" width="700">

### Testing Speaker Connection

Testing sambungan speaker ini dinilai dari keberhasilan sambungan speaker dengan penyimpanan berupa kartu MicroSD menggunakan DFPlayer. Pada hasil berikut, serial monitor menandakan DFPlayer berhasil tersambung.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/DFPlayer.png" alt="Speaker Connection Success" width="700">

### Testing Sambungan Web Server

Testing sambungan web server ini dinilai dari keberhasilan sambungan perangkat dengan suatu HTTP server yang mengelola koneksi antara perangkat dengan aplikasi. Pada hasil berikut, serial monitor menandakan web server berhasil dimulai.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Test%20Results/WebServer.png" alt="Web Server Start Success" width="700">

### Testing Android Application

Testing integrasi software dan hardware dari sistem melalui aplikasi android, didemonstrasikan dengan menampilkan antarmuka pengguna (UI) dari aplikasi. Pada hasil berikut, aplikasi ditampilkan dengan UI yang intuitif lengkap dengan tombol-tombol fungsionalitasnya.

<img src="https://github.com/Collosalbyte/SafeDrop/blob/main/Application%20Demo/App_Demo.jpg" alt="Application Demo" width="700">

### Performance Evaluation

Berdasarkan testing yang sudah dilakukan, sistem berhasil mencapai fungsionalitas berikut dengan baik:
- Notifikasi kepada penerima melalui buzzer secara real-time.
- Pengukuran berat paket yang diterima.
- Pembukaan dan penutupan kotak paket secara remote.
- Sambungan perangkat pada jaringan Wi-Fi
- Sambungan speaker secara remote melalui DFPlayer
- Sambungan sistem dan aplikasi melalui Web Server

## Conclusion

Proyek SafeDrop memperkenalkan solusi inovatif untuk meningkatkan efisiensi dan keamanan dalam proses pengiriman menggunakan konsep Internet of Things (IoT). Dengan memanfaatkan berbagai sensor dan aktuator, proyek ini memberikan pemantauan real-time terhadap status pengiriman dan kontrol yang lebih baik terhadap akses pintu. Poin-poin kunci yang dapat diambil:

### Deteksi Keberadaan Kurir:

- Kamera ESP32 CAM digunakan untuk mendeteksi keberadaan kurir di titik pengambilan dan pengantaran barang.

### Informasi Berat yang Akurat:

- Integrasi HX711 sebagai sensor berat di dalam wooden box memastikan informasi berat barang yang akurat.

### Pemberitahuan Suara Terprogram:

- Speaker digunakan untuk memberikan pemberitahuan suara yang dapat diprogram kepada penerima atau pengirim, meningkatkan komunikasi dalam proses pengiriman.

### Pusat Kontrol dan IoT:

- ESP32-WROOM berfungsi sebagai pusat kontrol yang mengoordinasikan semua komponen dalam sistem, memungkinkan manajemen jarak jauh melalui platform IoT.

## Future Work

### Implementasi di Cloud:

- Mengintegrasikan proyek dengan platform cloud seperti AWS atau Google Cloud untuk memungkinkan akses dari luar jaringan lokal.

### Optimasi Streaming Kamera:

- Menyempurnakan algoritma streaming kamera untuk memastikan transmisi video yang lancar dan responsif.

### Optimasi Pengurangan Delay:

- Mengoptimalkan sistem agar minim delay, termasuk pengurangan waktu respons dari setiap komponen.

