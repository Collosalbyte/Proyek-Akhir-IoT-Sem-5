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

![Diagram Blok Hardware](link_ke_gambar_blok_hardware.png)

1. **ESP32 CAM:**
   - Digunakan sebagai kamera untuk mendeteksi keberadaan kurir.
   
2. **HX711:**
   - Berfungsi sebagai penimbang berat di dalam wooden box.
   
3. **Servo:**
   - Menggerakkan wooden box sebagai tempat penyimpanan barang.
   
4. **Speaker:**
   - Memberikan pemberitahuan suara dari file MP3.
   
5. **ESP32-WROOM:**
   - Berfungsi sebagai pusat kontrol dan penghubung IoT.

## Network Infrastructure

(belum)

## Software Implementation Details

(belum)

## Test Results and Performance Evaluation

(belum)

## Conclusion and Future Work

(belum)


