#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "project12345";
const char *password = "project12345";

const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
const int IR_SENSOR_PIN = 2;
const int BUZZER_PIN = 3;

const float OVERWEIGHT_THRESHOLD = 250000.0;

HX711 scale;

LiquidCrystal_I2C lcd(0x27, 16, 2);

ESP8266WebServer server(80);

String vehicleNumber = "";
String driverName = "";

void setup()
{
    Serial.begin(115200);
    Serial.println("HX711 Demo");
    Serial.println("Initializing the scale");

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    lcd.begin();
    lcd.backlight();

    lcd.print("Initializing...");
    delay(1000);
    lcd.clear();

    lcd.print("Weight (kg):");

    scale.set_scale();
    scale.tare();

    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, []() {
        String page = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Scale Dashboard</title><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'></head><body><div class='container'><h1 class='mt-5'>Scale Dashboard</h1><div class='row'><div class='col-md-6'><p>Weight (kg): <span id='weight'></span></p><p id='message'></p><form id='vehicleForm'><div class='form-group'><label for='vehicleNumber'>Enter Vehicle Number:</label><input type='text' class='form-control' id='vehicleNumber' oninput='fetchOwnerName()'></div><div class='form-group'><label for='driverName'>Driver Name:</label><span id='driverName'></span></div><button type='button' class='btn btn-primary' onclick='window.print();'>Print</button></form></div></div></div><script>const owners = [{ driver: 'Mathi prakash.K', vehicleNumber: 'TN 38 N 6789' },{ driver: 'Madesh N', vehicleNumber: 'TN 38 M 6564' },{ driver: 'NANDHITHA N', vehicleNumber: 'TN 38 H 3456' }];setInterval(function () {fetch('/weight').then(response => response.json()).then(data => document.getElementById('weight').textContent = data.weight);}, 1000);setInterval(function () {fetch('/message').then(response => response.text()).then(data => {document.getElementById('message').textContent = data;if (data === 'Overweight! Fine bill printed.') {document.body.style.backgroundColor = 'white';}});}, 1000);function fetchOwnerName() {var vehicle = document.getElementById('vehicleNumber').value;var owner = owners.find(o => o.vehicleNumber === vehicle);if (owner) {document.getElementById('driverName').textContent = 'Driver Name: ' + owner.driver + ' | Vehicle Number: ' + owner.vehicleNumber;} else {document.getElementById('driverName').textContent = 'Driver not found';}}</script></body></html>";
        server.send(200, "text/html", page);
    });

    server.on("/weight", HTTP_GET, []() {
        float reading = scale.get_units();
        server.send(200, "application/json", "{\"weight\":" + String(reading, 1) + "}");
    });

    server.on("/message", HTTP_GET, []() {
        float weight = scale.get_units();
        if (weight > OVERWEIGHT_THRESHOLD)
        {
            digitalWrite(BUZZER_PIN, HIGH);
            server.send(200, "text/plain", "Overweight! Fine is 100RS.");
        }
        else
        {
            digitalWrite(BUZZER_PIN, LOW);
            server.send(200, "text/plain", "");
        }
    });
    server.on("/vehicle", HTTP_GET, []() {
        server.send(400, "text/plain", "Invalid vehicle number");
    });

    server.on("/vehicle/", HTTP_GET, []() {
        String vehicle = server.arg(0);
        if (vehicle == "TN 38 N 6789")
        {
            server.send(200, "text/plain", "Mathi prakash.K");
        }
        else if (vehicle == "TN 38 M 6564")
        {
            server.send(200, "text/plain", "Madesh N");
        }
        else if (vehicle == "TN 38 H 3456")
        {
            server.send(200, "text/plain", "NANDHITHA N");
        }
        else
        {
            server.send(404, "text/plain", "Vehicle number not found");
        }
    });

    server.begin();
}

void loop()
{
    server.handleClient();
    bool irBlocked = digitalRead(IR_SENSOR_PIN) == HIGH;

    if (irBlocked)
    {
        float reading = scale.get_units();
        
        Serial.print("Reading:\t");
        Serial.println(reading, 1);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(reading, 1);

        if (reading > OVERWEIGHT_THRESHOLD)
        {
            digitalWrite(BUZZER_PIN, HIGH);
            server.send(200, "text/plain", "Overweight! Fine bill printed.");
            lcd.setCursor(0, 0);
            lcd.print("Overweight!");
        }
        else
        {
            digitalWrite(BUZZER_PIN, LOW);
            server.send(200, "text/plain", "");
            lcd.setCursor(0, 0);
            lcd.print("            ");
        }
    }

    delay(100);
}