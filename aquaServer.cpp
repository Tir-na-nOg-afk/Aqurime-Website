#include <WebServer.h>
#include <aquaServer.h>
#include <WiFi.h>
#include <DisplayHandler.h>
#include "shared.h"

const char* ssid = "Smth"; //This should be altered to whatever Darius has.
const char* password = "Password";
WebServer server(80);

#include"pages.h"

void apiMotor(){
  char buf[200];
  snprintf(buf, sizeof(buf),R"({"phase":%d,"t":%.1f,"p1":%1f,"p2":%.1f,"max":%.0f,"times": [%.0f,%.0f,%.0f,%.0f]})",(int)phase,currentPhaseTime / 1000.0f, pump1Strength, pump2Strength, maxStrength, phaseTimes[0], phaseTimes[1], phaseTimes[2], phaseTimes[3]);
  server.send(200, "application/json", buf);
}

void  apiSetPhase(){
  if (!server.hasArg("p")){
    server.send(400, "text/plain", "missing p");
    return;
  }
  int p = server.arg("p").toInt();
  if (p < 0 || p > 3) {
    server.send(400, "text/plain", "bad phase");
    return;
  }
  phase = (Phase)p;
  currentPhaseTime = 0;
  pump1Strength = 0;
  pump2Strength = 0;
  server.send(200, "text/plain", "ok");
}

void apiSetPhaseTime(){
  if(!server.hasArg("p") || !server.hasArg("s")){
    server.send(400, "text/plain", "missing p or s");
    return;
  }
  int p = server.arg("p").toInt();
  float s = server.arg("s").toFloat();
  if (p < 0 || p > 3){ 
    server.send(400, "text/plain", "bad phase");
    return;
  }
  if (s < 1) s = 1;
  phaseTimes[p] = s;
  server.send(200, "text/plain", "ok");
}

void apiMaxStrength(){
  if(!server.hasArg("v")){
    server.send(400, "text/plain", "missing v");
    return;
  }
  maxStrength = constrain(server.arg("v").toFloat(), 0, MAX_STRENGTH_LIMIT);
  server.send(200, "text/plain", "ok");
}

void startServer(){
  WiFi.disconnect();
  WiFi.begin(ssid, password);  
  
  int timeout_counter = 0;
  while (WiFi.status() != WL_CONNECTED && timeout_counter < 60) {
    delay(500);
    Serial.print(".");
    timeout_counter++;
    displayConnection(0); //0 = connecting
    displayLoop();
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nConnection Failed!");
    Serial.print("WiFi Status Code: ");
    Serial.println(WiFi.status()); 
    // Common codes: 4 = WL_CONNECT_FAILED, 1 = WL_NO_SSID_AVAIL, 6 = WL_DISCONNECTED
    displayConnection(2); //2 = fail
    return; 
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  displayConnection(1); //1 = connected


  server.on("/", home);
  server.on("/lights", lights);
  server.on("/motors", motors);
  server.on("/sensors", sensors);
  server.on("/api/motor", apiMotor);
  server.on("/api/setPhase", apiSetPhase);
  server.on("/api/setPhaseTime", apiSetPhaseTime);
  server.on("/api/maxStrength", apiMaxStrength);
  //Set up the web server to handle different routes
  server.on("/", navLoop);
  //server.on("/26/off", handleGPIO26Off);
  //server.on("/26/on", handleGPIO26On);
  //server.on("/26/off", handleGPIO26Off);
  //server.on("/27/on", handleGPIO27On);
  //server.on("/27/off", handleGPIO27Off);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void handleServer(){
  server.handleClient();
}

void handleRoot(){
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #555555; }</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";

  server.send(200, "text/html", html);
}

void navLoop() {
  WiFiClient client = server.client();

  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    // ---------------- HTML ----------------
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<!DOCTYPE html>");
    client.println("<html>");

    // ---------------- HEAD ----------------
    client.println("<head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");

    client.println("<style>");

    // BODY
    client.println("body {");
    client.println("margin:0;");
    client.println("font-family:Arial;");
    client.println("background:black;");
    client.println("color:white;");
    client.println("}");

    // NAVBAR
    client.println(".navbar {");
    client.println("background:#111;");
    client.println("overflow:hidden;");
    client.println("padding:10px;");
    client.println("}");

    client.println(".navbar a {");
    client.println("float:left;");
    client.println("color:white;");
    client.println("text-decoration:none;");
    client.println("padding:14px 20px;");
    client.println("font-size:18px;");
    client.println("}");

    client.println(".navbar a:hover {");
    client.println("background:#333;");
    client.println("border-radius:5px;");
    client.println("}");

    // PAGE CONTENT
    client.println(".content {");
    client.println("padding:40px;");
    client.println("text-align:center;");
    client.println("}");

    client.println("h1 {");
    client.println("font-size:50px;");
    client.println("}");

    client.println("</style>");
    client.println("</head>");

    // ---------------- BODY ----------------
    client.println("<body>");

    // NAVIGATION BAR
    client.println("<div class='navbar'>");
    client.println("<a href='/'>Home</a>");
    client.println("<a href='/lights'>Lights</a>");
    client.println("<a href='/motor'>Motor</a>");
    client.println("<a href='/sensor'>Sensor</a>");
    client.println("</div>");

    // CONTENT
    client.println("<div class='content'>");

    // HOME PAGE
    if (request.indexOf("GET / ") >= 0) {

      client.println("<h1>HOME</h1>");
    }

    // LIGHTS PAGE
    else if (request.indexOf("GET /lights") >= 0) {

      client.println("<h1>LIGHTS</h1>");
    }

    // MOTOR PAGE
    else if (request.indexOf("GET /motor") >= 0) {

      client.println("<h1>MOTOR</h1>");
    }

    // SENSOR PAGE
    else if (request.indexOf("GET /sensor") >= 0) {

      client.println("<h1>SENSOR</h1>");
    }

    client.println("</div>");

    client.println("</body>");
    client.println("</html>");

    client.stop();
  }
}