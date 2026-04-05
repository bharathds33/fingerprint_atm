#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 =8 ;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int balance=0;
SoftwareSerial mySerial(2, 3);
int OTP=0,otp=0;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

String iot="";
#define b1 7
#define b2 6
#define b3 5
int motor=4;

unsigned char key[5];
unsigned int temp=0;
signed int val=0;


uint8_t id=1;
uint8_t dummyid=1;
int buzzer=A0;
int buzzer1=A1;

int idnumber=0;
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;

    
void setup()  
{
   pinMode(motor, OUTPUT);digitalWrite(motor,LOW);
    pinMode(buzzer1, OUTPUT);digitalWrite(buzzer1,HIGH);beep();
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
 lcd.begin(16, 2);delay(1000);
 lcd.setCursor(0,0); lcd.print("  ATM MACHINE");delay(1000);
 //lcd.setCursor(0,1); lcd.print("");delay(1000);
 Serial.begin(9600);

  finger.begin(57600);     // set the data rate for the sensor serial port
  
 pinMode(buzzer,OUTPUT);digitalWrite(buzzer,HIGH);
 
 

  while (!Serial);  
  delay(100);
 //Serial.println("\n\nFingerprint code by using Arduino");

  if (finger.verifyPassword()) {
   //// Serial.println("Found fingerprint sensor!");
  } else {
   // Serial.println("Did not find fingerprint sensor :(");
   // Serial.println("Please turn off and turn on the power");
    while (1) { delay(1); }
  }
delay(1000);lcd.clear();lcd.print("Ready to use>>>");  
}

void beep()
{
digitalWrite(buzzer,LOW);delay(1000);digitalWrite(buzzer,HIGH);delay(10);  
}

int keypad()
 {
  lcd.clear();lcd.print("USE 5 DIGIT MODE");delay(2000);lcd.clear(); 
  do
  {
   int i=digitalRead(b1);
  int d=digitalRead(b3);
  int e=digitalRead(b2);

  if(i==LOW)
  {
   val=val+1; 
   if(val==10)
   val=0;
   lcd.setCursor( temp,0);lcd.print(val);delay(500);
  }
  if(d==LOW)
  {
   val=val-1;
   if(val<0)
   val=9;
   if(val>9)
   val=0; 
   lcd.setCursor(temp,0);lcd.print(val);delay(500);
  }
  
if(e==LOW)
{
 key[temp]=val;
 lcd.setCursor(temp,0);lcd.print(val);delay(500);
 temp=temp+1;
 val=0;
}
  }while(temp<=4);

  
 if(temp==5)
 {
  lcd.clear();lcd.setCursor(3,1);
  lcd.print("key=");delay(100);
  lcd.print(key[0]);delay(100);
  lcd.print(key[1]);delay(100);
  lcd.print(key[2]);delay(100);
  lcd.print(key[3]);delay(100);
  lcd.print(key[4]);delay(100);
  temp=0;
   int amount=((key[0]*10000)+(key[1]*1000)+(key[2]*100)+(key[3]*10)+(key[4]*1));
lcd.clear();lcd.print("KEY:");lcd.print(amount);delay(1000);
return amount;

  
 }


}


void loop()                     // run over and over again
{
back:

 if(digitalRead(b1)==LOW)
 {
  lcd.clear();lcd.print("Enrolling....");delay(1000);
  getFingerprintEnroll();
  delay(1000);
  lcd.clear();
  lcd.clear();lcd.print("Ready to use>>>");  
 }


 
 if(digitalRead(b3)==LOW)
 {
   lcd.clear();lcd.print("identifing.....");delay(1000);
  idnumber=getFingerprintID();
  lcd.clear();lcd.print("FINGER ID:");lcd.print(idnumber);delay(1000);
  if(idnumber==1 || idnumber==2)
  {
  lcd.clear();lcd.print("Ac.No:XXX12345");delay(1000);
  lcd.setCursor(0,1);lcd.print("NAME:BHARATH");delay(1000);
  lcd.clear();lcd.print("BALANCE:");lcd.print(balance);delay(1000);
  //L iot="Ac.No:XXX12345 used by ATM NUMBER 123 account balance is:"+String(balance);delay(1000);Serial.println(iot);delay(1000);
  lcd.setCursor(0,1);lcd.print("ENTER CHOICE");
  lcd.clear();lcd.print("1:DEPOSIT");
  lcd.setCursor(0,1);lcd.print("2.withdrawal");delay(2000);
  int ch=keypad();
  if(ch==1)
  {
  lcd.clear();lcd.print("DEPOSIT SELECTED");
  lcd.setCursor(0,1);lcd.print("ENTER AMOUNT");delay(1000);
  int de=keypad();
   balance=balance+de;delay(1000);
   lcd.clear();lcd.print("BALANCE:");lcd.print(balance);delay(1000); 
    lcd.clear();lcd.print("THANK YOU");delay(1000);
    //iot=String(de)+"credited Ac.No:XXX12345 used by ATM NUMBER 123 account balance is:"+String(balance);delay(1000);Serial.println(iot);delay(1000);
    delay(1000);lcd.clear();lcd.print("Ready to use>>>");
    goto back;
  }

 else if(ch==2)
  {
  lcd.clear();lcd.print("WITHDRAWAL SELECTED");
  lcd.setCursor(0,1);lcd.print("ENTER AMOUNT");delay(1000);
  int wi=keypad();
  if(balance>wi)
  {
    balance=balance-wi;delay(1000);
    lcd.clear();lcd.print("BALANCE:");lcd.print(balance);delay(1000);
     lcd.clear();lcd.print("THANK YOU");delay(1000);
    // iot=String(wi)+"debited Ac.No:XXX12345 used by ATM NUMBER 123 account balance is:"+String(balance);delay(1000);Serial.println(iot);delay(1000);
     delay(1000);lcd.clear();lcd.print("Ready to use>>>");
    goto back;
  }
   else
   {
    lcd.clear();lcd.print("LOW BALANCE");delay(1000);
    digitalWrite(buzzer,LOW);delay(1000);digitalWrite(buzzer,HIGH);delay(10);
   // iot=" LOW BALANCED DETECTED "+" Ac.No:XXX12345 used by ATM NUMBER 123 account balance is:"+String(balance);delay(1000);Serial.println(iot);delay(1000);
    delay(1000);lcd.clear();lcd.print("Ready to use>>>");
    goto back; 
   }
  }
  else
  {
   lcd.clear();lcd.print("TRY AGAIN");delay(1000);
   digitalWrite(buzzer,LOW);delay(1000);digitalWrite(buzzer,HIGH);delay(10);
   delay(1000);lcd.clear();lcd.print("Ready to use>>>");
    goto back; 
  }
  }
 }
 

 
}
///////////////////////////////////////Enroll////////////////////////////////////////


uint8_t getFingerprintEnroll() {

  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); ////Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    
    ////Serial.println(p);
    ////Serial.println(FINGERPRINT_OK);
    ////Serial.println(FINGERPRINT_NOFINGER);
    ////Serial.println(FINGERPRINT_PACKETRECIEVEERR);
    ////Serial.println(FINGERPRINT_IMAGEFAIL);
    switch (p) {
    case FINGERPRINT_OK:              //result 0
      ////Serial.println("Image taken");
      
      break;
    case FINGERPRINT_NOFINGER:      //result 2
      ////Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      break;
    case FINGERPRINT_IMAGEFAIL:
      ////Serial.println("Imaging error");
      goto ending;
      break;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      break;
    }
    delay(1000);
  }

  // OK success!
  ////Serial.println("Image converting.......");
  p = finger.image2Tz(1);
  ////Serial.println(p);
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      ////Serial.println("Image too messy");
      goto ending;
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      return p;
  }
  delay(1000);
  
  ////Serial.println("Remove finger");
  delay(1000);lcd.clear();lcd.print("Remove finger");delay(1000);
  delay(2000);
  p = 0;
  ////Serial.println(FINGERPRINT_NOFINGER);
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  ////Serial.println(FINGERPRINT_NOFINGER);
  ////Serial.println(p);
  //Serial.print("ID "); ////Serial.println(id);
  p = -1;
  ////Serial.println("Place same finger again");
   delay(1000);lcd.clear();lcd.print("Same finger Again");delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    ////Serial.println(p);
    switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      break;
    case FINGERPRINT_IMAGEFAIL:
      ////Serial.println("Imaging error");
      goto ending;
      break;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  ////Serial.println(p);
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      ////Serial.println("Image too messy");
      goto ending;
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      return p;
  }
  
  // OK converted!
  //Serial.print("Creating model for #");  ////Serial.println(id);
  
  p = finger.createModel();
  ////Serial.println(p);
  if (p == FINGERPRINT_OK) {
    ////Serial.println("Prints matched!");
  // lcd.clear(); lcd.print("Prints matched!");delay(1000);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    ////Serial.println("Fingerprints did not match");
    lcd.clear();lcd.print("Fingerprints did not match");delay(1000);
    goto ending;
    return p;
  } else {
    ////Serial.println("Unknown error");
    goto ending;
    return p;
  }   
  
  //Serial.print("ID "); ////Serial.println(id);
  p = finger.storeModel(id);
  ////Serial.println(p);
  delay(1000);lcd.clear();lcd.print("Place is:");lcd.print(id);delay(1000);
  if (p == FINGERPRINT_OK) {
    ////Serial.println("Stored!");
     delay(1000);lcd.clear();lcd.print("Stored");delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    ////Serial.println("Could not store in that location");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    ////Serial.println("Error writing to flash");
    goto ending;
    return p;
  } else {
    ////Serial.println("Unknown error");
    goto ending;
    return p;
  }
id = id+1;delay(100);
      if (id == 0) {                  // ID #0 not allowed, try again!
      return;
      }
ending:
delay(10);
}

///////////////////////////////////Identifying//////////////////

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      ////Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      return p;
    case FINGERPRINT_IMAGEFAIL:
      ////Serial.println("Imaging error");
      goto ending;
      return p;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      ////Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      goto ending;
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ////Serial.println("Could not find fingerprint features");
      goto ending;
      return p;
    default:
      ////Serial.println("Unknown error");
      goto ending;
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    ////Serial.println("Found a print match!");
   // lcd.clear();lcd.print("Found a print match!");delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    ////Serial.println("Did not find a match");
    lcd.clear();lcd.print("Did not find a match");delay(1000);
    digitalWrite(buzzer,LOW);delay(1000);digitalWrite(buzzer,HIGH);delay(10);
    delay(1000);lcd.clear();lcd.print("Ready to use>>>"); 
    idnumber=0;delay(1000);beep();beep();  
     //Serial.println("DEVICE_TRY_To_ACCESS_BY_UNAUTHERISED_PERSON");delay(1000);
     lcd.clear();lcd.print("Ready to use>>>");
    goto ending;
    return p;
  } else {
    ////Serial.println("Unknown error");
    goto ending;
    return p;
  }   
  
  // found a match!
  //Serial.print("Found ID #"); //Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); ////Serial.println(finger.confidence); 
  delay(1000);lcd.clear();lcd.print("Found ID #");lcd.print(finger.fingerID); delay(2000);
  return finger.fingerID;
  ending:
  delay(10);
}
//////////////////////////////////////////////////Deleting finger////////////////////////////
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    ////Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    ////Serial.println("Could not delete in that location");
    goto ending;
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    ////Serial.println("Error writing to flash");
    goto ending;
    return p;
  } else {
    //Serial.print("Unknown error: 0x"); ////Serial.println(p, HEX);
    return p;
  }   
  ending:
  delay(10);
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '*') {
       lcd.clear();lcd.print("Door opened by *1"); 
    delay(1000);
    lcd.clear();lcd.print("Door open");delay(1000);
 // digitalWrite(m1,HIGH);  digitalWrite(m2,LOW);delay(2000);  
 // digitalWrite(m1,LOW);  digitalWrite(m2,LOW);delay(2000);

  lcd.clear();lcd.print("Door close");delay(1000);
 // digitalWrite(m2,HIGH);  digitalWrite(m1,LOW);delay(2000);  
 // digitalWrite(m1,LOW);  digitalWrite(m2,LOW);delay(2000);
      stringComplete = true;
    }
  }
}