 #include <Ultrasonic.h> // 引入 Ultrasonic 函式庫標頭檔
 const byte led_pin = 8; // 宣告 led 腳位常數
 int distance; // 宣告 distance 全域變數

Ultrasonic ultrasonic(12, 13); // ultrasonic(Trig 腳位, Echo 腳位)

void setup() {
  Serial.begin(9600); // 序列通訊設定 Baud rate 為 9600
  pinMode(led_pin, OUTPUT);
  ultrasonic.setTimeout(40000UL); // 設定等待超音波訊號回來的時間，40ms (最遠量測距離：6.8m)
}

void loop() {
  
  distance = ultrasonic.read(); // distance 變數紀錄讀值
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  led_control(); // led 控制函式
}

void led_control() {

  // 條件判斷
  if (distance >= 15) {
    digitalWrite(led_pin, LOW);
    delay(1000);
  } else {
    digitalWrite (led_pin, HIGH);
    delay (500);
    digitalWrite (led_pin, LOW);
    delay (500);
  }

}
