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
  
  calibTimer = millis();
  Connected = true;
  
  Route();
  newRoute();
  
  //SEND();
  //cycle();
}

void loopWifi(){}

//Cycle
void cycle() {
  for (int i = 0; i < 5; i++) {
    position=routeARR[i];
    delay(500);
    Serial.println(position);
    Serial.println(readResponse());
    SEND();
  }
}


//Route
void Route(){
  Serial.println("getting route...");
  client.println("GET /api/route/9b447137 HTTP/1.1");
  //client.println("Host: localhost:8081");
  //client.println("Connection: close");
  client.println();
  client.println();
  String a=readResponse();
  String input = getResponseBody(a);
  Serial.println(input);
  
  int count = 0;
  for (int i=0; i<input.length(); i++){
    if (isDigit(input[i]) ){
      routeARR[count]=input[i]-48;
      count++;
    }
  }

  Serial.print("The array is: ");
  for(int i = 0; i < 6; i++){
    Serial.print(routeARR[i]);
  }
  Serial.println();
  
}

//sending responses
void SEND() {
  Serial.print("Server Communication... ");
  
  String postBody("position=");
  postBody += position;

  Serial.println(postBody);
  
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
  
String getResponseBody(String response) {
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