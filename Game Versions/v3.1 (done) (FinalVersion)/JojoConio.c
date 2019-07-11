#include <conio2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


// Esturutra do Jogador
typedef struct{

        int posX;
        int posY;
        int posXI;
        int posYI;
        int moedas;
        int points;
        int mortes;

}Player;

// Estrutura dos Inimigos
typedef struct{

    int posX;
    int posY;
    char direction; // V vertical, H horizontal
    int vetor;


}Enemy;

// Game manager
typedef struct{

    int iniciado;
    char matrizJogo[40][10];
    Player player;
    int moedasTotais;
    int level;
    Enemy enemies[20];
    int contaInimigos;
    int contadorTempo;
    int pontosPerdidos;

}GameManager;

// Headers das funções
void menu(GameManager manager);
void levelStart(int level, int points, int mortes);
void paintPosicao(char character, int cor, int posX, int posY);
void movePlayer();
void update(GameManager manager);
void getKey(int *up, int *down, int *left, int *right, GameManager manager);
void desenhaPlayer(int posX, int posY);
void limpaRastro(int posX, int posY, char matrizJogo[40][10]);
void retornaNivel(char matriz[40][10], char[], GameManager manager);
void menuLoop(int selection, GameManager manager);
int zonaFinal(int posX, int posY, char matrizJogo[40][10]);
int conversaoGraficoMatriz(int pos);
int coletouTodas(int moedas, int total);
void checaContato(GameManager *manager);
void printaInfo(Player player, int level);
void checaFimLevel(GameManager manager);
void moveEnemies(GameManager *manager);
void move1Enemy(Enemy *enemy);
void continuar(GameManager manager);
void closeGame();
void verificaRecorde(int pontos, GameManager manager);
void menuRecorde(int pontos, int rank[], GameManager manager);
void novoRecorde(int id, GameManager manager, char name[10][60], int scoresA[10]);
void menuScores(GameManager manager);
void scoresLoop(GameManager manager);


// Esconde o cursor por fins estéticos
void hideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}


// Main
int main(){
    hideCursor();
    GameManager manager;
    menu(manager);

    return 0;
}


// Inicialização de level
void levelStart(int level, int points, int mortes){

    system("mode 80,30");

    // Formata o nome do arquivo do nível
    char stringLevel[20] = "nivel";
    char levelChar[4];

    sprintf(levelChar, "%d", level);
    strcat(stringLevel, levelChar);
    strcat(stringLevel, ".txt");

    GameManager manager;
    manager.iniciado = 1;
    manager.moedasTotais = 0;
    manager.contaInimigos = 0;
    manager.contadorTempo = 0;
    manager.pontosPerdidos = 0;


    // Instanciação do jogador
    manager.player.moedas = 0;
    if(level == 1){
        manager.player.points = 0;
        manager.player.mortes = 0;
    }
    else{
        manager.player.points = points;
        manager.player.mortes = mortes;
    }
    manager.level = level;


    retornaNivel(manager.matrizJogo, stringLevel, manager);



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
                manager.player.posXI = posicaoAtualGraficoX;
                manager.player.posYI = posicaoAtualGraficoY;
            }
            if(manager.matrizJogo[i][j] == 'C'){
                paintPosicao('C', 9, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'M'){
                manager.moedasTotais = manager.moedasTotais + 1;
                paintPosicao('M', YELLOW, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'V'){
                manager.enemies[manager.contaInimigos].direction = 'V';
                manager.enemies[manager.contaInimigos].vetor = 2;
                manager.enemies[manager.contaInimigos].posX = posicaoAtualGraficoX;
                manager.enemies[manager.contaInimigos].posY = posicaoAtualGraficoY;
                manager.contaInimigos = manager.contaInimigos + 1;
            }
            if(manager.matrizJogo[i][j] == 'H'){
                manager.enemies[manager.contaInimigos].direction = 'H';
                manager.enemies[manager.contaInimigos].vetor = 2;
                manager.enemies[manager.contaInimigos].posX = posicaoAtualGraficoX;
                manager.enemies[manager.contaInimigos].posY = posicaoAtualGraficoY;
                manager.contaInimigos = manager.contaInimigos + 1;
            }
        }
    }



    update(manager);


}

// Loop do jogo
void update(GameManager manager){


    while(1 == 1){

    int keyUp, keyDown, keyLeft, keyRight;

    // Quando tecla é pressionada
    if(kbhit()){
        getKey(&keyUp, &keyDown, &keyLeft, &keyRight, manager);
        movePlayer(&manager.player.posX, &manager.player.posY, keyUp, keyDown, keyLeft, keyRight, manager.matrizJogo);
    }

    desenhaPlayer(manager.player.posX, manager.player.posY);

    // movendo inimigos
    if(manager.contadorTempo % 1 == 0){
        moveEnemies(&manager);
    }

    // printando inimigos
    for(int i = 0;i < manager.contaInimigos;i++){
        if(manager.enemies[i].direction == 'V'){
            paintPosicao('V', 4, manager.enemies[i].posX, manager.enemies[i].posY);
        }
        else{
            paintPosicao('H', 4, manager.enemies[i].posX, manager.enemies[i].posY);
        }
    }


    checaFimLevel(manager);

    checaContato(&manager);
    printaInfo(manager.player, manager.level);


    Sleep(100);

    // A cada segundo
    if(manager.contadorTempo == 10){
        manager.pontosPerdidos+= 10;
    }
    else{
        manager.contadorTempo++;
    }


    }

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
void getKey(int *up, int *down, int *left, int *right, GameManager manager){

    *up = 0;
    *down = 0;
    *left = 0;
    *right = 0;


        char key = getch();

        if(key == 'à'){
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
            if(key == '\t'){
                menu(manager);
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

// Limpa a posição dada como argumento ou simplesmente printa o que está na matriz do jogo, como moedas ou 'tiles' de fim de nível
void limpaRastro(int posX, int posY, char matrizJogo[40][10]){

    if(matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] != 'C' && matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] != 'M'){
        putchxy(posX, posY, ' ');
        putchxy(posX+1, posY, ' ');
        putchxy(posX, posY+1, ' ');
        putchxy(posX+1, posY+1, ' ');
    }
    else if(matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] == 'C'){
        paintPosicao('C', 9, posX, posY);
    }
    else if(matrizJogo[conversaoGraficoMatriz(posX)][conversaoGraficoMatriz(posY)] == 'M'){
        paintPosicao('M', 14, posX, posY);
    }

}

//  Retorna a matriz do arquivo do nível do argumento
void retornaNivel(char matriz[40][10], char nomeArquivo[], GameManager manager){

    FILE *arquivo;

    if(arquivo = fopen(nomeArquivo, "r")){



        // lendo matriz do nivel
        int i = 0;
        int j = 0;
        while(j < 10){
            fread(&matriz[i][j], sizeof(char), 1, arquivo);

            // Checa se é quebra de linha
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

        verificaRecorde(manager.player.points, manager);

    }
}

// Exibe o menu

void menu(GameManager manager){

// Carrega o title screen do jogo
    system("mode 130,30");
    FILE *arquivo;
    char letra;
    gotoxy(50,29);
    textcolor(GREEN);
    printf("Bruno Correa, Daniel Gutschwager");
    gotoxy(1, 1);

    if(arquivo = fopen("logo.txt", "r")){

        int color = 0;

        do{

            if(color > 15){
                color = 0;
            }

            textcolor(color);

            fread(&letra, sizeof(char), 1, arquivo);
            if(!feof(arquivo)){
                printf("%c", letra);
            }
            color++;

        }while(!feof(arquivo));
        fclose(arquivo);
    }

    if(manager.iniciado != 1){

        gotoxy(60, 20);

        printf("New Game\n");

        gotoxy(61, 21);

        printf("Scores\n");

        gotoxy(62, 22);

        printf("Exit");

        menuLoop(0, manager);
    }
    else{
        gotoxy(60, 20);

        printf("Continue\n");

        gotoxy(60,21);

        printf("New Game\n");

        gotoxy(61, 22);

        printf("Scores\n");

        gotoxy(62, 23);

        printf("Exit");

        menuLoop(0, manager);
    }

}


// Loop de controle do menu principal
void menuLoop(int selection, GameManager manager){


    // Jogo recém iniciado
    if(manager.iniciado != 1){
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
                menuLoop(selection-1, manager);
            }

            else if((key == 's' || key == 'P') && selection < 3){
                Sleep(60);
                menuLoop(selection+1, manager);
            }
            // Novo Jogo
            else if(key == 13 && selection == 0){
                levelStart(1, 0, manager.player.mortes);
            }
            else if(key == 13 && selection == 1){
                menuScores(manager);
            }
            else if(key == 13 && selection == 2){
                closeGame();
            }
            else{
                Sleep(60);
                menuLoop(selection, manager);
            }


        }
        else{
            Sleep(60);
            menuLoop(selection, manager);
        }
    }
    // Jogo pausado
    else{
        switch(selection){

        case 0:
            putchxy(58, 20, '>');
            putchxy(58, 21, ' ');
            putchxy(59, 22, ' ');
            putchxy(60, 23, ' ');
            break;
        case 1:
            putchxy(58, 20, ' ');
            putchxy(58, 21, '>');
            putchxy(59, 22, ' ');
            putchxy(60, 23, ' ');
            break;
        case 2:
            putchxy(58, 20, ' ');
            putchxy(58, 21, ' ');
            putchxy(59, 22, '>');
            putchxy(60, 23, ' ');
            break;
        case 3:
            putchxy(58, 20, ' ');
            putchxy(58, 21, ' ');
            putchxy(59, 22, ' ');
            putchxy(60, 23, '>');
            break;


        }


        if(kbhit()){


           char key = getch();

            if(key != 'w' && key != 's' && key != 13){
                key = getch();
            }


            if((key == 'w' || key == 'H') && selection > 0){
                Sleep(60);
                menuLoop(selection-1, manager);
            }

            else if((key == 's' || key == 'P') && selection < 4){
                Sleep(60);
                menuLoop(selection+1, manager);
            }
            // Continuar
            else if(key == 13 && selection == 0){
                continuar(manager);
            }
            // Novo Jogo
            else if(key == 13 && selection == 1){
                levelStart(1, 0, manager.player.mortes);
            }
            // Scores
            else if(key == 13 && selection == 2){
                menuScores(manager);
            }
            // Exit
            else if(key == 13 && selection == 3){
                closeGame();
            }
            else{
                Sleep(60);
                menuLoop(selection, manager);
            }


        }
        else{
            Sleep(60);
            menuLoop(selection, manager);
        }
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

// Verifica os contatos possíveis do jogador com objetos na tela
void checaContato(GameManager *manager){

    // Contato com moeda
    if((*manager).matrizJogo[conversaoGraficoMatriz((*manager).player.posX)][conversaoGraficoMatriz((*manager).player.posY)] == 'M'){
        (*manager).player.moedas = (*manager).player.moedas + 1;
        (*manager).player.points = (*manager).player.points + 30;
        (*manager).matrizJogo[conversaoGraficoMatriz((*manager).player.posX)][conversaoGraficoMatriz((*manager).player.posY)] = '-';
    }
    // Contato com inimigo
    for(int i = 0; i < (*manager).contaInimigos; i++){
        if((*manager).player.posX == (*manager).enemies[i].posX && (*manager).player.posY == (*manager).enemies[i].posY){
        (*manager).player.posX = (*manager).player.posXI;
        (*manager).player.posY = (*manager).player.posYI;
        (*manager).player.mortes = (*manager).player.mortes + 1;
        }
    }


}

// Imprime na tela informações gerais como pontuação, nível atual e número de mortes
void printaInfo(Player player, int level){

    gotoxy(16, 25);
    textcolor(15);
    printf("Score: %d", player.points);
    gotoxy(32, 25);
    printf("Nivel: %d", level);
    gotoxy(48, 25);
    printf("Mortes: %d", player.mortes);



}

// Checa se o jogador coletou todas as moedas do nível
int coletouTodas(int moedas, int total){

    int retorno = 0;

    if(moedas == total){
        retorno = 1;
    }

    return retorno;
}

// Checa se o jogador completou o nível e o envia para o próximo
void checaFimLevel(GameManager manager){

    if(zonaFinal(manager.player.posX, manager.player.posY, manager.matrizJogo) && coletouTodas(manager.player.moedas, manager.moedasTotais)){
        levelStart(manager.level+1, manager.player.points + 1000 - manager.pontosPerdidos, manager.player.mortes);
    }

}

// Move o array de inimigos
void moveEnemies(GameManager *manager){

    gotoxy(40,30);
    for(int j = 0; j < (*manager).contaInimigos;j++){
        if((*manager).enemies[j].direction == 'V'){
            if((*manager).matrizJogo[conversaoGraficoMatriz((*manager).enemies[j].posX)][conversaoGraficoMatriz((*manager).enemies[j].posY)+((*manager).enemies[j].vetor/2)] == 'X'){
                (*manager).enemies[j].vetor = (*manager).enemies[j].vetor * -1;
            }
            (*manager).enemies[j].posY = (*manager).enemies[j].posY + (*manager).enemies[j].vetor;

           limpaRastro((*manager).enemies[j].posX, (*manager).enemies[j].posY - (*manager).enemies[j].vetor, (*manager).matrizJogo);





        }
        if((*manager).enemies[j].direction == 'H'){
            if((*manager).matrizJogo[conversaoGraficoMatriz((*manager).enemies[j].posX) +((*manager).enemies[j].vetor/2)][conversaoGraficoMatriz((*manager).enemies[j].posY)] == 'X'){
                (*manager).enemies[j].vetor = (*manager).enemies[j].vetor * -1;
            }
            (*manager).enemies[j].posX = (*manager).enemies[j].posX + (*manager).enemies[j].vetor;

            limpaRastro((*manager).enemies[j].posX - (*manager).enemies[j].vetor, (*manager).enemies[j].posY, (*manager).matrizJogo);



        }
    }


}

// Reinicia o jogo no estado atual
void continuar(GameManager manager){

    system("mode 80,30");
    for(int i = 0; i < 40;i++){
        for(int j = 0; j < 10;j++){

            // Pinta bordas
            int posicaoAtualGraficoX = (i*2)+1;
            int posicaoAtualGraficoY = (j*2)+1;

            if(manager.matrizJogo[i][j] == 'X'){
                paintPosicao('/', 15, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'C'){
                paintPosicao('C', 9, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
            if(manager.matrizJogo[i][j] == 'M'){
                paintPosicao('M', 14, posicaoAtualGraficoX, posicaoAtualGraficoY);
            }
        }
    }

    update(manager);

}

// Fecha o jogo
void closeGame(){

   for(int i = 1; i < 130; i++){
        for(int j = 20; j < 30;j++){
            putchxy(i, j, ' ');
        }
   }
   gotoxy(60, 25);
   printf("Obrigado por jogar!");
   exit(0);

}

// Verifica se um novo recorde foi atingido
void verificaRecorde(int pontos, GameManager manager){



    FILE *file;
    if(file = fopen("scores.txt", "r")){

        int id;
        char name[10][60];
        int cont = 0;
        int temp;
        int scoresA[10];
        int termina = 0;


        while(!feof(file)){

            fscanf(file, "%s %d\n", name[cont], &temp);
            scoresA[cont] = temp;
            if(pontos > temp && !termina){
                termina = 1;
                id = cont;
                cont++;
            }
            else{
                cont++;
            }


        }
        fclose(file);
        printf("%d", cont);

        if(termina){
            novoRecorde(id, manager, name, scoresA);
        }
        else{
            manager.iniciado = 0;
            menu(manager);
        }

    }

}


// Caso um novo recorde tenha sido atingido, esta função trata de salvá-lo nos top 10 scores
void novoRecorde(int id, GameManager manager, char name[10][60], int scoresA[10]){
    clrscr();

    char nome[60];
    int waste1;

    textcolor(WHITE);

    gotoxy(35, 5);
    printf("Novo recorde!");
    gotoxy(30, 10);
    printf("Sua pontuacao: %d", manager.player.points);
    gotoxy(24,12);

    textcolor(GREEN);
    printf("Digite o seu nome! (Sem espacos!)");
    gotoxy(35, 20);
    scanf("%s", nome);

    FILE *file;
    if(file = fopen("scores.txt", "w")){

        int cont = 0;

        while(cont < id){
            fprintf(file, "%s %d\n", name[cont], scoresA[cont]);
            cont++;
        }

        while(cont < 9){
            if(cont == id){
                fprintf(file, "%s %d\n", nome, manager.player.points);
                id = -1;
            }
            else{
                fprintf(file, "%s %d\n", name[cont], scoresA[cont]);
                cont++;
            }
        }

        fclose(file);

        manager.iniciado = 0;
        menu(manager);
    }
    else{
        manager.iniciado = 0;
        menu(manager);
        }


}

// Exibe as melhores pontuações para o jogador
void menuScores(GameManager manager){

    clrscr();
    FILE *file;

    char nome[200];
    int score;
    textcolor(GREEN);

    gotoxy(24,4);
    printf("Ranking - Nome - Score");

    if(file = fopen("scores.txt", "r")){
        for(int i = 0; i < 10;i++){
            fscanf(file, "%s %d\n", nome, &score);
            gotoxy(30, 5+i);
            printf("%d - %s - %d", i, nome, score);
        }

        gotoxy(30, 18);
        printf("Pressione qualquer tecla para voltar para o menu...");

        scoresLoop(manager);
    }

    else{
        menu(manager);
    }
}


// Aguarda instruções do jogador dentro do menu de pontuações
void scoresLoop(GameManager manager){

    while(1 == 1){
        int aux = 0;

        if(kbhit()){
            aux = 1;
        }

        if(aux){
            menu(manager);
        }
        else{
            Sleep(100);
        }
    }
}





