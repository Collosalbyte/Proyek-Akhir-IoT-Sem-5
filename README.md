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

## Test Results and Performance Evaluation

(belum)

## Conclusion and Future Work

(belum)


