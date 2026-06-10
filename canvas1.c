#include <stdio.h>
#include <stdlib.h>

#define ROWS 30
#define COLS 60

char picture[ROWS][COLS];

/* Clear picture */
void clearPicture()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            picture[i][j] = ' ';
}

/* Display picture */
void displayPicture()
{
    int i, j;

    printf("\n");

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
            printf("%c", picture[i][j]);

        printf("\n");
    }
}

/* Plot a point */
void plot(int x, int y, char ch)
{
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
        picture[y][x] = ch;
}

/* Filled Rectangle */
void drawRectangle(int x, int y, int width, int height)
{
    int i, j;

    for(i = y; i < y + height; i++)
        for(j = x; j < x + width; j++)
            plot(j, i, '*');
}

/* Filled Circle */
void drawCircle(int xc, int yc, int r)
{
    int x, y;

    for(y = yc - r; y <= yc + r; y++)
    {
        for(x = xc - r; x <= xc + r; x++)
        {
            int dx = x - xc;
            int dy = y - yc;

            if(dx * dx + dy * dy <= r * r)
                plot(x, y, 'O');
        }
    }
}

/* Filled Triangle */
void drawTriangle(int centerX, int topY, int height)
{
    int row, col;

    for(row = 0; row < height; row++)
    {
        for(col = centerX - row;
            col <= centerX + row;
            col++)
        {
            plot(col, topY + row, '*');
        }
    }
}

/* Line */
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        plot(x1, y1, '*');

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

int main()
{
    int choice;

    clearPicture();

    printf("Choose Shape\n");
    printf("1. Filled Rectangle\n");
    printf("2. Filled Circle\n");
    printf("3. Filled Triangle\n");
    printf("4. Line\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        {
            int x, y, width, height;

            printf("Top Left X: ");
            scanf("%d", &x);

            printf("Top Left Y: ");
            scanf("%d", &y);

            printf("Width: ");
            scanf("%d", &width);

            printf("Height: ");
            scanf("%d", &height);

            drawRectangle(x, y, width, height);
            break;
        }

        case 2:
        {
            int xc, yc, radius;

            printf("Center X: ");
            scanf("%d", &xc);

            printf("Center Y: ");
            scanf("%d", &yc);

            printf("Radius: ");
            scanf("%d", &radius);

            drawCircle(xc, yc, radius);
            break;
        }

        case 3:
        {
            int centerX, topY, height;

            printf("Center X: ");
            scanf("%d", &centerX);

            printf("Top Y: ");
            scanf("%d", &topY);

            printf("Height: ");
            scanf("%d", &height);

            drawTriangle(centerX, topY, height);
            break;
        }

        case 4:
        {
            int x1, y1, x2, y2;

            printf("x1 y1: ");
            scanf("%d%d", &x1, &y1);

            printf("x2 y2: ");
            scanf("%d%d", &x2, &y2);

            drawLine(x1, y1, x2, y2);
            break;
        }

        default:
            printf("Invalid Choice!\n");
            return 0;
    }

    printf("\nPicture:\n");
    displayPicture();

    return 0;
}