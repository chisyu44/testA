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

  char result;      //judge result ,
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
  result = '0';
  //initialize pin mode
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(stopSwitch, INPUT_PULLUP);
  pinMode(limit1, INPUT_PULLUP);
  pinMode(limit2, INPUT_PULLUP);
  pinMode(limit3, INPUT_PULLUP);
}

void motorctl::start(){
  digitalWrite(dir, HIGH);
  analogWrite(pwm, PwmSpeed);
  Serial.println("start");
  delay(100); //wait 100ms
}

void motorctl::stop(){
  digitalWrite(dir,LOW);
  analogWrite(pwm, 0);
  Serial.println("stop");
  delay(100); //wait 100ms
}

void motorctl::limitStatus(){
  if(digitalRead(limit1) == LOW || digitalRead(limit2) == LOW || digitalRead(limit3) == LOW) {
    //when push limit switch -> 1 0 1, 0 1 0, etc...
    this->stop();
    this->limit_status = 0;
    //結果を確認する処理を書く。そしてthis->resultに保存、




  } else {
    //not push limit switch -> 1 1 1 (because button is pull up)
    digitalWrite(dir, HIGH);
    analogWrite(pwm, PwmSpeed);
    this->limitStatus();
  }
}

void motorctl::update(){
  //when push stop switch -> motor stop and activate limitStatus()
  if(digitalRead(this->stopSwitch) == LOW){
    this->motor_status = 0;
    this->limit_status = 1;
    Serial.print("stop switch low");
    Serial.println(this->motor_status);
  }
  //モーターのstatus変数によって回転制御を反映させる
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
  motorctl m01(11, 12, 8, 3, 4, 5);
void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(startSwitch) == LOW){
    m01.motor_status = 1;
    Serial.print("start switch low");
    Serial.println(m01.motor_status);
  }

  m01.update();
 }
