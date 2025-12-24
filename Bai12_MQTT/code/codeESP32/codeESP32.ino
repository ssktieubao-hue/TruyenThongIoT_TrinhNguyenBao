#include <WiFi.h> // Dùng cho ESP32. Nếu dùng ESP8266 thì đổi thành <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- CẤU HÌNH (BẠN CẦN SỬA PHẦN NÀY) ---
const char* ssid = "Kha Hoan";
const char* password = "0981721403";
const char* mqtt_server = "192.168.2.31"; // IP của Home Assistant
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt-user"; // Tên user MQTT bạn tạo trên HA
const char* mqtt_pass = "13122005"; // Mật khẩu user MQTT
const char* student_id = "10123034"; // Ví dụ: 10120056
// ----------------------------------------

const char* topic_sensor = "iot/lab1/10123034/sensor"; // Sửa lại MASV vào đây luôn cho chắc

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESPClient-";
    clientId += String(random(0xffff), HEX);
    // Kết nối với user và pass
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Tạo dữ liệu JSON giả lập
  StaticJsonDocument<200> doc;
  doc["temp"] = random(250, 350) / 10.0; // Nhiệt độ 25.0 - 35.0
  doc["hum"] = random(400, 800) / 10.0;  // Độ ẩm 40.0 - 80.0

  char payload[128];
  serializeJson(doc, payload);

  // Gửi dữ liệu lên Topic
  Serial.print("Publishing message: ");
  Serial.println(payload);
  client.publish(topic_sensor, payload);

  delay(5000); // Gửi mỗi 5 giây
}