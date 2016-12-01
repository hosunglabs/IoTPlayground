#include <AddicoreRFID.h>
#include <SPI.h>
 
#define    uchar    unsigned char
#define    uint    unsigned int
 
//4 bytes tag serial number, the first 5 bytes for the checksum byte
uchar serNumA[5];
 
uchar fifobytes;
uchar fifoValue;
 
AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module
 
/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;
const int speakerPin = 8; //스피커가 연결된 디지털핀 설정
int RED1=3;
int RED2=6;
int BLUE1=2;
int BLUE2=5;
int GREEN1=4;
int GREEN2=7;
int COUNT1=0;
int COUNT2=0;
int ECOUNT1=0;
int ECOUNT2=0;
int A=0;
int B=0;
int C=0;
float cds_volt;
int adc_value=0.0;

//Maximum length of the array
#define MAX_LEN 16
 
void setup() {                
  Serial.begin(9600);                        // 시리얼 모니터 출력 설정
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);
  pinMode(RED1,OUTPUT);
  pinMode(RED2,OUTPUT);
   pinMode(BLUE1,OUTPUT);
    pinMode(BLUE2,OUTPUT);
    pinMode(GREEN1,OUTPUT);
    pinMode(GREEN2,OUTPUT);
  myRFID.AddicoreRFID_Init(); 
 digitalWrite(RED1,LOW); 
  digitalWrite(RED2,LOW); 
   digitalWrite(BLUE1,HIGH); 
    digitalWrite(BLUE2,HIGH);
    digitalWrite(GREEN1,HIGH); 
    digitalWrite(GREEN2,HIGH); 
}
 
void loop()
{
  adc_value=analogRead(1);
  cds_volt=adc_value*(5.0/1023.0);
 
  if(cds_volt>=2)
  {
    if(C==0)
    {
    Serial.println("comming the dark when kids go to home. partents call back the kids.");
    Serial.println("turn on the light");
     digitalWrite(RED1,HIGH ); 
  digitalWrite(RED2,HIGH); 
   digitalWrite(BLUE1,LOW); 
    digitalWrite(BLUE2,LOW);
    digitalWrite(GREEN1,HIGH); 
    digitalWrite(GREEN2,HIGH); 
    C++;
    }
    
  }
  else if(cds_volt<2)
  {

      digitalWrite(RED1,HIGH); 
     digitalWrite(RED2,HIGH); 
    digitalWrite(BLUE1,HIGH); 
    digitalWrite(BLUE2,HIGH);
    digitalWrite(GREEN1,HIGH); 
    digitalWrite(GREEN2,HIGH); 
    C=0;
  }
   int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
   if(A==1)
   {
   if (sensorValue1>=500)
  {
    ECOUNT1+=1;
  }
   }
   if(B==1)
   {
     if(sensorValue1>=500)
     {
       ECOUNT2++;
     }
   }
  String com="";
  while (Serial.available() >0)// 시리얼 입력이 있을때
    {
        com += (char) Serial.read(); // 한번에 한문자를 읽으면
        delay(5); // 5밀리초 동안 대기하고 다음 문자를 읽을 준비를 합니다.
    }
  delay(1000);
  delay(1);        // delay in between reads for stability
  uchar i, tmp, checksum1;
    uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;    //Selection operation block address 0 to 63
  String mynum = "";
  str[1] = 0x4400;
        
    //RFID 태그의 타입을 리턴
    status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);    
    if (status == MI_OK)    //MIFARE 카드일때만 작동
    {
          tone(speakerPin,2000,100);

            
    }
 
  //RFID 충돌방지, RFID 태그의 ID값(시리얼넘버) 등 저장된 값을 리턴함. 4Byte
    status = myRFID.AddicoreRFID_Anticoll(str);
    if (status == MI_OK)      //MIFARE 카드일때만 작동
    {
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];

            if(str[0]==2)
            {
              COUNT1+=1;
              if(COUNT1%2==1){
             Serial.println("202 203 East Lake Apartments Incheon jeonuijeong children have admission to the playground.");
              A=1;
              }
              else if(COUNT1%2==0){
                Serial.println("202 203 East Lake Apartments Incheon jeonuijeong children have get out to the playground.");
                 Serial.println("you earn ");
                 Serial.println(ECOUNT1);
                  Serial.println("point");
                 Serial.println("jeonuijeong children!!!!!!! \n");
                 A=0;
                 ECOUNT1=0;
              } 
            
 
            }
   else if(str[0]==174)
            {
             COUNT2+=1;
              if(COUNT2%2==1){
             Serial.println("202 103 East Lake Apartments Incheon jeonuijeong children have admission to the playground.");
             B=1;
              }
              else if(COUNT2%2==0){
                Serial.println("202 103 East Lake Apartments Incheon jeonuijeong children have get out to the playground.");
                Serial.println("you earn ");
    Serial.println(ECOUNT2);
    Serial.println("point");
    Serial.println("jeonuijeong children!!!! \n");
    B=0;
    ECOUNT2=0;
                
              } 
 
              
            }
            Serial.println();
            delay(1000);
             
    

            
    }
        myRFID.AddicoreRFID_Halt();           //Command tag into hibernation              
}
