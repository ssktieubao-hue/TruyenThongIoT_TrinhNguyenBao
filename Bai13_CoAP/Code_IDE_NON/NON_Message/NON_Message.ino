
#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

/* ========== CẤU HÌNH ========== */
const char* WIFI_SSID = "nigga";     // <--- THAY ĐỔI
const char* WIFI_PASS = "1234567892k5";        // <--- THAY ĐỔI

// IP của máy tính chạy Node.js Server
IPAddress coap_server_ip(10, 135, 15, 225);      // <--- THAY ĐỔI THEO IP MÁY TÍNH BẠN

const int coap_port = 5683;
const char* coap_resource = "sensor/temp";
const unsigned long SEND_INTERVAL = 5000; // Gửi mỗi 5s

WiFiUDP udp;
Coap coap(udp);
unsigned long lastSend = 0;

void setup_wifi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Hàm callback phản hồi khi nhận được ACK từ server
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[CoAP Response] Got Packet!");
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  Serial.print("Payload Server gui ve: ");
  Serial.println(p);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  // Đăng ký hàm phản hồi
  coap.response(callback_response);
  coap.start();
  Serial.println("CoAP client started");
}

void loop() {
  coap.loop(); // Quan trọng: Xử lý gói tin nhận về

  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();
    
    // Tạo giả lập nhiệt độ ngẫu nhiên từ 25-35 độ
    String payload = "temp=" + String(random(25, 35)); 

    Serial.println("--> Gửi CON Message...");

    // --- ĐOẠN CODE ĐÃ SỬA LỖI ---
    coap.send(
      coap_server_ip, 
      coap_port, 
      coap_resource, 
      COAP_NONCON, 
      COAP_POST, 
      NULL, 0,  // <--- QUAN TRỌNG: Thêm NULL và 0 vào đây để đủ 9 tham số
      (uint8_t*)payload.c_str(), 
      payload.length()
    );
    // ----------------------------
  }
}