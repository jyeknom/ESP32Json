#include <WiFi.h>
#include <HTTPClient.h>
#define PortaAlim  32 // ESP32 pino GIOP17 conectar o sensor no pino de alimen
#define PortaSinal 34 // ESP32 pino GIOP36 (ADC0) conectar o sensor no pino de sinal

const int PortaChuva = 35; // Definir porta de leitura do sensor de chuva

#define DELAY 20000

String NomeServidor = "https://enchenteapi.herokuapp.com/arduino/send";
unsigned long lastUpdate = 0;


void setup(){

  Serial.begin(115200); // Inicializar serial
  pinMode(PortaAlim, OUTPUT);   // Definir porta do sensor nidel de água como saída
  digitalWrite(PortaAlim, LOW); // Desliga o sensor
  char* ssid = "OPTIMUS";
  char* password = "C16111998";
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
    }
  Serial.println("Conectado ao Wi-FI");
  
}

void loop(){

  String toSend;
  String SendChuva;
  String SendNivel;
  long unsigned int now = millis();
  
  digitalWrite(PortaAlim, HIGH);  // Liga o sensor nível de água
  
  int leituraChuva = analogRead(PortaChuva); // Faz a leitura do sensor de chuva
  int leituraNivel = analogRead(PortaSinal); // Faz a leitura do sensor nível de água
  

  SendChuva = String(leituraChuva);
  SendNivel = String(leituraNivel);
  
  if((now - lastUpdate) > DELAY){

    if(WiFi.status()== WL_CONNECTED){
      
      HTTPClient http;
      http.begin(NomeServidor.c_str());
      http.addHeader("Content-Type", "application/json");

      toSend = "{\"id\":\"01\", \"valueWater\":"+ SendNivel+", \"valueRain\":"+SendChuva+"}";
      
      int httpResponse = http.POST(toSend.c_str());

      if (httpResponse > 0) {

        Serial.print("Resposta HTTP: ");
        Serial.println(httpResponse);
        Serial.println(http.getString());
        Serial.println(leituraChuva);
        Serial.print(leituraNivel);
        }
        else{

          Serial.print("Erro: ");
          Serial.println(httpResponse);
          
        }
        http.end();
      }
        else{

          Serial.println("WiFi Desconectado");
          
          }

         lastUpdate = millis();
    }
  
}
