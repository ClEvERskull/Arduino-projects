int semafor[8][3];
/*Vytvoríme si dvojrozmerné pole pre premenné, prvá premenná nám bude predstavovať semafor, zatiaľ čo druhá nám predstavuje LED diódu.*/
int n = 22;     //Vytvoríme si premennú do ktorej vložíme číslo prvého použitého pinu.
/* Pomocou cyklov for si do dvojrozmerného poľa uložíme čísla použivaných pinov.*/
for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            semafor[i][j] = n++;
        }
    }
    for(int i = 4; i < 8; i++){
        for(int j = 0; j < 2; j++){
            semafor[i][j] = n++;
        }
    }

void start(
    /* Pri spustení si nastavíme používané piny ako výstupné, urobíme tak už pomocou dvojrozmerného poľa ktoré sme vytvorili.*/
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            pinMode(semafor[i][j], OUTPUT);
        }
    }
    for(int i = 4; i < 8; i++){
        for(int j = 0; j < 2; j++){
            pinMode(semafor[i][j], OUTPUT);
        }
    }

    for(int i = 0; i < 8; i++)      //Pomocou tohto cyklu si nastavíme všetky semafory na červenú.
        digitalWrite(semafor[i][0], HIGH);  
}

void update(){
    /*Pomocou cyklu sa budeme prepínať medzi semaformi, všetke 4 semafory sú uložené po číslo 4, ak sa cyklus dostane na číslo 4, začne pracovať so semformi na prechody, všetké štyri semafory ukážu zelenú.*/
    for(int i = 0; i < 5; i++){
        if(i == 4){
            for(int i = 4; i < 8; i++){
                digitalWrite(semafor[i][0], LOW);
                digitalWrite(semafor[i][1], HIGH);
            }
            delay(5000);
        }
        else{
            digitalWrite(semafor[i][1], HIGH);
            digitalWrite(semafor[i][0], LOW);
            delay(750)
            digitalWrite(semafor[i][2], HIGH);
            digitalWrite(semafor[i][1], LOW);
            delay(4250);
            digitalWrite(semafor[i][2], LOW);
            digitalWrite(semafor[i][0], HIGH);
        }
    }
}