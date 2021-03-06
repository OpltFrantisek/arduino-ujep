#include <Arduino.h>
#include <SoftwareSerial.h>

#include <DTE.h>
#include <GSM.h>
#include <SMS.h>
#include <URC.h>

#define PINPOWER 9
#define DEBUG false

SoftwareSerial SSerial(10, 11);
DTE dte(SSerial, PINPOWER, DEBUG);
SMS sms(dte);
GSM gsm(dte);

void setup(void) {
  Serial.begin(9600);
  SSerial.begin(19200);
  pinMode(13,OUTPUT);
  Serial.println("Zapinam smskovac...");
  dte.powerReset();
  Serial.println("cekam na sit...");
  while (gsm.getNetworkRegistration().status != 1)
    ;
  Serial.print("pripojuji ");
  Serial.println(gsm.getOperator().oper);

 // Serial.print("Try send SMS To ");
 // Serial.println(gsm.getSubscriberNumber().number);
  //Serial.println();
}

void loop(void) {
  if (!dte.AT())
    dte.powerReset();

  if (Urc.newMessageIndication.updated) {
    struct Message m = sms.readSMS(Urc.newMessageIndication.index);
    Serial.println(m.address);
    Serial.println(m.data);
    if(m.data[0] == '1')
        digitalWrite(13,1);
    if(m.data[0] == '0')
        digitalWrite(13,0);
    m.data[0] = '0';
    Urc.newMessageIndication.updated = false;
  }
}