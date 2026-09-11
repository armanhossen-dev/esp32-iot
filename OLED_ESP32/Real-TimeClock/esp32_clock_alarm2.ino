#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// OLED Display Object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Wi-Fi Credentials
const char* ssid     = "AHRN OS";
const char* password = "@newEra3087knowTW";

// NTP Server Settings (UTC+6 for Bangladesh)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 0;

// Buzzer
#define BUZZER_PIN 5

// Alarm state
bool alarmActive = false;
bool isHourlyAlarm = false;    // True for 3s hourly chime, false for 10-min pattern
unsigned long alarmStartMs = 0;
int alarmBeepTarget = 0;       // 1 for :10, 2 for :20, ..., 5 for :50
int lastTriggeredMark = -1;    // Unique 10-min slot ID to prevent repeat triggers

const int ALARM_BEEP_ON_MS  = 200;
const int ALARM_BEEP_OFF_MS = 150;

void handleAlarmBeeper();

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Display connecting message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.println("Connecting Wi-Fi...");
  display.println(ssid);
  display.display();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Connection Failed!");
    display.println("Check router power");
    display.display();
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Connected!");
  display.println("Syncing Time...");
  display.display();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(1500);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  // Convert 24h hour to 12h hour & determine AM/PM
  int hour12 = timeinfo.tm_hour % 12;
  if (hour12 == 0) hour12 = 12;
  const char* ampm = (timeinfo.tm_hour >= 12) ? "PM" : "AM";

  // --- Alarm trigger: every 10 minutes ---
  if (timeinfo.tm_min % 10 == 0 && timeinfo.tm_sec == 0) {
    int mark = timeinfo.tm_hour * 6 + (timeinfo.tm_min / 10);
    if (mark != lastTriggeredMark) {
      lastTriggeredMark = mark;
      alarmActive = true;
      alarmStartMs = millis();

      if (timeinfo.tm_min == 0) {
        // Hourly alarm: 3-second continuous tone
        isHourlyAlarm = true;
        alarmBeepTarget = 0;
      } else {
        // 10-minute intervals: :10 = 1, :20 = 2, :30 = 3, :40 = 4, :50 = 5
        isHourlyAlarm = false;
        alarmBeepTarget = timeinfo.tm_min / 10;
      }
      tone(BUZZER_PIN, 1000);
    }
  }

  handleAlarmBeeper();

  // --- Draw Clock Interface ---
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18, 2);
  display.println(F("REAL-TIME CLOCK"));

  display.drawFastHLine(0, 14, 128, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(8, 22);
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour12, timeinfo.tm_min, timeinfo.tm_sec);
  display.print(timeStr);

  display.setTextSize(1);
  display.setCursor(106, 28);
  display.print(ampm);

  display.setCursor(28, 48);
  char dateStr[11];
  sprintf(dateStr, "%02d-%02d-%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  display.print(dateStr);

  if (alarmActive) {
    display.setCursor(90, 48);
    display.print("ALM");
  }

  display.display();
  delay(200);
}

void handleAlarmBeeper() {
  if (!alarmActive) return;

  unsigned long elapsed = millis() - alarmStartMs;

  if (isHourlyAlarm) {
    // Top of the hour: 3 seconds continuous buzz
    if (elapsed < 3000) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
      alarmActive = false;
    }
  } else {
    // 10-minute intervals: Pulsed beeps
    unsigned long cycle = ALARM_BEEP_ON_MS + ALARM_BEEP_OFF_MS;
    unsigned long cyclePos = elapsed % cycle;
    int currentBeep = elapsed / cycle;

    if (currentBeep >= alarmBeepTarget) {
      noTone(BUZZER_PIN);
      alarmActive = false;
      return;
    }

    if (cyclePos < ALARM_BEEP_ON_MS) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
    }
  }
}

/*
- **Buzzer Alarm Logic Upgrade:**
- **Top of the hour (`:00`):** Continuous **3-second** buzzer tone.
- **10-minute intervals:** Variable pulse chime count based on time (`:10` = 1 beep, `:20` = 2 beeps, `:30` = 3 beeps, `:40` = 4 beeps, `:50` = 5 beeps).
- Non-blocking alarm timing maintained using `millis()` to prevent screen delay stutter.
*/