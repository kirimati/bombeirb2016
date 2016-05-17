#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define length 30
#define height 15


#define EMPTY 0//
#define TREE 1// l'arbre est un petit bosquet vert designé par le "1"
#define STONE 17//
#define PLAYER 2
#define PRINCESS 3
#define EMPTYC 4//
#define BOMBDPC 20//
#define BOMBIPC 36//
#define BOMBDNC 52//
#define BOMBINC 68//
#define MONSTERC 84 //
#define LIFESUPC 100
#define BONUSBDN 21//
#define BONUSBIN 37//
#define BONUSBDP 51//
#define BONUSBIP 69//
#define BONUSLIFESUP 101
#define MONSTER 6//
#define BOMB 7
#define KEY 8
#define DOOR 145
#define DOOR_1 145 //001 Door to level 1.
#define DOOR_2 146	 //010 Door to level 2.
#define DOOR_3 147 //011 Door to level 3.
#define DOOR_4 148	 //100 Door to level 4.
#define DOOR_5 149	 //101 Door to level 5.
#define DOOR_6 150	 //110 Door to level 6.
#define DOOR_7 151	 //111 Door to level 7.


int main(){
	for (int j=0;j<8;j++){
	    
			FILE * f = NULL; 
           char myfile[16];
           sprintf(myfile, "map_%d", j);
           f = fopen(myfile, "w");
    

	int size=length*height;
	srand(time(NULL));
	
	fprintf(f,"%d:%d \n",length,height);
	//fprintf(f,"%d\n",PLAYER);
	
	int place_player = rand()%450;
	int place_door = rand()%450;
	int place_key = rand()%450;
		
	while (place_player == place_key || place_player == place_door || place_key == place_door ) {
		place_player = rand()%450;
		place_door = rand()%450;
		place_key = rand()%450;
	}
	
	for(int i=1;i<=size -3;i++){
		if (i== place_player) {
			fprintf(f,"%d ",PLAYER);
		}
		if (i== place_key) {
			fprintf(f,"%d ",KEY);
		}
		if (i== place_door) {
			if (j < 6){
			int door = DOOR + j +1;
			fprintf(f,"%d ",door);
		}else {
			fprintf(f,"%d ",PRINCESS);
		}
		}
		if(rand()%2 == 0){ //NOEUD 1
			fprintf(f,"%d ",EMPTY);
		}else {
			if (rand()%2 == 0){ //NOEUD 2
				fprintf(f,"%d ",TREE);
			}else {
				if (rand()%2 == 0){ //NOEUD 3
					if (rand()%2 == 0){
						fprintf(f,"%d ",STONE);
					}else {
						fprintf(f,"%d ",EMPTYC);
					}
				}else {
					if (rand()%2 == 0){ //NOEUD 4
						if (rand()%2 == 0){ //NOEUD 5
							if (rand()%2 == 0){//NOEUD 6
								fprintf(f,"%d ",BONUSBDN);
							}else {
								fprintf(f,"%d ",BONUSBDP);
							}
						}else {
							if (rand()%2 == 0){
								fprintf(f,"%d ",BONUSBIN);
							}else{
								fprintf(f,"%d ",BONUSBIP);
							}
						}
					}else {
						if(rand()%2 == 0){
							if(rand()%2 == 0){
								if (rand()%2 == 0){
									fprintf(f,"%d ",BOMBDPC);
								}else {
									fprintf(f,"%d ",BOMBDNC);
								}
							}else {
								if (rand()%2 == 0){
									fprintf(f,"%d ",BOMBINC);
								}else{
									fprintf(f,"%d ",BOMBIPC);
								}
							}
						}else {
							if (rand()%2 == 0){
								if (rand()%2 == 0){
									fprintf(f,"%d ",MONSTERC);
								}else {
									fprintf(f,"%d ",MONSTER);
								}
							}else {
								if (rand()%2 == 0){
									fprintf(f,"%d ",LIFESUPC);
								}else{
									fprintf(f,"%d ",BONUSLIFESUP);
								}
							}
						}
					}
				}
			}
		}
	if(i%length==0){
		fprintf(f,"\n");
	}
	}
	fclose(f);
}
	return 0;
}


