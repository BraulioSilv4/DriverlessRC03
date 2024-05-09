package com.example.a03rcontroller

import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.SeekBar
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import okhttp3.WebSocket
import okio.ByteString

class MainActivity : AppCompatActivity() {

    companion object {
        var webSocketConnection: WebSocket? = null
    }

    private lateinit var turnBar: SeekBar
    private lateinit var speedBar: SeekBar
    private lateinit var autoMode: SwitchCompat

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        setupWindowInsets()

        turnBar = findViewById(R.id.turnBar)
        speedBar = findViewById(R.id.speedBar)
        autoMode = findViewById(R.id.mode)

        turnBar.progress = 50

        turnBar.setOnSeekBarChangeListener(createSeekBarChangeListener{
            Log.e("Turn", turnBar.progress.toString())
            val value = turnBar.progress.toByte()
            val byteString = ByteString.of(0b00000001.toByte(), value)
            webSocketConnection?.send(byteString)

        })
        speedBar.setOnSeekBarChangeListener(createSeekBarChangeListener {
            Log.e("Speed", speedBar.progress.toString())
            val value = speedBar.progress.toByte()
            val byteString = ByteString.of(0b00000000.toByte(), value)
            webSocketConnection?.send(byteString)
        })
        autoMode.setOnCheckedChangeListener { _, isChecked ->
            val value = if (isChecked) 1.toByte() else 0.toByte()
            val byteString = ByteString.of(0b00000010.toByte(), value)
            webSocketConnection?.send(byteString)

            speedBar.isEnabled = !isChecked
            turnBar.isEnabled = !isChecked
        }
    }

    private fun setupWindowInsets() {
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
    }

    private fun createSeekBarChangeListener(onProgressChange: () -> Unit) = object: SeekBar.OnSeekBarChangeListener {
        override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
            onProgressChange()
        }

        override fun onStartTrackingTouch(seekBar: SeekBar?) {}

        override fun onStopTrackingTouch(seekBar: SeekBar?) {}
    }
}