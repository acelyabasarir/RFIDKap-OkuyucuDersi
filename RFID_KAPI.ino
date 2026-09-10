#include <SPI.h>        // SPI kütüphanesi
#include <MFRC522.h>    // RFID RC522 kütüphanesi
#include <Servo.h>      // Servo motor kütüphanesi

int RST_PIN = 9;        // RC522 RST pini
int SS_PIN = 10;        // RC522 SDA/SS pini
int servoPin = 8;       // Servo motor sinyal pini

Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);

// Yetkili kart ID'si
byte ID[4] = {238, 60, 248, 4};

void setup() {

  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  motor.attach(servoPin);

  // Servo başlangıçta kapalı konumda
  motor.write(0);

  Serial.println("Sistem hazir.");
  Serial.println("Kartinizi okutun...");
}

void loop() {

  // Yeni kart yoksa bekle
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Kart okunamazsa bekle
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Okunan kart ID'sini ekrana yazdır
  ekranaYazdir();

  // Kart ID'sini kontrol et
  if (rfid.uid.uidByte[0] == ID[0] &&
      rfid.uid.uidByte[1] == ID[1] &&
      rfid.uid.uidByte[2] == ID[2] &&
      rfid.uid.uidByte[3] == ID[3]) {

    Serial.println("Yetkili Kart");
    Serial.println("Kapi acildi");

    // Servo motoru aç
    motor.write(180);

    // 3 saniye açık bekle
    delay(3000);

    // Servo motoru kapat
    motor.write(0);

    Serial.println("Kapi kapandi");

    delay(1000);
  }

  else {

    Serial.println("Yetkisiz Kart");

    // Yetkisiz kartta motor kapalı kalsın
    motor.write(0);

    delay(1000);
  }

  // Kart iletişimini sonlandır
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}


// Kart ID'sini Serial Monitor'e yazdırır
void ekranaYazdir() {

  Serial.print("ID Numarasi: ");

  for (int sayac = 0; sayac < rfid.uid.size; sayac++) {

    Serial.print(rfid.uid.uidByte[sayac]);

    if (sayac < rfid.uid.size - 1) {
      Serial.print(" ");
    }
  }

  Serial.println();
}
