#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mastermind.h"
#include "Arduino.h"
#include "lcd_wrapper.h"



char* generate_code(bool repeat, int length){
	//Skontrolujeme ak� k�d m�me vytvori�, �i to je v�bec mo�ne, ak sa nesm� ��sla opakova� a m� by� v��� ako 10 znakov tak je to nemo�n�.
	if( ( (repeat==false) && (length>10) ) || (length<1) ){
		return NULL;
	}
	//premenn� i a j pou��vam iba na pohyb v cykle, premenn� number na ukladanie vygenerovan�ho ��sla.
	int number[length];
	char* kod=(char*)calloc((length+1),sizeof(char));					//pointer na premenn� kde bude v�sledn� k�d ulo�en�
	//vygenerujeme si ��slo o d��ke length, pou�ijeme na generovanie pomocou arduino analogov�ho �umu na pine A1, zad�me si rozmedze 0 a� 10, kde minimaln� ��slo je v rozmedz� no maxim�lne nie
	for(int i=0;i<length;i++){
		number[i]=random(0,10);
		//vytvor�me cyklus na kontrolu opakovania ��siel
		for(int j=0;j<i;j++){
			if( (number[i]==number[j]) && (repeat==false) ){
				if(number[i]>1){
					number[i]--;
				}else{
                    number[i]=9;
				}
			}
		}
	}
	//ulo��me premenn� do pointra
	for(int i=0;i<length;i++){
		kod[i]=number[i]+48;
	}
	//na posledn� miesto pointra uklad�me ukon�ovac� znak, urob�me to tak v�ade kde sa premenn� zobrazuje, lebo zvykne sa zaplni� nahodn�mi znakmi a arduino je ve�mi citliv� na pr�cu s pam�ou
 	kod[length]='\0';
	//vr�time pointer
	return kod;
}

void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b){
	//vytvor�me premenn� i a j ktor� sl��ia na pohyb v cykloch a premenn� a a b, ktor� pou�ijeme na lok�ln� �pravy, ktor� ulo��me do peg_a a peg_b
	int i=0,j=0,a=0,b=0;
	//vytvor�me si premenn� length, ktor� pou�ijeme v premennej foundNumber, do premennej foundNumber si zazna��me �i ��slo na danej poz�ci� u� bolo n�jdene
	int length=strlen(secret);
  	int foundNumber[length];

	//cyklus na peg_a...rozsvecovanie �erven�ch svetiel...sta�� zis�ova� �i je ��slo zhodn� na rovnakej poz�ci�
	for(i=0;i<length;i++){
        foundNumber[i]=0;
		if(secret[i]==guess[i]){
			a++;
            foundNumber[i]=1;
		}
	}
	/*cyklus na peg_b...rozsvecovanie modr�ch di�d
	skontrolujeme si ci nahodou nie je cislo zaznacene ako foundNumber, ak je tak ho preskocime ak nie tak pokracujeme,
	budeme porovnavat kazde cislo s kazdym, preto cyklus v cykle, v druhom cykle si znova skontrolujeme ci nie je cislo pouzite, aby sme rovnake cislo nepouzili viackrat,
	vlozime si podmienku ze ak by b bolo viac ako je 4 - a tak sa ma zastavit. je to preto ze mame 4 diody cervene a 4 modre, no vzdy mozme pouzit len jednu farbu, nie obe naraz,
	takze vzdy mame 4 diody, v ktorych ma cervena farba mat prevahu a je dolezitejsia.
	*/
	i=0;
	for(i=0;i<length;i++){
        while( (foundNumber[i]==1) && (i<length) ){
            i++;
        }
		for(j=0;j<length;j++){
            if(foundNumber[i] != 1){
			
				if(guess[j]==secret[i]){
					b++;
					foundNumber[i]=1;
				}
			}
		}
	}
	*peg_a=a;
	*peg_b=b;
}

void turn_off_leds(){
	//Vypnneme si ledky, vsetky od ich pociatocnej pozicie, pin 6 az po pin 13.
	for(int i=6;i<=13;i++){
		digitalWrite(i,LOW);
	}
}

void render_leds(const int peg_a, const int peg_b){
	//pou�ijeme premenn� i na zazna�enie prvej poz�cie ledky a premenn� ratac ako po�itadlo ko�ko kr�t nejak� ledku rozsvieti�.
	int i=6,ratac=0;
	if(peg_a>0){
	//pin ledky zvy�ujem pred rozsvietenim a po rozsvieteni, zatia� �o pri modr�ch len po rozsvieten�, je to preto �e �erven� ledky s� na neparnej poz�ci�
        while(ratac<peg_a){
            i++;
            digitalWrite(i,HIGH);
            ratac++;
            i++;
        }
	}
    ratac=0;
    if(peg_b>0){
        while(ratac<peg_b){
            digitalWrite(i,HIGH);
            ratac++;
            i=i+2;
        }
    }
}

void play_game(char* secret){
    lcd_clear();
    char **history = calloc(10, sizeof(char *));										//Vytvor�me si smern�k na premenn�
	char number[10][4];															//dvojrozmern� pole n�m pom��e uklada� veci do hist�rie
	char digit[4];																	//premenn� digit pou�ijeme len na zvy�ovanie hodnoty ��sla na niektorej poz�ci�, po stla�en� enteru si digit ulo��me do number
    int m=4;																		//premenn� m sl��i len na d��ku re�azca, je mo�ne to zameni� za strlen a pracova� potom so v�etk�mi mo�n�mi vstupmi
    char* guess=(char*)calloc((m+1),sizeof(char));								//pointer guess budeme odosiela� na vyhodnotenie
	int counter=0,i=0, j = 0,peg_a=0, peg_b=0,history_rdr=0;					//premenn� counter, pre po��tanie �ahu, i pre pohyb v cykloch, peg_a a peg_b pre rozsvietenie di�d a history_rdr pre poz�ciu v hist�ri�
	bool gameWon=false, change=false;											//boolovsk� premenn� pre vyhrat� hru a pre zmenu displeja
	
	for(i = 0; i < 10; i++){
    for(j= 0; j < 4; j++){
      guess[j] = '0';
    }
    guess[4] = '\0';
    history[i] = guess;
	}
	lcd_print_at(0,0,"uhadni cislo");
	while(counter<10){                                  //counter po��ta pokusy, m�me 10 pokusov na uhadnutie ��sla
       gameWon = false;
        turn_off_leds();
        number[counter][m]='\0';
        for(i=0;i<4;i++){															//najprv si do digit ulo��me v�ade nulu, a n�sledne aj zmen�me hodnotu v number, aby n�hodou ak sme klikli na hist�riu nevyhodilo n�m n�hodn� znaky
            digit[i]='0';
            lcd_set_cursor(1,i);
            number[counter][i]=digit[i];
            lcd_print(&number[counter][i]);
        }
        while(analogRead(A0)<900){													//��tam �i nebolo stla�en� tla�idlo enter, ak nie tak sme zacyklen� vn�tri
            lcd_print_at(0,0,"uhadni cislo");
	/*v�ade reagujeme na stla�enie tla�idla, a �ak�me �o u�ivate� stla�i.
	tla�idla sa za�inaj� od pinu 2...jediny pin 2 ak stla�ime tak pocka 150ms a skontroluje �i ho dr�ime alebo sme pustili, ak ho dr�ime otvori sa historia, ak sme pustili navy�i sa
	prve cislo
	*/
            if(digitalRead(2)==HIGH){
                delay(150);
                if(digitalRead(2)==LOW){
                    if((digit[0]-48)<9)
                        digit[0]++;
                    else{
                        digit[0]='0';
                    }
                    change=true;
                }
                while(digitalRead(2)==HIGH){
                    if(digitalRead(3)==HIGH){
                        if(history_rdr > 0)
                          history_rdr--;
                         else
                          history_rdr = 9;
                        turn_off_leds();
                    }
                    if(digitalRead(4)==HIGH){
                        if(history_rdr < 9)
                          history_rdr++;
                        else
                          history_rdr = 0;
                        turn_off_leds();
                    }
                    render_history(secret,history,history_rdr);
                    digit[0]='0';
                }
                for(i=0;i<4;i++){
                    number[counter][i]=digit[i];
                    lcd_set_cursor(3,i);
                    lcd_print(&number[counter][i]);
                }
            }
            if(digitalRead(3)==HIGH){
                while(digitalRead(3)==HIGH);
                if((digit[1]-48)<9)
                    digit[1]++;
                else{
                    digit[1]='0';
                }
                change=true;
            }
            if(digitalRead(4)==HIGH){
              while(digitalRead(4)==HIGH);
                if((digit[2]-48)<9)
                    digit[2]++;
                else{
                    digit[2]='0';
                }
                change=true;
            }
            if(digitalRead(5)==HIGH){
              while(digitalRead(5)==HIGH);
                if((digit[3]-48)<9)
                    digit[3]++;
                else{
                    digit[3]='0';
                }
                change=true;
            }
            delay(100);
   //ak sme kdeko�vek zmenili ��slo, nastavili sme bool premenn� change na true a tak zmenime zobrazenie displeja a zmenime change na false aby sme znova mohli meni� zobrazovanei na displeji
            if(change==true){
                for(i=0;i<4;i++){
                    number[counter][i]=digit[i];
                    lcd_set_cursor(1,i);
                    lcd_print(&number[counter][i]);
                }
                change=false;
            }
        }
        char* text=calloc(5,sizeof(char));												//pointer text pou�ijeme iba aby sme ukladali hodnoty do smernika
        text[4]='\0';
        for(i=0;i<4;i++){
            guess[i]=digit[i];															//po stla�en� enteru si ulo��me zvolen� ��sla do pointera a po�leme na vyhodnotenie
            text[i]=number[counter][i];
        }
        guess[m]='\0';
        get_score(secret,guess,&peg_a,&peg_b);
        render_leds(peg_a,peg_b);
        history[counter]=text;
        history[10]='\0';
        Serial.println(peg_a);
        if(peg_a == 4){																	//ak s� v�etky 4 ledky �erven� znamen� to �e me vyhrali hru
            gameWon=true;		
        }
        delay(1000);
        if(gameWon==true){
        lcd_clear();
        lcd_print_at(0,0,"vyhral si");
        delay(1000);
        while(analogRead(A0)<900);
        counter = 10;
    }
    turn_off_leds();
        counter++;
	}
	//vyhodnotime ci sme vyhrali hru alebo prehrali
	if(gameWon != true){
		lcd_clear();
        lcd_print_at(0,0,"prehral si");
		while(analogRead(A0)<900);
	}
}


void render_history(char* secret, char** history, const int entry_nr){
	int i=0,peg_a=0,peg_b=0;
    char digit[4];
	lcd_clear();
    lcd_print_at(0,0,"Historia");
    digit[4]='\0';
    history[entry_nr][4]='\0';
    for(i=0;i<4;i++){
      digit[i]=history[entry_nr][i];
      lcd_set_cursor(1,i);
      lcd_print(&digit[i]);
    }
    get_score(secret,history[entry_nr],&peg_a,&peg_b);
    render_leds(peg_a,peg_b);
    delay(200);
    if(digitalRead(2)==LOW){
        turn_off_leds();
        lcd_clear();
    }
}
