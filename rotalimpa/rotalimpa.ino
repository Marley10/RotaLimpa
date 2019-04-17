//Projeto RotaLimpa BELOTOUR
//Integrantes Brício.MS&& Marley.RL
//Codigo Modificado 09/04/2019 Por Marley Luciano


#include <ESP8266mDNS.h>
#include <ESP8266mDNS_Legacy.h>
#include <LEAmDNS.h>
#include <LEAmDNS_lwIPdefs.h>
#include <LEAmDNS_Priv.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h> 
#include <Hash.h>
#include <FS.h>


// Definindo as portas utilizadas pelo sensor

#define trigPin D1
#define echoPin D2
#define LED_RED     12
#define LED_GREEN   15
#define LED_BLUE    13

// Insira sua rede e sua senha

const char* nome = "demolidor";
const char* senha = "123456789";

// Configurado na porta 80 

ESP8266WebServer server(80);

// Declaração das variaveis utilizadas no código

int wifiStatus;
String textoHTML;
long duracao;
float distancia, aux;
String alerta;
String PROJETO_ROTA_LIMPA; 
//int alerta1;

// Envia o texto para o server

void handleRoot() {
   
  
   textoHTML = "<meta http-equiv= refresh content= 3>";
 
   textoHTML += "<!DOCTYPE html><html><head><title>ESP8266 WEB</title>";//Identificaçao e Titulo.
    textoHTML += "<style>h1{font-size:2vw;color:black;}</style></head>";//Cria uma nova fonte de tamanho e cor X.
   textoHTML += " <br> PROJETO_ROTA_LIMPA";
   
   textoHTML += "<!DOCTYPE html><html><head><title>ESP8266 WEB</title>";//Identificaçao e Titulo.
   textoHTML += PROJETO_ROTA_LIMPA;
   textoHTML += "<br> Horario: ";
   textoHTML += "<style>h1{font-size:2vw;color:black;}</style></head>";//Cria uma nova fonte de tamanho e cor X.
   textoHTML += duracao;
   
   textoHTML += "Distancia: ";
   textoHTML += distancia;
   textoHTML += "<style>h1{font-size:9vw;color:red;}</style></head>";//Cria uma nova fonte de tamanho e cor X.
   textoHTML += "<br> LIXEIRA: ";
   
    textoHTML+= "<body bgcolor='ffffff'><center><h1>";//Cor do Background
   textoHTML += alerta;
 
  server.send(200, "text/html", textoHTML);

   
   textoHTML += "<meta name='viewport' content='user-scalable=no'>";//Desabilita o Zoom.
   textoHTML += "<style>h1{font-size:2vw;color:black;}</style></head>";//Cria uma nova fonte de tamanho e cor X.
   textoHTML+= "<body bgcolor='ffffff'><center><h1>";//Cor do Background

}

// Tratativas de erro

void handleNotFound(){
  String message = "Arquivo não foi encontrado \n\n";
  message += "URI: ";
  message += server.uri();
  message += "\n Método: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\n Argumentos: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){


  Serial.begin(115200);
  delay(200);

  //Fazendo a conexão
  Serial.print("Iniciando a conexão da rede! ");
  Serial.println(nome);
  WiFi.begin(nome, senha);


 // Aguardando pela conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 

  
  // Pinos de comunicação que receberam os dados
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
 
  digitalWrite(LED_RED, 1);
  digitalWrite(LED_GREEN, 1);
  digitalWrite(LED_BLUE, 1);
 
  
  
//  WiFi.mode(WIFI_STA);


// Fazendo a conexão

  wifiStatus = WiFi.status(); 

  if( wifiStatus == WL_CONNECTED){
    Serial.println("Uhullllllll.... O nodeMCU esta conectado! ");
    Serial.print("O IP utilizado na conexão é: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    }
    else {
      Serial.println("Ahhhhhhhhhh... O nodeMCU não esta conectado!");
      }
  delay(1000);

  // Enviando o texto HTML para o server

// Primeira opção
  server.on("/", handleRoot);
  

// Segunda opção
  server.on("/inline", [](){
    server.send(200, "text/plain", "2 opção de envio");
  });

// Caso ocorra um erro, (tratativas do erro)

  server.onNotFound(handleNotFound);

  //Inicia o server

  server.begin();
  Serial.println("HTTP server iniciado");
}

void loop(void){
  
  // Deixa o server em loop, assim se houver alguma mudança nas informações, ele atualiza

    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);
  server.handleClient();
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH ,1000000);
  aux = duracao;
  distancia = (aux*340/10000)/2; // distancia em centimetros
  Serial.print("Distancia: ");
  Serial.println(distancia);

  Serial.print("Horario: "); 
  Serial.println(duracao);
  
  if ( distancia <= 15){
    digitalWrite(LED_RED, HIGH);
    alerta = "CHEIA";
    }
  else{
    digitalWrite(LED_GREEN, HIGH);
    alerta = "DISPONIVEL";
  }
  //alerta = ( distancia <= 15 );
  Serial.print("LIXEIRA: ");
  Serial.println(alerta);

  
  //alerta = ( distancia>15 , LED_RED HIGH);
  //alerta1 = ( distancia < 15 , LED_GREEN LOW);
  delay(1000);
}

