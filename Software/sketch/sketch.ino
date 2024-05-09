#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(D4,D0);


String fnc_user_ID(){
mfrc522.PICC_ReadCardSerial();
String user_ID="";
for (byte i=0; i<mfrc522.uid.size; i++){
user_ID += (mfrc522.uid.uidByte[i] < 0x10 ? "0": " ") + String(mfrc522.uid.uidByte[i], HEX);
}
return user_ID;
}


void setup() {
SPI.begin();
mfrc522.PCD_Init();
Serial.begin(115200);
pinMode(D12, OUTPUT);
}

void loop() {
if (mfrc522.PICC_IsNewCardPresent()) {
Serial.print("Kart ID:");
Serial.println(fnc_user_ID());
digitalWrite(D12, HIGH);
delay(10);
} else {
digitalWrite(D12, LOW);
}
}