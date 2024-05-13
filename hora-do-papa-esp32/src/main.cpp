//
// Project by Henrique Silva Simplicio
// 

#include <Arduino.h>
#include <A4988.h>
#include <uRTCLib.h>

// ===================================
// Global utils
// ===================================

// Create stepper motor
#define MOTOR_STEPS 200
#define DIR 4
#define STP 18
#define MS1 32
#define MS2 33
#define MS3 19

A4988 stepper(MOTOR_STEPS, DIR, STP, MS1, MS2, MS3);

// Create RTC
uRTCLib rtc;

// Define useful types
typedef struct {
  int hour;
  int minute;
} time_t;

typedef bool repeat_t[7];

typedef struct {
  time_t time;
  repeat_t repeat;
} activation_t;

// Change the number of activations
activation_t activation[2];

// Define buzzer pin
#define BUZZER 25

// ===================================
// Functions declarations
// ===================================

void printRTC();

// ===================================
// Setup
// ===================================

void setup() {
  Serial.begin(115200);
  delay(3); // wait for console opening

  stepper.begin(3, 1);

  URTCLIB_WIRE.begin();

  // To adjust RTC initial time if needed
  // Only run it once
  // rtc.set(0, 34, 17, 7, 22, 4, 23);  // (second, minute, hour, dayOfWeek, dayOfMonth, month, year)

  // Initialize times
  activation[0] = {
    .time {7, 0},
    .repeat {true, true, true, true, true, true, true}
  };

  activation[1] = {
    .time {17, 0},
    .repeat {true, true, true, true, true, true, true}
  };
}

// ===================================
// Loop
// ===================================

void loop() {
  // Refresh data from RTC HW in RTC class object so flags like
  // rtc.lostPower(), rtc.getEOSCFlag(), etc, can get populated
  rtc.refresh();

  printRTC();

  for (const activation_t &ac : activation) {
    // Conditions for activation
    bool dayOfWeekOK = ac.repeat[rtc.dayOfWeek()-1];
    bool hourOK = (ac.time.hour == rtc.hour());
    bool minuteOK = (ac.time.minute == rtc.minute());

    if (dayOfWeekOK) {
      if (hourOK && minuteOK) {
        Serial.print("Hora do Papá!\n");
        tone(BUZZER, 400, 1000);
        stepper.rotate(60);
        delay(60000); // To not activate twice during the same HH:MM
      }
    }
  }
  
  delay(1000);
}

// ===================================
// Functions definitions
// ===================================

//
// Print date and time in the format
// Data e Hora: YYYY/MM/DD (dayOfWeek) HH:MM:SS
// to the serial monitor
//
void printRTC() {
  // Define the names of the days of the week in Portuguese
  char daysOfWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sábado"};

  Serial.print("Data e Hora: ");
  Serial.print(rtc.year());
  Serial.print('/');
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.day());

  Serial.print(" (");
  Serial.print(daysOfWeek[rtc.dayOfWeek()-1]);
  Serial.print(") ");

  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.print(rtc.minute());
  Serial.print(':');
  Serial.println(rtc.second());
}
