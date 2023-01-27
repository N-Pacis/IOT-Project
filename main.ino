#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

/* esp8266 credentials */
char* ssid = "Benax-WiFi(2.4G)";
char* password = "";

/*
  patterns
  0 - orange
  1 - red
  2 - flash simultaneously
  3 - fade simultaneously
  4 - flash orange
  5 - flash red
  6 - fade orange
  7 - fade red
  99 - none
  100 - all
*/

int pattern = 0;

/* fading variables */
int brightness = 0;    // how bright the LED is
int fadeAmount = 7;    // how many points to fade the LED by

char webpage[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>X-lights</title>
        <style type="text/css">
            * {
                box-sizing: border-box;
                padding: 0em;
                margin: 0;
            }
            :root {
                --dark-background: #121a35;
                --secondary-bg: #030814;
                --orange: orange;
            }
            body {
                background-color: var(--dark-background);
                width: 100%;
                height: 100vh;
                display: flex;
                justify-content: center;
                align-items: center;
                flex-direction: column;
                font-family: "montserrat", sans-serif;
            }
            h4 {
                color: white;
                text-transform: uppercase;
            }
    
            h4 > span {
                color: dodgerblue;
                font-size: 1.5em;
            }
    
            p {
                color: rgba(255, 255, 255, 0.726);
                font-size: 0.9em;
                margin-top: 50px;
            }
    
            a {
                color: white;
                text-decoration: none;
                text-transform: uppercase;
                font-size: 1em;
                padding: 30px 50px;
                font-weight: 600;
                font-size:10px;
                display: inline-block;
                background-color: var(--secondary-bg);
                margin: 10px;
                border-radius: 10px;
                border: 1px solid rgba(255, 255, 255, 0.192);
                transition: all .2s;
                box-shadow: 0 0 5px rgba(255, 255, 255, 0.1);
            }
    
            a:hover {
                transform: scale(1.02);
            }
    
            div {
                margin: 20px;
                display: flex;
                justify-content: center;
                align-items: center;
                min-width: 380px;
                width: 70%;
                text-align: center;
                margin: 20px auto;
            }
    
            div:first-of-type a {
                background-color: green;
                border: 1px solid rgba(255, 255, 255, 0.37);
                color: rgba(255, 255, 255, 0.);
                font-weight: 700;
            }
    
            div:first-of-type a:last-of-type {
                background-color: rgb(168, 0, 0);
            }
            @media screen and (max-width: 500px) {
              body {
                  height: auto;
                  padding: 20px 10px;
                  margin-top:20px;
                  display: block;
              }
              div {
                  width: 100%;
              }
              a {
                  width: 44%;
              }
              h4 {
                  text-align: left;
                  font-size: 1.2em;
                  margin-left: 20px;
              }
  
              p {
                  margin-top:10px;
                  margin-left: 20px;
                  font-weight: 500;
              }
          }  
        </style>
    </head>
    <body>
        <h4>Welcome at <span>xLights</span></h4>
        <p>Play around with these buttons😁</p>
        <div>
            <a href="?pattern=1" id="1">ON</a>
            <a href="?pattern=0" id="0">OFF</a>
            <a href="?pattern=2" id="2">FADE</a>            
        </div>
        <script type="text/javascript">
            var pattern = (new URL(window.location).searchParams).get("pattern")
            document.getElementById(pattern).style.border = "3px dodgerblue solid"
        </script>
    </body>
  </html>
)=====";

void setup() {
  pinMode(D1, OUTPUT); //orange
  pinMode(D2, OUTPUT); //red
  pinMode(D7, OUTPUT); //green for connection
  digitalWrite(D7, LOW);
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  Serial.print("Connecting.");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500); 
  }
  // digitalWrite(D7, HIGH);
  Serial.println("");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  server.on("/", [](){
      server.send_P(200, "text/html", webpage);
      if(server.arg("pattern")){
        pattern = server.arg("pattern").toInt();
      }
      else {
        pattern = 0;
      }
    }
  );
  server.begin();
}


void all(){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
}

void none(){
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
}
void switchToRed(){
   digitalWrite(D1, LOW);
   digitalWrite(D2, HIGH);  
}

void switchToOrange(){
   digitalWrite(D2, LOW);
   digitalWrite(D1, HIGH);  
}

void flashOrange(int tym){
  digitalWrite(D2, LOW);
  digitalWrite(D1, HIGH);
  delay(tym);
  digitalWrite(D1, LOW);
  delay(tym);
}

void flashRed(int tym){
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  delay(tym);
  digitalWrite(D2, LOW);
  delay(tym);
}

void fadeOrange(){
  digitalWrite(D2, LOW);
  analogWrite(D1, brightness);
  brightness = brightness + fadeAmount; // change the brightness for next time through the loop:
  if (brightness <= 0 || brightness >= 255) { // reverse the direction of the fading at the ends of the fade:
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

void fadeRed(){
  digitalWrite(D1, LOW);
  analogWrite(D2, brightness);
  brightness = brightness + fadeAmount; // change the brightness for next time through the loop:
  if (brightness <= 0 || brightness >= 255) { // reverse the direction of the fading at the ends of the fade:
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

void fadeBoth(){
  analogWrite(D1, brightness);
  analogWrite(D2, 255 - brightness);
  brightness = brightness + fadeAmount; // change the brightness for next time through the loop:
  if (brightness <= 0 || brightness >= 255) { // reverse the direction of the fading at the ends of the fade:
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

void fade(){
    analogWrite(D7, 255 - brightness);
  brightness = brightness + fadeAmount; // change the brightness for next time through the loop:
  if (brightness <= 0 || brightness >= 255) { // reverse the direction of the fading at the ends of the fade:
    fadeAmount = -fadeAmount;
  }
  delay(30);
}


// the loop function runs over and over again forever
void loop() {
  server.handleClient();
  switch(pattern){
    case 0:
      digitalWrite(D7, HIGH);
      break;
    case 1:
      digitalWrite(D7, LOW);
      break;
    case 2:
      fade();
    default:
      digitalWrite(D7, LOW); 
  }
}