#include <Fingerprint.h>
#include <EEPROM.h>

#define MODEM_RX 16
#define MODEM_TX 17
#define mySerial Serial2
Fingerprint finger = Fingerprint(&mySerial);

int tombol = 34;//34.35.36.39 input only
int valbol = 0;
int tombel = 35;
int valbel = 0;

//int led1 = 22;
uint8_t urutan;
uint8_t urutanset;
uint8_t memory;
uint8_t memoryset;
uint8_t id;


void setup()  
{
  Serial.begin(9600);
  finger.begin(57600);
  pinMode(tombol, INPUT);
  pinMode(tombel, INPUT);
  //pinMode(led1, OUTPUT);
  //digitalWrite(led1, LOW);

   if (finger.verifyPassword()) 
   {
    Serial.println("Found fingerprint sensor!");
   }
}


uint8_t readnumber(void) 
{
  finger.getTemplateCount();
  urutan = finger.templateCount;
  valbol = digitalRead(tombol);
  Serial.print("valbol = "); Serial.println(valbol);    
  if(valbol == HIGH) 
  {
  //digitalWrite(led1, HIGH);
  urutan++;
  urutanset = urutan;
  }
  
  if(valbol == HIGH) 
  {
  EEPROM.write(urutanset,memoryset);  
  }
  
  delay(1000);
  //digitalWrite(led1, LOW);
  EEPROM.read(urutanset);
  Serial.write(urutanset);

  uint8_t num;
  num = urutanset;
  Serial.print("num = "); Serial.println(num);
}

void loop() 
{
  valbol = digitalRead(tombol);  
  if(valbol == HIGH) 
  {
  readnumber();
  }
  
  valbel = digitalRead(tombel);  
  if(valbel == HIGH) 
  {
  Serial.println("tempel jari...");
  id = urutanset;  
  Serial.print("ID"); Serial.println(id);  
  getFingerprintEnroll() ; 
  }

}

uint8_t getFingerprintEnroll() 
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}