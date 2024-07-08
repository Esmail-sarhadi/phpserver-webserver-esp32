# IoT Multi-Sensor Monitor with ESP32

This project is an Internet of Things (IoT) application that uses an ESP32 microcontroller to monitor various environmental parameters. It collects data from a DHT21 sensor (temperature and humidity), a Pulse sensor, and an MQ135 gas sensor (CO2 levels). The data is displayed on a web page hosted on the ESP32, where users can also toggle a connected lamp and download recorded data.

## Features

- **WiFi Connectivity**: Connects to a specified WiFi network.
- **Multiple Sensors**: Reads data from DHT21, Pulse sensor, and MQ135 sensor.
- **Web Server**: Hosts a web page to display sensor data and control a lamp.
- **SPIFFS**: Stores data locally on the ESP32 filesystem.
- **JSON**: Uses JSON for data interchange between the ESP32 and the web interface.

## Hardware Required

- ESP32 microcontroller
- DHT21 temperature and humidity sensor
- Pulse sensor
- MQ135 gas sensor
- LED lamp (optional)
- Resistors, breadboard, and jumper wires

## Software Required

- Arduino IDE with ESP32 support
- ArduinoJson library
- DHT sensor library
- WiFi library
- WebServer library
- SPIFFS library
- HTTPClient library

## Circuit Diagram

Connect the sensors to the ESP32 as follows:

### DHT21 Sensor
- **VCC** to 3.3V
- **GND** to GND
- **DATA** to GPIO4

### Pulse Sensor
- **Purple wire** to GPIO36

### MQ135 Sensor
- **Analog output** to GPIO34

### Lamp
- **Control pin** to GPIO2 (with appropriate resistor)

## Code Explanation

1. **Library Inclusions**: Includes necessary libraries for WiFi, web server, DHT sensor, SPIFFS, and JSON.
2. **WiFi Configuration**: Sets up WiFi credentials for network connection.
3. **Sensor Initialization**: Initializes the DHT21, Pulse sensor, and MQ135 sensor.
4. **Web Server Routes**: Defines routes for the main page, toggling the lamp, reading sensor data, and downloading data.
5. **Main Page**: Serves an HTML page that displays sensor data and provides buttons for toggling the lamp and downloading data.
6. **Data Handling**: Reads sensor data, stores it in SPIFFS, and sends it to a remote server.

## Setup and Installation

1. **Install Arduino IDE**: Make sure you have the Arduino IDE installed on your computer.
2. **Install ESP32 Board in Arduino IDE**:
   - Go to `File` > `Preferences`.
   - Add `https://dl.espressif.com/dl/package_esp32_index.json` to the `Additional Board Manager URLs`.
   - Open the `Boards Manager` from `Tools` > `Board` and install the `esp32` package.
3. **Install Required Libraries**:
   - Go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Install the following libraries: `ArduinoJson`, `DHT sensor library`, `WiFi`, `WebServer`, `SPIFFS`, `HTTPClient`.
4. **Connect ESP32 to Your Computer**: Use a USB cable to connect the ESP32 to your computer.
5. **Upload the Code**: Open the provided code in Arduino IDE and upload it to the ESP32.

## Running the Project

1. **Power the ESP32**: After uploading the code, power the ESP32 using the USB connection or an external power source.
2. **Connect to WiFi**: The ESP32 will attempt to connect to the specified WiFi network.
3. **Access the Web Interface**:
   - Open a web browser on a device connected to the same WiFi network.
   - Enter the IP address of the ESP32 displayed in the Serial Monitor.
4. **Monitor Data**: View temperature, humidity, CO2 levels, and heart rate data, toggle the lamp, and download recorded data from the web interface.

## Contribution

Feel free to fork this repository and contribute by submitting pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
