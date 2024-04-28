#include <SoftwareSerial.h> // Include the SoftwareSerial library for creating serial communication with the FONA module.
#include "Adafruit_FONA.h" // Include the Adafruit_FONA library for controlling the FONA module.

#define FONA_RX 2 // Define the pin number used for receiving data from the FONA module.
#define FONA_TX 3 // Define the pin number used for transmitting data to the FONA module.
#define FONA_RST 4 // Define the pin number used for resetting the FONA module.

SoftwareSerial fonaSS(FONA_RX, FONA_TX); // Create a SoftwareSerial object named 'fonaSS' for communication with the FONA module.
Adafruit_FONA fona = Adafruit_FONA(FONA_RST); // Create an Adafruit_FONA object named 'fona' for controlling the FONA module.

char gasalert[141] = "Gas Leakage Detected"; // Define a character array to store the gas leakage alert message.
int gas_sensor_pin = A0; // Define the analog pin number connected to the gas sensor.

void setup() {
  pinMode(gas_sensor_pin, INPUT); // Set the gas sensor pin as an input.
  Serial.begin(115200); // Initialize serial communication with a baud rate of 115200.
  Serial.println(F("Initializing.... (May take 3 seconds)")); // Print an initialization message.
  delay(5000); // Wait for 5 seconds.
  fonaSS.begin(115200); // Initialize software serial communication with the FONA module at a baud rate of 115200.
  
  if (!fona.begin(fonaSS)) { // Check if communication with the FONA module is successful.
    Serial.println(F("Couldn't find FONA")); // Print an error message if FONA module is not found.
    while (1); // Enter an infinite loop if FONA module is not found.
  }
  
  fona.print("AT+CSMP=17,167,0,0\r"); // Set the SMS text mode parameters.
  Serial.println(F("FONA is OK")); // Print a success message if FONA module is initialized successfully.
}

void loop() {
  int gas_value = analogRead(gas_sensor_pin); // Read the analog value from the gas sensor.
  Serial.print("GasValue: "); // Print the label for gas value.
  Serial.println(gas_value); // Print the gas value.
  
  if (gas_value > 530 || gas_value > 250) { // Check if gas value exceeds certain thresholds.
    Serial.println("Gas Leakage Detected"); // Print a gas leakage alert message.
    make_multi_call(); // Call the function to make multiple calls.
    send_multi_sms(); // Call the function to send multiple SMS alerts.
  }
}

void send_multi_sms() {
  if (PHONE_1 != "") { // Check if PHONE_1 number is not empty.
    Serial.print("Phone 1: "); // Print the label for PHONE_1 number.
    fona.sendSMS(PHONE_1, gasalert); // Send an SMS alert to PHONE_1 with the gas leakage message.
    delay(10000); // Wait for 10 seconds.
  }
}

void make_multi_call() {
  if (PHONE_1 != "") { // Check if PHONE_1 number is not empty.
    Serial.print("Phone 1: "); // Print the label for PHONE_1 number.
    make_call(PHONE_1); // Call the function to make a call to PHONE_1.
    delay(20000); // Wait for 20 seconds.
  }
}

void make_call(String phone) {
  Serial.println("calling...."); // Print a message indicating the call is being made.
  fona.println("ATD" + phone + ";"); // Initiate a call to the specified phone number.
  delay(20000); // Wait for 20 seconds (call duration).
  fona.println("ATH"); // Hang up the call.
  delay(1000); // Wait for 1 second.
}
