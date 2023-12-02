#include <FastLED.h>
#include <WiFi.h>


#define NUM_LEDS 120
#define DATA_PIN 5
#define LED_TYPE WS2812B

#define LED_R       17
#define LED_B       18
#define LED_G       19  

CRGB leds[NUM_LEDS];

char *tokenList[10];


// Replace with your network credentials
const char* ssid = "magnus🥚phone";
const char* password = "qwer5671";  

//const char* ssid = "ROM Fritz 6591";
//const char* password = "DINGO@hulm*0314#"; 

// Set web server port number to 80
WiFiServer server(80);
String mode = "OFF";


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 100;


CRGB hexToRGB(String hexColorString) {
  // Remove '#' if present
  hexColorString.replace("#", "");

  // Convert the hexadecimal string to a numeric value
  uint32_t hexColor = (uint32_t)strtol(hexColorString.c_str(), NULL, 16);

  // Extract the red, green, and blue components from the numeric value
  uint8_t r = (hexColor >> 16) & 0xFF;
  uint8_t g = (hexColor >> 8) & 0xFF;
  uint8_t b = hexColor & 0xFF;

  // Return the CRGB object representing the RGB color
  return CRGB(r, g, b);
}

void connectToWifi() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    handleLEDs();
    FastLED.show();
  }
  
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  Serial.println("\nWifi connected!");

  
}

void sendSuccessResponse(WiFiClient &client) {
  client.println("HTTP/1.1 204 No Content");
  client.println("Connection: close");
  client.println();
}

void handleClient(WiFiClient client) {
    String postData = "";                   // String to hold the HTTP request
    boolean bodyStarted = false;

    // reead from client until disconnection or timeout
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // If there's bytes to read from the client,
        char c = client.read();             // Read a byte
        postData += c;
      }
    }

    sendSuccessResponse(client);
    
    // split recieved data into header and body
    String body;
    String header;
    int emptyLineIndex = postData.indexOf("\r\n\r\n");
    if (emptyLineIndex != -1) {
      header = postData.substring(0, emptyLineIndex);
      body = postData.substring(emptyLineIndex + 4);
    } else {
      Serial.println("Empty line not found.");
    }
    // end
    client.stop();
  

    // parse Body
    String head = body.substring(0,3);
    Serial.println("head -> " + head);

    if (head == "COL") {
      body = body.substring(3);
      Serial.println("body ->" + body);
      int R, G, B;
      CRGB color; 
      if (body[0] == '#') {
        color = hexToRGB(body);
      }
      else {
        // Find the positions of commas
        int firstComma = body.indexOf(',');
        int secondComma = body.indexOf(',', firstComma + 1);

        // Extract substrings and convert to integers
        R = body.substring(0, firstComma).toInt();
        G = body.substring(firstComma + 1, secondComma).toInt();
        B = body.substring(secondComma + 1).toInt();

        color = CRGB(R,G,B);
      }

        fill_solid(leds, NUM_LEDS, color);
        mode = "COL";
    }
    else if (head == "WAV"){
      mode = "WAV";
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
    else if (head == "OFF") 
    {
      mode = "OFF";
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
    else
      Serial.println("head not implemented: " + head);
    FastLED.show();
}

void handleLEDs() {
  if (mode == "COL"){ 
    // do nothing
  }

  else if (mode == "OFF") {
    // do nothing
  }

  else if (mode == "WAV") {
    // do nothing
    updateLEDs(leds);
    shiftAll(leds);
    delay(80);
  }
    
  
}

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(64);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  connectToWifi();

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    handleClient(client);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi connection lost.");
    connectToWifi();
  }

  handleLEDs();
  FastLED.show();
}
