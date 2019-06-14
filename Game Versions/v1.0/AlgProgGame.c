#include <conio2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


void paintBorder(char character);
void movePlayer();
void update(int posX, int posY);
void getKey();


int main(){

    system("mode 80,20");
    paintBorder('/');
    int matrizJogo[80][20];
    int posX = 5, posY = 5; // posicoes player

    update(posX, posY);

}

void update(int posX, int posY){





    int keyUp, keyDown, keyLeft, keyRight;


    getKey(&keyUp, &keyDown, &keyLeft, &keyRight);



    movePlayer(&posX, &posY, keyUp, keyDown, keyLeft, keyRight);



    putchxy(posX, posY, 'X');





    update(posX, posY);

}


void paintBorder(char character){

    for(int i = 1; i <= 80;i++){
        putchxy(i,1,character);
    }
    for(int i = 1; i <= 80;i++){
        putchxy(i,20,character);
    }
    for(int i = 1; i <= 20;i++){
        putchxy(1,i,character);
    }
    for(int i = 1; i <= 20;i++){
        putchxy(80,i,character);
    }

}

void movePlayer(int *posX, int *posY, int keyUp, int keyDown, int keyLeft, int keyRight){

    if(keyUp){
        *posY = *posY -1;
    }
    if(keyDown){
        *posY = *posY +1;
    }
    if(keyLeft){
        *posX = *posX -1;
    }
    if(keyRight){
        *posX = *posX +1;
    }



}

void getKey(int *up, int *down, int *left, int *right){

    *up = 0;
    *down = 0;
    *left = 0;
    *right = 0;

    char key = getch();


        if(key == 'a' || key == 'A'){
            *left = 1;
        }
        if(key == 'w' || key == 'W'){
            *up = 1;
        }
        if(key == 'd' || key == 'D'){
            *right = 1;
        }
        if(key == 's' || key == 'S'){
            *down = 1;
        }

}

