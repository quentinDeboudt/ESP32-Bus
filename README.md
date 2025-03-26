# Affichage des Horaires de Bus avec ESP32

Ce projet utilise un ESP32 pour récupérer les horaires de bus depuis une API et les afficher sur un écran LCD. Il est conçu pour aider les utilisateurs à consulter les horaires de passage des bus directement depuis un appareil alimenté par ESP32.

## Fonctionnalités
- Affiche les destinations des bus et leurs horaires d'arrivée sur un écran LCD 16x2.
- Récupère les données depuis une API externe (`https://data.explore.star.fr/`) pour obtenir les horaires des bus.
- Met à jour les horaires affichés lorsqu'un bouton est pressé.
- Calcule automatiquement le temps restant avant le passage du prochain bus en fonction de l'heure actuelle.

## Composants
- **Carte ESP32 Dev Board** : Contrôleur principal
- **Écran LCD 16x2** : Affichage des horaires de bus
- **Bouton poussoir (GPIO 4)** : Permet de rafraîchir les données des horaires
- **Connexion Wi-Fi** : Nécessaire pour se connecter à l'API et récupérer les données des horaires

## Installation

### Prérequis
- **Plateforme Arduino IDE ou Visual Studio Code avec PlatformIO** : Pour programmer l'ESP32.
- **Bibliothèques nécessaires** :
  - `WiFi.h` : Pour la connexion Wi-Fi.
  - `HTTPClient.h` : Pour envoyer des requêtes HTTP et récupérer des données.
  - `LiquidCrystal.h` : Pour l'écran LCD.
  - `ArduinoJson.h` : Pour le traitement des données JSON.
  - `NTPClient.h` : Pour obtenir l'heure locale via NTP.

### Étapes
1. Clonez ce dépôt ou téléchargez-le en tant que fichier ZIP.
2. Ouvrez le projet dans votre IDE (Arduino IDE ou VSCode avec PlatformIO).
3. Remplissez vos informations Wi-Fi dans les variables `ssid` et `password`.
4. Téléversez le code sur votre ESP32.

### Connexion du matériel
- **Écran LCD** : Connectez l'écran LCD à l'ESP32 selon les broches définies dans le code :
  - RS -> GPIO 14
  - Enable -> GPIO 27
  - D4 -> GPIO 26
  - D5 -> GPIO 25
  - D6 -> GPIO 33
  - D7 -> GPIO 32
- **Bouton poussoir** : Connectez un bouton poussoir à la broche GPIO 4. Il sera utilisé pour rafraîchir les horaires de bus.

## Utilisation

1. Lorsque le projet démarre, l'ESP32 tente de se connecter au Wi-Fi et affiche un message de connexion.
2. Une fois connecté, il récupère les horaires de bus et les affiche sur l'écran LCD.
3. Vous pouvez appuyer sur le bouton poussoir pour actualiser les horaires.

## Fonctionnement du code

1. **Récupération des horaires** : Le code envoie une requête HTTP à l'API de Star Rennes pour obtenir les horaires des bus à l'arrêt spécifié.
2. **Affichage des horaires** : Les horaires des bus sont extraits de la réponse JSON et affichés sur l'écran LCD. Les horaires sont affichés par groupe de 3, séparés par des virgules.
3. **Calcul du temps restant** : Le temps restant avant l'arrivée du bus est calculé en fonction de l'heure actuelle et de l'heure de passage du bus.

## Dépannage

- **Problèmes de connexion Wi-Fi** : Assurez-vous que l'ESP32 est à portée de votre réseau Wi-Fi et que les informations `ssid` et `password` sont correctes.
- **Problèmes de récupération des données de l'API** : Si l'API ne répond pas, vérifiez votre connexion Internet et que l'API est accessible depuis votre réseau.
- **Problèmes d'affichage sur l'écran LCD** : Vérifiez les connexions de l'écran LCD aux broches de l'ESP32 et assurez-vous que la bibliothèque `LiquidCrystal` est correctement installée.

## License



![image](https://github.com/user-attachments/assets/b0ba140b-d81f-4ce7-8b69-b0bb351c693c)


