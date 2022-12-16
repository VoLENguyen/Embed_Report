
/// blynk tk cua ban
#define BLYNK_TEMPLATE_ID "TMPL5jSUFBWu"
#define BLYNK_DEVICE_NAME "Temp and Hum"
#define BLYNK_AUTH_TOKEN "bWEnSos4z4jXSGO5ltrAGKMBMyQsMDlO"
///////// ---- /////////
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <stdio.h>
#include <Adafruit_Sensor.h>
/* DINH NGHIA CHO DHT11*/

#define DHTPIN 10 /*chân out duoc noi với GPIO 10 */
#define DHTTYPE DHT11
#define bt 9
/* dinh nghia nut nhan va den */
/* nut nhan ao ( virtualpin) */
#define Vir_Pin1 V1
#define Vir_Pin2 V2
#define Vir_Pin3 V3
#define Vir_Pin4 V4

// nut nhan
#define BTN1 5
#define BTN2 4
#define BTN3 0
#define BTN4 2
// relay va led
#define LED_1 14
#define LED_2 12
#define LED_3 13
#define LED_4 15
/* ------- */

int relaystate1 = 0;
int relaystate2 = 0;
int relaystate3 = 0;
int relaystate4 = 0;

/* ------- */
BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;
// char SSID[] = ".......";
// char pass[] = ".......";
WiFiManager wifiManager;

/*gửi trạng thái nút nhấn từ blynk về esp*/

BLYNK_WRITE(Vir_Pin1)
{
  relaystate1 = param.asInt(); // gán giá chị cho biến V1
  digitalWrite(LED_1, !relaystate1);
  Serial.print("Relay 1 = ");
  if (relaystate1 == 0)
    Serial.println("OFF");
  else
    Serial.println("ON");
}
BLYNK_WRITE(Vir_Pin2)
{
  relaystate2 = param.asInt(); // gắn giá chị cho V2
  digitalWrite(LED_2, !relaystate2);
  Serial.print("Relay 2 ");
  if (relaystate2 == 0)
    Serial.println("OFF");
  else
    Serial.println("ON");
}
BLYNK_WRITE(Vir_Pin3)
{
  relaystate3 = param.asInt();
  digitalWrite(LED_3, !relaystate3);
  Serial.print("Relay 3 ");
  if (relaystate3 == 0)
    Serial.println("OFF");
  else
    Serial.println("on");
}
BLYNK_WRITE(Vir_Pin4)
{
  relaystate4 = param.asInt(); // gán giá chị cho biến V1
  digitalWrite(LED_4, !relaystate4);
  Serial.print("Relay 4 ");
  if (relaystate4 == 0)
    Serial.println("OFF");
  else
    Serial.println("ON");
}

/* cài đặt  thông số */

// Đồng bộ trạng thái khi mất nguồn

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(Vir_Pin1);
  Blynk.syncVirtual(Vir_Pin2);
  Blynk.syncVirtual(Vir_Pin3);
  Blynk.syncVirtual(Vir_Pin4);
}
void setup()
{
  long t = millis();
  pinMode(bt, INPUT); // bt wire D8 and 3.3V
  EEPROM.begin(512);
  Serial.begin(115200);
  // Blynk.begin(auth, SSID, pass);
  Serial.println("DHT11__test!");
  dht.begin(); // Bắt đầu đọc dữ liệu
  delay(3000);
  if (digitalRead(bt) == 1)
  {
    wifiManager.resetSettings();
    delay(1000);
  }
  //  wifiManager.resetSettings();
  // Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("ESP_8266");
  // if you get here you have connected to the WiFi
  Serial.println("connected...ok :)");
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  //-----------------------
  //-------------------------------------------
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  //-------------------------------------------
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  //-------------------------------------------
  // During Starting all Relays should TURN OFF
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  //--------------------------------------------
  Blynk.virtualWrite(Vir_Pin1, !relaystate1);
  Blynk.virtualWrite(Vir_Pin2, !relaystate2);
  Blynk.virtualWrite(Vir_Pin3, !relaystate3);
  Blynk.virtualWrite(Vir_Pin4, !relaystate4);
}

void sendSensor()
{
  float Hum = dht.readHumidity();
  // Gan gia tri do am vao bien h
  float Temp = dht.readTemperature();
  // Gan gia tri nhiet do vao bien t

  // gan value vào virtual pin trên BLynk
  Blynk.virtualWrite(V10, Hum);
  Blynk.virtualWrite(V11, Temp);

  /* Display on Serial */
  Serial.print("Hum : ");
  Serial.print(Hum);
  Serial.print("%\t");

  Serial.print("Temp : ");
  Serial.print(Temp);
  Serial.print("%\t");
  Serial.println(" *C ");
}
void Control_relay(int relay)
{
  //------------------------------------------------
  if (relay == 1)
  {
    relaystate1 = !relaystate1;
    digitalWrite(LED_1, relaystate1);
    Serial.print("Relay 1 ");
    if (relaystate1 == 0)
      Serial.println("OFF");
    else
      Serial.println("ON");
  }
  //------------------------------------------------
  else if (relay == 2)
  {
    relaystate2 = !relaystate2;
    digitalWrite(LED_2, relaystate2);
    Serial.print("Relay 2 ");
    if (relaystate2 == 0)
      Serial.println("OFF");
    else
      Serial.println("ON");
  }
  //------------------------------------------------
  else if (relay == 3)
  {
    relaystate3 = !relaystate3;
    digitalWrite(LED_3, relaystate3);
    Serial.print("Relay 3 ");
    if (relaystate3 == 0)
      Serial.println("OFF");
    else
      Serial.println("ON");
  }
  //------------------------------------------------
  else if (relay == 4)
  {
    relaystate4 = !relaystate4;
    digitalWrite(LED_4, relaystate4);
    Serial.print("Relay 4 ");
    if (relaystate4 == 0)
      Serial.println("OFF");
    else
      Serial.println("ON");
  }
}
void listen_push_buttons()
{
  //--------------------------------------------------------------------------
  if (digitalRead(BTN1) == LOW)
  {
    Control_relay(1);
    Blynk.virtualWrite(Vir_Pin1, !relaystate1); // update button state
    while (digitalRead(BTN1) == LOW)
    {
    }
  }
  //--------------------------------------------------------------------------
  else if (digitalRead(BTN2) == LOW)
  {
    Control_relay(2);
    Blynk.virtualWrite(Vir_Pin2, !relaystate2); // update button state
    while (digitalRead(BTN2) == LOW)
    {
    }
  }
  //--------------------------------------------------------------------------
  else if (digitalRead(BTN3) == LOW)
  {
    Control_relay(3);
    Blynk.virtualWrite(Vir_Pin3, !relaystate3); // update button state
    while (digitalRead(BTN3) == LOW)
    {
    }
  }
  //--------------------------------------------------------------------------
  else if (digitalRead(BTN4) == LOW)
  {
    Control_relay(4);
    Blynk.virtualWrite(Vir_Pin4, !relaystate4); // update button state
    while (digitalRead(BTN4) == LOW)
    {
    }
  }
  //--------------------------------------------------------------------------
}
void loop()
{
  Serial.println(digitalRead(bt));
  Blynk.run();
  timer.run();
  listen_push_buttons();
}