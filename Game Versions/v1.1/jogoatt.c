#include <conio2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{

        int posX;
        int posY;
        int points;

    }Player;

void paintBorder(char character);
void movePlayer();
void update(Player player);
void getKey();
void desenhaPlayer(int posX, int posY);
void limpaRastro(int posX, int posY);


int main(){


    textcolor(15);

    Player player = {4, 4, 0};

    system("mode 80,40");
    paintBorder('/');
    int matrizJogo[80][20];

    update(player);


}

void update(Player player){



    int keyUp, keyDown, keyLeft, keyRight;


    getKey(&keyUp, &keyDown, &keyLeft, &keyRight);



    movePlayer(&player.posX, &player.posY, keyUp, keyDown, keyLeft, keyRight);



    desenhaPlayer(player.posX, player.posY);
    putchxy(10,10, 'P');

    if(player.posX == 10 && player.posY == 10){
        player.points++;
    }

    gotoxy(20, 30);
    printf("Score: %d", player.points);



    Sleep(60);

    update(player);

}


void paintBorder(char character){ //imprime as bordas

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

void movePlayer(int *posX, int *posY, int keyUp, int keyDown, int keyLeft, int keyRight){ //move o player se dentro dos limites das bordas

    limpaRastro(*posX, *posY);

    if(keyUp && *posY > 3){
        *posY = *posY -2;
    }
    if(keyDown && *posY < 18){
        *posY = *posY +2;
    }
    if(keyLeft && *posX > 3){
        *posX = *posX -2;
    }
    if(keyRight && *posX < 78){
        *posX = *posX +2;
    }






}

void getKey(int *up, int *down, int *left, int *right){ //reconhece botão de movimento

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

void desenhaPlayer(int posX, int posY){

    textcolor(2);
    putchxy(posX, posY, 'X');
    putchxy(posX+1, posY, 'X');
    putchxy(posX, posY+1, 'X');
    putchxy(posX+1, posY+1, 'X');
    textcolor(15);

}

void limpaRastro(int posX, int posY){

    putchxy(posX, posY, ' ');
    putchxy(posX+1, posY, ' ');
    putchxy(posX, posY+1, ' ');
    putchxy(posX+1, posY+1, ' ');

}



