//
// Project by Henrique Silva Simplicio
// 

#include <A4988.h>
#include <uRTCLib.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

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

JsonDocument globalDB;

// Define buzzer pin
#define BUZZER 25

// ===================================
// Functions declarations
// ===================================

void printRTC();
void initSPIFFS();
JsonDocument loadDB();
void saveDB(const JsonDocument newDB);

// ===================================
// Setup
// ===================================

void setup() {
  Serial.begin(115200);
  delay(3); // wait for console opening

  initSPIFFS();

  // Initialize times
  globalDB = loadDB();

  stepper.begin(3, 1);

  URTCLIB_WIRE.begin();

  // To adjust RTC initial time if needed
  // Only run it once
  // rtc.set(0, 34, 17, 7, 22, 4, 23);  // (second, minute, hour, dayOfWeek, dayOfMonth, month, year)
}

// ===================================
// Loop
// ===================================

void loop() {
  // Refresh data from RTC HW in RTC class object so flags like
  // rtc.lostPower(), rtc.getEOSCFlag(), etc, can get populated
  rtc.refresh();

  printRTC();

  JsonArray activations = globalDB["activations"];

  for (JsonArray::iterator it=activations.begin(); it!=activations.end(); ++it) {
    String time = (*it)["time"];
    int hour = time.substring(0, 2).toInt();
    int minute = time.substring(3, -1).toInt();

    // Conditions for activation
    bool dayOfWeekOK = (*it)["repeat"][rtc.dayOfWeek()-1];
    bool hourOK = (hour == rtc.hour());
    bool minuteOK = (minute == rtc.minute());

    if (dayOfWeekOK && hourOK && minuteOK) {
      Serial.print("Hora do Papá!\n");
      tone(BUZZER, 400, 1000);
      stepper.rotate(60);
      // To not activate twice during the same HH:MM
      delay(60000);
      break;
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

void initSPIFFS() {
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully");
  }

  Serial.println("An error has occurred while mounting SPIFFS. Retrying in 10s");
}

// Loads DB from the file to the global JsonDocument
JsonDocument loadDB() {
  Serial.println("Loading file ...");

  // Open file for reading
  File file = SPIFFS.open("/bd.json", "r");
  if (!file) {
      Serial.println("Failed to open file for reading");
  }

  JsonDocument db;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(db, file);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
  }

  file.close();
  Serial.println("File loaded successfully");
  return db;
}

// Saves DB to the file
void saveDB(const JsonDocument newDB) {
  Serial.println("Saving file ...");

  // Open file for writing
  File file = SPIFFS.open("/bd.json", "w");
  if (!file) {
      Serial.println("Failed to open file for writting");
  }

  JsonDocument doc = newDB;

  // Serialize JSON to file
  if (serializeJsonPretty(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
  Serial.println("File saved successfully");

  globalDB = doc;
}