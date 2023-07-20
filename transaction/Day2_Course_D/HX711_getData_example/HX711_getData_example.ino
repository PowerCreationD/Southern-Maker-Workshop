#include "HX711.h"

// HX711 接線設定
const int DT_PIN = 2;
const int SCK_PIN = 3;

const int scale_factor = 378; //磅秤比例參數，從校正程式中得出

HX711 scale; // 以名為 HX711 的 Class，創建一個名為 scale 的 Object

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing the scale");
  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("Before setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);	//未設定磅秤比例參數前的數值

  scale.set_scale(scale_factor);       // 設定磅秤比例參數
  scale.tare();				        // 歸零

  Serial.println("After setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);  //設定磅秤比例參數後的數值
  Serial.println("Readings:");  //出現這個訊息後，再放置東西在電子秤上
}


void loop() {
  
  Serial.println(scale.get_units(10), 0); 

  scale.power_down();			        // 進入睡眠模式
  delay(100);
  scale.power_up();               // 結束睡眠模式
}