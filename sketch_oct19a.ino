const int PWM = 80;
const int Moter_pwm = 5;
const int Moter_dir = 4; 
const int SWitch = 2;
const int SWitch_start = 8;

//moter status 0 => stop 1 -> start 
int moter_state_old = 0 ;
int moter_state = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(Moter_dir, OUTPUT);
  pinMode(Moter_pwm, OUTPUT);
  pinMode(SWitch, INPUT_PULLUP);
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

void moter_status(){
  switch (moter_state) {
    case 0:
      moter_state_old = moter_state;
      moter_state = 1;
      Serial.println("moter_status -> 1");
      break;
    case 1:
      moter_state_old = moter_state;
      moter_state = 0;
      Serial.println("moter_status -> 0");
      break;
    default:
      moter_state = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(SWitch) == LOW ){
   moter_status();
  }
  
  switch(moter_state){
    case 0:
      moter_stop();
      break;
    case 1:
      moter_start();
      break;
  }
}


