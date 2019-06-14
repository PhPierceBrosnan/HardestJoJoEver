#include <conio2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


// Esturutra do Jogador
typedef struct{

        int posX;
        int posY;
        int points;

    }Player;

// Estrutura dos Inimigos
typedef struct{

    int posX;
    int posY;
    char direction;
}Enemy;

// Headers das funções
void levelStart(int level, int points);
void paintBorder(char character, int posX, int posY);
void movePlayer();
void update(Player player, char matrizJogo[40][10]);
void getKey();
void desenhaPlayer(int posX, int posY);
void limpaRastro(int posX, int posY);
void retornaNivel(char matriz[40][10], char[]);



// Main
int main(){

    system("mode 90,40");
    levelStart(1, 0);

    return 0;
}


// Inicialização de level
void levelStart(int level, int points){

    // Formata o nome do arquivo do nível
    char stringLevel[20] = "nivel";
    char levelChar[4];

    sprintf(levelChar, "%d", level);
    strcat(stringLevel, levelChar);
    strcat(stringLevel, ".txt");

    char matrizJogo[40][10];

    retornaNivel(matrizJogo, stringLevel);

    // Instanciação do jogador
    Player player;

    // Instanciação do array de obstáculos

    Enemy inimigos[20];
    int contadorInimigos = 0;

    // Inicialização do ambiente de jogabilidade

    for(int i = 0; i < 40;i++){
        for(int j = 0; j < 10;j++){
            if(matrizJogo[i][j] == 'X'){
                paintBorder('/', (i*2)+1, (j*2)+1);
            }
            if(matrizJogo[i][j] == 'O'){
                player.posX = (i*2)+1;
                player.posY = (j*2)+1;
            }
            if(matrizJogo[i][j] == 'C'){

            }
        }
    }



    update(player, matrizJogo);


}

// Loop do jogo
void update(Player player, char matrizJogo[40][10]){


    int keyUp, keyDown, keyLeft, keyRight;

    if(kbhit()){
        getKey(&keyUp, &keyDown, &keyLeft, &keyRight);
        movePlayer(&player.posX, &player.posY, keyUp, keyDown, keyLeft, keyRight, matrizJogo);
    }

    desenhaPlayer(player.posX, player.posY);





    Sleep(60);

    update(player, matrizJogo);

}

// Imprime os limites do nível
void paintBorder(char character, int posX, int posY){

    putchxy(posX, posY, character);
    putchxy(posX+1, posY, character);
    putchxy(posX, posY+1, character);
    putchxy(posX+1, posY+1, character);


}

// Move o jogador
void movePlayer(int *posX, int *posY, int keyUp, int keyDown, int keyLeft, int keyRight, char matrizJogo[40][10]){

    limpaRastro(*posX, *posY);

    if(keyUp && matrizJogo[(*posX-1)/2][((*posY-1)/2)-1] != 'X'){
        *posY = *posY -2;
    }
    if(keyDown && matrizJogo[(*posX-1)/2][((*posY-1)/2)+1] != 'X'){
        *posY = *posY +2;
    }
    if(keyLeft && matrizJogo[((*posX-1)/2)-1][(*posY-1)/2] != 'X'){
        *posX = *posX -2;
    }
    if(keyRight && matrizJogo[((*posX-1)/2)+1][(*posY-1)/2] != 'X'){
        *posX = *posX +2;
    }


}

// Checa a tecla pressionada
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

// Desenha o jogador na tela
void desenhaPlayer(int posX, int posY){

    textcolor(2);
    putchxy(posX, posY, 'X');
    putchxy(posX+1, posY, 'X');
    putchxy(posX, posY+1, 'X');
    putchxy(posX+1, posY+1, 'X');
    textcolor(15);

}

// Limpa a última posiçao do jogador
void limpaRastro(int posX, int posY){

    putchxy(posX, posY, ' ');
    putchxy(posX+1, posY, ' ');
    putchxy(posX, posY+1, ' ');
    putchxy(posX+1, posY+1, ' ');

}

//  Retorna a matriz do arquivo do nível do argumento
void retornaNivel(char matriz[40][10], char nomeArquivo[]){

    FILE *arquivo;

    if(arquivo = fopen(nomeArquivo, "r")){

        // lendo matriz do nivel
        int i = 0;
        int j = 0;
        while(j < 10){
            fread(&matriz[i][j], sizeof(char), 1, arquivo);

            // Checa se o caractere é Enter
            if(matriz[i][j] == '\n'){
                fread(&matriz[i][j], sizeof(char), 1, arquivo);
            }
            i++;
            if(i == 40){
                i = 0;
                j++;
            }
        }


        fclose(arquivo);
    }
    else{
        printf("ERro");
    }
}



