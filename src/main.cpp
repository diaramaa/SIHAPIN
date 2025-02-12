#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <time.h>

const char* SSID = "diarama";
const char* PASSWORD = "qwerty13";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7*3600;
const int   daylightOffset_sec = 0;

#define DHT_PIN 25
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

const int LDR_PIN = 36;

const int SERVO_PIN = 4;
Servo servo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct tm timeinfo;

struct sensors {
  float temperature;
  float humidity;
  int ldr;
} data;

void setup_wifi() {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

char* printLocalTime() {
  static char formattedTime[30]; // Use a static buffer to avoid returning a local variable
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    strcpy(formattedTime, "Time: Error");
    return formattedTime;
  }
  strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return formattedTime;
}

void taskLCDUpdate(void *pvParameters) {
  for(;;) {

    char* time = printLocalTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.setCursor(0, 1);
    lcd.print(time);
    vTaskDelay(pdMS_TO_TICKS(3500)); // Delay for 2 seconds

    // Read DHT sensor
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    if (isnan(data.temperature) || isnan(data.humidity)) {
      // Serial.println("[ERROR] Failed to read from DHT sensor");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[ERROR] DHT sensor");
      lcd.setCursor(0, 1);
      lcd.print("Read failed!");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: " + String(data.temperature) + "C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: " + String(data.humidity) + "%");
    }
    vTaskDelay(pdMS_TO_TICKS(3500)); // Delay for 2 seconds

    // Read LDR sensor
    data.ldr = analogRead(LDR_PIN);
    int ldr_intensity = map(data.ldr, 0, 4095, 100, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LDR: " + String(ldr_intensity) + "%");
    vTaskDelay(pdMS_TO_TICKS(3500)); // Delay for 2 seconds
  }
}



void taskServoControl(void *pvParameters) {
  while (1) {
    // Example servo control logic
    for (int pos = 0; pos <= 180; pos += 1) {
      servo.write(pos);
      vTaskDelay(pdMS_TO_TICKS(15)); // Delay for 15ms
    }
    for (int pos = 180; pos >= 0; pos -= 1) {
      servo.write(pos);
      vTaskDelay(pdMS_TO_TICKS(15)); // Delay for 15ms
    }
  }
}

void pakanIkan_task(void *pvParameters) {
  //Jadwal pakan ikan: 7, 15, 23
  for(;;){
    if(getLocalTime(&timeinfo)){
      int currentHour = timeinfo.tm_hour;
      int currentMinute = timeinfo.tm_min;
      int currentSecond = timeinfo.tm_sec;
      if (currentSecond >= 03 && currentSecond <= 10) {
        Serial.println("[DEBUG] Servo ON (PAKAN IKAN)");
        servo.write(0);
        delay(500);
        servo.write(150);
        delay(500);
        servo.write(0);
        delay(500);
        servo.write(150);
        delay(500);
      } else {
        servo.write(0);
      }
    }else{
      Serial.println("Failed to obtain time");
    }
  }
  vTaskDelay(pdMS_TO_TICKS(500));
}

void setup() {
  Serial.begin(115200);
  
  pinMode(DHT_PIN, INPUT);
  
  Wire.begin();
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  servo.attach(SERVO_PIN);
  servo.write(0);
  
  Serial.println("[WiFi] Connecting to " + String(SSID));
  lcd.setCursor(0, 0);
  lcd.print("[WiFi] Connecting to " + String(SSID));
  setup_wifi();
  
  Serial.println("[WiFi] Connected!");
  Serial.println("IP: " + WiFi.localIP().toString());
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("[WiFi] Connected!");
  lcd.setCursor(0, 1);
  lcd.print("IP: " + WiFi.localIP().toString());
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  delay(2000);

  // Create tasks
  xTaskCreate(
    taskLCDUpdate,    // Function to implement the task
    "LCD Update",     // Name of the task
    4096,            // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );

  // xTaskCreate(
  //   taskServoControl, // Function to implement the task
  //   "Servo Control",  // Name of the task
  //   1024,            // Stack size in words
  //   NULL,             // Task input parameter
  //   2,                // Priority of the task
  //   NULL              // Task handle
  // );

  xTaskCreate(
    pakanIkan_task, // Function to implement the task
    "Pakan Ikan",  // Name of the task
    4096,            // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );

}

void loop() {
  // Empty loop because tasks are handled by FreeRTOS
}