#include "HX711.h" // 引入 HX711 函式庫標頭檔

// HX711 接線設定
const int DT_PIN = 2;  // 與 HX711 通訊腳位
const int SCK_PIN = 3; // 與 HX711 通訊腳位
const int sample_weight = 156;  //基準物品的真實重量(公克)

HX711 scale; // 以名為 HX711 的 Class，創建一個名為 scale 的 Object

void setup() {
  Serial.begin(9600); // 序列通訊設定 Baud rate 為 9600
  scale.begin(DT_PIN, SCK_PIN); // 初始化與 HX711 模組的通訊
  scale.set_scale();  // 設置磅秤比例參數
  scale.tare(); // 歸零
  Serial.println("Nothing on the scale.");
  Serial.println(scale.get_units(10)); // 取得10次重量讀值的平均
  Serial.println("Please put sample object on to the scale..."); //提示放上基準物品
}

void loop() {
  float current_weight = scale.get_units(10);  // 取得10次重量讀值的平均
  float scale_factor = (current_weight/sample_weight);
  Serial.print("Scale factor:  ");
  Serial.println(scale_factor,0);  // 顯示比例參數，紀錄起來，會用在正式秤重的程式中
  
}