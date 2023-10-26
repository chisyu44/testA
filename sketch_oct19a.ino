//pwmの設定
const int PWM = 80;
//モータードライバのpwmに接続されているpin
const int Moter_pwm = 5;
//モータードライバのdirに接続されているpin
const int Moter_dir = 4; 

const int SWitch = 2;

const int SWitch_start = 8;

const byte limit1 = 7;

int limit_state = 0;

int result = 0;

//moter status 0 => stop 1 -> start 
int moter_state_old = 0 ;
int moter_state = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(Moter_dir, OUTPUT);
  pinMode(Moter_pwm, OUTPUT);
  pinMode(SWitch, INPUT_PULLUP);
  pinMode(SWitch_start, INPUT_PULLUP);
  pinMode(limit1, INPUT_PULLUP);
  Serial.begin(9600);
}

void moter_start(){
  digitalWrite(Moter_dir, LOW);
  analogWrite(Moter_pwm, PWM);
  Serial.println("start");
  delay(100);
}

void moter_stop(){
  digitalWrite(Moter_dir,LOW);
  analogWrite(Moter_pwm, 0);
  Serial.println("stop");
  delay(1000);
}

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

void limit_status(){
  switch (digitalRead(limit1)){
    case 0:
      moter_stop();
      limit_state = 0;
      result = 1;
      break;
    case 1:
      digitalWrite(Moter_dir, LOW);
      analogWrite(Moter_pwm, PWM);
      limit_status();  
      break;
  }
  /*if(digitalRead(limit1) == LOW ){
    moter_stop();
    return 1;
  } else {
    digitalWrite(Moter_dir, LOW);
    analogWrite(Moter_pwm, PWM);
    limit_status();
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:
  //stop
  if(digitalRead(SWitch) == LOW ){
    moter_state = 1;
  //モーターの回転してるかしてないかをしめす変数の値を変更する関数
   moter_status();
   limit_state = 1;
  }
  //start
  if(digitalRead(SWitch_start) == LOW){
    moter_state = 0;
    result = 0;
    moter_status();
   }

  if (limit_state == 1){
    limit_status();
    Serial.print("limit_status result -> ");
    Serial.println(result);
  }
  //モーターの状態を示す変数の値を実際の動作に反映
  switch(moter_state){
    case 0:
      moter_stop();
      break;
    case 1:
      moter_start();
      break;
  }
}


