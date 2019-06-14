#include <conio2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


// Esturutra do Jogador
typedef struct{

        int posX;
        int posY;
        int moedas;
        int points;

}Player;

// Estrutura dos Inimigos
typedef struct{

    int posX;
    int posY;
    char direction;
}Enemy;

// Game manager
typedef struct{

    char matrizJogo[40][10];
    Player player;
    int moedasTotais;
    int level;
    Enemy enemies[20];
    int contaInimigos;

}GameManager;

// Headers das funções
void menu();
void levelStart(int level, int points);
void paintPosicao(char character, int cor, int posX, int posY);
void movePlayer();
void update(GameManager manager);
void getKey();
void desenhaPlayer(int posX, int posY);
void limpaRastro(int posX, int posY, char matrizJogo[40][10]);
void retornaNivel(char matriz[40][10], char[]);
void menuLoop(int selection);
int zonaFinal(int posX, int posY, char matrizJogo[40][10]);
int conversaoGraficoMatriz(int pos);
int coletouTodas(int moedas, int total);
void checaContato(Player *player, char matrizJogo[40][10]);
void printaInfo(Player player, int level);
void checaFimLevel(GameManager manager);


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

    GameManager manager;
    manager.moedasTotais = 0;


    retornaNivel(manager.matrizJogo, stringLevel);

    // Instanciação do jogador
    manager.player.moedas = 0;
    if(level == 1){
        manager.player.points = 0;
    }
    else{
        manager.player.points = points;
    }
    manager.level = level;

    // Instanciação do array de obstáculos

    manager.contaInimigos = 0;

    // Inicialização do ambiente de jogabilidade

    for(int i = 0; i < 40;i++){
        for(int j = 0; j < 10;j++){

            // Converte posicao da matriz para posicao do grafico
            int posicaoAtualGraficoX = (i*2)+1;
            int posicaoAtualGraficoY = (j*2)+1;

            if(manager.matrizJogo[i][j] == 'X'){
                paintPosicao('/', 15, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'O'){
                manager.player.posX = posicaoAtualGraficoX;
                manager.player.posY = posicaoAtualGraficoY;
            }
            if(manager.matrizJogo[i][j] == 'C'){
                paintPosicao('C', 9, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'M'){
                manager.moedasTotais = manager.moedasTotais + 1;
                paintPosicao('M', 14, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
        }
    }



    update(manager);


}

// Loop do jogo
void update(GameManager manager){


    int keyUp, keyDown, keyLeft, keyRight;

    // Quando tecla é pressionada
    if(kbhit()){
        getKey(&keyUp, &keyDown, &keyLeft, &keyRight);
        movePlayer(&manager.player.posX, &manager.player.posY, keyUp, keyDown, keyLeft, keyRight, manager.matrizJogo);
    }

    desenhaPlayer(manager.player.posX, manager.player.posY);


    checaFimLevel(manager);

    checaContato(&manager.player, manager.matrizJogo);
    printaInfo(manager.player, manager.level);

    Sleep(60);

    update(manager);

}

// Imprime os limites do nível
void paintPosicao(char character, int cor, int posX, int posY){

    textcolor(cor);
    putchxy(posX, posY, character);
    putchxy(posX+1, posY, character);
    putchxy(posX, posY+1, character);
    putchxy(posX+1, posY+1, character);


}

// Move o jogador
void movePlayer(int *posX, int *posY, int keyUp, int keyDown, int keyLeft, int keyRight, char matrizJogo[40][10]){

    limpaRastro(*posX, *posY, matrizJogo);
    int posicaoAtualX = conversaoGraficoMatriz(*posX);
    int posicaoAtualY = conversaoGraficoMatriz(*posY);

    if(keyUp && matrizJogo[posicaoAtualX][posicaoAtualY-1] != 'X'){
        *posY = *posY -2;
    }
    if(keyDown && matrizJogo[posicaoAtualX][posicaoAtualY+1] != 'X'){
        *posY = *posY +2;
    }
    if(keyLeft && matrizJogo[posicaoAtualX-1][posicaoAtualY] != 'X'){
        *posX = *posX -2;
    }
    if(keyRight && matrizJogo[posicaoAtualX+1][posicaoAtualY] != 'X'){
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
void limpaRastro(int posX, int posY, char matrizJogo[40][10]){

    if(matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] != 'C'){
        putchxy(posX, posY, ' ');
        putchxy(posX+1, posY, ' ');
        putchxy(posX, posY+1, ' ');
        putchxy(posX+1, posY+1, ' ');
    }
    else{
        paintPosicao('C', 9, posX, posY);
    }

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
        menu();
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

    menuLoop(0);

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

// Converte posicao do grafico para posicao da matriz do jogo
int conversaoGraficoMatriz(int pos){

    pos--;
    pos/= 2;

    return pos;
}

// Checa se player está na posiçao da zona final
int zonaFinal(int posX, int posY, char matrizJogo[40][10]){

    int retorno = 0;

    if(matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] == 'C'){
        retorno = 1;
    }

    return retorno;

}

void checaContato(Player *player, char matrizJogo[40][10]){

    if(matrizJogo[conversaoGraficoMatriz((*player).posX)][conversaoGraficoMatriz((*player).posY)] == 'M'){
        (*player).moedas = (*player).moedas + 1;
        (*player).points = (*player).points + 30;
        matrizJogo[conversaoGraficoMatriz((*player).posX)][conversaoGraficoMatriz((*player).posY)] = '-';
    }

}

void printaInfo(Player player, int level){

    gotoxy(20, 30);
    textcolor(15);
    printf("Score: %d", player.points);
    gotoxy(40, 30);
    printf("Nivel: %d", level);

}

int coletouTodas(int moedas, int total){

    int retorno = 0;

    if(moedas == total){
        retorno = 1;
    }

    return retorno;
}

void checaFimLevel(GameManager manager){

    if(zonaFinal(manager.player.posX, manager.player.posY, manager.matrizJogo) && coletouTodas(manager.player.moedas, manager.moedasTotais)){
        levelStart(manager.level+1, manager.player.points);
    }

}



