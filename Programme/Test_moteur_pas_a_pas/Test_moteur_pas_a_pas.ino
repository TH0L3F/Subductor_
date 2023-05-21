void setup() {
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int y = 0; y < 100; y++){
    for(int i = 0; i < 800; i++){
      digitalWrite(5,HIGH);
      delay(1);
      digitalWrite(5,LOW);
      delay(1);
    }
  }
  while(1);
}
//IIIII