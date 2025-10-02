#include <Arduino.h>
#include <ESP8266WiFi.h> // if error here,
#include <espnow.h>      // ensure [ENV: ESP01] is selected

// IMPORTANT: For deep sleep to work on an ESP-01, you must
// connect the RST pin to the GPIO16 pin (often labeled XPD_DCDC).
// Most ESP-01 modules don't have GPIO16 exposed, so this feature might not be usable.
// If you cannot use deep sleep, remove the ESP.deepSleep() line and use delay() in the loop.

// Set the sleep time in microseconds. 30 minutes = 1800 seconds
#define SLEEP_DURATION 1800e6 // 30 minutes
// #define SLEEP_DURATION 10e6   // Use 10 seconds for testing

// TODO: Define the analog pin for the soil sensor
#define SOIL_SENSOR_PIN A0

// This structure must be identical on both the sender and receiver
typedef struct struct_message
{
    float soil_moisture;
    float battery_voltage; // Placeholder for now
} struct_message;

// Create a structured object to hold the readings
struct_message sensorReadings;

// MAC Address of the receiver board
uint8_t receiverMac[] = {0xA0, 0x85, 0xE3, 0xE1, 0x2E, 0x70};

// Callback function that is executed when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("Send status: ");
    if (sendStatus == 0)
    {
        Serial.println("Success");
    }
    else
    {
        Serial.println("Failed");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(100); // Allow serial to stabilize
    Serial.println("\n--- Soil Sensor Node ---");

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        ESP.deepSleep(SLEEP_DURATION);
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

    // --- Read Sensor and Send Data ---
    // The ESP-01's ADC reads from 0-1V. Make sure your sensor output matches this.
    // If your sensor outputs up to 3.3V, you'll need a voltage divider.
    int sensorValue = analogRead(SOIL_SENSOR_PIN);
    Serial.printf("Raw sensor value: %d\n", sensorValue);

    // Convert the raw value to a percentage (0-1023 -> 0-100%).
    // You will need to calibrate this mapping for your specific sensor.
    // For example, 750 might be "dry" (0%) and 350 might be "wet" (100%).
    sensorReadings.soil_moisture = map(sensorValue, 750, 350, 0, 100);
    sensorReadings.battery_voltage = 3.7; // Placeholder value

    // Send the structured data
    esp_now_send(receiverMac, (uint8_t *)&sensorReadings, sizeof(sensorReadings));

    Serial.printf("Sending Moisture: %.2f %%\n", sensorReadings.soil_moisture);

    // Wait a moment for the data to be sent before sleeping
    delay(250);

    // --- Go to Sleep ---
    Serial.printf("Going to sleep for %llu seconds...\n", (uint64_t)(SLEEP_DURATION / 1e6));
    ESP.deepSleep(SLEEP_DURATION);
}

void loop()
{
    // This part is never reached because the device goes to sleep in setup()
}