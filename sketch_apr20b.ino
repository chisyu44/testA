/*----------------------------------------------------- 
  This program is not perfect. 
  It is not usefull.
  You need write pin number that you use.
  
------------------------------------------------------*/
//start or stop , write pin number
#define SW0 4
// limit swich when 3 slot , write pin number
// future add slot number
#define LMSW1 5
#define LMSW2 6
#define LMSW3 7
//moter pwm pin , need conect pwm pin (3,5,6,9,10,11) last name PWMP
#define MO1  9
// moter pwm bulue. 0~255
#define MO1_PWM 255
// moter controll pin  
#define MO1_CTL 10
// Silial Port maybe not need
//#define SPORT

//moter status 0:stop 1:spin
unsigned int M_status = 0;

void setup() {
  /* -- !CUTION! ----------------------------------------
      input pin is pullup -> nomal status is HIGH , 
                          -> switch on is LOW
                          
      Must look Moter driver specifications.(shiyousyo)
      moter driver ctl is unknown.
  ------------------------------------------------------*/
  
  // put your setup code here, to run once:
  pinMode(SW0, INPUT_PULLUP);
  // pullup or down which?
  pinMode(LMSW1, INPUT_PULLUP);
  pinMode(LMSW2, INPUT_PULLUP);
  pinMode(LMSW3, INPUT_PULLUP);
  // output maybe not need
  //PWMP is analog output
  //pinMode(PWMP, OUTPUT);
  pinMode(MO1_CTL, OUTPUT);
  //serial 
  Serial.begin(9600);

}


//limit Switch judge system
/*next stage 
    int judge(sw1,sw2,sw3){
      int lmsw1 = digitalRead(sw1);
      int lmsw2 = digitalRead(sw2);
      int lmsw3 = digitalRead(sw3); 
    }
*/
String judge(){
  int lmsw1 = digitalRead(LMSW1);
  int lmsw2 = digitalRead(LMSW2);
  int lmsw3 = digitalRead(LMSW3);
  while(lmsw1 == LOW & lmsw2 == LOW & lmsw3 == LOW){
    //digitalWrite(MO1_CTL, HIGH);
    analogWrite(MO1, MO1_PWM);
    M_status = 1;
  }
  analogWrite(MO1, 0);
  //digitalWrite(MO1_CTL, LOW);
  M_status = 0;
  String result = String(lmsw1) + String(lmsw2) + String(lmsw3);
  return result;
  /* new idea memo . output judge
  String str = String(lmsw1) + String(lmsw2) + String(lmsw3);
  switch(str){
    case 101: char result = a;
    break;
    case 100: char result = b;
    break;
    case 010: char result = c;
    break;
  }
  return result
  */
}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MO1_CTL, HIGH);
  if (SW0 == LOW){
    switch (M_status) {
      case 0:
        //moter seiten hanten ctl
        //digitalWrite(MO1_CTL, HIGH);
        analogWrite(MO1, MO1_PWM);
        delay(10);
        M_status = 1;
        Serial.println("start spin.");
        break;
      case 1:
        analogWrite(MO1, 0);
        //digitalWrite(MO1_CTL, LOW);
        delay(10);
        M_status = 0;
        // insert limit sw judge systeme
        String sw_status = judge();
        Serial.print("OUTPUT limit switch status -> ");
        Serial.println(sw_status);
        break;
    }
  }
}