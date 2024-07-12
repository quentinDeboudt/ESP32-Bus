#include <LiquidCrystal.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Remplacez par vos identifiants WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "...";

// URL de l'API
const char* apiURL = "https://data.explore.star.fr/api/explore/v2.1/catalog/datasets/tco-bus-circulation-passages-tr/records?limit=20&refine=precision%3A%22Temps%20r%C3%A9el%22&refine=idarret%3A%221273%22";

// Initialise l'écran LCD
LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

void setup() {
  Serial.begin(115200);

  // Initialise l'écran LCD et règle le nombre de colonnes et lignes
  lcd.begin(16, 2);
  lcd.print("Connexion...");

  // Connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connexion au WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connecte au WiFi");
  lcd.clear();
  lcd.print("WiFi Connecte");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(apiURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        //get data into Json:
        JsonObject record = doc["results"][0];
        const char* nomarret = record["destination"].as<const char*>();
        const char* arrivalTime = record["arrivee"].as<const char*>();

        // Transforme l'heure d'arrivée
        String transformedArrivalTime = transformDateTime(arrivalTime);

        // Limite le texte à 16 caractères pour chaque ligne
        String displayNomArret = String(nomarret);
        if (displayNomArret.length() > 16) {
          displayNomArret = displayNomArret.substring(0, 16);
        }

        // Affiche les données sur l'écran LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(displayNomArret);
        lcd.setCursor(0, 1);
        lcd.print(transformedArrivalTime);

      } else {
        Serial.print("Erreur de parsing JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Erreur de requete HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Deconnecte du WiFi");
  }

  delay(6000); // Attendre 60 secondes avant de refaire une requête
}

// Fonction pour transformer la date et l'heure
String transformDateTime(const char* dateTime) {
  
  String dateTimeStr = String(dateTime);
  String year = dateTimeStr.substring(2, 4);
  String month = dateTimeStr.substring(5, 7);
  String day = dateTimeStr.substring(8, 10);
  int hour = dateTimeStr.substring(11, 13).toInt();
  String minute = dateTimeStr.substring(14, 16);

  hour += 2;

  if (hour >= 24) {
    hour -= 24;
    int dayInt = day.toInt();
    dayInt += 1;
    day = String(dayInt);
  }

  return String(hour) + ":" + minute + " " + day + "-" + month + "-" + year;
}