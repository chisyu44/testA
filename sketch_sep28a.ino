const int M01=5;
int M01_PWM=100;
const int M01_ctl=4;
int a = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(M01_ctl, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  
    Serial.println("run");
    digitalWrite(M01_ctl, HIGH);
    analogWrite(M01, M01_PWM); 
    digitalWrite(LED_BUILTIN, HIGH);

  
  
}
