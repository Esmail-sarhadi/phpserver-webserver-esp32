#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <SPIFFS.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "your wifi name";
const char* password = "your pass";

// Server URL
const char* serverUrl = "http://your-server-name/index.php";

// Pulse Sensor
int PulseSensorPurplePin = 36; // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED = LED_BUILTIN;         // The on-board Arduino LED
int Signal;                    // Holds the incoming raw data. Signal value can range from 0-4095
int Threshold = 1900;          // Determine which Signal to "count as a beat", and which to ignore

// MQ135 Gas Sensor
const int mq135Pin = 34; // Analog input pin for MQ135 sensor

// DHT Sensor
#define DHTPIN 4         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21    // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// GPIO settings for the lamp
#define LAMP_PIN 2

// Create a web server
WebServer server(80);

// Variables to hold sensor data
int heartRate;
int co2;
float humidity;
float temperature;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Initialize sensors
  pinMode(LED, OUTPUT); // Pin that will blink to your heartbeat
  pinMode(LAMP_PIN, OUTPUT);
  digitalWrite(LAMP_PIN, LOW); // Turn off the lamp initially
  dht.begin();

  // Set route for the main page
  server.on("/", handle_OnConnect);
  server.on("/toggle-lamp", handle_ToggleLamp);
  server.on("/read-data", handle_ReadData);
  server.on("/download", handle_Download);
  server.onNotFound(handle_NotFound);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  readSensors();
  sendDataToServer();
}

void readSensors() {
  // Read data from Pulse Sensor
  Signal = analogRead(PulseSensorPurplePin); // Read the PulseSensor's value
  if (Signal > Threshold) {                  // If the signal is above Threshold, then "turn-on" Arduino's on-Board LED
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);                  // Else, the signal must be below Threshold, so "turn-off" this LED
  }
  heartRate = Signal;

  // Read data from MQ135 Sensor
  co2 = analogRead(mq135Pin); // Read analog value from MQ135

  // Read data from DHT Sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) { // Check if WiFi is connected
    HTTPClient http;
    http.begin(serverUrl);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "device=ESP32&temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&co2=" + String(co2) + "&heart_rate=" + String(heartRate);

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void handle_OnConnect() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Sensor Data</title>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f2f2f2; color: #333; text-align: center; padding: 50px; }";
  html += "h1 { color: #4CAF50; }";
  html += ".data { font-size: 1.5em; margin: 20px 0; opacity: 0; animation: fadeIn 2s forwards; }";
  html += ".container { background-color: #fff; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); display: inline-block; padding: 20px 40px; opacity: 0; animation: fadeIn 1s forwards; }";
  html += ".icon { font-size: 2em; margin-right: 10px; }";
  html += ".btn { padding: 10px 20px; font-size: 1em; margin-top: 20px; cursor: pointer; border: none; border-radius: 5px; background-color: #4CAF50; color: white; }";
  html += ".btn:hover { background-color: #45a049; }";
  html += "@keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }";
  html += "</style></head>";
  html += "<body><div class='container'>";
  html += "<h1><i class='fas fa-microchip icon'></i>Sensor Data</h1>";
  html += "<p class='data' id='temp'><i class='fas fa-thermometer-half icon'></i>Temperature: -- °C</p>";
  html += "<p class='data' id='humidity'><i class='fas fa-tint icon'></i>Humidity: -- %</p>";
  html += "<p class='data' id='co2'><i class='fas fa-wind icon'></i>CO2: --</p>";
  html += "<p class='data' id='heartRate'><i class='fas fa-heartbeat icon'></i>Heart Rate: --</p>";
  html += "<p class='data'><i class='fas fa-wifi icon'></i>WiFi Status: Connected</p>";
  html += "<p class='data'><i class='fas fa-server icon'></i>Web Server Running</p>";
  html += "<button class='btn' onclick='toggleLamp()'><i class='fas fa-lightbulb icon'></i>Toggle Lamp</button>";
  html += "<button class='btn' onclick='downloadFile()'><i class='fas fa-download icon'></i>Download Data</button>";
  html += "</div>";
  html += "<script>";
  html += "function toggleLamp() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/toggle-lamp', true);";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      alert('Lamp state toggled.');";
  html += "    }";
  html += "  };";
  html += "  xhr.send();";
  html += "}";
  html += "function downloadFile() {";
  html += "  window.location.href = '/download';";
  html += "}";
  html += "function updateColors(temp, humidity, co2, heartRate) {";
  html += "  var tempElement = document.getElementById('temp');";
  html += "  var humidityElement = document.getElementById('humidity');";
  html += "  var co2Element = document.getElementById('co2');";
  html += "  var heartRateElement = document.getElementById('heartRate');";
  html += "  if (temp > 24) { tempElement.style.color = 'red'; }";
  html += "  else if (temp < 21) { tempElement.style.color = 'blue'; }";
  html += "  else { tempElement.style.color = 'green'; }";
  html += "  if (humidity > 50) { humidityElement.style.color = 'red'; }";
  html += "  else if (humidity < 40) { humidityElement.style.color = 'blue'; }";
  html += "  else { humidityElement.style.color = 'green'; }";
  html += "  if (co2 > 400) { co2Element.style.color = 'red'; }";
  html += "  else { co2Element.style.color = 'green'; }";
  html += "  if (heartRate > 1000) { heartRateElement.style.color = 'red'; }";
  html += "  else { heartRateElement.style.color = 'green'; }";
  html += "}";
  html += "setInterval(function() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/read-data', true);";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      var data = JSON.parse(xhr.responseText);";
  html += "      document.getElementById('temp').innerHTML = '<i class=\"fas fa-thermometer-half icon\"></i>Temperature: ' + data.temperature + ' °C';";
  html += "      document.getElementById('humidity').innerHTML = '<i class=\"fas fa-tint icon\"></i>Humidity: ' + data.humidity + ' %';";
  html += "      document.getElementById('co2').innerHTML = '<i class=\"fas fa-wind icon\"></i>CO2: ' + data.co2;"; 
  html += "      document.getElementById('heartRate').innerHTML = '<i class=\"fas fa-heartbeat icon\"></i>Heart Rate: ' + data.heartRate;"; 
  html += "      updateColors(data.temperature, data.humidity, data.co2, data.heartRate);"; 
  html += "    }"; 
  html += "  };"; 
  html += "  xhr.send();"; 
  html += "}, 1000);"; // Update every second 
  html += "</script></body></html>";

  server.send(200, "text/html", html);
}

void handle_ToggleLamp() {
  int lampState = digitalRead(LAMP_PIN);
  digitalWrite(LAMP_PIN, !lampState); // Toggle lamp state
  server.send(200, "text/plain", "Lamp state toggled.");
  delay(3000); // Keep the lamp on for 3 seconds
  digitalWrite(LAMP_PIN, LOW); // Turn off the lamp
}

void handle_ReadData() {
  readSensors();

  // Save data to a file
  File file = SPIFFS.open("/data.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  file.printf("Temperature: %.2f °C, Humidity: %.2f %%, CO2: %d, Heart Rate: %d\n", temperature, humidity, co2, heartRate);
  file.close();

  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"co2\":" + String(co2) + ",";
  json += "\"heartRate\":" + String(heartRate);
  json += "}";
  server.send(200, "application/json", json);
}

void handle_Download() {
  File file = SPIFFS.open("/data.txt", FILE_READ);
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }

  server.streamFile(file, "text/plain");
  file.close();
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
