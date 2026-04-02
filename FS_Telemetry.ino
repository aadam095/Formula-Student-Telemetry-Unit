#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// --- CONFIGURATION ---
// The Chip Select (CS) pin for the SD card module. 
// On the ESP32 DevKit V1, this is usually Pin 5.
#define SD_CS 5 

Adafruit_MPU6050 mpu;

void setup(void) {
  // 1. Initialize Serial Monitor (for debugging on your laptop)
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for console to open

  Serial.println("Formula Student Telemetry Unit - Booting...");

  // 2. Initialize SD Card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("FAILED! Check your wiring.");
    return; // Stop here if SD fails
  }
  Serial.println("SD Card OK.");

  // 3. Create the CSV Header
  // We open the file in APPEND mode to add new data
  File file = SD.open("/telemetry.csv", FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  // Write the column names so Excel knows what the numbers mean
  if(file.size() == 0) {
    file.println("Time_ms,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z");
  }
  file.close();

  // 4. Initialize MPU6050 Sensor
  Serial.print("Initializing MPU6050 sensor...");
  if (!mpu.begin()) {
    Serial.println("FAILED! Sensor not connected.");
    while (1) { delay(10); } // Stop here if sensor fails
  }
  Serial.println("Sensor OK.");

  // 5. Sensor Tuning (Formula Student Specs)
  // Range: 8G is perfect for cars (cornering usually peaks at 1.5-2.0G)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
  // Gyro: 500 deg/s is enough for spin detection
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  
  // Filter: 21Hz bandwidth removes engine vibration noise
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("System Ready. Recording...");
  delay(100);
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Open the file to save this batch of data
  File file = SD.open("/telemetry.csv", FILE_APPEND);
  if(file){
    // Write the timestamp (Milliseconds since start)
    file.print(millis()); 
    file.print(",");
    
    // Write Accelerometer Data (m/s^2)
    file.print(a.acceleration.x); 
    file.print(",");
    file.print(a.acceleration.y); 
    file.print(",");
    file.print(a.acceleration.z); 
    file.print(",");
    
    // Write Gyroscope Data (rad/s)
    file.print(g.gyro.x); 
    file.print(",");
    file.print(g.gyro.y); 
    file.print(",");
    file.println(g.gyro.z); // println adds a new line for the next row
    
    file.close(); // Save and close immediately to prevent data loss
    
    // Debug to Serial Monitor (Optional - allows you to see it working)
    Serial.print("Recording... X: ");
    Serial.print(a.acceleration.x);
    Serial.println(" m/s^2");
  } else {
    Serial.println("Error writing to file");
  }

  // Sampling Rate
  // delay(50) = approx 20 readings per second (20Hz). 
  // Good balance between data detail and SD card speed.
  delay(50);
}