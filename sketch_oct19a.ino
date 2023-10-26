//pwmの設定
const int PWM = 80;
//モータードライバのpwmに接続されているpin(回転速度)
const byte Moter_pwm = 5;
//モータードライバのdirに接続されているpin(回転方向)
const byte Moter_dir = 4; 
//回転停止用のボタンが接続されているpin
const byte SWitch_stop = 2;
//回転開始用のボタンが接続されているpin
const byte SWitch_start = 8;
//リミットスイッチが接続されているpin
const byte limit1 = 7;
//リミットスイッチの判定が有効か無効かをしめす。0が無効。1が有効。
int limit_state = 0;
//判定の結果
int result = 0;

//moter status 0 => stop 1 -> start 
int moter_state_old = 0 ;
//モーターの状態を示す。０が停止。１が回転中。
int moter_state = 0;

void setup() {
  //出力の設定
  pinMode(Moter_dir, OUTPUT);
  pinMode(Moter_pwm, OUTPUT);
  //入力の設定。プルアップしている。(押されてないとHIGH,押されるとLOWになる）
  pinMode(SWitch_stop, INPUT_PULLUP);　
  pinMode(SWitch_start, INPUT_PULLUP);
  pinMode(limit1, INPUT_PULLUP);
  //シリアル通信の設定
  Serial.begin(9600);
}

//モーターを回すための関数
void moter_start(){
  digitalWrite(Moter_dir, LOW);  //回転方向の設定
  analogWrite(Moter_pwm, PWM);  //PWM(速さ)を最初に設定したMoter_pwn変数の値(80)に設定
  Serial.println("start");　//ログ
  delay(100);  //100ms待つ
}

//モーターを停止させるための関数
void moter_stop(){
  digitalWrite(Moter_dir,LOW);　//回転方向の設定
  analogWrite(Moter_pwm, 0);  //PWM(速さ)を０に設定。停止
  Serial.println("stop");  //ログ
  delay(1000);  //1000ms待つことで確実に停止させる。startとstopのスイッチが分かれるため50ms程度で十分。次回変更
}

//モーターの状態を変更するための関数。次回消去予定
void moter_status() {
  switch (moter_state) {
    case 0:
      moter_state_old = moter_state;
      moter_state = 1;
      Serial.println("moter_status -> 1");
      break;
    case 1:
      moter_state_old = moter_state;
      moter_state = 0;
      Serial.print("->");
      Serial.println(moter_state_old);
      Serial.println("moter_status -> 0");
      break;
    default:
      moter_state = 0;
  }
}

//リミットスイッチの状態を読み取ってリミットスイッチが押されるように回転を調整する関数
void limit_status(){
  switch (digitalRead(limit1)){
    //リミットスイッチが押されているとき
    case 0:
      moter_stop();　//モーター停止
      limit_state = 0; //この関数を無効にし実行されなくする。
      result = 1;   //リミットスイッチの判定の結果を保存する。
      break;
    //リミットスイッチが押されていないとき
    case 1:
      //モーターを回転させる。
      digitalWrite(Moter_dir, LOW);
      analogWrite(Moter_pwm, PWM);
      //関数自身を呼び出してループする。-> リミットスイッチが押されるまで回転し続ける。
      //リミットスイッチが押されるまで回り続ける。
      limit_status();  
      break;
  }
  
}


void loop() {
  //ストップスイッチが押されたとき、
  if(digitalRead(SWitch_stop) == LOW ){
    //moter＿state変数の値を0に書き換える。
    moter_state = 0;
    //リミットスイッチの判定を有効にする
    limit_state = 1;
  }
  
  //ストップスイッチが押されたとき
  if(digitalRead(SWitch_start) == LOW){
    //moter＿state変数の値を1に書き換える。
    moter_state = 1;
    //判定結果をリセット。
    result = 0;
   }

  //判定が有効になっているとき
  if (limit_state == 1){
    //判定を行う関数を実行。
    limit_status();
    //結果をシリアル通信で出力。
    Serial.print("limit_status result -> ");
    Serial.println(result);
  }
  //モーターの状態を示す変数の値を実際の動作に反映
  switch(moter_state){
    //回転停止
    case 0:
      moter_stop();
      break;
    //回転開始
    case 1:
      moter_start();
      break;
  }
}


