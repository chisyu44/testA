//const int LED = 13;
const int BUTTON = 7;
int val = 0;

const int M01=5;
int M01_PWM=80;
const int M01_ctl=4;

void setup() {
  // put your setup code here, to run once:
//  pinMode(LED,OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);

  pinMode(M01_ctl, OUTPUT);
  pinMode(M01, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(BUTTON);


if(val == HIGH){
  Serial.println("HIGH");
  delay(10);
} else {
  Serial.print("LOW");
  delay(10);
}

digitalWrite(M01_ctl,HIGH);
analogWrite(M01, M01_PWM);
}