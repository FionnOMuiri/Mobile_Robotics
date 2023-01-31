//9b447137

void setupWifi() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Connecting to network: ");
  Serial.print(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }
  Serial.println("Connected");
  Serial.print("Obtaining IP address");
  Serial.flush();

  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  connect();
  SEND();
  

  // read response
  String response = readResponse();
  Serial.println("-----------------------");
  Serial.println(response);
  // get status code
  int statusCode = getStatusCode(response);
  Serial.println("-----------------------");
  Serial.println(statusCode);
  if (statusCode == 200) {
  // success, read body
  String body = getResponseBody(response);
  Serial.println("-----------------------");
  Serial.println(body);
  // check if at final destination
    if (!body.equals("undefined")) {
    destination = body.toInt();
    }
  }

  calibTimer = millis();
  Connected = true;
  
  cycle();
}

void loopWifi(){}

void cycle() {
  for (int i = 0; i < 5; i++) {
    position++;
    delay(2000);
    SEND();
  }
}

//sending responses
void SEND() {
  String postBody("position=");
  postBody += position;
  
  client.println("POST /api/arrived/9b447137 HTTP/1.1");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postBody.length());
  // Serial.println("Connection: close");
  client.println();
  
  client.print(postBody);
  //client.println(position);
  //Serial.print(position);
  //client.println("Connection: close");
  Serial.print("\n");
}

//recieving responses
String readResponse() {
  char buffer[BUFSIZE];
  memset(buffer, 0, BUFSIZE);
  client.readBytes(buffer, BUFSIZE);
  String response(buffer);
  return response;
  }
  
int getStatusCode(String& response) {
  String code = response.substring(9, 12);
  return code.toInt();
}
  
String getResponseBody(String& response) {
  int split = response.indexOf("\r\n\r\n");
  String body = response.substring(split+4, response.length());
  body.trim();
  return body;
}

bool connect() {
  if (!client.connect(server, port)) {
    Serial.println("error connecting to server");
    return false;
  }
  //Serial.println("NO error connecting to server");
  return true;
}
