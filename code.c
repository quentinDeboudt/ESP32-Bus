#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <map>

// Replace with your Wi-Fi information
const char* ssid = "NameWifi";
const char* password = "Password";

// Paris time zone
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;  // UTC+1
const int   daylightOffset_sec = 3600; // +1h d'été

/**
 * API Star Rennes: (https://data.explore.star.fr/api/explore/v2.1/console)
 * refine = idarret%3A"0123" (search for your id_arret: https://data.explore.star.fr/map/+106b59a6e2294b7b/edit/)
 */
const char* apiURL = "https://data.explore.star.fr/api/explore/v2.1/catalog/datasets/tco-bus-circulation-passages-tr/records?&order_by=arrivee&limit=6&refine=idarret%3A%221273%22&lang=fr&timezone=Europe%2FParis&include_links=false";

// Initialize the LCD screen with the specified pins (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

// Using GPIO 4 for the push button
const int buttonPin = 4;    
bool lastButtonState = HIGH;
bool buttonState = HIGH;

/**
 * @brief Displays the bsu schedules on the LCD screen.
 * @param array A JSON array containing objects with bus schedules.
 */
void showSchedules(JsonArray array) {

  int displayingData = 0;

  while (displayingData < 4) {
    for (JsonObject record : array) {
      lcd.clear();

      // Display the destination
      const char* destination = record["destination"];
      lcd.setCursor(0, 0);
      lcd.print(destination);

      // Display the arrival times for the destination
      JsonArray arrivees = record["arrivees"];
      if (arrivees.size() > 0) {
        lcd.setCursor(0, 1);
        for (int i = 0; i < arrivees.size(); i++) {
          if (i > 0){
            lcd.print(", ");// Separate times with a comma
          }
          lcd.print(String(arrivees[i].as<int>()));
        }
      }
      
      //wait five seconds before displaying other destinations
      delay(5000);
      displayingData++;
    }
  }
}

/**
 * @brief Calculates the difference in minutes between the current time and the bus arrival time.
 * 
 * This function gets the current time, adjusts for the local timezone, and calculates the difference in minutes
 * between the current time and a given bus arrival time.
 * 
 * @param busTime The bus arrival time as a string in the format "YYYY-MM-DDTHH:MM:SS+01:00".
 * @return int The number of minutes remaining until the bus arrives, or 0 if the bus is already past.
 */
int timeDifferenceInMinutes(String busTime) {
  // Configure timezone and synchronize time with NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // Attempt to get the local time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    lcd.clear();
    lcd.println("Error Time");
    return 0;
  }

  // Adjust current time
  int currentHour = (timeinfo.tm_hour - 1);
  int currentMinute = timeinfo.tm_min;

  // Extract bus arrival time
  int busHour = busTime.substring(11, 13).toInt();
  int busMinute = busTime.substring(14, 16).toInt();

  // Convert both current time and bus time to total minutes since midnight
  int currentTotalMinutes = currentHour * 60 + currentMinute;
  int busTotalMinutes = busHour * 60 + busMinute;

  // Calculate the remaining time in minutes
  int minutesRemaining = (busTotalMinutes - currentTotalMinutes)-1;
  
  // If the bus has already passed, return 0 minute
  if (minutesRemaining < 0) {
    minutesRemaining = 0;
  }

  return minutesRemaining;
}

/**
 * @brief Get data from API.
 * @callgraph showSchedules().
 */
void refreshBusData() {

  // Displaying an update message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mise a jour.....");
 
  // Loading bar displayed while waiting for WiFi connection
  int cursor = 0;
  lcd.setCursor(0, 20);
  while (cursor < 16) {
    lcd.setCursor(cursor, 2);
    lcd.print(".");
    cursor++;

    if(cursor >= 9){
      if(WiFi.status() != WL_CONNECTED){
          cursor = 9;
      }
    }
    delay(60);
  }

  

  if (WiFi.status() == WL_CONNECTED) {

    // Displaying an loading message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loading...");

    // Sends a GET request to the specified API URL
    HTTPClient http; 
    http.begin(apiURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      // Retrieves the response payload as a string
      String payload = http.getString();

      // Parse the received JSON payload into a StaticJsonDocument
      StaticJsonDocument<4096> doc;
      DeserializationError error = deserializeJson(doc, payload);
      JsonArray results = doc["results"].as<JsonArray>();

      // Create a new StaticJsonDocument to store the filtered JSON data
      StaticJsonDocument<1024> filteredDoc;
      JsonArray groupedArray = filteredDoc.to<JsonArray>();

      /**
       * Check if JSON deserialization was successful
       * create [{"destination": "Name", "arrivees": [ 1, 2, 3]}, {...}]
       */
      if (!error) {

        // Initialize variables before filtering them
        String previousDestination = "";
        JsonArray currentGroup;
        std::map<String, JsonArray>destinationCount;

        // Iterate over each record in the array
        for (JsonObject record : results) {
          const char* destC = record["destination"].as<const char*>();
          if (destC == nullptr) continue;

          // Checks if the destination already exists in the table
          String destination = String(destC);
          if (destinationCount.find(destination) == destinationCount.end()) {
            // Creates a new object with the destination name
            JsonObject newRecord = groupedArray.createNestedObject();
            newRecord["destination"] = destination;
            destinationCount[destination] = newRecord.createNestedArray("arrivees");
          }

          //do not retrieve the slot if it's smaller than 1
          int timeDifference = timeDifferenceInMinutes(record["arrivee"].as<const char*>());
          if(timeDifference > 1){
            // Add arrival time only if < 3 slots
            if (destinationCount[destination].size() < 3) {
              destinationCount[destination].add(timeDifferenceInMinutes(record["arrivee"].as<const char*>()));
            }
          }
        }

        // Call the function to display the data on the screen
        showSchedules(groupedArray);

      } else {
        //error parsing Json:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Erreu$$or JSON");
      }
    } else {
      //HTTP request error:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error request");
    }

    http.end();
  } else {
    //Wifi disconnected:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error Wifi");
  }
}

/**
 * @brief Initialize the system, configure the LCD, WiFi and button.
 * @callgraph refreshBusData().
 */
void setup() {
  Serial.begin(115200);

  // Number of columns and rows on the screen
  lcd.begin(16, 2);

  // Configure GPIO 4 for push button
  pinMode(buttonPin, INPUT_PULLUP);  

  // Connection to wifi
  WiFi.begin(ssid, password);
  
  lcd.print("Connexion");
  int cursor = 9;
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(cursor, 0);
    lcd.print(".");
    if(cursor <= 16){
      cursor++;
    }
    delay(50);
  }

  refreshBusData();
}

void loop() {
  // Read the current state of the butto
  buttonState = digitalRead(buttonPin);  // Lire l'état du bouton

  // Check if the button was pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
    refreshBusData();
  }

  // Store the current button state to detect the next button press in the next loop iteration
  lastButtonState = buttonState;  
}
