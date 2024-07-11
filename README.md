# Affichage des Arrivées de Bus avec ESP32

## Vue d'ensemble

Ce projet utilise un microcontrôleur ESP32 pour afficher les informations d'arrivée en temps réel des bus sur un écran LCD QAPASS 1602A. Les données sont récupérées depuis l'API STAR et affichées de manière concise, avec la possibilité de contrôler l'affichage à l'aide d'une télécommande infrarouge.

## Fonctionnalités

- **Connectivité WiFi** : L'ESP32 se connecte à un réseau WiFi pour récupérer les données des bus.
- **Données en temps réel** : Récupère les informations d'arrivée des bus en temps réel depuis l'API STAR.
- **Affichage LCD** : Affiche le nom de l'arrêt de bus et l'heure d'arrivée sur un écran LCD 16x2.
- **Contrôle par télécommande infrarouge** : Permet d'allumer/éteindre l'écran et de naviguer entre les enregistrements des bus à l'aide d'une télécommande infrarouge.
- **Conversion de la date et de l'heure** : Convertit et affiche la date et l'heure dans un format lisible (Format par défault: UTC).

## Composants

- **Microcontrôleur ESP32** : Le cœur du projet, responsable de la connexion WiFi, de la récupération des données et du contrôle de l'affichage.
- **Écran LCD QAPASS 1602A** : Un écran LCD 16x2 pour afficher les informations d'arrivée des bus.
- **Module récepteur infrarouge** : Reçoit les signaux d'une télécommande infrarouge pour contrôler l'affichage.
- **Réseau WiFi** : Nécessaire pour récupérer les données depuis l'API STAR.
- **Télécommande infrarouge** : Pour contrôler l'affichage (allumer/éteindre et changer d'enregistrements).

## Configuration et Installation

### Connexions Matérielles

- **ESP32 vers LCD** :
  - VSS à GND
  - VDD à 5V
  - SDA à GPIO21
  - SCL à GPIO22
- **ESP32 vers récepteur infrarouge** :
  - OUT à GPIO4
  - VCC à 3.3V
  - GND à GND

### Installation Logicielle

- **Bibliothèques** :
  - `LiquidCrystal` : Pour contrôler l'affichage LCD.
  - `WiFi` : Pour connecter à un réseau WiFi.
  - `HTTPClient` : Pour effectuer des requêtes HTTP vers l'API.
  - `ArduinoJson` : Pour analyser les données JSON provenant de l'API.
  - `IRremote` : Pour recevoir et décoder les signaux infrarouges.

### Configuration

- Mettez à jour les variables `ssid` et `password` avec vos informations d'identification WiFi.
- Remplacez l'URL de l'API si nécessaire.
- Personnalisez les codes de contrôle de la télécommande infrarouge selon votre télécommande.

## Utilisation

1. **Téléchargez le code** sur votre ESP32 en utilisant l'IDE Arduino.
2. **Connexion au WiFi** : L'ESP32 se connectera automatiquement au réseau WiFi configuré.
3. **Récupération des données** : L'ESP32 récupérera les données d'arrivée des bus en temps réel depuis l'API STAR.
4. **Affichage des données** : Le nom de l'arrêt de bus et l'heure d'arrivée seront affichés sur l'écran LCD.
5. **Contrôle de l'affichage** : Utilisez la télécommande infrarouge pour allumer/éteindre l'écran et naviguer entre les enregistrements des bus.
