#include <Arduino.h>
#include <MqttConnector.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;
extern MqttConnector* mqtt;

extern String MQTT_CLIENT_ID;
extern String MQTT_PREFIX;

extern int relayPin;
extern int relayPinState;
extern char myName[];

extern boolean light;

extern void colorWipe(uint32_t c, uint8_t wait);
extern void rainbow(uint8_t wait);
extern uint32_t Wheel(byte WheelPos);


void register_receive_hooks() {
  mqtt->on_subscribe([&](MQTT::Subscribe * sub) -> void {
    Serial.printf("myName = %s \r\n", myName);
    sub->add_topic(MQTT_PREFIX + myName + "/$/+");
    sub->add_topic(MQTT_PREFIX + MQTT_CLIENT_ID + "/$/+");
  });

  mqtt->on_before_message_arrived_once([&](void) { });

  mqtt->on_message([&](const MQTT::Publish & pub) { });

  mqtt->on_after_message_arrived([&](String topic, String cmd, String payload) {
    Serial.printf("topic: %s\r\n", topic.c_str());
    Serial.printf("cmd: %s\r\n", cmd.c_str());
    Serial.printf("payload: %s\r\n", payload.c_str());


    if (cmd == "$/command") {
      if (payload == "ON") {
        digitalWrite(relayPin, HIGH);
        digitalWrite(LED_PIN, LOW);
        relayPinState = HIGH;
        light = true;
        //        strip.setBrightness(50);
        //        //        colorWipe(strip.Color(0, 0, 255), 50);
        //        rainbow(100);
        //        strip.show();
      }
      else if (payload == "OFF") {
        digitalWrite(relayPin, LOW);
        digitalWrite(LED_PIN, HIGH);
        relayPinState = LOW;
        light = false;
        //        strip.setBrightness(50);
        //        colorWipe(strip.Color(0, 0, 0), 50);
        //        //  rainbow(20);
        //        strip.show();
      }
    }
    else if (cmd == "$/reboot") {
      ESP.restart();
    }
    else {
      // another message.
    }
  });
}
