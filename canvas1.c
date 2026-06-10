#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 24
#define MAX_OBJECTS 100

#define EMPTY '_'

typedef enum {
    LINE = 1,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1, x2, y2;
    int x3, y3;
    int radius;
    int active;
    char ch;
} Object;

char picture[HEIGHT][WIDTH];
Object objects[MAX_OBJECTS];
int objectCount = 0;

/* Canvas functions */
void clearPicture() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            picture[i][j] = EMPTY;
}

void setPixel(int x, int y, char ch) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        picture[y][x] = ch;
}

void displayPicture() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf("%c", picture[i][j]);
        printf("\n");
    }
}

/* Drawing functions */
void drawLine(int x1, int y1, int x2, int y2, char ch) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1, ch);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2, char ch) {
    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            setPixel(x, y, ch);
}

void drawCircle(int cx, int cy, int radius, char ch) {
    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            double dist = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            if (dist <= radius) setPixel(x, y, ch);
        }
    }
}

/* Helper for triangle filling */
int interpolate(int x0, int y0, int x1, int y1, int y) {
    if (y1 == y0) return x0;
    return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
}

/* Filled triangle using scanline */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    // Sort vertices by y-coordinate
    if (y2 < y1) { int tx=x1; x1=x2; x2=tx; int ty=y1; y1=y2; y2=ty; }
    if (y3 < y1) { int tx=x1; x1=x3; x3=tx; int ty=y1; y1=y3; y3=ty; }
    if (y3 < y2) { int tx=x2; x2=x3; x3=tx; int ty=y2; y2=y3; y3=ty; }

    // Upper part
    for (int y = y1; y <= y2; y++) {
        int xa = interpolate(x1,y1,x3,y3,y);
        int xb = interpolate(x1,y1,x2,y2,y);
        if (xa > xb) { int tmp=xa; xa=xb; xb=tmp; }
        for (int x = xa; x <= xb; x++) setPixel(x,y,ch);
    }

    // Lower part
    for (int y = y2; y <= y3; y++) {
        int xa = interpolate(x1,y1,x3,y3,y);
        int xb = interpolate(x2,y2,x3,y3,y);
        if (xa > xb) { int tmp=xa; xa=xb; xb=tmp; }
        for (int x = xa; x <= xb; x++) setPixel(x,y,ch);
    }
}

/* Render all active objects */
void renderObjects() {
    clearPicture();
    for (int i = 0; i < objectCount; i++) {
        if (!objects[i].active) continue;
        switch (objects[i].type) {
            case LINE: drawLine(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].ch); break;
            case RECTANGLE: drawRectangle(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].ch); break;
            case CIRCLE: drawCircle(objects[i].x1, objects[i].y1, objects[i].radius, objects[i].ch); break;
            case TRIANGLE: fillTriangle(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].x3, objects[i].y3, objects[i].ch); break;
        }
    }
}

/* Object management */
void addObject() {
    Object obj; int shape;
    printf("\nChoose shape type:\n1. Line\n2. Rectangle\n3. Circle\n4. Triangle\nEnter shape type: ");
    scanf("%d", &shape);
    obj.type = shape; obj.active = 1;

    printf("\nEnter character to draw with: ");
    scanf(" %c", &obj.ch);

    if (shape == LINE) {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2);
    } else if (shape == RECTANGLE) {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2);
    } else if (shape == CIRCLE) {
        printf("Enter center x y and radius: ");
        scanf("%d %d %d", &obj.x1,&obj.y1,&obj.radius);
    } else if (shape == TRIANGLE) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2,&obj.x3,&obj.y3);
    }

    objects[objectCount] = obj;
    objectCount++;
}

void listObjects() {
    for (int i = 0; i < objectCount; i++)
        if (objects[i].active)
            printf("Index %d Type %d Char %c\n", i, objects[i].type, objects[i].ch);
}

/* Main menu */
int main() {
    int choice;
    while (1) {
        printf("2D Graphics Editor\n");
        printf("1. Add object\n2. Delete object\n3. Modify object\n4. Display picture\n5. List objects\n0. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) addObject();
        else if (choice == 4) { renderObjects(); printf("\n"); displayPicture(); }
        else if (choice == 5) listObjects();
        else if (choice == 0) break;

        printf("\n");
    }
    return 0;
}
