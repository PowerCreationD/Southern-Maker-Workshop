 #include <Ultrasonic.h> // 引入 Ultrasonic 函式庫標頭檔

Ultrasonic ultrasonic(12, 13); // ultrasonic(Trig 腳位, Echo 腳位)

void setup() {
  Serial.begin(9600); // 序列通訊設定 Baud rate 為 9600
  ultrasonic.setTimeout(40000UL); // 設定等待超音波訊號回來的時間，40ms (最遠量測距離：6.8m)
}

void loop() {
  Serial.print("Distance in CM: ");
  Serial.println(ultrasonic.read()); // 顯示距離讀值
  delay(1000);
}