#include <SoftwareSerial.h>
const byte rxPin = 2; //The pin on which to recieve serial data
const byte txPin = 3; //The pin on which to transmit serial data
#define DEBUG true
String data[5];
String state, timegps, latitude, longitude;

SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin(9600); //baud rate here
  mySerial.begin(9600);
  delay(1000);
  setgps();
}

void loop() {

  getgps("AT+CGNSINF",100,DEBUG); //command to retrieve gps info
  sendData("AT+CCID", 3000, DEBUG); //shows sim card number
  sendData("AT+CREG?", 3000, DEBUG);  //shows if the network has currently indicated registration and gives location elemetnts
  sendData("AT+CGATT=1", 1000, DEBUG);  //attatches to GPRS service
  sendData("AT+CGACT=1,1", 1000, DEBUG);  //activates packet data protocol
  sendData("AT+CSTT=APN,USR,PASS", 3000, DEBUG); //sets access protocol name, username, and password if needed
  sendData("AT+CIICR", 1000, DEBUG);  //bring up wireless conection
  sendData("AT+CIFSR", 1000, DEBUG);  //gets local ip address
  //sendData("AT+CDNSGIP=\"server domain\"", 1000, DEBUG);  //queries ip given domain name
  sendData("AT+CIPSTART=\"TCP\",ipadress,port", 5000, DEBUG);  //starts tcp connection
  delay(10000);
  sendData("AT+CIPSEND=100", 1000, DEBUG);  //sends data through connection num is length of data
  sendData(data, 1000, DEBUG);  //sends the data through connection
  delay(2000);
  sendData("AT+CIPCLOSE", 1000, DEBUG); //closes tcp connection


}

void setgps(void)
{
  myserial.println( "AT+CGNSPWR=1"); //Turns on GNSS power
  delay(1000)
  myserial.println( "AT+CGNSSEQ=RMC"); //setds NMEA to RMC
  delay(1000)


}

void getgps(String command , const int timeout , boolean debug) {

  mySerial.println(command);
  long int time = millis();
  int i = 0;

  while ((time + timeout) > millis()) {
    while (mySerial.available()) {
      char c = mySerial.read();
      if (c != ‘, ’) {
        data[i] += c;
        delay(100);
      } else {
        i++;
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
} exitL:
  if (debug) {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude = data[4];
  }
}
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  mySerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (mySerial.available())
    {
      char c = mySerial.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
