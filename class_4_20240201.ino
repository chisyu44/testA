//#include <Arduino.h>
#include <stdio.h>
//debug log 1->on
int debugLog = 0;

const int startSwitch = 13;

const int m01Pwm = 9;
const int m01Dir = 12;
const int m01Stop = 6;
const int m01Limit1 = 3;
const int m01Limit2 = 4;
const int m01Limit3 = 5;

const int m02Pwm = 10;
const int m02Dir = 12;
const int m02Stop = 7;
const int m02Limit1 = 14;
const int m02Limit2 = 15;
const int m02Limit3 = 16;

const int m03Pwm = 11;
const int m03Dir = 12;
const int m03Stop = 8;
const int m03Limit1 = 17;
const int m03Limit2 = 18;
const int m03Limit3 = 19;

void setup() {
  // put your setup code here, to run once:
  pinMode(startSwitch, INPUT_PULLUP);
  Serial.begin(9600);
}


class motorctl {
private:

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
  String name;     //this class name 
  //function's
  motorctl(int pwm, int dir, int stop, int limit1, int limit2, int limit3, String name);
  void start(void);
  void stop(void);
  void limitStatus(void);
  void update(void);

};

//コンストラクタ
motorctl::motorctl(int pwm, int dir, int stop, int limit1, int limit2, int limit3, String name) {
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
  //this class name 
  this->name = name;
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
  if(debugLog == 1){
    Serial.print(this->name);
     Serial.println(".start()-->start");
  }
 
  delay(100); //wait 100ms
}

//停止させる
void motorctl::stop(){
  //dirの出力でスイッチのライト光らせれるかも
  digitalWrite(dir, HIGH);
  analogWrite(pwm, 0);
  if(debugLog == 1){
  Serial.print(this->name);
  Serial.println(".stop()-->stop");
  }

  delay(100); //wait 100ms
}

//リールの位置調整を行う。絵柄が見えるところにそろうようにする関数。
void motorctl::limitStatus(){
  //while処理のがいい気がする this いらないの？
  if(digitalRead(this->limit1) == LOW || digitalRead(this->limit2) == LOW || digitalRead(this->limit3) == LOW) {
    //when push limit switch -> 1 0 1, 0 1 0, etc...
    this->stop();
    this->limit_status = 0;
    //結果を確認する処理を書く。そしてthis->resultに保存、
    int tmp1 = digitalRead(this->limit1);
    int tmp2 = digitalRead(this->limit2);
    int tmp3 = digitalRead(this->limit3);
    this->result = tmp1*100 + tmp2*10 + tmp3*1;

    //リミットスイッチが反応して止まった後に状態を確認するまでにラグがある。
    //上記から、tmp1~3の値が１になり正常に判定が動かない時がある。
    //そのため、この時点で正常に働いていなかった場合111となるときなのでもう一度呼び出して検証する。
    
    if (this->result == 111){
      this->limitStatus();
    }
    
    if(debugLog == 1){
          //----test code-----log 
      Serial.print(this->name);
      Serial.println(".limitStatus --> ");
      Serial.println(tmp1);
      Serial.println(tmp2);
      Serial.println(tmp3);
      Serial.println(result);
    }


  } else {
    //not push limit switch -> 1 1 1 (because button is pull up)
    digitalWrite(this->dir, HIGH);
    analogWrite(this->pwm, this->PwmSpeed / 2);
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
      if(debugLog == 1){
        Serial.print(this->name);
        Serial.print(".update()-->stop switch low :motor_status=");
        Serial.println(this->motor_status);
      }

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
  if(debugLog == 1){
    Serial.println("upload()-->output result value");
    Serial.println(m1);
    Serial.println(m2);
    Serial.println(m3);
  }
  

  if(m1 != 111 && m2 != 111 && m3 != 111){
    if(debugLog == 1){
      Serial.println("upload()-->check");   
    }
    

    if( m1 == m2 && m2 == m3){
      int i = m1 ;
      //Serial.printをSerial.write に変更する。
      if(debugLog == 1){
        Serial.println("upload()->switch");
      }

      switch(i){
        //旋盤
        case 0:
          Serial.write("a");
          return 1;
          break;
        //溶接
        case 100:
          Serial.write("b");
          return 1;
          break;
        //フライス
        case 110:
          Serial.write("c");
          return 1;
          break;
        //ボール盤
        case 10:
          Serial.write("d");
          return 1;
          break;
        //ねこ
        case 11:
          Serial.write("e");
          return 1;
          break;
        //ワイヤー
        case 1:
          Serial.write("f");
          return 1;
          break;
        // 上記以外はあたりではないので外れとして処理。デフォルトははずれ
        default:
          Serial.write("n");
          if(debugLog == 1){
            Serial.println("/nupload()-->defalut");
          }
          return 1;
        }
    } else {
          //はずれの処理
        Serial.write("n");
        if(debugLog == 1){
          Serial.println("/nupload()->n(m1=m2=m3)");
        }
        return 1;
    }
    //delay(5000);
  } else{
    if(debugLog == 1){
      Serial.println("upload()--> not stop all slot Can't get result");
    }

  return 0;
  }
}

//オブジェクトの作成　リール一つにつき一つ
//引数でスイッチのpinを
  motorctl m01(m01Pwm, m01Dir, m01Stop, m01Limit1, m01Limit2, m01Limit3, "m01");
  motorctl m02(m02Pwm, m02Dir, m02Stop, m02Limit1, m02Limit2, m02Limit3, "m02");
  motorctl m03(m03Pwm, m03Dir, m03Stop, m03Limit1, m03Limit2, m03Limit3, "m03");


void loop() {
  // put your main code here, to run repeatedly:

  //スタートスイッチが押されたとき、もろもろの初期化　　　　　 
  if(digitalRead(startSwitch) == LOW){
    if(m01.motor_status == 0 && m02.motor_status == 0 && m03.motor_status == 0){
      m01.motor_status = 1;
      m01.limit_status = 0;
      m01.result = 111;

      m02.motor_status = 1;
      m02.limit_status = 0;
      m02.result = 111;
    
      m03.motor_status = 1;
      m03.limit_status = 0;
      m03.result = 111;

      //log
      if(debugLog == 1){
        Serial.print("loop()-->start switch low");
        Serial.println(m01.motor_status);
        Serial.println(m02.motor_status);
        Serial.println(m03.motor_status);
      }
    } else {
      Serial.println("loop()--> all ready started, can't start");
    }
  }

  m01.update();
  m02.update();
  m03.update();

  int tmp = 0;
  //ここでリミットスイッチの状態の結果を送信
  //送信したら返り値が１。してなければ０をかえす。
  //現在リール一つ分での結果を使って検証してる。
  tmp = upload(m01.result,m02.result,m03.result);
  //upload()の返り値が1のとき、
  if(tmp == 1){
    //リミットスイッチの各リールの結果を格納している .result の値を初期化する。
    m01.result = 111;
    m02.result = 111;
    m03.result = 111;
    if(debugLog == 1){
      Serial.println("\nupload()-->send and clear this->.result data");
    }
  }  
  if(debugLog == 2){
      int l01 = digitalRead(m01.limit1)*100 + digitalRead(m01.limit2)*10 + digitalRead(m01.limit3);
      int l02 = digitalRead(m02.limit1)*100 + digitalRead(m02.limit2)*10 + digitalRead(m02.limit3);
      int l03 = digitalRead(m03.limit1)*100 + digitalRead(m03.limit2)*10 + digitalRead(m03.limit3);
      Serial.print("limit status-->");
      Serial.print(l01);
      Serial.print(" ");
      Serial.print(l02);
      Serial.print(" ");
      Serial.println(l03);
  }
 }

 void serialEventRun(){
  char data = Serial.read();
  if(data == 's'){
    debugLog = 1;
  }
  switch (data){
    case 'd':
      debugLog = 1;
      break;
    case 'e':
      debugLog = 2;
      break;
    case 'c':
      debugLog = 0;
      break;
    case 'p':
      Serial.println("emaergency stop");
      Serial.println("to exit , please send char 'c' use serial");
      while (1){
        analogWrite(m01.pwm, 0);
        analogWrite(m02.pwm, 0);
        analogWrite(m03.pwm, 0);
        m01.motor_status = 0;
        m02.motor_status = 0;
        m03.motor_status = 0;
        if(Serial.available() > 0){
          char data1 = Serial.read();
          if(data1 == 'c'){
            Serial.println("send 'c' will exit");
            break;
          }
        }
      }
    
    default:
      break;
  }
 }