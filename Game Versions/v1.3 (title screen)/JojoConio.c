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
void menu();
void levelStart(int level, int points);
void paintBorder(char character, int posX, int posY);
void movePlayer();
void update(Player player, char matrizJogo[40][10]);
void getKey();
void desenhaPlayer(int posX, int posY);
void limpaRastro(int posX, int posY);
void retornaNivel(char matriz[40][10], char[]);
void menuLoop(int selection);



// Main
int main(){

    menu();

    return 0;
}


// Inicialização de level
void levelStart(int level, int points){

    system("mode 80,40");

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

        if(key != 'a' && key != 'w' && key != 's' && key != 'd'){
            key = getch();
        }


            if(key == 'a' || key == 'K'){
                *left = 1;
            }
            if(key == 'w' || key == 'H'){
                *up = 1;
            }
            if(key == 'd' || key == 'M'){
                *right = 1;
            }
            if(key == 's' || key == 'P'){
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
        printf("Erro");
    }
}

// Exibe o menu

void menu(){

// Carrega o title screen do jogo
    system("mode 130,30");
    FILE *arquivo;
    char letra;
    textcolor(15);
    gotoxy(1, 1);
    int selection = 0;

    if(arquivo = fopen("logo.txt", "r")){

        do{

            fread(&letra, sizeof(char), 1, arquivo);
            if(!feof(arquivo)){
                printf("%c", letra);
            }

        }while(!feof(arquivo));
        fclose(arquivo);
    }

    gotoxy(60, 20);

    printf("New Game\n");

    gotoxy(61, 21);

    printf("Scores\n");

    gotoxy(62, 22);

    printf("Exit");

    menuLoop(selection);

}

void menuLoop(int selection){

    switch(selection){

    case 0:
        putchxy(58, 20, '>');
        putchxy(59, 21, ' ');
        putchxy(60, 22, ' ');
        break;
    case 1:
        putchxy(59, 21, '>');
        putchxy(58, 20, ' ');
        putchxy(60, 22, ' ');
        break;
    case 2:
        putchxy(60, 22, '>');
        putchxy(58, 20, ' ');
        putchxy(59, 21, ' ');
        break;


    }


    if(kbhit()){


       char key = getch();

        if(key != 'w' && key != 's' && key != 13){
            key = getch();
        }


        if((key == 'w' || key == 'H') && selection > 0){
            Sleep(60);
            menuLoop(selection-1);
        }

        else if((key == 's' || key == 'P') && selection < 3){
            Sleep(60);
            menuLoop(selection+1);
        }
        else if(key == 13 && selection == 0){
            levelStart(1, 0);
        }
        else{
            Sleep(60);
            menuLoop(selection);
        }


    }
    else{
        Sleep(60);
        menuLoop(selection);
    }

}



