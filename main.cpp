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

    Point(int x = 0, int y = 0)
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

    friend bool operator<(const Point &a, const Point &b)
    {
        if ((a.posX < b.posX) && (a.posY < b.posY))
            return true;

        return false;
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
    return sqrt(pow((dst.posX - src.posX), 2) + pow((dst.posY - src.posY), 2));
}

void drawLine(Point src, Point dst)
{
    while ((src.posX != dst.posX) && (src.posY != dst.posY)) 
    {
        float h = distance(src, dst);
        float x = round((dst.posX - src.posX) / h); 
        float y = round((dst.posY - src.posY) / h);
        
        if (src.posX != dst.posX)
        {
            if (src.posX > dst.posY)
                src.posX -= x;
            else
                src.posX += x;
        }

        if (src.posY != dst.posY)
        {
            if (src.posY > dst.posY)
                src.posY -= y;
            else 
                src.posY += y;
        }

        grid[src.posX][src.posY] = '-';
        // printf("X: %f | Y: %f | H: %f\n", x, y, h);
    }
}

void drawTriangle(Point src, Point dst)
{
    // for (int yl = src.posY + 1; yl != dst.posY; yl++)
    //     grid[yl][src.posX] = '|';

    if (src.posX != dst.posX)
    {
        const int offset = (src.posX < dst.posX) ? 1 : -1;
        int xl = src.posX + offset;
        while (xl != dst.posX)
        {
            grid[src.posY][xl] = '-';
            xl += offset;
        }
    }

    if (src.posY != dst.posY)
    {
        const int offset = (src.posY < dst.posY) ? 1 : -1;
        int yl = src.posY + offset;
        while (yl != dst.posY)
        {
            grid[yl][dst.posX] = '|';
            yl += offset;
        }
    }
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
        int bx = round((cos(rad) * 6) + a.posX);
        int by = round((sin(rad) * 6) + a.posY);

        b.move(bx, by);

        r += 10;

        printf("R: %d | Rad: %f | Sin: %f | X: %d | Y: %d\n", r, rad, sin(rad), bx, by);
        printf("B =\n\tX: %d | Y: %d\n", b.posX, b.posY);
        printf("Distance from A to B:\n\tX: %d | Y: %d | H: %f\n", abs(b.posX - a.posX), abs(b.posY - a.posY), distance(a, b));

        a.draw();
        b.draw();

        // drawLine(a, b);
        drawTriangle(a, b);
        if (a.posX > b.posX)
            printf("A is in front of B\n");
        else
            printf("A is behind of B\n");

        if (a.posY > b.posY)
            printf("A is below B\n");
        else
            printf("A is above B\n");

        render();

        memset(grid, ' ', sizeof(grid));
    }
    
    return 0;
}