#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define SNAKEXSTARTINGPOS (18);
#define SNAKEYSTARTINGPOS (8);

struct snake 
{
    int x;
    int y;
    int tailX[2048];
    int tailY[2048];
    int score;
} snake;

struct fruit 
{
    int x;
    int y;
} fruit;

enum snakeState {ALIVE, DEAD};

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} direction;

void cursesInit();
void drawBarrier();
void drawFruit();
void getCurrentDirection();
void drawSnake();

int main() 
{
    direction currentDir = RIGHT;

    enum snakeState snakeState;
    snakeState = ALIVE;

    int barrierX = 60 ;
    int barrierY = (barrierX / 2) - 4;
    int speed = 200; // this is ms, the game refreshes every 0.2 seconds

    char keyboardInput;

    struct fruit fruit;
    fruit.x = rand() % (barrierX - 3) + 1;
    fruit.y = rand() % (barrierY - 4) + 1; // (40 / 2 - 4) - 4 + 1 == 13

    struct snake snake;
    snake.x = barrierX / 2;
    snake.y = barrierY / 2;
    snake.score = 2;

    srand(time(NULL)); 
    cursesInit();

    while (snakeState != DEAD) // game loop
    {
        int prevX = snake.tailX[0];
        int prevY = snake.tailY[0];
        int prev2x, prev2y;
        snake.tailX[0] = snake.x;
        snake.tailY[0] = snake.y;

        for (int i = 1; i < snake.score; i++)
        {
            prev2x = snake.tailX[i];            
            prev2y = snake.tailY[i];            
            snake.tailX[i] = prevX;
            snake.tailY[i] = prevY;
            prevX = prev2x;
            prevY = prev2y;
        }

        keyboardInput = getch();
        switch(keyboardInput)
        {
            case 'k': case 'w':
                currentDir = UP;
                break;
            case 'j': case 's':
                currentDir = DOWN;
                break;
            case 'h': case 'a':
                currentDir = LEFT;
                break;
            case 'l': case 'd':
                currentDir = RIGHT;
                break;
            case 'q':
                snakeState = DEAD;
        }

        erase();
        switch(currentDir)
        {
            case UP:
                snake.y--;
                timeout(speed);
                break;
            case DOWN:
                snake.y++;
                timeout(speed);
                break;
            case LEFT:
                snake.x--;
                timeout(speed / 2);
                break;
            case RIGHT:
                snake.x++;
                timeout(speed / 2);
                break;
        }

        // check if you've smacked into the barrier
        if (snake.x >= barrierX || snake.x <= 0 || snake.y >= barrierY || snake.y <= 0)
            snakeState = DEAD;

        // check to see if we have picked up fruit
        if (snake.x == fruit.x && snake.y == fruit.y)
        {
            snake.score++;
            fruit.x = rand() % 57 + 1;
            fruit.y = rand() % 23 + 1;
        }

        drawBarrier(barrierX, barrierY, "-", "|");
        drawFruit(fruit.x, fruit.y, "@");
        drawSnake(snake.x, snake.y, "O");

        for (int k = 0; k < snake.score; k++)
            mvprintw(snake.tailY[k], snake.tailX[k], "o"); // why does mvprintw go Y then X

        for (int i = 0; i < snake.score; i++)
        {
            if (snake.tailX[i] == snake.x && snake.tailY[i] == snake.y)
                snakeState = DEAD;            
        }

        mvprintw(barrierY, barrierX-10, "score: %d", snake.score);
        fflush(stdout);

    }
    endwin();
    printf("you lost!\nyour score was %d\n", snake.score);
    return 0;
}

void cursesInit()
{
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(100);
}

void drawBarrier(int xSize, int ySize, char horizontalChar[], char verticalChar[])
{
    for (int i = 0; i <= xSize; i++) 
    {
        mvprintw(0, i, "%s", horizontalChar);
        mvprintw(ySize, i, "%s", horizontalChar);
    }

    for (int i = 0; i <= ySize; i++) 
    {
        mvprintw(i, 0, "%s", verticalChar);
        mvprintw(i, xSize, "%s", verticalChar);
    }
}

void drawFruit(int xPos, int yPos, char fruitIcon[])
{
    mvprintw(yPos, xPos, "%s", fruitIcon);
}

void drawSnake(int xPos, int yPos, char snakeHeadIcon[])
{
    mvprintw(yPos,xPos, "%s", snakeHeadIcon);
}
