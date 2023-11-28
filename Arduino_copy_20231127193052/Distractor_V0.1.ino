#include <LiquidCrystal.h>

#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245

// Define the notes in the melody of the tune
const int FIRST_MELODY[] = {
  NOTE_B5, NOTE_A4, NOTE_B4, NOTE_FS5, NOTE_AS4, NOTE_E5, NOTE_C5, NOTE_A5, NOTE_A4, NOTE_FS5, NOTE_B4,
  NOTE_B5, NOTE_AS4, NOTE_FS5, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_A5, NOTE_B4, NOTE_AS4, NOTE_FS5, NOTE_CS5,
  NOTE_B5, NOTE_B4, NOTE_E5, NOTE_AS4, NOTE_FS5, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_A5, NOTE_B4, NOTE_FS5, NOTE_AS4,
  NOTE_B5, NOTE_C5, NOTE_FS5, NOTE_A4, NOTE_E5, NOTE_A4, NOTE_A5, NOTE_AS4, NOTE_FS5, NOTE_B4, NOTE_B5
};

const int FIRST_DURATIONS[] = {
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8
                  };

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

const int SPEAKER_PIN = 12;
const int trigPin = 9;
const int echoPin = 10;
const int piezoPin = 12;

long duration;
int distanceCm, distanceInch;
unsigned long previousMillis = 0;
const long interval = 120000; // 20 minutes in milliseconds

void setup() {
  lcd.begin(16, 2);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(piezoPin, OUTPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  distanceInch = duration * 0.0133 / 2;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time left: ");
  
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - previousMillis;

  if (elapsedTime < interval) {
    // Display the remaining time
    lcd.print((interval - elapsedTime) / 60000); // Convert milliseconds to minutes
    lcd.print(" min");
    
    // Piezo
    if (distanceInch > 5) {
      for (int i = 0; i < 45; i++) {
        tone(SPEAKER_PIN, FIRST_MELODY[i], 1500 / FIRST_DURATIONS[i]);
        delay(1800 / FIRST_DURATIONS[i]);
        noTone(SPEAKER_PIN);

        if (distanceInch < 5)
          break;
  }
    } else {
      noTone(piezoPin);
    }
  } else {
    // Timer expired
    lcd.print("Get Moving!");
    noTone(piezoPin);

    // Reset the timer
    previousMillis = currentMillis;
  }

  if (distanceInch > 20) {
    // Reset the timer when the distance is greater than 20 inches
    previousMillis = currentMillis;
  }

  delay(10);
}
