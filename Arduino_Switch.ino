#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define relay 5

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;


byte nuidPICC[4];
int state = 0;
String card[2] = {"6af1c880", "1916f4b2"};

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  pinMode(relay, OUTPUT);
  rfid.PCD_Init(); // Init MFRC522
  digitalWrite(relay, HIGH);
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

}

void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;


  if ( ! rfid.PICC_ReadCardSerial())
    return;

  if (state == 1) {
    digitalWrite(relay, HIGH);
    Serial.println("matikan");
  }
  for (int a = 0; a <= sizeof(card); a++) {
    if (String(printHex(rfid.uid.uidByte, rfid.uid.size)) == card[a]) {
      Serial.println("ketemu");
      if (state == 1) {
        digitalWrite(relay, HIGH);
        Serial.println("matikan");
        state = 0;
      } else {
        digitalWrite(relay, LOW);
        state = 1;
      }
    } else Serial.println("Salah!");
  }
  Serial.println(String(printHex(rfid.uid.uidByte, rfid.uid.size)));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

}

String printHex(byte *buffer, byte bufferSize) {
  String hex = "";
  String zz[4];
  for (byte i = 0; i < bufferSize; i++) {

    buffer[i] < 0x10 ? " 0" : " ";

    hex = hex + String(buffer[i], HEX);
  }
  //Serial.println("DATA="+hex);
  return hex;
}
