#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    if (buffer[i] < 0x10) Serial.print("0");
    Serial.print(buffer[i], HEX);
    if (i < bufferSize - 1) Serial.print(" ");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial (optional)
  Serial.println(F("RC522 debug starting..."));

  SPI.begin();          // Init SPI bus
  delay(50);
  mfrc522.PCD_Init();   // Init MFRC522
  delay(50);

  // report firmware/version (if available)
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Version: 0x")); Serial.println(version, HEX);
  if (version == 0x00 || version == 0xFF) {
    Serial.println(F("Warning: could not read version. Check wiring and 3.3V power!"));
  } else {
    Serial.println(F("RC522 seems alive."));
  }
  Serial.println(F("Ready — place a tag near the antenna."));
}

void loop() {
  // look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("Failed to read card serial."));
    delay(200);
    return;
  }

  Serial.print(F("Card detected - UID: "));
  printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("SAK: 0x")); Serial.println(mfrc522.uid.sak, HEX);

  // halt PICC and stop encryption on PCD
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(700);
}
