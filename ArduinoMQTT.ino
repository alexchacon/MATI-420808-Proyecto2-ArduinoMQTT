#include <SoftwareSerial.h>
#include <dht.h>

#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>


#define dht_apin 7 // Analog Pin sensor is connected to
 
#define RX 10
#define TX 11
#define WIFI_AP "AlexChaconMobile"
#define WIFI_PASSWORD "xxx"
#define TOKEN "Arduino"
char thingsboardServer[] = "52.55.12.253";
String AP = "software colombia";
String PASS = "xxx"; 

String API = "206KNPXA4Y6D8X7V";
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

int countTrueCommand;
int countTimeCommand; 
int valSensor = 1;
int status = WL_IDLE_STATUS;
const int temperaturePin = 0;

unsigned long lastSend;
dht DHT;

// Initialize the Ethernet client object
WiFiEspClient espClient;

PubSubClient client(espClient);

SoftwareSerial esp8266(RX,TX); 

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  
  pinMode(4, OUTPUT);
  lastSend = 0;
  /*
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
*/
InitWiFi();
  client.setServer( thingsboardServer, 1883 );
}


void InitWiFi()
{
  // initialize serial for ESP module
  //esp8266.begin(9600);
  // initialize ESP module
  WiFi.init(&esp8266);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  Serial.println("Connecting to AP 2222222...");
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Arduino Uno Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

void loop() {
  /*
  getSensorDataHumidity();
  valSensor = getSensorData();
  String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
  sendCommand("AT+CIPMUX=1",5,"OK");
  
  if (sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK"))
  {
    sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
    esp8266.println(getData);
    delay(15000);
    countTrueCommand++;
    //sendCommand("AT+CIPCLOSE=0",5,"OK");
  }
  else
  {
    sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  }

  //Aqui lampara
 
  digitalWrite(4, HIGH);
  delay(3000);
  digitalWrite(4, LOW);
  delay(3000);
Serial.print("Antes");
  client.publish( "temperature", "Hola Alex");
  Serial.print("Antes");*/

  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("Connected to AP");
  }

  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }

  client.loop();
}


void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  
  client.publish( "temperature", "Hola Alex" );
  Serial.println( "FIN comunicado" );
}

int getSensorData(){
  //return random(1000); 

  float voltage, degreesC, degreesF;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage);

  Serial.print("  Deg C: ");
  Serial.print(degreesC);
  
  //degreesF = degreesC * (9.0/5.0) + 32.0;
  return degreesC;
}


int getSensorDataHumidity(){
  DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
 
}

float getVoltage(int pin)
{
  // This function has one input parameter, the analog pin number
  // to read. You might notice that this function does not have
  // "void" in front of it; this is because it returns a floating-
  // point value, which is the true voltage on that pin (0 to 5V).

  // You can write your own functions that take in parameters
  // and return values. Here's how:

    // To take in parameters, put their type and name in the
    // parenthesis after the function name (see above). You can
    // have multiple parameters, separated with commas.

    // To return a value, put the type BEFORE the function name
    // (see "float", above), and use a return() statement in your code
    // to actually return the value (see below).

    // If you don't need to get any parameters, you can just put
    // "()" after the function name.

    // If you don't need to return a value, just write "void" before
    // the function name.

  // Here's the return statement for this function. We're doing
  // all the math we need to do within this statement:

 // return (analogRead(pin) * 0.004882814);
  return (analogRead(pin) * 500)/1023;

  // This equation converts the 0 to 1023 value that analogRead()
  // returns, into a 0.0 to 5.0 value that is the true voltage
  // being read at that pin.
}

boolean sendCommand(String command, int maxTime, char readReplay[]) {

  boolean found = false; 
  
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  else
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  return found;
 }
