#include <WiFi.h>
#include <HTTPClient.h>

#define blue_led 9 // pin for blue led management
#define green_led 41 // pin for green led management
#define red_led 40 // pin for red led management
#define yellow_led 9 // pin yellow blue led management

const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldrPin = 4;  // the number of the pushbutton pin
int threshold=600;

void setup() {

  // initial config for leds management as ESP32's outputs 
  pinMode(blue_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(yellow_led,OUTPUT);

  // inputs initializing
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input

  digitalWrite(blue_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);

  Serial.begin(9600); // serial monitor debug configuration with baud rate set for 9600

  WiFi.begin("Wokwi-GUEST", ""); // WiFi connection using "Wokwi-GUEST" SSID

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Succesfully connected to WiFi!"); // considering that the above loop worked, the ESP32 is now connected to WiFi (another option is to put this command inside the bellow if statement)

  // verifies the button state
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    unsigned long timeNow = millis();
    unsigned long lastMoment = 0;

    if (timeNow - lastMoment >= 500) {
      Serial.println("Button pressed!");
    };
  } else {
    Serial.println("Button not pressed!");
  }

  if(WiFi.status() == WL_CONNECTED){ // if the ESP32 is connected to the internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // HTTP requirement endpoint

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // HTTP response code

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){
    unsigned long timeNow = millis();
    unsigned long lastMoment = 0;

    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);

    if(timeNow - lastMoment >= 1000) {
      digitalWrite(yellow_led, HIGH);
    } else if (timeNow - lastMoment >= 2000) {
      digitalWrite(yellow_led, LOW);
      lastMoment = timeNow;
    };

  }else{
    unsigned long timeNow = millis();
    unsigned long lastMoment = 0;

    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);

    digitalWrite(green_led, HIGH);
    if (timeNow - lastMoment >= 3000) {
      digitalWrite(green_led, LOW);
      digitalWrite(yellow_led, HIGH);
    } else if (timeNow - lastMoment >= 5000) {
      digitalWrite(yellow_led, LOW);
      digitalWrite(red_led, HIGH);

      if (buttonState == HIGH) {
        timeNow = millis();
        lastMoment = 0;
        if (timeNow - lastMoment >= 500) {
          timeNow += 9000;
        }
      };
    } else if (timeNow - lastMoment >= 10000) {
      digitalWrite(red_led, LOW);
      lastMoment = timeNow;
    };
  }
}