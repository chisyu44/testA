//PWM ctl Pin
const int M01=5;
//PWM setting
int PWM=100;
//change lol
const int M01_ctl=4;

const int LED = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(M01_ctl, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void run() {
    Serial.println("run");
    digitalWrite(M01_ctl, HIGH);
    analogWrite(M01, PWM); 
    }

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
    run();
    digitalWrite(LED_BUILTIN, HIGH);

  
  
}

