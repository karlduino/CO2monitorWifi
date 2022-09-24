// CO2 monitor with Senseair S8 and 16x2 LCD display

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "s8_uart.h"
#include <Wire.h>
#include <LCD_I2C.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include "private.h"

#define STARTUP_DELAY_MS    8000
#define TIME_BETW_UPDATE_MS 5000
#define SAFE_LEVEL  1000
#define BAD_LEVEL   1400
#define MAX_WIFI_TRIES 10
#define MS_BETW_POSTS 60000 // 60 sec between posts to google form
#define MS_TIMEOUT 5000

// Pins used for SenseAir S8 sensor
#define S8_TX_PIN 13 // yellow (middle pin on 5-pin header)
#define S8_RX_PIN 12 // green  (to left of that, looking down at the label)

// The RX/TX labels are really confusing
//
// what're defined here as S8_RX and S8_TX are the pins
// where the S8 is receiving and transmitting, respectively
//
// They're connected to the opposite pins on the arduino,
// RX <-> TX, TX <-> RX
//
// and the SoftwareSerial object should get the
// arguments (arduino_RX, arduino_TX)
SoftwareSerial S8_serial(S8_TX_PIN, S8_RX_PIN);

// set up special characters for LCD display
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t subscript2[8] = {0x0, 0x0, 0x0, 0x1E, 0x03, 0x06, 0x0C, 0x1F };
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t smile[8] = {0x0,0x0,0xa,0x0,0x0,0x11,0xe,0x0};
uint8_t frown[8] = {0x0,0x0,0xa,0x0,0x0,0xe,0x11,0x0};

S8_UART *sensor_S8;
S8_sensor sensor;

LCD_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

WiFiClientSecure client;

#ifdef HOME
// #define PRIVATE_SSID "ssid"         // defined in private.h
// #define PRIVATE_PASSWORD "password" // defined in private.h
#else
#define EAP_IDENTITY "anonymous@wisc.edu"
// #define EAP_USERNAME "username" // defined in private.h
// #define EAP_PASSWORD "password" // defined in private.h
const char* ssid = "eduroam"; // Eduroam SSID
#endif

const char* api_host = "docs.google.com";
unsigned long last_post_time=0;
unsigned long last_measurement=0;

// random session ID
String sessionID;

// docs.google.com root certificate
const char* root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n" \
    "A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n" \
    "b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n" \
    "MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n" \
    "YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n" \
    "aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n" \
    "jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n" \
    "xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n" \
    "1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n" \
    "snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n" \
    "U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n" \
    "9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n" \
    "BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n" \
    "AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n" \
    "yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n" \
    "38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n" \
    "AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n" \
    "DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n" \
    "HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n"                            \
    "-----END CERTIFICATE-----\n";


void setup() {

  // start up the LCD display
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting up...");
  lcd.createChar(0, subscript2);
  lcd.createChar(1, heart);
  lcd.createChar(2, smile);
  lcd.createChar(3, frown);

  // Initialize S8 sensor
  S8_serial.begin(S8_BAUDRATE);
  sensor_S8 = new S8_UART(S8_serial);

  // Check if S8 is available
  sensor_S8->get_firmware_version(sensor.firm_version);
  int len = strlen(sensor.firm_version);
  int n_tries = 1;
  while (len == 0 && n_tries < 120) {
      delay(250);
      sensor_S8->get_firmware_version(sensor.firm_version);
      len = strlen(sensor.firm_version);
      n_tries++; // give up after 120 tries
  }

  // Show basic S8 sensor info
  lcd.setCursor(0,0);
  lcd.print("SenseAir S8      ");
  lcd.setCursor(0,1);
  lcd.print("Firmware:        ");
  lcd.setCursor(10,1);
  if (len == 0) lcd.print("N/A");
  else lcd.print(sensor.firm_version);
  sensor.sensor_id = sensor_S8->get_sensor_ID();

  // start up wifi
  #ifdef HOME
  WiFi.begin(PRIVATE_SSID, PRIVATE_PASSWORD);
  #else
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode

  // cert-file-free eduroam with PEAP (or TTLS)
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
  #endif

  int wifi_tries = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    wifi_tries++;
    if(wifi_tries > MAX_WIFI_TRIES) {
      break;
    }
  }

  // google root certificate
  client.setCACert(root_ca);

  // create a random session ID (15 random digits)
  randomSeed(analogRead(0));
  for(int i=0; i<15; i++) {
    sessionID += random(0,9);
  }

  // wait a bit to try to avoid first reading of 0 ppm
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Warming up...   ");

  // beating heart
  if(STARTUP_DELAY_MS > n_tries * 250) { // beating heart
    int n_beats = (STARTUP_DELAY_MS - n_tries*250)/1000 + 1;

    for(int beat=0; beat < n_beats; beat++) {
      lcd.setCursor(14,1);
      lcd.printByte(1);
      delay(800);

      lcd.setCursor(14,1);
      lcd.print(" ");
      delay(200);
    }
  }

  // constant part of the display
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("ppm CO");
  lcd.setCursor(11,0);
  lcd.printByte(0);  // subscript 2

  lcd.setCursor(0,1);
  lcd.print("max ");
  lcd.setCursor(9,1);
  lcd.print("ave ");
}

int max_co2 = 0;
double ave_co2 = 0.0;
unsigned long n_measurements = 0;

void loop() {

  char buffer[17]; // to hold formatted print

  if(last_measurement==0 || (millis() - last_measurement > TIME_BETW_UPDATE_MS)) {
      last_measurement = millis();

      // clear bits from the display
      lcd.setCursor(0,0);
      lcd.print("    ");
      lcd.setCursor(4,1);
      lcd.print("    ");
      lcd.setCursor(13, 0);
      lcd.print("   ");
      lcd.setCursor(12, 1);
      lcd.print("    ");

      // Get and display CO2 measure
      sensor.co2 = sensor_S8->get_co2();
      lcd.setCursor(0,0);
      sprintf(buffer, "%4d", sensor.co2);
      lcd.print(buffer);

      // max since start-up
      if (sensor.co2 > max_co2) { max_co2 = sensor.co2; }
      lcd.setCursor(4,1);
      sprintf(buffer, "%-4d", max_co2);
      lcd.print(buffer);

      // average
      if(n_measurements == 0) {
          ave_co2 = (double) sensor.co2;
      } else {
          ave_co2 = (ave_co2 * (double)n_measurements + (double)sensor.co2) / (double)(n_measurements+1);
      }
      n_measurements++;
      int ave_co2_int = round(ave_co2);
      lcd.setCursor(12,1);
      sprintf(buffer, "%4d", ave_co2_int);
      lcd.print(buffer);

      // print warning
      lcd.setCursor(15,0);
      if(sensor.co2 <= SAFE_LEVEL) {
          lcd.printByte(2); // smile
      } else if(sensor.co2 < BAD_LEVEL) {
          lcd.printByte(3); // frown
      } else {
          lcd.setCursor(13,0);
          lcd.print("! !");
          lcd.setCursor(14,0);
          lcd.printByte(3); // frown
      }
  } // if update of measurement

  // post form data?
  if(WiFi.status() == WL_CONNECTED && (last_post_time==0 || (millis() - last_post_time > MS_BETW_POSTS))) {
      last_post_time = millis();

      // connect to client
      const int httpPort = 443;
      if(!client.connect(api_host, httpPort)) {
          return;
      }

      // build up URI for request
      String url = "https://";
      url += api_host;
      url += PRIVATE_API_CALL; // defined in private.h
      url += "&";
      url += PRIVATE_ENTRY1; // defined in private.h
      url += "=";
      url += SERIAL_NUMBER;  // defined in private.h
      url += "&";
      url += PRIVATE_ENTRY2; // defined in private.h
      url += "=";
      url += sessionID; // random
      url += "&";
      url += PRIVATE_ENTRY3; // defined in private.h
      url += "=";
      url += sensor.co2;
      url += "&";
      url += PRIVATE_ENTRY4; // defined in private.h
      url += "=";
      url += max_co2;
      url += "&";
      url += PRIVATE_ENTRY5; // defined in private.h
      url += "=";
      url += ave_co2;

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + api_host + "\r\n" +
                   "Connection: close\r\n\r\n");
      unsigned long timeout = millis();
      while(client.available() == 0) {
          if(millis() - timeout > MS_TIMEOUT) {
              client.stop();
              return;
          }
      }

      client.stop();
  } // end post form data

  delay(100);

}
