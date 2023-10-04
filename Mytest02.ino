//PWM ctl Pin
const int M01=5;
//PWM setting
int PWM=100;
//change lol
const int M01_ctl=4;

const int LED = 13;
//start stop
const int Switch = 8;
//Hantei
const int Judge = 7;
//Switch status 
int sig = 0;
//Judge status
int sig1 = 0;
// run stop status
int status = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(M01_ctl, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Switch, INPUT);

}

void run() {
    Serial.println("run");
    digitalWrite(M01_ctl, HIGH);
    analogWrite(M01, PWM); 
}

void stop() {
    Serial.println("stop");
    digitalWrite(M01_ctl, HIGH);
    analogWrite(M01, 0); 
}

void loop() {
  // put your main code here, to run repeatedly:
    sig = digitalRead(Switch);
    sig1 = digitalRead(Judge) ;
    if(sig == HIGH){
        if(status = 0){
            //start status
            status = 1;
        } else {
            //stop stautus
            status =0;
        }
        delay(50);
    }  
  
    if(status = 1){
        run();
    } else {
        stop();
        if(sig1 == HIGH){
            digitalWrite(LED,HIGH);
            delay(500);
            digitalWrite(LED,LOW);
            delay(500);
        } else {
            Serial.println("run_check");
            digitalWrite(M01_ctl, HIGH);
            analogWrite(M01, 50);
        }
    }
    
    
    if(sig1 == HIGH) {
        digitalWrite(LED, HIGH);
    } else {
        digitalWrite(LED,HIGH);
    }
  
  
}

