/*
Software by TH0L3F
*/

//Utilitaire pour l'écran LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define len_lcd 20 //Nombre de caractère en longueur de l'écran
#define high_lcd 4 //Nombre de lignes de l'écran
#define lcd_adress 0x27 //Adresse de l'écran
LiquidCrystal_I2C lcd(lcd_adress,len_lcd,high_lcd);  //Configurer l'écran 
bool screen_refresh = false;

//Encodeur rotatif KY-040
//Parameters
#define CLK 2
#define DT 3
#define SW 4
int counter = 0;
bool pos_chng = 0;
unsigned long old_pos = 0; //Ancienne position du tapis en dizieme de um
unsigned long new_pos = 0; //Nouvelle position du tapis en dizieme de um
bool ax = false; //Ancien état de la broche A
bool bx = false; //Ancien état de la broche B
bool a = false; //Nouvel état de la broche A
bool b = false; //Nouvel état de la broche B
bool btn = 0;
bool abtn = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;

//Buzzer
#define pin_buzzer 10

//Driver stepper
#define DR_step 5 //Broche d'impulsion pour chaques pas
#define DR_dir 6 //Broche de sélection de la direction
const long step_per_revolution = 800; // Nombre de par par révolution du moteur pas à pas
const long init_r = 180000; //rayon initial de l'axe d'enroulement en en dizieme de um
const long red = 100; //Coefficient de réduction du moteur
long step_per_rev_red = step_per_revolution*red; //Nombre de pas du moteur pour faire un tour en sortie du réducteur
long e = 4000; //Epaisseur du tissus en en dizieme de um
long n_step_f = 0;//Enregistre le nombre de pas effectuer depuis le début du tour actuel
long n_tour = 2;//Enregistre le nombre de tour effectuer depuis le démarrage de la machine
unsigned long mm_step = 0;//Permet de sauvegarder le nombre de dizieme de um effectuer à chaques pas.
long pi =  31415; //Pi en dizieme de um
long delay_step = 100; //Temps d'attente entre deux pas.

//Choix de mode
bool mode = 0;  //Permet de sauvegarder le mode choisie au démarrage
                //0 mode incrémental
                //1 mode continue
int sel;//Sauvegarde de la position en cours
char c1;//Affichage du choix 
char c2;//Affichage du choix
char c3;//Affichage du choix

//Mode continue
#define v1 5 //5mm/h
#define v2 10 //10mm/h
int vitesse = 0; //Vitesse choisir au démarrage

void setup(){
  //Configurer l'éc&ran LCD
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK), readRotary, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), readRotary, CHANGE);

  pinMode(SW, INPUT_PULLUP);

  pinMode(pin_buzzer, OUTPUT);

  pinMode(DR_step, OUTPUT);
  pinMode(DR_dir, OUTPUT);
  mm_step = (((e*n_tour+init_r)/red)*2*pi)/step_per_revolution/1000;

  //Affichage au démarrage
  screen(mode);
  delay(2000);
  screen(1);

  delay(1);
  delay(1);
  delay(1);
  delay(1);
  delay(1);
  delay(1);
  delay(1);
  delay(1);

}

void loop(){
  if(screen_refresh){
    screen(mode); //Mise à jour de l'écran
    screen_refresh = false;
  }
  if(pos_chng){
    if((mode == 2) || (mode == 3)){
      new_pos = (counter/4)*100000;
    }
    else if(mode == 1){
      sel = (counter/4)%3;
    }
    pos_chng = 0;
    screen_refresh = 1;

  }
  button();//Permet de récupérer les infos sur le bouton et d'executer les actions associés.
}

void button(){
  btn = digitalRead(SW);
  if((mode == 3)||(mode == 2)){
    if(abtn == HIGH && btn == LOW){       // bouton appuyé
      t1 = millis();//t1 le temps horloge au début de l'appuie
    }
    else if(abtn == LOW && btn == HIGH) { // bouton relaché
      t2 = millis();//t2 le temps horloge à la fin de l'appuie
      long t3 = t2 - t1; //t3 le temps appuyès du bouton
      if(t3 > 1000){
        //Le bouton a été appuyé pendant plus d'une seconde.
        move();
        screen_refresh = 1;
      }
    }
  }
  else if(mode == 1){
    if(sel == 0){
      mode = 2;
    }
    else {
      mode = 3;
      if(sel == 1){
        vitesse = v1;
      }
      else{
        vitesse = v2;
      }
    }
    screen_refresh = 1;
  }
  // enregistre le dernier état
  abtn = btn;
}

void move(){
  //Permet de faire le déplacement entre la position voulue et la position actuelle
  //Ce programme fait une approximation qui est de considérer que l'enroulement ce fait en suivant la règle suivante:
  //Chaque tour de film augmente le diamètre de l'axe d'enroulement de 2 fois e pour l'enroulement suivant.

  if(mode == 2){
    while(old_pos < new_pos){//Tant que la position n'est pas atteinte on bouge

      //Calcul du déplacement en fonction du nombre de tours effectués
      if(n_step_f > step_per_rev_red-1){
        //Si on vient de finir un tour on recalcul le nombre de pas à effectuer pour le suivant
        n_tour++;
        mm_step = (((e*n_tour+init_r)/red)*2*pi)/step_per_revolution/1000;
        n_step_f = 0;
        delay_step = vitesse/mm_step;//Calcul du nouveau délai par pas pour obtenir une vitesse continue
        delay(100);
      }
      //On fait un pas
      digitalWrite(DR_step,HIGH);
      delayMicroseconds(delay_step);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
      digitalWrite(DR_step,LOW);
      delayMicroseconds(delay_step);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
      old_pos += mm_step;//On enregistre la nouvelle position
      n_step_f++;
    }
  }
  else if(mode == 3){
    while(1){
      //Calcul du déplacement en fonction du nombre de tours effectués
      if(n_step_f > step_per_rev_red-1){
        //Si on vient de finir un tour on recalcul le nombre de pas à effectuer pour le suivant
        n_tour++;
        mm_step = (((e*n_tour+init_r)/red)*2*pi)/step_per_revolution/1000;
        n_step_f = 0;
        delay(100);
      }
      //On fait un pas
      digitalWrite(DR_step,HIGH);
      delayMicroseconds(delay_step);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
      digitalWrite(DR_step,LOW);
      delayMicroseconds(delay_step);//Petite pause entre chaques pas pour limiter la vitesse de rotation et donc de déplacement du moteur.
      old_pos += mm_step;//On enregistre la nouvelle position
      n_step_f++;
    }
  }
}

void screen(int mode){
  delay(1);
  if(mode == 0){ //Ecran de démarrage
    lcd.setCursor(0,0);//On remet le curseur en haut à gauche
    lcd.clear();//On efface l'écran avant d'afficher de nouvelles informations
    affichage(0,"Subductor en cours",1);
    affichage(1,"de demarrage.",1);
    affichage(2,"Patientez...",1);
    affichage(3,"Fait par TH0L3F",1);
  }
  else if(mode == 1){
    lcd.setCursor(0,0);//On remet le curseur en haut à gauche
    lcd.clear();//On efface l'écran avant d'afficher de nouvelles informations
    affichage(0,"Choix du mode",2);
    if (sel == 0){ //Curseur sur choix 1
      c1 = "*";
      c2 = "";
      c3 = "";
    }
    else if(sel == 1){ //Curseur sur choix 2
      c1 = "";
      c2 = "*";
      c3 = "";
    } 
    else if(sel == 2){ //Curseur sur choix 3
      c1 = "";
      c2 = "";
      c3 = "*";
    }
    affichage(1,c1 + " Mode incrémental" ,2);
    affichage(2,c2 + " Mode continue " + String(v1) ,2);
    affichage(2,c3 + " Mode continue " + String(v2) ,2);
  }
  else if(mode == 2){ //Ecran mode incrémental
    lcd.setCursor(0,0);//On remet le curseur en haut à gauche
    lcd.clear();//On efface l'écran avant d'afficher de nouvelles informations
    affichage(0,"Position",2);
    affichage(1,"old : " + String(old_pos/(100000))+" mm",2);
    affichage(2,"new : " + String(new_pos/(100000))+" mm",2);
    affichage(3,String((float)mm_step/100.0)+" um/step",2);//100
  }
  else if(mode == 3){ //Ecran mode continue
    lcd.setCursor(0,0);//On remet le curseur en haut à gauche
    lcd.clear();//On efface l'écran avant d'afficher de nouvelles informations
    affichage(0,"Vitesse",2);
    affichage(1, String(vitesse)+"mm/h",2);
    affichage(3, String((float)mm_step/100.0)+" um/step",2);//100
  }
}

void affichage(int ligne, String text, int type){
  if(type == 1){ //Affichage du texte au centre de la ligne
    lcd.setCursor ( int(len_lcd/2)-int(text.length()/2), ligne ); 
    lcd.print(text); 
  }
  else if(type == 2){//Affichage aligner à gauche
    lcd.setCursor ( 0, ligne);
    lcd.print(text);
  }
  else if(type == 2){//Affichage aligner à droite
    lcd.setCursor ((len_lcd - text.length()), ligne);
    lcd.print(text);
  }
  return;
}

void readRotary(){
  a = digitalRead(CLK);
  b = digitalRead(DT);
  if(ax == a && bx == b){return ;}//Pas de mouvements
  else if(ax && bx && !a && b){counter++;}//Mouvement dans le sens 1
  else if(!ax && bx && !a && !b){counter++;}//Mouvement dans le sens 1
  else if(!ax && !bx && a && !b){counter++;}//Mouvement dans le sens 1
  else if(ax && !bx && a && b){counter++;}//Mouvement dans le sens 1
  else if(ax && bx && a && !b){counter--;}//Mouvement dans le sens 2
  else if(ax && !bx && !a && !b){counter--;}//Mouvement dans le sens 2
  else if(!ax && !bx && !a && b){counter--;}//Mouvement dans le sens 2
  else if(!ax && bx && a && b){counter--;}//Mouvement dans le sens 2
  else{}//Il y a eu un saut d'un état
  if(counter < 0){
    counter = 0;
  }
  ax = a;//Sauvegarde des nouveaux paramètres
  bx = b;//Sauvegarde des nouveaux paramètres
  pos_chng = 1;
}