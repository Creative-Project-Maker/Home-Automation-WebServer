  /* Simple ESP32 Web Server  
  *  The ESP32 Wifi is configured as Access Point.  
  *    
  */  
  //check it is ESP32 or ESP8266
 #ifdef ESP8266
#include <ESP8266WiFi.h>
//initialize the relays for ESP8266 
 #define RELAY_1  14  
 #define RELAY_2  12
 #define RELAY_3  13
 #define RELAY_4  15  
#elif defined(ESP32)
#include <WiFi.h>
//initialize the relays for ESP32 
 #define RELAY_1  25  
 #define RELAY_2  26  
 #define RELAY_3  27
 #define RELAY_4  14  
#else
#error "Board not found"
#endif
 
   
 // Create the objects for server and client  
 WiFiServer server(80);  
 WiFiClient client;  
   
 const char* ssid   = "Home-Automation-WebServer";// This is the SSID that ESP32 will broadcast  
 const char* password = "12345678";     // password should be atleast 8 characters to make it work  
   
 // Create the global variable  
 String http;  
 String Relay1State = "off";  
 String Relay2State = "off";  
 String Relay3State = "off";
 String Relay4State = "off";  
   
 void setup() {  
  Serial.begin(115200); //begin the serial communication at 115200 baud rate
  //set the relays as output
  pinMode(RELAY_1, OUTPUT);  
  pinMode(RELAY_2, OUTPUT);  
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  //by default the relays will be turned of 
  //for active low  
  digitalWrite(RELAY_1, HIGH);  
  digitalWrite(RELAY_2, HIGH);  
  digitalWrite(RELAY_3, HIGH); 
  digitalWrite(RELAY_4, HIGH); 
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
   
  // Create the ESP32 access point  
  /*  
   * Alternative:  
   * softAP(const char* ssid,  
   *     const char* password,  
   *     int channel,  
   *     int ssid_hidden,   
   *     int max_connection  
   *       
   *     where:  
   *      ssid - this is the SSID that will be broadcast by ESP32  
   *          maximum of 63 characters  
   *      password - this is the password to connect to ESP32  
   *          minimum of 8 characters to function  
   *          Put NULL to make it open to public  
   *      channel - wifi channels (ranging from 1 to 13)  
   *      ssid_hidden - sets the SSID as broadcast or hidden  
   *          0: broadcast SSID  
   *          1: hidden SSID,   
   *           you need to type the exact SSID name in order to connect  
   *      max_connection - maximum number of connected clients  
   *          accepts 1 to 4 only  
   *            
   */  
  WiFi.softAP(ssid, password);  
   
  Serial.println( "" );  
  Serial.println( "WiFi AP is now running" );  
  Serial.println( "IP address: " );  
  Serial.println( WiFi.softAPIP() );  
   
  // Start our ESP32 server  
  server.begin();  
 }  
   
 void loop(){  
    
  if ( client = server.available() ) {  // Checks if a new client tries to connect to our server  
   Serial.println("New Client.");  
   String clientData = "";  
   while ( client.connected() ) {    // Wait until the client finish sending HTTP request  
    if ( client.available() ) {     // If there is a data,  
     char c = client.read();      //  read one character  
     http += c;            //  then parse it  
     Serial.write(c);    
     if (c == '\n') {         // If the character is carriage return,   
                      //  it means end of http request from client  
      if (clientData.length() == 0) { //  Now that the clientData is cleared,  
       sendResponse();        //    perform the necessary action  
       updateRelay();  
       updateWebpage();  
       break;  
      } else {  
       clientData = "";       //  First, clear the clientData  
      }  
     } else if (c != '\r') {      // Or if the character is NOT new line  
      clientData += c;        //  store the character to the clientData variable  
     }  
    }  
   }   
   http = "";  
   client.stop();            // Disconnect the client.  
   Serial.println("Client disconnected.");  
   Serial.println("");  
  }  
 }  
   
 void sendResponse() {  
  // Send the HTTP response headers  
  client.println("HTTP/1.1 200 OK");  
  client.println("Content-type:text/html");  
  client.println("Connection: close");  
  client.println();   
 }  
   
 void updateWebpage() {  
  // In here we will display / update the webpage by sending the HTML   
  //  to the connected client  
  // In order for us to use the HTTP GET functionality,  
  //  the HTML hyperlinks or href is use in the buttons.   
  //  So that, when you press the buttons, it will send a request to the   
  //  web server with the href links by which our ESP32 web server will  
  //  be check using HTTP GET and execute the equivalent action  
    
  // Send the whole HTML  
  client.println("<!DOCTYPE html><html>");  
  client.println("<head>");  
  client.println("<title>ESP32 WiFi Station</title>");  
  client.println("</head>");  
    
  // Web Page Heading  
  client.println("<center><body><h1>ESP32/ESP8266 Home Automation Web Server</h1></center>");  
   
  // Display buttons for Relay 1  
  client.println("<center><p>Relay 1 is " + Relay1State + "</p></center>");    
  if (Relay1State == "off") {  
   client.println("<center><p><a href=\"/RELAY_1/on\"><button>Turn ON</button></a></p></center>");  
  } else {  
   client.println("<center><p><a href=\"/RELAY_1/off\"><button>Turn OFF</button></a></p></center>");  
  }   
   
  client.print("<hr>");  
    
  // Display buttons for Relay 2  
  client.println("<center><p>Relay 2 is " + Relay2State + "</p></center>");    
  if (Relay3State == "off") {  
   client.println("<center><p><a href=\"/Relay_2/on\"><button>Turn ON</button></a></p></center>");  
  } else {  
   client.println("<center><p><a href=\"/Relay_2/off\"><button>Turn OFF</button></a></p></center>");  
  }   
   
  client.print("<hr>");  
      
  // Display buttons for Relay 3  
  client.println("<center><p>Relay 3 is " + Relay3State + "</p></center>");    
  if (Relay3State == "off") {  
   client.println("<center><p><a href=\"/Relay_3/on\"><button>Turn ON</button></a></p></center>");  
  } else {  
   client.println("<center><p><a href=\"/Relay_3/off\"><button>Turn OFF</button></a></p></center>");  
  }  
 client.print("<hr>"); 
   // Display buttons for Relay 4  
  client.println("<center><p>Relay 4 is " + Relay4State + "</p></center>");    
  if (Relay4State == "off") {  
   client.println("<center><p><a href=\"/Relay_4/on\"><button>Turn ON</button></a></p></center>");  
  } else {  
   client.println("<center><p><a href=\"/Relay_4/off\"><button>Turn OFF</button></a></p></center>");  
  }  
   
  client.println("</body></html>");  
  client.println();  
 }  
   
 void updateRelay() {  
  // In here we will check the HTTP request of the connected client  
  //  using the HTTP GET function,  
  //  Then turns the RELAY on / off according to the HTTP request  
  if    (http.indexOf("GET /RELAY_1/on") >= 0) {  
   Serial.println("Relay 1 on");  
   Relay1State = "on";  
   digitalWrite(RELAY_1, LOW);  
  } else if (http.indexOf("GET /RELAY_1/off") >= 0) {  
   Serial.println("Relay 1 off");  
   Relay1State = "off";  
   digitalWrite(RELAY_1, HIGH);  
  } else if (http.indexOf("GET /Relay_2/on") >= 0) {  
   Serial.println("Relay 2 on");  
   Relay2State = "on";  
   digitalWrite(RELAY_2, LOW);  
  } else if (http.indexOf("GET /Relay_2/off") >= 0) {  
   Serial.println("Relay 2 off");  
   Relay2State = "off";  
   digitalWrite(RELAY_2, HIGH);  
  } else if (http.indexOf("GET /RELAY_3/on") >= 0) {  
   Serial.println("Relay 3 on");  
   Relay3State = "on";  
   digitalWrite(RELAY_3, LOW);  
  } else if (http.indexOf("GET /RELAY_3/off") >= 0) {  
   Serial.println("Relay 3 off");  
   Relay3State = "off";  
   digitalWrite(RELAY_3, HIGH);  
  }  else if (http.indexOf("GET /RELAY_4/on") >= 0) {  
   Serial.println("Relay 4 on");  
   Relay4State = "on";  
   digitalWrite(RELAY_4, LOW);  
  } else if (http.indexOf("GET /RELAY_4/off") >= 0) {  
   Serial.println("Relay 4 off");  
   Relay4State = "off";  
   digitalWrite(RELAY_4, HIGH);  
  }  
 }  
