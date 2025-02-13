#include <creds.h>
#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <time.h>
#include <EEPROM.h>
#include <GravityTDS.h>
#include <DFRobot_PH.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_INA219.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7*3600;
const int   daylightOffset_sec = 0;

// Current Sensor
Adafruit_INA219 ina219;
float vmax;
float vmin;
float power_W = 0;
float totalEnergy_Wh = 0;
unsigned long currentTime;
unsigned long elapsedTime;
unsigned long lastTime;

BlynkTimer timer;

// pH Sensor
#define PH_PIN 27
float volt_PH;
DFRobot_PH ph;

// DHT Sensor
#define DHT_PIN 25
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// LDR Sensor
const int LDR_PIN = 36;

// TDS Sensor
const int TDS_PIN = 35;
#define EEPROM_SIZE 512
GravityTDS gravityTds;
float Temp= 25;

// Servo Motor
const int SERVO_PIN = 4;
Servo servo;

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

struct tm timeinfo;

struct sensors {
  float temperature;
  float humidity;
  int ldr;
  float phValue;
  float tdsValue;
  float energy_Wh;
  float batteryPercentage;
} data;

struct random{
  float temperature;
  float humidity;
  int ldr;
  float phValue;
  float tdsValue;
  float energy_Wh;
  float batteryPercentage;
} randomData;

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

void taskSensorRead(void *pvParameters) {
  for(;;) {
    // Read DHT sensor
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    if (isnan(data.temperature) || isnan(data.humidity)) {
      Serial.println("[ERROR] Failed to read from DHT sensor");
    } else {
      Serial.println("DHT Sensor Data:");
      Serial.println("Temperature: " + String(data.temperature) + "C");
      Serial.println("Humidity: " + String(data.humidity) + "%");
    }
    
    // Read LDR sensor
    int ldr_read = analogRead(LDR_PIN);
    data.ldr = map(ldr_read, 0, 4095, 100, 0);
    Serial.println("LDR Sensor Data:");
    Serial.println("LDR Intensity: " + String(data.ldr) + "%");
    
    //Read pH Sensor
    volt_PH = analogRead(PH_PIN)/4096.0 * 5000;
    data.phValue = ph.readPH(volt_PH, data.temperature);
    Serial.println("pH Sensor Data:");
    Serial.println("pH: " + String(data.phValue));
    
    //Read TDS sensor
    gravityTds.setTemperature(data.temperature);
    gravityTds.update();
    data.tdsValue = gravityTds.getTdsValue();
    Serial.println("TDS Sensor Data:");
    Serial.println("TDS: " + String(data.tdsValue) + "ppm");

    // Read Current Sensor
    currentTime = millis();
    elapsedTime = currentTime - lastTime;
    if(elapsedTime > 1000){
      lastTime = currentTime;
      float shuntvoltage = ina219.getShuntVoltage_mV();
      float busvoltage = ina219.getBusVoltage_V();
      float current_mA = ina219.getCurrent_mA();
      float loadvoltage = busvoltage + (shuntvoltage / 1000);
      power_W = loadvoltage * current_mA;
      data.energy_Wh += power_W * (elapsedTime / 3600000.0);

      // Calculate battery percentage
      vmax = 5.0;                                       // Tegangan maksimum pada saat baterai full
      vmin = 2.5;                                       // Tegangan minimum pada saat baterai kritis
      data.batteryPercentage = ((loadvoltage - vmin) / (vmax - vmin)) * 100.0;
      if (data.batteryPercentage > 100) {
        data.batteryPercentage = 100;
      } else if (data.batteryPercentage < 0) {
        data.batteryPercentage = 0;
      }
      // Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
      // Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
      // Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
      // Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
      // Serial.print("Power:         "); Serial.print(power_W); Serial.println(" W");
      Serial.print("Total Energy:  "); Serial.print(data.energy_Wh); Serial.println(" Wh");
      Serial.print("Battery Percentage: "); Serial.print(data.batteryPercentage); Serial.println(" %");
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

void randomDataGenerator(void *pvParameters) {
  for(;;) {
    randomData.temperature = random(20, 30);
    randomData.humidity = random(50, 70);
    randomData.ldr = random(0, 100);
    randomData.phValue = random(0, 14);
    randomData.tdsValue = random(0, 1000);
    randomData.energy_Wh = random(0, 100);
    randomData.batteryPercentage = random(0, 100);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  
}

//Blynk Update
void blynkUpdate() {
  Blynk.virtualWrite(V0, randomData.temperature);
  Blynk.virtualWrite(V1, randomData.humidity);
  Blynk.virtualWrite(V2, randomData.ldr);
  Blynk.virtualWrite(V3, randomData.phValue);
  Blynk.virtualWrite(V4, randomData.tdsValue);
  Blynk.virtualWrite(V5, randomData.energy_Wh);
  Blynk.virtualWrite(V6, randomData.batteryPercentage);
}



void taskLCDUpdate(void *pvParameters) {
  for(;;) {

    char* time = printLocalTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.setCursor(0, 1);
    lcd.print(time);
    vTaskDelay(pdMS_TO_TICKS(3000)); 

    //Write LCD for DHT Sensor
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
    vTaskDelay(pdMS_TO_TICKS(3000)); 

    //Write LCD for LDR Sensor
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LDR Sensor Data: ");
    lcd.setCursor(0, 1);
    lcd.print("Value: " + String(data.ldr) + "%");
    vTaskDelay(pdMS_TO_TICKS(3000)); 

    //Write LCD for pH Sensor
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pH Sensor Data: ");
    lcd.setCursor(0, 1);
    lcd.print("Value: " + String(data.phValue));
    vTaskDelay(pdMS_TO_TICKS(3000));

    //Write LCD for TDS Sensor
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TDS Sensor Data: ");
    lcd.setCursor(0, 1);
    lcd.print("Value: " + String(data.phValue, 0) + "ppm");
    vTaskDelay(pdMS_TO_TICKS(3000)); 


  }
}

void pakanIkan_task(void *pvParameters) {
  //Jadwal pakan ikan: 7, 15, 23
  for(;;){
    if(getLocalTime(&timeinfo)){
      int currentHour = timeinfo.tm_hour;
      int currentMinute = timeinfo.tm_min;
      int currentSecond = timeinfo.tm_sec;
      if (currentSecond >= 3 && currentSecond <= 5) {
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

  EEPROM.begin(EEPROM_SIZE);
  gravityTds.setPin(TDS_PIN);
  gravityTds.setAref(3.3);
  gravityTds.setAdcRange(4096);
  gravityTds.begin();

  ph.begin();
  
  pinMode(DHT_PIN, INPUT);
  
  Wire.begin();
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  servo.attach(SERVO_PIN);
  servo.write(0);

  // if (!ina219.begin()) {
  //   Serial.println("Failed to find INA219 chip");
  //   while (1) { delay(10); }
  // }

  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  
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

  xTaskCreate(
    taskSensorRead,   // Function to implement the task
    "Sensor Read",    // Name of the task
    4096,            // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );

  // xTaskCreate(
  //   randomDataGenerator, 
  //   "Random Data Generator",
  //   2048,
  //   NULL,
  //   1,
  //   NULL
  // );

  xTaskCreate(
    pakanIkan_task, // Function to implement the task
    "Pakan Ikan",  // Name of the task
    2048,            // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );

  timer.setInterval(1000L, blynkUpdate);

}

void loop() {
  // Empty loop because tasks are handled by FreeRTOS
  Blynk.run();
  timer.run();
}