#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHT22PIN 13

DHT dht(DHT22PIN, DHT22);

const char *ssid = "Wokwi-GUEST";
const char *password = "";
const char *mqtt_server = "test.mosquitto.org";
const int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
float temp = 0;
float hum = 0;

const char *topic_temperature = "/sensor/data/temperature";
const char *topic_humidity = "/sensor/data/humidity";
const char *topic_command = "/sic/command/mqtt";

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    { // perintah tunggu esp32 sampi terkoneksi ke wifi
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{ // perintah untuk menampilkan data ketika esp32 di setting sebagai subscriber
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    { // mengecek jumlah data yang ada di topik mqtt
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(2, LOW); // Turn the LED on (Note that LOW is the voltage level
                              // but actually the LED is on; this is because
                              // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(2, HIGH); // Turn the LED off by making the voltage HIGH
    }
}

void reconnect()
{ // perintah koneksi esp32 ke mqtt broker baik itu sebagai publusher atau subscriber
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // perintah membuat client id agar mqtt broker mengenali board yang kita gunakan
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("Connected");
            // Once connected, publish an announcement...
            client.publish("/sic/mqtt", "Hello!"); // perintah publish data ke alamat topik yang di setting
            // ... and resubscribe
            client.subscribe(topic_command); // perintah subscribe data ke mqtt broker
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, port);
    client.setCallback(callback);
    dht.begin();
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();

    if (now - lastMsg > 2000)
    {
        lastMsg = now;
        float humi = dht.readHumidity();
        float temp = dht.readTemperature();

        String temp_string = String(temp, 2);
        client.publish(topic_temperature, temp_string.c_str());

        String hum_string = String(humi, 2);
        client.publish(topic_temperature, hum_string.c_str());

        Serial.print("Temperature: ");
        Serial.println(temp);
        Serial.print("Humidity: ");
        Serial.println(humi);
    }
    // put your main code here, to run repeatedly:
    delay(10); // this speeds up the simulation
}
