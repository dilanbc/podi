#include <ESP8266WiFi.h>
const char* ssid = "SLT-HOME-WIFI";
const char* password = "dilan27114";
IPAddress ip(192, 168, 8, 177);


int LED1 = 2; // led connected to D7
int LED3 = 4; //led for connet indigate
int LED2 = 5;
WiFiServer server(80);

void setup()
{
  Serial.begin(74880);

  pinMode(LED1, OUTPUT);
pinMode(LED3, OUTPUT);    //******************
pinMode(LED2, OUTPUT);
pinMode(A0,INPUT);
digitalWrite(LED1,LOW);
digitalWrite(LED2,LOW);
  digitalWrite(LED3, LOW);


  Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED3,1);
  delay(250);
  digitalWrite(LED3,0);
  delay(250);
  digitalWrite(LED3,1);
  delay(250);
  digitalWrite(LED3,0);
  Serial.println("WiFi connected");  
  server.begin();  // Starts the Server
  Serial.println("Server started");

  Serial.print("IP Address of network: "); // Prints IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 int value1 = LOW;
int value2 =LOW;
void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');  // waiting for request
  Serial.println(request);
  client.flush();
//...........Bulb 1 &2 ............
 
  if(request.indexOf("/LED1=ON") != -1)
  {
    digitalWrite(LED1, HIGH); // Turn ON LED
    value1 = HIGH;
  }
  if(request.indexOf("/LED1=OFF") != -1)
  {
    digitalWrite(LED1, LOW); // Turn OFF LED
    value1 = LOW;
  }

  if(request.indexOf("/LED2=ON") != -1)
  {
    digitalWrite(LED2, HIGH); // Turn ON LED
    value2 = HIGH;
  }
  if(request.indexOf("/LED2=OFF") != -1)
  {
    digitalWrite(LED2, LOW); // Turn OFF LED
    value2 = LOW;
  }
  int a = analogRead(A0);
/*------------------HTML Page Creation---------------------*/

  client.println("<!doctype html>");
client.println("<html>");
client.println("<head>");
client.println("<meta charset=\"utf-8"">");
client.println("<title>Arduino</title>");
client.println("<style type= \"text\"css\">");

client.print("<meta http-equiv=\"refresh\" content=\"0.5\">");
client.print("<font-style=\"normal\" font-size=\"24px\">");
client.println("</style>");
client.println("</head>");
client.println("<body bgcolor=\"FBFA90\" leftmargin=\120\" topmargin=\"100\">");
client.println("<p></p>");
client.println("<p>&nbsp; &nbsp;&nbsp;</p>");
client.println("<p></p>");
client.println("<table border=\"1\" style=\"height=\"162px\" width=\"100.171%\" border-collapse=\"collapse\" border-color=\" black\" border-style=\" solid\" height=\"162\">");
client.println("<tbody>");
client.println("<tr>");

client.println("<td style=\"width\" 33.3333%\" text-align=\"center\">LED 1</td>");
client.println("<td style=\"width\" 33.3333%\" text-align=\" center\">LED 2</td>");
client.println("<td style=\"width\" 33.3333%\" text-align=\" center\">VALUE</td>");
client.println("</tr>");
client.println("<tr>");
if(value1 == HIGH){client.println("<td style=\"width: 33.3333%\" text-align=\" center\"><a href=\"/LED1=OFF\"\"><button>OFF</button><a></td>");}
if(value1 == LOW){client.println("<td style=\"width=\" 33.3333%\" text-align=\" center\"><a href=\"/LED1=ON\"\"><button>ON</button><a></td>");}
if(value2 == HIGH){client.println("<td style=\"width=\" 33.3333%\" text-align=\" center\"><a href=\"/LED2=OFF\"\"><button>OFF</button><a></td>");}
if(value2 == LOW){client.println("<td style=\" width=\"33.3333%\" text-align=\"center\"><a href=\"/LED2=ON\"\"><button>ON</button><a></td>");}
client.println(a);
client.println("</tr></tbody></table></body></html>");
client.println("");
client.println("");
client.println("");
client.println("");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
