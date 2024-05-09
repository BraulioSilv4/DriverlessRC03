package com.example.a03rcontroller

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import kotlinx.coroutines.CompletableDeferred
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.WebSocket
import okhttp3.WebSocketListener

class ConnectToWebsocket : AppCompatActivity() {
    private lateinit var websocketurl: EditText
    private lateinit var connect: Button

    private val client = OkHttpClient()

    private var isConnected: CompletableDeferred<Boolean>? = null

    private val listener = object : WebSocketListener() {
        override fun onOpen(webSocket: WebSocket, response: okhttp3.Response) {
            println("WebSocket connected!")
            isConnected?.complete(true)
        }

        override fun onMessage(webSocket: WebSocket, text: String) {
            println("Received message: $text")
        }

        override fun onClosing(webSocket: WebSocket, code: Int, reason: String) {
            println("WebSocket closing: $reason")
        }

        override fun onFailure(webSocket: WebSocket, t: Throwable, response: okhttp3.Response?) {
            println("WebSocket failed: ${t.message}")
            isConnected?.completeExceptionally(t)
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_connect_to_websocket)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        websocketurl = findViewById(R.id.websocket_url)
        connect = findViewById(R.id.connect)
        
        connect.setOnClickListener {
            val socketUrl = websocketurl.text.toString()
            if (socketUrl.isEmpty()) {
                Toast.makeText(this, "Please enter a WebSocket URL", Toast.LENGTH_SHORT).show()
            } else {
                isConnected = CompletableDeferred()
                val request = Request.Builder()
                    .url(socketUrl)
                    .build()
                val webSocketConnection = client.newWebSocket(request, listener)
                MainActivity.webSocketConnection = webSocketConnection

                GlobalScope.launch(Dispatchers.Main) {
                    try {
                        if (isConnected?.await() == true) {
                            Toast.makeText(this@ConnectToWebsocket, "Connected to WebSocket", Toast.LENGTH_SHORT).show()
                            val intent = Intent(this@ConnectToWebsocket, MainActivity::class.java)
                            startActivity(intent)
                        }
                    } catch (e: Exception) {
                        Toast.makeText(this@ConnectToWebsocket, "Failed to connect to WebSocket", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        }
    }
}