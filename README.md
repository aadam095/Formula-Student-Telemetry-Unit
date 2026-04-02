# Formula Student Telemetry Unit

A low-cost 6-DOF vehicle telemetry unit built for Formula Student, replacing a ~£300 commercial unit for approximately £28.50.

## Hardware
- ESP32 DevKit V1 (dual-core microcontroller)
- MPU-6050 IMU (6-axis accelerometer + gyroscope)
- MicroSD card module
- Custom 3D printed enclosure (designed in Siemens NX)

## Features
- Records 6-axis IMU data (acceleration + gyroscope) to CSV at 20Hz
- Append mode logging — data survives power cycles
- Engine vibration filtered via 21Hz bandwidth filter
- 8G accelerometer range optimised for Formula Student cornering loads
- CSV output compatible with Excel and MATLAB for post-session analysis

## File Structure
- `FS_Telemetry.ino` — Arduino firmware for ESP32
- `FS_Telemetry_Case_V1.stl` — 3D printable enclosure (case)
- `FS_Telemetry_Lid_V1.stl` — 3D printable enclosure (lid)
- `FS_Telemetry_Case_V1` — Siemens NX source CAD file

## Setup
1. Install Arduino IDE and add ESP32 board support
2. Install libraries: Adafruit MPU6050, Adafruit Sensor
3. Connect MPU6050 via I2C, SD card module to SPI (CS = Pin 5)
4. Flash `FS_Telemetry.ino` to ESP32
5. Insert SD card — unit begins logging automatically on power-up

## Data Output
CSV format: `Time_ms, Accel_X, Accel_Y, Accel_Z, Gyro_X, Gyro_Y, Gyro_Z`