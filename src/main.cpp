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
#include <PubSubClient.h>

//MQTT
const char broker[] = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient mqtt(espClient); //mqtt
#define MSG_BUFFER_SIZE  (512)  //

//RTC
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
#define PH_PIN 32
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
  float voltage;
  float energy_Wh;
  int batteryPercentage;
  
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

//Blynk Update
void blynkUpdate() {
  Blynk.virtualWrite(V0, data.temperature);
  Blynk.virtualWrite(V1, data.humidity);
  Blynk.virtualWrite(V2, data.ldr);
  Blynk.virtualWrite(V3, data.phValue);
  Blynk.virtualWrite(V4, data.tdsValue);
  Blynk.virtualWrite(V5, data.energy_Wh);
  Blynk.virtualWrite(V6, data.batteryPercentage);
}

void taskSensorRead(void *pvParameters) {
  // UBaseType_t uxHighWaterMark;
  for(;;) {
    // uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    // Serial.print("High water mark: ");
    // Serial.println(uxHighWaterMark);
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
    volt_PH = analogRead(PH_PIN)/4095.0 * 3300;
    data.phValue = ph.readPH(volt_PH, data.temperature);
    Serial.println("pH Sensor Data:");
    Serial.println("pH: " + String(data.phValue));

    //Read TDS sensor
    gravityTds.setTemperature(data.temperature);
    gravityTds.update();
    data.tdsValue = gravityTds.getTdsValue();
    Serial.println("TDS Sensor Data:");
    Serial.println("TDS: " + String(data.tdsValue) + "ppm");

    //Read Current Sensor
    float shuntvoltage = ina219.getShuntVoltage_mV(); // Membaca tegangan shunt (dalam mV) dari sensor INA219
    float busvoltage = ina219.getBusVoltage_V();      // Membaca tegangan bus (dalam V) dari sensor INA219
    float current_mA = ina219.getCurrent_mA();        // Membaca arus (dalam mA) dari sensor INA219
    float power_mW = ina219.getPower_mW();            // Membaca daya (dalam mW) dari sensor INA219
    
    float loadvoltage = busvoltage + (shuntvoltage / 1000);   //Tegangan beban

    // Calculate battery percentage
    vmax = 8.0;                                       // Tegangan maksimum pada saat baterai full
    vmin = 5;                                       // Tegangan minimum pada saat baterai kritis
    int batteryPercentage = ((loadvoltage - vmin) / (vmax - vmin)) * 100.0;

    currentTime = millis();                           // Waktu saat ini dalam milidetik
    elapsedTime = currentTime - lastTime;             // Selisih waktu sejak pengukuran terakhir
    lastTime = currentTime;                           // Perbarui lastTime untuk pengukuran berikutnya

    // Konversi power dari mW ke W dan waktu dari ms ke jam
    float power_W = power_mW / 1000.0;
    float elapsedTime_h = elapsedTime / 3600000.0; // 1 jam = 3600000 ms

    // Tambahkan energi ke total
    totalEnergy_Wh += power_W * elapsedTime_h;

    Serial.println("Current Sensor Data:");
    Serial.println("Load Voltage: " + String(loadvoltage) + "V");
    Serial.println("Energy: " + String(totalEnergy_Wh) + "Wh");
    Serial.println("Battery Percentage: " + String(batteryPercentage) + "%");

    data.voltage = loadvoltage;
    data.energy_Wh = totalEnergy_Wh;
    data.batteryPercentage = batteryPercentage;

    // blynkUpdate();

    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      mqtt.publish("sihapin/mqtt", "sihapin");
      // ... and resubscribe
      mqtt.subscribe("sihapin/mqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void MQTT_Send(void *pvParameters) {
  for(;;){
    if (!mqtt.connected()) {
      reconnect();
    }
    mqtt.loop();

    char msg[MSG_BUFFER_SIZE];
    snprintf(msg, MSG_BUFFER_SIZE, "%.2f#%.2f#%.2f#%.2f#%.2f#%.2f#%d", data.temperature, data.humidity, data.phValue, data.tdsValue, data.voltage, data.energy_Wh, data.batteryPercentage);
    mqtt.publish("sihapin/message", msg);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
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
    lcd.print("Value: " + String(data.tdsValue) + " ppm");
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
      if (currentSecond >= 3 && currentSecond <= 7) {
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
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);
  gravityTds.setPin(TDS_PIN);
  gravityTds.setAref(3.3);
  gravityTds.setAdcRange(4096);
  gravityTds.begin();

  ina219.begin();

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

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(callback);
  
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
    8192,            // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );

  xTaskCreate(
    MQTT_Send,
    "MQTT Send",
    4096,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    pakanIkan_task, // Function to implement the task
    "Pakan Ikan",  // Name of the task
    2048,            // Stack size in words
    NULL,             // Task input parameter
    2,                // Priority of the task
    NULL              // Task handle
  );

  timer.setInterval(1000L, blynkUpdate);

}

void loop() {
  // Empty loop because tasks are handled by FreeRTOS
  Blynk.run();
  timer.run();
}