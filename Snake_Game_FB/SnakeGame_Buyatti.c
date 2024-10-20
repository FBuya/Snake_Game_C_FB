#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // para detectar teclas
#include <time.h>  // para generar posiciones aleatorias de la comida
#include <windows.h>


#define WIDTH 50
#define HEIGHT 30
#define numObstaclesMedium 10
#define TAB_OFFSET 3





int gameOver;  // Variable para indicar si el juego terminó
int x, y, foodX, foodY, score;
int tailX[100], tailY[100];  // Coordenadas de la cola de la serpiente
int nTail;  // Longitud de la cola
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };  // Direcciones posibles
enum eDirection dir;
int skinSelect;
int unlockSkinScored;
char locked[] = "LOCKED";
char snakeSkinsDefined[7][10] = {"Ooooo","\x40\x7E\x7E\x7E\x7E","O\x7E\x7E\x7E\x7E","=----","E====","\x9D\x93\x93\x93\x93","\xDB\xDC\xDC\xDC\xDC"};
char continuePlaying;
int selectorDifficulty;
int numColors = 6;
char colorsName[6][20] = {"\033[0;31mRed","\033[0;33mYellow","\033[0;32mGreen","\033[0;36mCyan","\033[0;34mBlue","\033[0;35mPurple"};
char colors[6][20] = {"\033[0;31m","\033[0;33m","\033[0;32m","\033[0;36m","\033[0;34m","\033[0;35m"};
int colorSelected;






void setConsoleSize(int width, int height) {
    char command[100];
    // Construimos el comando para fijar el tamaño de la consola
    sprintf(command, "mode con cols=%d lines=%d", width, height);
    system(command);

    // Opcional: deshabilitar la posibilidad de redimensionar la ventana de la consola
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;  // Deshabilitar el botón de maximizar
    style &= ~WS_SIZEBOX;      // Deshabilitar el redimensionamiento manual
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}




//MUEVE EL CURSOR AL INICIO DF LA CONSOLA
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 10;  // Tamaño del cursor
    cursorInfo.bVisible = FALSE;  // Cursor invisible
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setup(){

    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    foodX = (rand() % (WIDTH - 2)) + 1;
    foodY = (rand() % (HEIGHT - 2)) + 1;
    score = 0;

}




void generateFood(){

    int validPosition=0;

    while(!validPosition){
        foodX = (rand() % (WIDTH - 2)) + 1;
        foodY = (rand() % (HEIGHT - 2)) + 1;
        if(foodX != x || foodY != y){
            validPosition = 1;
            for(int i = 0; i < nTail; i++){
                if(tailX[i] == foodX && tailY[i] == foodY){
                    validPosition = 0;
                    break;
                }
            }
        }
    }
}

int unlockScored(){

    FILE *maxScoreSaved = fopen("maxScoreSaved.txt","r");
    int maxScored = 0;

    if(maxScoreSaved != NULL){

        fscanf(maxScoreSaved,"%d",&maxScored);

        fclose(maxScoreSaved);
    }

    unlockSkinScored = maxScored;

    if(unlockSkinScored < 100){
        return 1;
    }else if(unlockSkinScored >= 100 && unlockSkinScored < 300){
        return 2;
    }else if(unlockSkinScored >= 300 && unlockSkinScored < 500){
        return 3;
    }else if(unlockSkinScored >= 500 && unlockSkinScored < 700){
        return 4;
    }else if(unlockSkinScored >= 700 && unlockSkinScored < 1000){
        return 5;
    }else if(unlockSkinScored >= 1000 && unlockSkinScored < 2000){
        return 6;
    }else if(unlockSkinScored >= 2000){
        return 7;
    }




}

/*void menuDifficultySelector(){
    int result;

    do{
        printf("\n\n\n\t\t\tChoose Difficulty:\n");
        printf("\t\t\t1- Easy\n\t\t\t2- Medium\n\t\t\t3- Hard\n");
        printf("\t\t\t");
        result = scanf("%d",&selectorDifficulty);
        printf("\033[H\033[J");

        // Si scanf devuelve 1, el input es un entero válido
        if (result != 1) {
            printf("\033[H\033[J");
            // Limpiar el buffer y mostrar mensaje de error
            while (getchar() != '\n');
            selectorDifficulty = -1;
        }

        if(selectorDifficulty < 1 || selectorDifficulty > 3){
            printf("\n\t\t\tWRONG OPTION!\n");
        }
    }while(selectorDifficulty < 1 || selectorDifficulty > 3);
}*/

void menuSelecColorSnake(){

    int result;

    do{
        printf("\n\n\n\t\t\tChoose the Color of your Snake:\n");
        for(int i = 0; i < numColors; i++){
            printf("\t\t\t%d. %s\n",i+1,colorsName[i]);
            printf("\033[0m");
        }
        printf("\t\t\t");
        result = scanf("%d",&colorSelected);
        printf("\033[H\033[J");

        if(result != 1){
            while(getchar()!= '\n'){
                colorSelected = -1;
            }
        }


        if(colorSelected < 1 || colorSelected > numColors){
            printf("\033[0;31m");
            printf("\n\t\t\tWRONG OPTION!\n");
            printf("\033[0m");
        }

    }while(colorSelected < 1 || colorSelected > numColors);

    colorSelected--;
}

void menu(){

    int result;
    skinSelect;
    int numberSkinsUnlocked, totalSkins = 7;
    gameOver = 0;
    numberSkinsUnlocked = unlockScored();

    do{
        printf("\n\n\n\t\t\tChoose your Snake-Skin:\n");
        for(int i = 0; i < numberSkinsUnlocked; i++){
            printf("\t\t\t%d. %s\n",i+1,snakeSkinsDefined[i]);

        }
        for(int i = numberSkinsUnlocked +1; i < totalSkins + 1; i++){
                printf("\t\t\t%d. %s\n",i,locked);
            }
        printf("\t\t\t");
        result = scanf("%d",&skinSelect);
        printf("\033[H\033[J");

        // Si scanf devuelve 1, el input es un entero válido
        if (result != 1) {
            //printf("\033[H\033[J");
            // Limpiar el buffer y mostrar mensaje de error
            while (getchar() != '\n');
            skinSelect = -1;
        }

        if(skinSelect > numberSkinsUnlocked && skinSelect < totalSkins + 1){
            printf("\033[0;31m");
            printf("\n\t\tSKIN LOCKED! YOU NEED MORE POINTS!\n");
            printf("\033[0m");
        }else if(skinSelect < 1 || skinSelect > totalSkins){
            printf("\033[0;31m");
            printf("\n\t\t\tWRONG OPTION!\n");
            printf("\033[0m");
        }


    }while(skinSelect < 1 || skinSelect > numberSkinsUnlocked);

    //menuDifficultySelector();
    menuSelecColorSnake();

}

void gameOverMenu(){


    if(gameOver){
        printf("\033[H\033[J");
        do{
            printf("\n\n\t\t\tGame Over!\n");
            maxScoreSaved();

            printf("\t\t\tPlay Again (y/n): ");
            scanf(" %c",&continuePlaying);
            printf("\033[H\033[J");
            if(continuePlaying != 'y' && continuePlaying != 'n' && continuePlaying != 'Y' && continuePlaying != 'N'){
                printf("\n\t\t\tWRONG OPTION!\n");
            }

        }while(continuePlaying != 'y' && continuePlaying != 'n' && continuePlaying != 'Y' && continuePlaying != 'N');
    }


}


void draw(){

    if(continuePlaying == 'Y' || continuePlaying == 'y'){
        setup();
        nTail=0;
        continuePlaying = 'z';
    }

    gotoXY(0, 0);

    //BORDE SUPERIOR
    for(int i = 0; i < WIDTH + 2; i++){
        if(i == 0){
            printf("\t\t\t\033[0;31m%c",201);
            printf("\033[0m");
        }
        if(i != 0 && i != WIDTH + 1){
            printf("\033[0;31m%c",205);
            printf("\033[0m");
        }
        if(i == WIDTH + 1){
            printf("\033[0;31m%c",187);
            printf("\033[0m");
        }
    }


    //TABLERO, SERPIENTE Y COMIDA
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){

            if(j==0){
                if(i != 0){
                    printf("\t\t\t\033[0;31m%c",186);//BORDES LATERALES Izquierdo
                    printf("\033[0m");
                }
            }

            if(i == y && j == x){
                switch(skinSelect){
                case 1:
                    printf("%sO",colors[colorSelected]); //CABEZA SERPIENTE
                    printf("\033[0m");
                    break;
                case 2:
                    printf("%s%c",colors[colorSelected],64);
                    printf("\033[0m");
                    break;
                case 3:
                    printf("%sO",colors[colorSelected]);
                    printf("\033[0m");
                    break;
                case 4:
                    printf("%s=",colors[colorSelected]);
                    printf("\033[0m");
                    break;
                case 5:
                    printf("%sE",colors[colorSelected]);
                    printf("\033[0m");
                    break;
                case 6:
                    printf("%s%c",colors[colorSelected],157);
                    printf("\033[0m");
                    break;
                case 7:
                    printf("%s%c",colors[colorSelected],219);
                    printf("\033[0m");
                    break;
                }

            }else if(i == foodY && j == foodX ){
                printf("\033[0;33m%c",184); //COMIDA
                printf("\033[0m");
            }else{
                int print = 0;
                for(int k = 0; k < nTail; k++){
                    if(tailX[k] == j && tailY[k] == i){
                        //COLA SERPIENTE
                        switch(skinSelect){
                        case 1:
                            printf("%so",colors[colorSelected]);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 2:
                            printf("%s%c",colors[colorSelected],126);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 3:
                            printf("%s%c",colors[colorSelected],126);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 4:
                            printf("%s-",colors[colorSelected]);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 5:
                            printf("%s=",colors[colorSelected]);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 6:
                            printf("%s%c",colors[colorSelected],147);
                            printf("\033[0m");
                            print = 1;
                            break;
                        case 7:
                            printf("%s%c",colors[colorSelected],220);
                            printf("\033[0m");
                            print=1;
                            break;
                        }


                    }
                }
                if(!print){
                    printf(" ");
                }
            }
            //BORDES LATERALES
            if(j == WIDTH - 1){
                if(i != 0){
                    printf("\033[0;31m%c",186);
                    printf("\033[0m");
                }


            }
        }
        printf("\n");
    }



    //BORDES INFERIORES
    for(int i = 0; i < WIDTH + 2; i++){
        if(i == 0){
            printf("\t\t\t\033[0;31m%c",200);
            printf("\033[0m");
        }
        if(i != 0 && i != WIDTH + 1){
            printf("\033[0;31m%c",205);
            printf("\033[0m");
        }
        if(i == WIDTH + 1){
            printf("\033[0;31m%c",188);
            printf("\033[0m");
        }
    }
    printf("\n");

    //PUNTUACION
    printf("\t\t\t\tScore: \033[0;32m%d\n",score);
    printf("\033[0m");

    FILE *maxScoreSaved = fopen("maxScoreSaved.txt","r");
    int maxScored = 0;

    if(maxScoreSaved != NULL){

        fscanf(maxScoreSaved,"%d",&maxScored);

        fclose(maxScoreSaved);
    }
    printf("\t\t\t\tRecord: %d\n",maxScored);
}

void input(){

    if(_kbhit()){ //DETECTA SI EL JUGADOR PRESIONA UNA TECLA

        switch(_getch()){
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                dir = STOP;
                break;
            case 'A':
                dir = LEFT;
                break;
            case 'D':
                dir = RIGHT;
                break;
            case 'W':
                dir = UP;
                break;
            case 'S':
                dir = DOWN;
                break;
            case 'X':
                dir = STOP;
                break;

        }
    }

}

void logic(){

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for(int i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case STOP:
        dir = STOP;
    default:
        break;
    }

    //COLISIONES CON LO BORDES DEL MAPA
    if(x >= WIDTH){
        //x = 0;
        gameOver = 1;
    }else if(x < 0){
        //x = WIDTH - 1;
        gameOver = 1;
    }

    if(y >= HEIGHT){
        //y = 0;
        gameOver = 1;
    }else if(y < 1){
        //y = HEIGHT -1;
        gameOver = 1;
    }

    //COLISIONES CON LA COMIDA
    if(x == foodX && y == foodY){
        score += 10;
        nTail++;
        generateFood();
    }

    //COLISIONES CON LA COLA
    for(int i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y){
            gameOver = 1;
        }
    }
}

void speedIncrease(){

    if(score < 30){
        _sleep(100);
    }else if(score >= 30 && score < 60){
        _sleep(80);
    }else if(score >= 60 && score < 100){
        _sleep(60);
    }else if(score >= 100 && score < 120){
        _sleep(40);
    }else if(score >= 120 && score < 140){
        _sleep(20);
    }else if(score >= 140){
        _sleep(0);
    }
}


void maxScoreSaved(){

    FILE *maxScoreSaved = fopen("maxScoreSaved.txt","r");
    int maxScored = 0;

    if(maxScoreSaved != NULL){

        fscanf(maxScoreSaved,"%d",&maxScored);

        fclose(maxScoreSaved);
    }

    maxScoreSaved = fopen("maxScoreSaved.txt","w");


    if(maxScoreSaved != NULL){

        if(score >= maxScored){
            fprintf(maxScoreSaved,"%d", score);
        }else{
            fprintf(maxScoreSaved,"%d",maxScored);
        }
        fclose(maxScoreSaved);
    }else{
        printf("Error...");
    }

    if(score > maxScored){
        printf("\t\t\tNew Record!: %d\n\n",score);
    }

}



int main(){

    setConsoleSize(WIDTH + 80, HEIGHT + 10);
    hideCursor();
    setup();
    srand(time(NULL));

    unlockScored();
    do{
        menu();
        printf("\033[H\033[J");

        while(!gameOver){
            draw();
            input();
            logic();
            speedIncrease();
        }
        //printf("\033[H\033[J");
        gameOverMenu();
    }while(continuePlaying == 'y' || continuePlaying == 'Y');

    return 0;
}
