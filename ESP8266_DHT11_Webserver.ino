#include <ESP8266WiFi.h>
#include "DHT.h"
 
const char* ssid = "Your SSID";
const char* password = "Password";
 
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int mint=100;
int maxt=0;
int minh=100;
int maxh=0;

WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
  dht.begin(); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Return the response
   client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv=\"refresh\" content=\"10\">");
  // client.println("<br />"); 
  client.println(""); //  do not forget this one
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
 
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (t < mint) {mint = t;}
  if (t > maxt) {maxt = t;}
  if (h < minh) {minh = h;}
  if (h > maxh) {maxh = h;}

          client.println("<h1>Weather Station</h1>");
          client.print("<br />");
          client.print("Temperature : ");
          client.println(t, 1);  
          client.println("C");
          client.print("<br />");
          client.print("Humidity : ");
          client.print(h, 0);
          client.println("%"); 
          client.println("<br />");
          client.println("<br />");  
          client.print("Minimum Temprature : ");
          client.print(mint, 1);
          client.println("C");
          client.println("<br />");  
          client.print("maximum Temprature : ");
          client.print(maxt, 1);
          client.println("C");
          client.println("<br />");
          client.println("<br />");  
          client.print("Minumum Humidity : ");
          client.print(minh,1);
          client.println("%");
          client.println("<br />");  
          client.println("Maximum Humidity : ");
          client.print(maxh,1);
          client.println("%");
          client.println("<br />");
          client.println("<br />");
          client.print("Uptime : ");
           client.print(tmr.time());
          client.println("MSec");  
          client.println("</html>");
       
  delay(2);
  Serial.println("Client disconnected");
 
 
}
