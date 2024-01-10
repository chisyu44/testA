//#include <Arduino.h>
#include <stdio.h>

const int startSwitch = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(startSwitch, INPUT_PULLUP);
  Serial.begin(9600);
}


class motorctl {
private:
  
  //void attach();


public:
  //pin number
  int pwm;         //motor pwm
  int dir;         //motor dir
  int stopSwitch;  //stop switch
  int limit1;      //limit switch 1
  int limit2;      //limit switch 2
  int limit3;      //limit switch 3
  //status
  int motor_status;
  int limit_status;

  int PwmSpeed;

  int result;      //judge result ,
  //function's
  motorctl(int pwm, int dir, int stop, int limit1, int limit2, int limit3);
  void start(void);
  void stop(void);
  void limitStatus(void);
  void update(void);

};

//コンストラクタ
motorctl::motorctl(int pwm, int dir, int stop, int limit1, int limit2, int limit3) {
  //initialize pin number
  this->pwm = pwm;
  this->dir = dir;
  this->stopSwitch = stop;
  this->limit1 = limit1;
  this->limit2 = limit2;
  this->limit3 = limit3;
  //initialize status
  this->motor_status=0;   //0 => stop, 1 -> start
  this->limit_status=0;   //0 => deactive, 1 -> active
  //initialize pwm speed
  PwmSpeed = 80;
  //initialize 
  result = 111; //スイッチが押されていないとき1なので
  //initialize pin mode
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(stopSwitch, INPUT_PULLUP);
  pinMode(limit1, INPUT_PULLUP);
  pinMode(limit2, INPUT_PULLUP);
  pinMode(limit3, INPUT_PULLUP);
}

//回転させる
void motorctl::start(){
  digitalWrite(dir, HIGH);
  analogWrite(pwm, PwmSpeed);

  Serial.println("start");
  delay(100); //wait 100ms
}

//停止させる
void motorctl::stop(){
  digitalWrite(dir, HIGH);
  analogWrite(pwm, 0);

  Serial.println("stop");
  delay(100); //wait 100ms
}

//リールの位置調整を行う。絵柄が見えるところにそろうようにする関数。
void motorctl::limitStatus(){
  //while処理のがいい気がする
  if(digitalRead(limit1) == LOW || digitalRead(limit2) == LOW || digitalRead(limit3) == LOW) {
    //when push limit switch -> 1 0 1, 0 1 0, etc...
    this->stop();
    this->limit_status = 0;
    //結果を確認する処理を書く。そしてthis->resultに保存、

    //----test code-----log 
    int tmp1 = digitalRead(limit1);
    int tmp2 = digitalRead(limit2);
    int tmp3 = digitalRead(limit3);

    Serial.println(tmp1);
    Serial.println(tmp2);
    Serial.println(tmp3);
    this->result = tmp1*100 + tmp2*10 + tmp3*1;
    Serial.println(result);
    delay(10000);

  } else {
    //not push limit switch -> 1 1 1 (because button is pull up)
    digitalWrite(dir, HIGH);
    analogWrite(pwm, PwmSpeed);
    this->limitStatus();
  }
}

//ストップスイッチの処理。回転の開始と停止の関数を数値によって呼び出す
void motorctl::update(){
  //when push stop switch -> motor stop and activate limitStatus()
  if(digitalRead(this->stopSwitch) == LOW){
    //モーターが回っているときのみ有効にする
    if(this->motor_status == 1){
      this->motor_status = 0;
      this->limit_status = 1;
      //log
      Serial.print("stop switch low");
      Serial.println(this->motor_status);
    }
  }
  //モーターのstatus変数によって回転制御を反映させる
  //1:回転　0:停止
  if(this->motor_status == 1){
    this->start();
  } else {
    this->stop();
  }
  //判定が有効時、判定用関数を実行
  if(this->limit_status == 1){
    this->limitStatus();
  }
}


//引数はかくリールのresultの値 クラスの値から参照できるようにするref。
int upload( int m1,  int m2, int m3){
  //log
  Serial.println("func upload()");
  Serial.println(m1);
  Serial.println(m2);
  Serial.println(m3);

  if(m1 != 111 && m2 != 111 && m3 != 111){

    Serial.println("cheak");
    if( m1 == m2 && m2 == m3){

      //Serial.printをSerial.write に変更する。
      Serial.println("switch");
      switch(m1){
        //旋盤
        case 0:
          Serial.write("a");
          return 1;
          break;
        //ワイヤー放電
        case 1:
          Serial.write("b");
          return 1;
          break;
        //猫
        case 11:
          Serial.write("c");
          return 1;
          break;
        //ボール盤
        case 10:
          Serial.write("d");
          return 1;
          break;
        //フライス
        case 110:
          Serial.write("e");
          return 1;
          break;
        //溶接
        case 100:
          Serial.write("f");
          return 1;
          break;
        // 上記以外はあたりではないので外れとして処理。デフォルトははずれ
        default:
          Serial.write("n");
          return 1;
        }
    } else {
          //はずれの処理
        Serial.write("n");
        return 1;
    }
    //delay(5000);
  } else{
  return 0;
  }
}

//オブジェクトの作成　リール一つにつき一つ
//引数でスイッチのpinを
  motorctl m01(11, 12, 8, 3, 4, 5);


void loop() {
  // put your main code here, to run repeatedly:

  //スタートスイッチが押されたとき、もろもろの初期化　　　　　 
  if(digitalRead(startSwitch) == LOW){
    m01.motor_status = 1;
    m01.limit_status = 0;
    m01.result = 111;
    //log
    Serial.print("start switch low");
    Serial.println(m01.motor_status);
  }

  m01.update();
  int tmp = 0;
  //ここでリミットスイッチの状態の結果を送信
  //送信したら返り値が１。してなければ０をかえす。
  //現在リール一つ分での結果を使って検証してる。
  tmp = upload(m01.result,m01.result,m01.result);
  if(tmp == 1){
    m01.limit_status = 111;
  }
 }
