#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include "CugoArduinoMode.h"
#include "MotorController.h"

// Prototype 宣告
void CMD_EXECUTE();

// 宣告使用的電機數量
MotorController motor_controllers[2];

const byte modeLED = 12;
char receivedChar;
char userCommand;
// boolean newData = false;

//初期設定
void setup()
{
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  pinMode(4, OUTPUT);
  pinMode(12, OUTPUT);

  init_display();
  init_PID();
  init_KOPROPO(runMode,OLD_PWM_IN_PIN0_VALUE,OLD_PWM_IN_PIN1_VALUE,OLD_PWM_IN_PIN2_VALUE);
  init_ARDUINO_CMD();
  init_SPI();
}

//loop内重複執行
void loop()
{
  current_time = micros();  // 大約需要 40 分鐘左右直到溢位（Overflow）發生
  if (current_time - prev_time_10ms > 10000) 
  {
    job_10ms();
    prev_time_10ms = current_time;
  }
  display_detail(motor_controllers);// 根據需要，可以修改 CugoArduinoMode 中的第5至8行
}

void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    if (!isSpace(receivedChar)) {
      userCommand = receivedChar;
      String desc = "receivedChar: ";
      String msg = desc + receivedChar;
      Serial.println(msg);
    }
    // newData = true;
  }
}

// void showNewData() {
//   if (newData == true) {

//     newData = false;

//     switch (receivedChar) {
//       case '0':  // receiving 0 turn off light
//         Serial.println("turn off light!");
//         digitalWrite(4, LOW);
//         break;
//       case '1':  // receiving 1 will turn on light
//         Serial.println("turn on light!");
//         digitalWrite(4, HIGH);
//         break;
//       case '2':  // moving forward
//         // CMD_EXECUTE();
//         Serial.println("move forward!");
//         break;
//     }
//   }
// }

//中斷處理
ISR(PCINT2_vect)
{
  if (OLD_PWM_IN_PIN0_VALUE != digitalRead(PWM_IN_PIN0))
  {
    if (LOW == OLD_PWM_IN_PIN0_VALUE)
    { // 啟動時的處理
      PIN_UP(0);
    }
    else
    { // 關機時的處理
      PIN_DOWN(0);
    }
    OLD_PWM_IN_PIN0_VALUE = OLD_PWM_IN_PIN0_VALUE ? LOW : HIGH;
  }
  if (OLD_PWM_IN_PIN1_VALUE != digitalRead(PWM_IN_PIN1))
  {  
    if (LOW == OLD_PWM_IN_PIN1_VALUE)
    { // 立ち上がり時の処理
      PIN_UP(1);
    }
    else
    { // 立下り時の処理
      PIN_DOWN(1);
    }
    OLD_PWM_IN_PIN1_VALUE = OLD_PWM_IN_PIN1_VALUE ? LOW : HIGH;
  }
  if (OLD_PWM_IN_PIN2_VALUE != digitalRead(PWM_IN_PIN2))
  {
    if (LOW == OLD_PWM_IN_PIN2_VALUE)
    { // 立ち上がり時の処理
      PIN_UP(2);
    }
    else
    { // 立下り時の処理
      PIN_DOWN(2);
    }
    OLD_PWM_IN_PIN2_VALUE = OLD_PWM_IN_PIN2_VALUE ? LOW : HIGH;
  }
}

//自動走行モード(arduino_mode)の実行
void arduino_mode(char usrcmd)
{
  digitalWrite(LED_BUILTIN, HIGH);  // ARDUINO_MODEでLED点灯
  CMD_EXECUTE(usrcmd);
  for (int i = 0; i < MOTOR_NUM; i++) { 
    motor_controllers[i].driveMotor();
  }
}

//モード切り替わり確認
void check_mode_change()
{
  noInterrupts();      //中斷停止
  rcTime[0] = time[0];
  rcTime[1] = time[1];
  rcTime[2] = time[2];
  interrupts();     //中斷開始

  if (ARDUINO_MODE_IN < rcTime[1] && rcTime[1] < CUGO_PROPO_MAX_B) // 當MR-8出現異常值時，不進行轉換
  {
    if (runMode != ARDUINO_MODE)
    { // 當模式改變時 (RC→ARDUINO)
      Serial.println(F("##########################"));                  
      Serial.println(F("### モード:ARDUINO_MODE ###"));
      Serial.println(F("##########################"));            
      stop_motor_immediately(motor_controllers);
      reset_arduino_mode_flags();
      reset_pid_gain(motor_controllers);
    }
    runMode = ARDUINO_MODE;
  }
  else if (ARDUINO_MODE_OUT > rcTime[1] && CUGO_PROPO_MIN_B < rcTime[1]) // 當MR-8出現異常值時，不進行轉換
  {
    if (runMode != RC_MODE)
    { // 當模式改變時 (ARDUINO→RC)
      Serial.println(F("##########################"));                  
      Serial.println(F("###   モード:RC_MODE    ###"));
      Serial.println(F("##########################"));            
      reset_arduino_mode_flags();
      Serial.println(userCommand);
      userCommand = '\0';
      Serial.println(userCommand);
    }
    runMode = RC_MODE;
  }
  
  if (ARDUINO_MODE == runMode)
  {
    digitalWrite(modeLED, HIGH);
    
    if (userCommand != '\0') { 
      arduino_mode(userCommand);
      Serial.println(end_arduino_mode);
    }
  }
  else
  {
    digitalWrite(modeLED, LOW);
    Serial.println(userCommand);
    userCommand = '\0';
    Serial.println(userCommand);
    rc_mode(rcTime,motor_controllers);
  }
}

//割り込み時の実行処理関連
void init_PID()
{
  //Serial.println(F("#   init_PID"));//確認用
  pinMode(PIN_ENCODER_L_A, INPUT_PULLUP);     //A相用信号入力　入力割り込みpinを使用　内蔵プルアップ有効
  pinMode(PIN_ENCODER_L_B, INPUT_PULLUP);     //B相用信号入力　内蔵プルアップ有効
  pinMode(PIN_ENCODER_R_A, INPUT_PULLUP);     //A相用信号入力　入力割り込みpinを使用　内蔵プルアップ有効
  pinMode(PIN_ENCODER_R_B, INPUT_PULLUP);     //B相用信号入力　内蔵プルアップ有効

  // LEFTインスタンス有効化
  motor_controllers[MOTOR_LEFT] = MotorController(PIN_ENCODER_L_A, PIN_ENCODER_L_B, PIN_MOTOR_L, 2048, 600, 100, L_LPF, L_KP, L_KI, L_KD, L_reverse);
  // RIGHTインスタンス有効化
  motor_controllers[MOTOR_RIGHT] = MotorController(PIN_ENCODER_R_A, PIN_ENCODER_R_B, PIN_MOTOR_R, 2048, 600, 100, R_LPF, R_KP, R_KI, R_KD, R_reverse);

  // エンコーダカウンタは純正のハードウェア割り込みピンを使用
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_L_A), leftEncHandler, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_R_A), rightEncHandler, RISING);

  // 初期値でモータ指示。起動時に停止を入力しないと保護機能が働き、回りません。
  motor_direct_instructions(1500, 1500,motor_controllers); //直接停止命令を出す
  delay(100); // すぐに別の値でモータを回そうとするとガクガクするので落ち着くまで待つ。10ms程度でも問題なし。
}

//割り込み時の実行処理関連
void leftEncHandler()
{
  motor_controllers[MOTOR_LEFT].updateEnc();
}

//割り込み時の実行処理関連
void rightEncHandler()
{
  motor_controllers[MOTOR_RIGHT].updateEnc();
}
//10ミリ秒毎に実行
void job_10ms()
{
  recvOneChar();
  showNewData();
  check_mode_change();
}

/***** Arduino学習用プログラミングはここから *****/
/* ヒント：使えるコマンドリスト */
/*
 * ★基本コマンド★
 *    button();          //ボタンの押し待ち     
 *    matsu(1000);       //1000ミリ秒待機       □詳細：入力した()内の数字ミリ秒だけ待機する。1000ミリ秒＝1秒。
 *    susumu(1.0);       //1.0m前に進む         □詳細：入力した()内の数字m 分だけ前に進む
 *    sagaru(1.0);       //1.0m後ろに進む       □詳細：入力した()内の数字m 分だけ後ろに進む
 *    migimawari45();    //45度右に回転する 
 *    migimawari90();    //90度右に回転する
 *    migimawari180();   //180度右に回転する
 *    hidarimawari45();  //45度右に回転する
 *    hidarimawari90();  //90度右に回転する
 *    hidarimawari180(); //180度右に回転する
 * 
 * ★★応用コマンド★★
 *    susumu(1.0,90);                //上限速度90rpmで1.0m前に進む         □詳細：()内に移動距離と上限速度を設定。上限速度は最大180rpmで距離が短いと上限速度に到達しない場合もある。
 *    sagaru(1.0,90);                //上限速度90rpmで1.0m後ろに進む      　□詳細：()内の設定はsusumu(1.0,90)と同様
 *    migimawari45(90);              //上限速度90rpmで45度右に回転する      □詳細：()内に上限速度を設定。上限速度は最大180rpmで距離が短いと上限速度に到達しない場合もある。
 *    migimawari90(90);              //上限速度90rpmで90度右に回転する      □詳細：()内の設定はmigimawari45(90)と同様
 *    migimawari180(90);             //上限速度90rpmで180度右に回転する     □詳細：()内の設定はmigimawari45(90)と同様
 *    hidarimawari45(90);            //上限速度90rpmで45度左に回転する      □詳細：()内の設定はmigimawari45(90)と同様
 *    hidarimawari90(90);            //上限速度90rpmで90度左に回転する      □詳細：()内の設定はmigimawari45(90)と同様
 *    hidarimawari180(90);           //上限速度90rpmで180度左に回転する     □詳細：()内の設定はmigimawari45(90)と同様
 *    turn_clockwise(60,90);         //上限速度90rpmで60度右に回転する      □詳細：()内に回転角度と上限速度を設定。上限速度は最大180rpmで距離が短いと上限速度に到達しない場合もある。
 *    turn_counter_clockwise(60,90); //上限速度90rpmで60度左に回転する      □詳細：()内の設定はturn_clockwise(60,90)と同様
*/

//コマンドの実行
void CMD_EXECUTE(char usrcmd)
{
  cmd_manager(motor_controllers);  // おまじない

  // ここから↓を改造していこう！

  //例：ボタンが押されたら1mの正方形を描く動き
  
  // button();//ボタンが押されるまで待つ

  switch (usrcmd) {
    case 'f':
      // Serial.println("EXECUTE moving forward 1m");
      susumu(1.0);
      break;
    case 'm': 
      // Serial.println("EXECUTE mission 1");
      mission1();
      break;
  }

  cmd_end(motor_controllers);      // おまじない
}

void mission1() {

  susumu(1.0);//1.0m 進む
  matsu(1000);//1000ミリ秒(1秒) 待つ
  migimawari90();//右回りに90度回転
  matsu(1000);//1000ミリ秒(1秒) 待つ
   
  susumu(1.0);
  matsu(1000);
  migimawari90();
  matsu(1000); 
  
  susumu(1.0);
  matsu(1000);
  migimawari90();
  matsu(1000); 
  
  susumu(1.0);
  matsu(1000);
  migimawari90();
  matsu(1000); 

}
