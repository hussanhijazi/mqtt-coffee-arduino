
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "xxx";
const char* password = "xxx";
const char* mqtt_server = "iot.eclipse.org";
const char* coffee_topic = "gdgfoz/coffeeiot";

WiFiClient espClient;

PubSubClient client(espClient);

const int gpio = D2;

void setup_wifi() {

  delay(100);
  Serial.println();
  Serial.print("Conectando-se a rede WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a Rede Wifi ");
  Serial.print("( ");
  Serial.print(ssid);
  Serial.println(" )");
  Serial.print("Endereco IP do seu Arduino: " );
  Serial.println(WiFi.localIP());
}

void reconnect() {

  while (!client.connected())
  {

    Serial.println("Estabelecendo conexao com o servidor MQTT");

    if (client.connect("ESP8266Client2608"))
    {

      Serial.print("Conectado ao Servidor MQTT: ");
      Serial.println(mqtt_server);

      client.subscribe(coffee_topic);

    } else {

      Serial.print("Falha, rc= ");
      Serial.println(client.state());
      Serial.println("Nova tentativa em 5 segundos");
      delay(5000);

    }
  }
}

void callback(String topic, byte* message, unsigned int length) {

  Serial.print("[Mensagem Recebida] Topico: ");
  Serial.print(topic);
  Serial.print(" Mensagem: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];

  }
  Serial.println();

  if (topic == coffee_topic) {
    if (messageTemp == "on") {
      digitalWrite(gpio, HIGH);
      Serial.print("Ligado");

    }
    else if (messageTemp == "off") {
      digitalWrite(gpio, LOW);
      Serial.print("Desligado");

    }
  }
}

void setup() {

  pinMode(gpio, OUTPUT);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }

  if (!client.loop())
    client.connect("ESP8266Client2608");
    
  client.loop();
}

