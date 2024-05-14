//
// Project by Henrique Silva Simplicio
// 

#include <A4988.h>
#include <uRTCLib.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

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

// Define buzzer pin
#define BUZZER 25

// Replace with your network credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define WIFI_TIMEOUT_MS 20000

JsonDocument globalDB;

// ===================================
// Functions declarations
// ===================================

void printRTC();
JsonDocument loadDB();
void saveDB(const JsonDocument newDB);

// ===================================
// Tasks
// ===================================

void vKeepWiFiAlive(void * parameters) {
  // Verifying stack size used
  // UBaseType_t uxHighWaterMark;
  // uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  // Serial.print("Começo da KeepWiFiAlive: ");
  // Serial.println(uxHighWaterMark);
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
      Serial.print(".");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    Serial.println();

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi not connected. Retrying in 10s");
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }
    
    Serial.println("WiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void vActivationSystem(void *parameters) {
  // Verifying stack size
  // UBaseType_t uxHighWaterMark;
  // uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  // Serial.print("Começo da ActivationSystem: ");
  // Serial.println(uxHighWaterMark);

  globalDB = loadDB();
  
  stepper.begin(3, 1);

  URTCLIB_WIRE.begin();

  // To adjust RTC initial time if needed
  // Only run it once
  // rtc.set(0, 34, 17, 7, 22, 4, 23);  // (second, minute, hour, dayOfWeek, dayOfMonth, month, year)

  for (;;) {
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
        Serial.println("Hora do Papá!");
        tone(BUZZER, 400, 1000);
        stepper.rotate(60);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
        break;
      }
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Guarantee SPIFFS initialization
void vInitSPIFFS(void *parameters) {
  for (;;) {
    if (SPIFFS.begin(true)) {
      Serial.println("SPIFFS mounted successfully");
      vTaskDelete(NULL);
    }

    Serial.println("An error has occurred while mounting SPIFFS. Retrying in 10s");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

// ===================================
// Setup
// ===================================

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(vInitSPIFFS, "Guarantee SPIFFS Initialization", 5000, NULL, 3, NULL, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(vKeepWiFiAlive, "Keep WiFi Alive", 5000, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(vActivationSystem, "Activation System", 5000, NULL, 2, NULL, 0);
}

// ===================================
// Loop
// ===================================

// Not used
void loop() {
  // Minimize the loop() time consumption, without having to delete the task
  vTaskDelay(100);
}

// ===================================
// Functions definitions
// ===================================

// Print date and time in the format
// Data e Hora: YYYY/MM/DD (dayOfWeek) HH:MM:SS
// to the serial monitor
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