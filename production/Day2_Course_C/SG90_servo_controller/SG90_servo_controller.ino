#include <Servo.h>   //載入函式庫，這是內建的，不用安裝

Servo myservo;  // 建立SERVO物件


void setup() {
  myservo.attach(9);  // 設定要將伺服馬達接到哪一個PIN腳
}

void loop() {   
  myservo.write(0);  //旋轉到0度，就是一般所說的歸零
  delay(2000);
  myservo.write(90); //旋轉到90度
  delay(2000);
  // myservo.write(0); //旋轉到180度
  // delay(2000);
  // myservo.write(90);
  // delay(2000);
}