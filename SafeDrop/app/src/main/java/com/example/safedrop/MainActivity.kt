package com.example.safedrop

import android.app.NotificationManager
import android.os.Build
import android.os.Bundle
import android.widget.Button
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.safedrop.ui.theme.SafeDropTheme
import androidx.appcompat.app.AppCompatActivity
import okhttp3.OkHttpClient
import okhttp3.Request
import java.io.IOException
import java.net.HttpURLConnection
import kotlin.concurrent.thread
import android.os.AsyncTask
import androidx.core.app.NotificationCompat
import java.net.URL
import android.app.NotificationChannel
import android.util.Log
import android.widget.TextView
import kotlinx.coroutines.*
import java.util.concurrent.Executors
import java.util.concurrent.TimeUnit
class MainActivity : AppCompatActivity() {
    private val executorService = Executors.newSingleThreadScheduledExecutor()

    private val client = OkHttpClient()
    private val serverUrl = "http://192.168.1.115/button-status" // Ganti dengan IP ESP32
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Misalnya, ketika tombol diklik, buka pintu
        findViewById<Button>(R.id.openDoorButton).setOnClickListener {
            sendCommandToESP32("http://192.168.1.115/open")
        }

        // Misalnya, ketika tombol lain diklik, tutup pintu
        findViewById<Button>(R.id.closeDoorButton).setOnClickListener {
            sendCommandToESP32("http://192.168.1.115/close")
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val name = getString(R.string.channel_name)
            val descriptionText = getString(R.string.channel_description)
            val importance = NotificationManager.IMPORTANCE_DEFAULT
            val channel = NotificationChannel(CHANNEL_ID, name, importance).apply {
                description = descriptionText
            }
            val notificationManager: NotificationManager =
                getSystemService(NOTIFICATION_SERVICE) as NotificationManager
            notificationManager.createNotificationChannel(channel)
        }
        val btnGetWeight: Button = findViewById(R.id.btnGetWeight)
        val tvWeight: TextView = findViewById(R.id.tvWeight)

        btnGetWeight.setOnClickListener {
            fetchWeight(tvWeight)
        }
        scheduleButtonStatusCheck()
    }
    private fun fetchWeight(tvWeight: TextView) {
        val client = OkHttpClient()
        val url = "http://192.168.1.115/weight" // Ganti dengan IP ESP32 Anda

        val request = Request.Builder()
            .url(url)
            .build()

        client.newCall(request).enqueue(object : okhttp3.Callback {
            override fun onFailure(call: okhttp3.Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: okhttp3.Call, response: okhttp3.Response) {
                if (response.isSuccessful) {
                    val myResponse = response.body?.string()

                    runOnUiThread {
                        tvWeight.text = myResponse ?: "Error"
                    }
                }
            }
        })
    }
    private fun scheduleButtonStatusCheck() {
        val period = 2L // Periksa setiap 5 detik, sesuaikan sesuai kebutuhan
        executorService.scheduleAtFixedRate({ checkButtonStatus() }, 0, period, TimeUnit.SECONDS)
    }
    private fun checkButtonStatus() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val url = URL(serverUrl)
                with(url.openConnection() as HttpURLConnection) {
                    requestMethod = "GET"
                    connectTimeout = 3000 // Timeout setelah 10 detik
                    readTimeout = 3000

                    Log.d("CheckButtonStatus", "Response Code: $responseCode")

                    if (responseCode == HttpURLConnection.HTTP_OK) {
                        inputStream.bufferedReader().use { reader ->
                            val response = reader.readLine()
                            Log.d("CheckButtonStatus", "Response: $response")

                            if (response.isNotEmpty()) {
                                withContext(Dispatchers.Main) {
                                    showNotification("Ding Dong!!", "Ada tamu.")
                                }
                            }
                        }
                    } else {
                        Log.d("CheckButtonStatus", "Response not OK")
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    // Menampilkan log atau error di sini
                }
                //Log.e("CheckButtonStatus", "Error: ${e.message}", e)
            }
        }
    }


    private fun showNotification(title: String, content: String) {
        val builder = NotificationCompat.Builder(this, CHANNEL_ID)
            .setSmallIcon(R.drawable.ic_notification) // Ganti dengan icon yang Anda inginkan
            .setContentTitle(title)
            .setContentText(content)
            .setPriority(NotificationCompat.PRIORITY_DEFAULT)

        val notificationManager: NotificationManager =
            getSystemService(NOTIFICATION_SERVICE) as NotificationManager
        notificationManager.notify(1, builder.build())
    }
    override fun onDestroy() {
        super.onDestroy()
        executorService.shutdownNow() // Memastikan tidak ada task yang tertinggal
    }
    companion object {
        private const val CHANNEL_ID = "channel_id"
    }
    private fun sendCommandToESP32(url: String) {
        thread {
            try {
                val request = Request.Builder()
                    .url(url)
                    .build()

                client.newCall(request).execute().use { response ->
                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    // Handle response here (jika diperlukan)
                }
            } catch (e: Exception) {
                e.printStackTrace()
                // Handle error here (jika diperlukan)
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    SafeDropTheme {
        Greeting("Android")
    }
}