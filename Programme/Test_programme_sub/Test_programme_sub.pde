const float step_per_revolution = 800.0; // Nombre de par par révolution du moteur pas à pas
const float init_D = 35.0; //Diametre initial de l'axe d'enroulement
const float red = 100.0; //Coefficient de réduction du moteur
const float e = 0.3; //Epaisseur du tissus
int n_step_f = 0;//Enregistre le nombre de pas effectuer depuis le début du tour actuel
int n_tour = 2;//Enregistre le nombre de tour effectuer depuis le démarrage de la machine
float mm_step = 0;//Permet de sauvegarder le nombre de mm effectuer à chaques pas.
float pi =  3.14159265359;

int counter = 0;
bool pos_chng = 0;
float old_pos = 0;
float act_pos = 0;
float new_pos = 0;

while(old_pos < new_pos){//Tant que la position n'est pas atteinte on bouge
  //Calcul du déplacement en fonction du nombre de tours effectués
  if(n_step_f > (int)(step_per_revolution*red)){
    //Si on vient de finir un tour on recalcul le nombre de pas à effectuer pour le suivant
    n_tour++;
    mm_step = (2*pi*(e*n_tour+init_D/2))/(step_per_revolution*red);
    n_step_f = 0;
  }
  n_step_f++;
}
