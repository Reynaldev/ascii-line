#include <iostream>
#include <cstring>
#include <cmath>

#if defined _WIN32
    #include <Windows.h>
    #define clearScreen() system("cls");
#elif defined __linux__
    #define clearScreen() puts("\033[2J");
#endif

#define PI 3.14

#define GRID_W 16

char grid[GRID_W][GRID_W];

struct Point
{
    int posX, posY;

    Point(int x, int y)
    {
        this->posX = x;
        this->posY = y;
    }

    void draw()
    {
        grid[posY][posX] = 'O';
    }

    void move(int x, int y)
    {
        this->posX = x;
        this->posY = y;
    }
};

template <typename T>
constexpr int ArrLength(T &arr) { return sizeof(arr) / sizeof(*arr); }

template <typename T>
constexpr inline T Clamp01(T val, T min, T max)
{
    if (val > max)
        return val = (T)1;

    if (val < min)
        return val = (T)0;

    val = ((val - min) / (max - min));

    return val;
}

float distance(Point src, Point dst)
{
    if (src.posX >= dst.posY)
        return sqrt(pow((dst.posX - src.posX), 2) + pow((dst.posY - src.posY), 2));
    else 
        return sqrt(pow((src.posX - dst.posX), 2) + pow((src.posY - dst.posY), 2));
}

void drawLine(Point src, Point dst)
{
    while ((src.posX != dst.posX - 1) && (src.posY != dst.posY - 1)) 
    {
        float h = distance(src, dst);
        float x = round((dst.posX - src.posX) / h); 
        float y = round((dst.posY - src.posY) / h);
        
        // if (src.posX != dst.posX)
        src.posX += x;

        // if (src.posY != dst.posY)
        src.posY += y;

        grid[src.posX][src.posY] = '-';
        // printf("X: %f | Y: %f | H: %f\n", x, y, h);
    }
}

void drawTriangle(Point src, Point dst)
{
    for (int yl = src.posY + 1; yl != dst.posY; yl++)
        grid[yl][src.posX] = '|';

    for (int xl = src.posX; xl != dst.posX; xl++)
        grid[dst.posY][xl] = '-';
}

void render()
{
    for (int y = 0; y < GRID_W; y++)
    {
        for (int x = 0; x < GRID_W; x++)
            printf("%c ", grid[y][x]);

        putchar('\n');
    }
}

int main()
{
    memset(grid, ' ', sizeof(grid));

    Point a(8, 8);
    Point b(8, 8);

    clock_t t = clock();
    while (true)
    {
        int d = clock() - t;

        if ((d % 1000) != 0)
            continue;

        clearScreen();

        static int r = 0;

        float rad = (float)((r * PI) / 180);
        int bx = round((sin(rad) * 4) + a.posX);
        int by = round((cos(rad) * 4) + a.posY);

        b.move(bx, by);

        r += 10;

        printf("R: %d | Rad: %f | Sin: %f | X: %d | Y: %d\n", r, rad, sin(rad), bx, by);

        a.draw();
        b.draw();

        drawLine(a, b);
        // drawTriangle(a, b);

        render();

        memset(grid, ' ', sizeof(grid));
    }
    
    return 0;
}