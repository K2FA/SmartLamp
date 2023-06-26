#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define sensorLDR A0
// WiFi
const char*ssid = "JTI-POLINEMA";
const char*password = "jtifast!";
const char* mqttTopic = "cahaya/ilahi";

// MQTT
const char* mqtt_server = "broker.hivemq.com";
bool isMqttConnected = false;
LiquidCrystal_I2C lcd(0x27,16,2);

// Pin


#define RED_LED D5 //led warna merah
#define GREEN_LED D6 //led warna hijau
#define BLUE_LED D7 //led warnah biru

int nilaiSensor;

WiFiClient espClient;
PubSubClient client(espClient);



void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      isMqttConnected = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED,OUTPUT);//atur pin-pin digital sebagai output
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("SMART LAMP");
  lcd.setCursor(2,1);
  lcd.print("Kelompok 03");
  delay(2000);
  lcd.clear();



  setup_wifi();
  client.setServer(mqtt_server, 1883);
  // SPI.begin();
  // mfrc522.PCD_Init();
  // pinMode(LED_PIN, OUTPUT); // Inisialisasi LED mati
  // Serial.println("Scan RFID card to get UID...");

  // doorServo.attach(D1); // Ganti D0 dengan pin yang kamu pilih
  // doorServo.write(0);  // Set posisi awal servo (tertutup)
}

void loop() {
  if (!client.connected() && !isMqttConnected) {
    reconnect();
  }
  if (!client.loop()) {
    client.connect("ESP8266Client");
  }


  nilaiSensor = analogRead(sensorLDR);

 lcd.setCursor(0, 0);
 lcd.print("Cahaya : ");

 if(nilaiSensor < 500){
    lcd.setCursor(0,0);
   lcd.print("Kondisi : Terang");
   digitalWrite(RED_LED, HIGH);
   digitalWrite(GREEN_LED, HIGH);
   digitalWrite(BLUE_LED, HIGH);
   client.publish(mqttTopic, "ON");
   
 }else{
   lcd.setCursor(0,0);
   lcd.print("Kondisi : Gelap");
   digitalWrite(RED_LED, LOW);
   digitalWrite(GREEN_LED, LOW);
   digitalWrite(BLUE_LED, LOW);
   client.publish(mqttTopic, "OFF");
    
 }
 lcd.setCursor(0, 1);
 lcd.print("Sensor: ");
 lcd.print(nilaiSensor);
 
 Serial.println(nilaiSensor);
//  lcd.print("    "); 
  
 delay(500);
}
