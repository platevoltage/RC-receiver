#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>
#include <esp_wifi.h>
#include <ESP32Servo.h>

#define LED_PIN 8
#define LED_COUNT 1
CRGB leds[LED_COUNT];

Servo steering;  
Servo drive;  
// int steeringPos = 0;  
int steeringPin = 16;
int drivePin = 18;

// Structure to hold incoming data
typedef struct struct_message {
    int steering;
    int drive;
} struct_message;

// MAC address of the sender (transmitter)
uint8_t senderAddress[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}; // Replace with the MAC address of your sender

// Callback function that will be executed when data is received

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
    Serial.print("Received data from: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(mac_addr[i], HEX);
        if (i < 5) Serial.print(":");
    }
    Serial.println();

    // Parse incoming data into struct_message
    struct_message *msg = (struct_message *)data;

    // Print received data
    // Serial.print("Text: ");
    // Serial.println(msg->text);
    Serial.print("Value: ");
    Serial.println(msg->steering);
    Serial.println();
    // leds[0] = msg->value;
    steering.write(msg->steering);
    drive.write(msg->drive);
    // FastLED.show();
}

void setup() {

    Serial.begin(115200);

    // Initialize Serial Monitor
    Serial.println("Receiver setup");

    // Set device as WiFi Station
    WiFi.mode(WIFI_STA);
    esp_wifi_set_max_tx_power(84);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callback function
    esp_now_register_recv_cb(OnDataRecv);

    // Add sender (transmitter) as peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, senderAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(64);
    delay(5000);
    leds[0] = 0xFF0000;
    FastLED.show();

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	steering.setPeriodHertz(50);    // standard 50 hz servo
	steering.attach(steeringPin, 1000, 2000);
    drive.setPeriodHertz(50);    // standard 50 hz servo
	drive.attach(drivePin, 1000, 2000);
}

// int color = 0;
void loop() {
	// for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
	// for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
}
