const byte led_pin = 13; // 宣告腳位常數

// 設定參數函式，從頭到尾只會被執行一次
void setup() {

  pinMode(led_pin, OUTPUT); // 設定 D13 腳位為輸出

}

// loop 函式，將不停地重複執行
void loop() {

  digitalWrite(led_pin, HIGH); // D13 腳位輸出高電位
  delay(1000); // 延遲 1000 ms
  digitalWrite(led_pin, LOW);
  delay(1000); // 延遲 1000 ms

}
