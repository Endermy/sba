#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>
// 在这里写入服务器地址:端口
#define SERVER_IP "http://input.domain.name:1009"
// 在这里写入wifi名
#define STASSID "wifi"
// 在这里写入wifi密码
#define STAPSK "password"
// 在这里写入子节点名字
#define ESPname ESP1

void setup()
{
  Serial.begin(9600);
  Serial.println();
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void http_post()
{
  WiFiClient client;
  HTTPClient http;
  string domainname = SERVER_IP + "/postData";
  http.begin(client, domainname);
  Serial.print("[HTTP] begin...\n");

  std::string tempPostContent = "ESPname=ESP1,data1=" + std::to_string(analogRead(A0));
  const String postContent = tempPostContent.c_str();
  int httpCode = http.POST(postContent);

  if (httpCode > 0)
  {

    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK)
    {
      const String &payload = http.getString();
      Serial.println("received payload:");
      Serial.print(">>");
      Serial.println(payload);
    }
  }
  else
  {
    Serial.printf("[HTTP] POST... failed\n>>error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    http_post();
  }
  delay(300);
}