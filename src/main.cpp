#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "icon.h"
#include "page.h"

#define NRESULTS 2

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 22
#define E_PIN -1 
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 18

#define RED 0xE000
#define BLUE 0x019F
#define WHITE 0xFFFF
#define BLACK 0x0000
#define YELLOW 0xF7E0

MatrixPanel_I2S_DMA *display = nullptr;

HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN, _pins);


typedef struct {
	const char train;
	const char* stop_id;
	const char* station;
	int uptown[NRESULTS];
	int downtown[NRESULTS];
} display_page;

const char ssid[] = "Verizon_QX4SHW";
const char password[] = "tulip-hawk7-bib";
const char ca_cert[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";
const char server[] = "bcns2d5cx6.execute-api.us-west-1.amazonaws.com";
const char code_delimiter[] = " ";
const char header_delimiter[] = "\r\n\r\n";
const char ok[] = "200";

WiFiClientSecure client;
int ptr;
int npages;

Page* pages[5];

int make_request(const display_page& page) {
	Serial.printf("[REQUEST] Requesting times for %c trains at %s\n", 
			page.train, page.station);
	if (client.connect(server, 443)) {
		client.printf("GET /fetch?train=%c&stop-id=%s&n-results=%d HTTP/1.0\r\n",
				page.train, page.stop_id, NRESULTS);
		client.printf("Host: %s\r\n", server);
		client.println();
		return 0;
	}
	return 1;
}

int read_response(display_page& page) {
	int bytes_read = 0;
	char buf[4095];

	// Spin until the client has responded.
	// Requests for 1, 2, 3, 4, 5, 6, 7, and S trains take a while.
	while (!client.available()) {
		delay(100);
		if (!client.connected()) {
			Serial.println("[RESPONSE] Connection ended unexpectedly");
			client.stop();
			return 1;
		}
	}

	// Read the response into a buffer.
	while (client.available() && bytes_read < sizeof(buf)) {
		buf[bytes_read++] = client.read();
	}

	client.stop();
	buf[bytes_read - 1] = 0;

	char* data = strstr(buf, header_delimiter) + strlen(header_delimiter);
	strtok(buf, code_delimiter);
	char* response_code = strtok(NULL, code_delimiter);

	if (strcmp(response_code, ok)) {
		Serial.printf("[RESPONSE] Returned error code %s\n", response_code);
		return 1;
	}

	const int capacity = JSON_OBJECT_SIZE(4) + 2*JSON_ARRAY_SIZE(NRESULTS);
	StaticJsonDocument<capacity> doc;

	Serial.println(data);

	DeserializationError err = deserializeJson(doc, data);
	if (err) {
		Serial.printf("[RESPONSE] JSON error %s\n", err.c_str());
		return 1;
	}

	for (int i = 0; i < NRESULTS; i++) {
		page.uptown[i] = (int)doc["uptown"][i];
		page.downtown[i] = (int)doc["downtown"][i];
	}

	return 0;
}

void setup() {
	Serial.begin(9600);
	// WiFi.begin(ssid, password);
	// Serial.println();

	// while (WiFi.status() != WL_CONNECTED) {
	// 	delay(500);
	// 	Serial.printf("[WIFI] Waiting to connect to %s\n", ssid);
	// }
	// Serial.printf("[WIFI] Connected to %s\n", ssid);
	// client.setCACert(ca_cert);

	ptr = 0;
	npages = sizeof(pages) / sizeof(pages[0]);

	mxconfig.gpio.e = 18;
	mxconfig.clkphase = false;
	mxconfig.driver = HUB75_I2S_CFG::FM6126A;

	display = new MatrixPanel_I2S_DMA(mxconfig);
	display->begin();
	display->setBrightness8(100); //0-255
	display->clearScreen();
	display->fillRect(0, 0, display->width(), 11, WHITE);
	display->fillRect(0, 11, display->width(), 21, 0x31A6);

	pages[0] = new Page('A', "A24", "59 street", display, BLUE, 2);
	pages[1] = new Page('1', "125", "59 street", display, RED, 2);
	pages[2] = new Page('C', "125", "59 street", display, BLUE, 2);
	pages[3] = new Page('E', "D14", "7 ave", display, BLUE, 2);
	pages[4] = new Page('Q', "D14", "57 street", display, YELLOW, 2);
	
}

void loop() {
	// delay(1000);
	Page* page = pages[ptr++];
	ptr %= npages;

	// if (make_request(page)) {
	// 	Serial.printf("[REQUEST] Failed to make request for %c trains at %s\n",
	// 			page.train, page.station);
	// 	return;
	// }

	// if (read_response(page)) {
	// 	Serial.printf("[RESPONSE] Failed to read response for %c trains at %s\n",
	// 			page.train, page.station);
	// 	return;
	// }

	// Serial.printf("Next uptown %c trains at %s arriving in %d, %d minutes\n",
	// 			page.train, page.station, page.uptown[0], page.uptown[1]);
	// Serial.printf("Next downtown %c trains at %s arriving in %d, %d minutes\n",
	// 			page.train, page.station, page.downtown[0], page.downtown[1]);
	// Serial.println();

	// drawIcon(0, 0, red);
	// drawLetter(3, 2, ICON_1, white);
	// delay(5000);

	// drawIcon(0, 0, blue);
	// drawLetter(3, 2, ICON_A, white);
	// delay(5000);
	page->display();
	delay(5000);
}
