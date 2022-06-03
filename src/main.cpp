#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include <string.h>

#define NPAGES 5

typedef struct {
	const char train;
	const char* stop_id;
	const char* name;
	std::string uptown[2];
	std::string downtown[2];
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
int ptr;

display_page pages[NPAGES] = {
	{'A', "A24", "59 street", {}, {}},
	{'C', "A24", "59 street", {}, {}},
	{'E', "A24", "59 street", {}, {}},
	{'1', "125", "59 street", {}, {}},
	{'E', "R14", "57 street", {}, {}}
};

int make_request(const display_page& page) {
	Serial.printf("[REQUEST] Requesting times for %c trains at %s\n", 
			page.train, page.name);
	if (client.connect(server, 443)) {
		client.printf("GET /fetch?train=%c&stop-id=%s&n-results=2 HTTP/1.0\r\n",
				page.train, page.stop_id);
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
	// TODO: Bug test this for infinite loops.
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

	buf[bytes_read - 1] = 0;
	client.stop();
	Serial.printf("[RESPONSE]\n%s\n", buf);

	std::string response(buf);
	return 0;
}

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
	ptr = 0;
}

void loop() {
	delay(1000);
	display_page& page = pages[ptr];
	ptr = (++ptr) % NPAGES;

	if (make_request(page)) {
		Serial.printf("[REQUEST] Failed to make request for %c trains at %s\n",
				page.train, page.name);
		return;
	}

	if (read_response(page)) {
		Serial.printf("[RESPONSE] Failed to read response for %c trains at %s\n",
				page.train, page.name);
		return;
	}
}
