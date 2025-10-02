#include <Arduino.h>
#include <WiFi.h>    // if error here,
#include <esp_now.h> // ensure [ENV: ESP32] is selected

// TODO: Include libraries for your specific hardware
// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <TFT_eSPI.h>

// TODO: Define the GPIO pins you are using
#define RELAY_PIN 1
#define BUZZER_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT22

// This structure must be identical on both the sender and receiver
typedef struct struct_message
{
  float soil_moisture;
  float battery_voltage;
} struct_message;

// Create a structured object to hold the incoming data
struct_message incomingReadings;

// Flag to indicate when new data is received
volatile bool newData = false;

// Callback function that is executed when ESP-NOW data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  // Copy the incoming data into our structured object
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

  // Print the MAC address of the sender
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.printf("Received data from: %s\n", macStr);

  // Set the flag to true to process the data in the main loop
  newData = true;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("--- Green Guardian Receiver ---");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Print this board's MAC address
  Serial.print("My MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the callback function to be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  // TODO: Set up your hardware
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with the pump off
                                // Initialize DHT sensor, LCD display, etc.
}

void loop()
{
  // Check if new data has been received
  if (newData)
  {
    newData = false; // Reset the flag

    // Process the new readings
    Serial.printf("Soil Moisture: %.2f %%\n", incomingReadings.soil_moisture);
    Serial.printf("Sensor Battery: %.2f V\n\n", incomingReadings.battery_voltage);

    // --- Your Custom Logic Goes Here ---

    // TODO: Update your LCD display with the new values
    // tft.println(incomingReadings.soil_moisture);

    // Example: Simple watering logic
    if (incomingReadings.soil_moisture < 35.0)
    {
      Serial.println("Soil is dry. Turning on the pump for 5 seconds.");
      digitalWrite(RELAY_PIN, HIGH); // Turn pump on
      delay(5000);                   // Run for 5 seconds
      digitalWrite(RELAY_PIN, LOW);  // Turn pump off
    }
    else
    {
      Serial.println("Soil moisture is sufficient. Pump remains off.");
      digitalWrite(RELAY_PIN, LOW);
    }
  }

  // TODO: Read local sensors (like the DHT22) periodically
  // You can do this every few seconds, independent of receiving ESP-NOW data
  delay(100);
}