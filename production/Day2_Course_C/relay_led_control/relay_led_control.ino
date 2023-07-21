const byte IN1_PIN = 12; // 宣告觸發腳位為 D12

void setup() {
  pinMode(IN1_PIN, OUTPUT); // 設定為數位輸出
}

void loop() {
  // 開關交替
  digitalWrite(IN1_PIN, HIGH);  
  delay(1000);                      
  digitalWrite(IN1_PIN, LOW);   
  delay(1000);                      
}