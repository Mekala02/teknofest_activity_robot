#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Deneyap_Servo.h>

// DeneyapKart Erisim Noktasi (AP) ismi
const char *ssid = "DeneyapKart";
// DeneyapKart Erisim Noktasi (AP) sifresi
const char *password = "dydkmpv10";

// WiFi server initialization
WiFiServer server(80);

int servo_pin = D8;
// Motor A connections
int enA = D11; // D11
int in1 = D12; // D12
int in2 = D13; // D13
// Motor B connections
int enB = D10; // D10
int in3 = D14; // D14
int in4 = D15; // D15
Servo myservo;
MFRC522 mfrc522(D4,D0);
int servo_position;
int servo_command;
int servo_open = 80;
int servo_close = 150;

unsigned long StartTime = millis();
unsigned long CurrentTime = millis();

String rfid_close = "53 33 27 3b";
String rfid_open = "09 c3 f4 6e";

void setup()
{
  myservo.attach(servo_pin); 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring Access Point...");

  // Start the access point
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
  myservo.write(servo_open);
}

void loop()
{
  if (mfrc522.PICC_IsNewCardPresent())
  {
    if (fnc_user_ID().substring(0)==rfid_open)
    {
      myservo.write(servo_open);
    }
    else if(fnc_user_ID().substring(1)==rfid_close)
    {
      myservo.write(servo_close);
    }
  }
  // else
  // {
  //   servo_command = servo_open;
  // }

  // if (servo_command != servo_position)
  // {
  //   CurrentTime = millis();
  //   if (CurrentTime - StartTime > 500)
  //   {
  //     servo_position = servo_command;
  //     myservo.write(servo_position);
  //     StartTime = CurrentTime;
  //   }
  // }

  // Check for client connections
  WiFiClient client = server.available();  
  // If client is connected
  if (client)
  {
    Serial.println("New client request.");
    String currentLine = "";
    while (client.connected()) 
    {
      // If data available from client
      if (client.available()) 
      {
        // Read character from client
        char c = client.read();
        // Print character to serial monitor
        Serial.write(c);
        // If end of line is reached
        if (c == '\n') 
        {
          // If it's an empty line
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><title>Robot Control</title></head>");
            client.println("<body>");
            client.println("<h1>Control the robot:</h1>");
            client.println("<a href=\"/N\">Stop</a><br>");
            client.println("<a href=\"/F\">Forward</a><br>");
            client.println("<a href=\"/B\">Backward</a><br>");
            client.println("<a href=\"/L\">Left</a><br>");
            client.println("<a href=\"/R\">Right</a><br>");

            // if (mfrc522.PICC_IsNewCardPresent())
            // {
            //   client.println(fnc_user_ID());
            // }

            client.println("</body>");
            client.println("</html>");
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /F"))
        {
          forward();
        }
        else if (currentLine.endsWith("GET /B"))
        {
          backward();
        }
        else if (currentLine.endsWith("GET /R"))
        {
          right();
        }
        else if (currentLine.endsWith("GET /L"))
        {
          left();
        }
        else if (currentLine.endsWith("GET /N"))
        {
          stopMotors();
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}


// Function to stop both motors
void stopMotors()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Function to move forward
void forward()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 150);
  analogWrite(enB, 150);
}

// Function to move backward
void backward()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 150);
  analogWrite(enB, 150);
}

// Function to turn left
void left()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 150);
  analogWrite(enB, 150);
}

// Function to turn right
void right()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 150);
  analogWrite(enB, 150);
}

String fnc_user_ID()
{
  mfrc522.PICC_ReadCardSerial();
  String user_ID="";
  for (byte i=0; i<mfrc522.uid.size; i++)
  {
    user_ID += (mfrc522.uid.uidByte[i] < 0x10 ? "0": " ") + String(mfrc522.uid.uidByte[i], HEX);
  }
  return user_ID;
}