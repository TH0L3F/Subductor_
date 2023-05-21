#define DR_step 5 //Broche d'impulsion pour chaques pas
#define DR_dir 6 //Broche de sélection de la direction

void setup() {
  // put your setup code here, to run once:
  pinMode(DR_step, OUTPUT);
  pinMode(DR_dir, OUTPUT);
  digitalWrite(DR_dir,LOW);
  delay(1);
  delay(1);
  delay(1);
  delay(1);
  delay(1);
}

void loop() {
    for(long i = 0; i < 80000; i++){
      digitalWrite(DR_step,HIGH);
      delayMicroseconds(100);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
      digitalWrite(DR_step,LOW);
      delayMicroseconds(100);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
    }
    while(1);
}
