#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>

#define NPAGES 5

typedef struct {
	const char train;
	const char* stop_id;
	const char* name;
} display_page;

const char ssid[] = "zemog";
const char password[] = "sugzemog27";
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

WiFiClientSecure client;
int bytes_read;
int ptr = 0;
char buf[4095];

display_page pages[NPAGES] = {
	{'A', "A24", "59 street"},
	{'C', "A24", "59 street"},
	{'E', "A24", "59 street"},
	{'1', "125", "59 street"},
	{'E', "R14", "57 street"}
};

void setup() {
	Serial.begin(9600);
	WiFi.begin(ssid, password);
	Serial.println();

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.printf("[WIFI] Waiting to connect to %s\n", ssid);
	}
	Serial.printf("[WIFI] Connected to %s\n", ssid);
	client.setCACert(ca_cert);
}

void loop() {
	delay(1000);
	bytes_read = 0;

	display_page* page = &pages[ptr++];

	Serial.println("[DATA] Requesting data for ACE trains from MTA's API");
	if (client.connect(server, 443)) {
		Serial.println("[DATA] Connected to server");
		client.printf("GET /fetch?train=%c&stop-id=%s&n-results=2 HTTP/1.0\r\n",
				page->train, page->stop_id);
		client.printf("Host: %s\r\n", server);
		client.println();
	} else {
		Serial.println("[DATA] Request failed");
		return;
	}
	Serial.println("[DATA] Finished API request");

	while (!client.available()) {
		delay(100);
	}

	while (client.available() && bytes_read < sizeof(buf)) {
		char c = client.read();
		buf[bytes_read++] = c;
	}

	buf[bytes_read] = 0;
	client.stop();

	ptr = ptr % NPAGES;

	Serial.printf("[DATA]\n%s\n", buf);
}
