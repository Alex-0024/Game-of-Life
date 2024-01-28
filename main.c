#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

/*
* Game of Life
* Rules of the game:
* A dead cell becomes alive if there are exactly three living cells next to it.
* If a living cell is surrounded by two or three living cells, then that cell remains alive.
* Otherwise the cell becomes dead.
* If there is not a single living cell left on the field or no changes occur, the game ends.
*/

#define WIDTH 150
#define HEIGHT 49
#define LIFE (char)219
#define NO_LIFE ' '


void show_Start(int* speed, int* density, int* mode);
void init_Map(char(*map)[WIDTH+1], int height, int width, int density, int mode);
void check_Map(const char(*map)[WIDTH+1], char(*mas)[WIDTH+1], int height, int width);
void copy_Mas_to_Map(char(*map)[WIDTH+1],const char(*mas)[WIDTH+1], size_t size);
void show_Map(char(*map)[WIDTH+1], int height);

int main()
{
    srand(time(NULL));

    char Map[HEIGHT][WIDTH+1];
    char Mas[HEIGHT][WIDTH+1] = {};
    int speed, density, mode;

    show_Start(&speed, &density, &mode);
    system("cls");
    init_Map(Map, HEIGHT, WIDTH, density, mode);
    show_Map(Map, HEIGHT);
    Sleep(1000);

    do
    {
        show_Map(Map, HEIGHT);
        check_Map(Map, Mas, HEIGHT, WIDTH);
        copy_Mas_to_Map(Map, Mas, sizeof(Mas));
        Sleep(500 / speed);
    }
    while(GetKeyState (VK_ESCAPE) >= 0);

    return 0;
}

void show_Start(int* speed, int* density, int* mode)
{
    puts("Game of Life");
    printf("You need to use the window console size that is at least width %d and height %d\n", WIDTH, HEIGHT+1);

    do
    {
        printf("input speed (1-500): ");
        scanf("%d", speed);
    }
    while(*speed < 1 || *speed > 500);

    do
    {
        printf("input density (2-10): ");
        scanf("%d", density);
    }
    while(*density < 2 || *density > 10);

    do
    {
        #define CNT_MODE 4
        puts("input mode (1-4) (you can create new modes)");
        puts("1. random");
        puts("2. horizontal line");
        puts("3. vertical line");
        puts("4. glider");
        scanf("%d", mode);
        if (*mode < 1 || *mode > CNT_MODE)
            puts("Not use now");
    }
    while(*mode < 1 || *mode > CNT_MODE);

    puts("Press 's' for start");
    char start = 0;
    while(start != 's')
        start = getch();
}

void no_life_all_Map(char(*map)[WIDTH+1], int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            map[i][j] = NO_LIFE;
        }
        map[i][width] = '\0';
    }
}

void init_Map(char(*map)[WIDTH+1], int height, int width, int density, int mode)
{
    no_life_all_Map(map, height, width);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int c;
            switch (mode)
            {
                case 1:
                    c = rand()%density;
                    if (c == 0)
                        map[i][j] = LIFE;
                    break;
                case 2:
                    if (i % 4 == 0 && i < height-4)
                    {
                        map[i][j] = LIFE;
                        map[i+1][j] = LIFE;
                    }
                    break;
                case 3:
                    if (j % 4 == 0 && j < width-4)
                    {
                        map[i][j] = LIFE;
                        map[i][j+1] = LIFE;
                    }
                    break;
                case 4:
                    if (i == 1 && j == 1)
                    {
                        map[i][j] = LIFE;
                        map[i+1][j+1] = LIFE;
                        map[i+1][j+2] = LIFE;
                        map[i+2][j] = LIFE;
                        map[i+2][j+1] = LIFE;
                    }
                    break;
            }
        }
    }
}

bool symbol_in_map(int h, int w, int height, int width)
{
    return (h >= 0 && h < height && w >= 0 && w < width);
}

int near_life(const char(*map)[WIDTH+1], int i, int j, int height, int width)
{
    int count_life = 0;
    for(int h = i-1; h <= i+1; h++)
        for(int w = j-1; w <= j+1; w++)
        {
            if (h == i && w == j)
                continue;
            if (symbol_in_map(h, w, height, width) && map[h][w] == LIFE)
                count_life++;
        }

    return count_life;
}

void check_Map(const char (*map)[WIDTH+1], char (*mas)[WIDTH+1], int height, int width)
{
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            int life_around = near_life(map, i, j, height, width);
            if (map[i][j] == NO_LIFE && life_around == 3)
                mas[i][j] = LIFE;
            else if (map[i][j] == LIFE && (life_around == 2 || life_around == 3) )
                mas[i][j] = LIFE;
            else
                mas[i][j] = NO_LIFE;
        }
}

void copy_Mas_to_Map(char(*map)[WIDTH+1],const char(*mas)[WIDTH+1], size_t size)
{
    memcpy(map, mas, size);
}

void setcur (int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void show_Map(char(*map)[WIDTH+1], int height)
{
    setcur (0,0);
    for(int i = 0; i < height; i++)
    {
        printf("%s", map[i]);
        printf("\n");
    }
    printf("Press Esc to exit");
}

