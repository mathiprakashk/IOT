# Overweight Detection System for Vehicles

This project involves a smart vehicle weighing system that uses an **HX711 weight sensor**, an **IR sensor**, a **buzzer**, an **LCD display**, and an **ESP8266 WiFi module**. It identifies overweight vehicles, displays relevant information, and provides a fine notification.

## Features

1. **Vehicle Weight Measurement**: Measures the weight of vehicles using an HX711 load cell module.
2. **Overweight Detection**: Alerts when the vehicle exceeds a predefined weight threshold.
3. **Driver and Vehicle Information**: Displays driver and vehicle details for registered vehicles.
4. **Web Dashboard**: Hosts a web interface to monitor weight and view driver details in real-time.
5. **Fine Notification**: Displays a fine notification for overweight vehicles.
6. **WiFi Connectivity**: Uses ESP8266 for wireless communication.
7. **Buzzer Alert**: Activates a buzzer for overweight vehicles.

## Components Used

1. **HX711 Load Cell Module**: For weight measurement.
2. **IR Sensor**: Detects vehicle presence.
3. **Buzzer**: Provides an audible alert for overweight vehicles.
4. **ESP8266 WiFi Module**: Hosts a web server and connects to WiFi.
5. **LCD Display (I2C)**: Displays weight and system messages.
6. **Power Supply**: Powers the components.

## Prerequisites

1. Arduino IDE with the following libraries installed:
   - [HX711 Library](https://github.com/bogde/HX711)
   - [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
   - ESP8266 WiFi libraries (bundled with the ESP8266 core)
2. **ESP8266 Core** for Arduino IDE. [Setup instructions](https://github.com/esp8266/Arduino).

## Circuit Diagram

- Connect the HX711 load cell to the ESP8266 as follows:
  - **DOUT → GPIO12 (D6)**
  - **SCK → GPIO13 (D7)**
- IR sensor to GPIO2 (D4).
- Buzzer to GPIO3 (D3).
- LCD I2C to the corresponding SDA and SCL pins of the ESP8266.
- Power the setup with a 5V power supply.

## Setup Instructions

1. Clone this repository or download the ZIP file.
2. Open the code in Arduino IDE.
3. Modify the following lines in the code to match your WiFi credentials:
   ```cpp
   const char *ssid = "your-SSID";
   const char *password = "your-password";
   ```
4. Upload the code to the ESP8266 board.

## How It Works

1. **Initialization**: The system initializes the sensors, scale, LCD, and WiFi connection.
2. **Vehicle Detection**: When the IR sensor detects a vehicle, it measures the weight using the HX711 module.
3. **Weight Display**: Displays the weight on the LCD and updates it on the web interface.
4. **Overweight Alert**: If the weight exceeds the threshold, it activates the buzzer and displays a fine notification.
5. **Web Dashboard**: The web interface displays:
   - Real-time weight
   - Driver and vehicle information
   - Overweight messages (if applicable).

### Web Interface

The web interface provides:
- Real-time weight updates.
- A form to enter the vehicle number, which fetches the corresponding driver's name from a predefined list.

### Fine Logic

- If the weight exceeds `250,000 kg`, the system:
  - Activates the buzzer.
  - Displays "Overweight!" and a fine message on the LCD and the web interface.

## Example Web Dashboard

- **URL**: `http://<ESP8266_IP_ADDRESS>`
- Web Interface Features:
  - Real-time weight updates.
  - Input vehicle number to fetch driver details.
  - Fine notification for overweight vehicles.

## Demo

![System Setup](https://via.placeholder.com/600x300)  
*A sample setup showing the LCD, buzzer, and load cell connected to the ESP8266.*
**pictures:**
Project Workflow:
![project workflow](https://github.com/user-attachments/assets/f8213aea-6184-443a-9b41-974127b4bf48)

Hardware:
1 . ![kit full setup](https://github.com/user-attachments/assets/9cb7aa75-6214-4448-9ef4-db120767d953)
2 . ![overweight lcd](https://github.com/user-attachments/assets/1a5756dd-416b-4cdb-98f8-47f0ec9b5cb1)
Web page
3 . ![web1](https://github.com/user-attachments/assets/50dedaa2-203e-4dd7-bc35-12c722fe17e8)
4 . ![web2](https://github.com/user-attachments/assets/0e7595b6-3854-45e0-877d-75134479e519)






## License

This project is licensed under the MIT License. You are free to modify, use, and distribute the code with attribution.
