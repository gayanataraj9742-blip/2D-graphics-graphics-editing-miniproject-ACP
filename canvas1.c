#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum {
    LINE = 1,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1, x2, y2, x3, y3;
    int radius;
    int active;
    char ch;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* Canvas functions */
void initCanvas() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = ' ';
}

void plot(int x, int y, char ch) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = ch;
}

void displayCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
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
        plot(x1, y1, ch);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void drawRectangle(int x, int y, int w, int h, char ch) {
    for (int i = x; i < x + w; i++) {
        plot(i, y, ch);
        plot(i, y + h - 1, ch);
    }
    for (int i = y; i < y + h; i++) {
        plot(x, i, ch);
        plot(x + w - 1, i, ch);
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    drawLine(x1, y1, x2, y2, ch);
    drawLine(x2, y2, x3, y3, ch);
    drawLine(x3, y3, x1, y1, ch);
}

void drawCircle(int xc, int yc, int r, char ch) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (x <= y) {
        plot(xc + x, yc + y, ch); plot(xc - x, yc + y, ch);
        plot(xc + x, yc - y, ch); plot(xc - x, yc - y, ch);
        plot(xc + y, yc + x, ch); plot(xc - y, yc + x, ch);
        plot(xc + y, yc - x, ch); plot(xc - y, yc - x, ch);
        if (d < 0) d += 4 * x + 6;
        else { d += 4 * (x - y) + 10; y--; }
        x++;
    }
}

/* Render all active objects */
void renderObjects() {
    initCanvas();
    for (int i = 0; i < objectCount; i++) {
        if (!objects[i].active) continue;
        switch (objects[i].type) {
            case LINE: drawLine(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].ch); break;
            case RECTANGLE: drawRectangle(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].ch); break;
            case CIRCLE: drawCircle(objects[i].x1, objects[i].y1, objects[i].radius, objects[i].ch); break;
            case TRIANGLE: drawTriangle(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2, objects[i].x3, objects[i].y3, objects[i].ch); break;
        }
    }
}

/* Object management */
void addObject() {
    Object obj; int shape;
    printf("Choose shape (1=Line,2=Rectangle,3=Circle,4=Triangle): ");
    scanf("%d", &shape);
    obj.type = shape; obj.active = 1;
    printf("Enter character to draw with: "); scanf(" %c", &obj.ch);

    if (shape == LINE) scanf("%d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2);
    else if (shape == RECTANGLE) scanf("%d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2);
    else if (shape == CIRCLE) scanf("%d %d %d", &obj.x1,&obj.y1,&obj.radius);
    else if (shape == TRIANGLE) scanf("%d %d %d %d %d %d", &obj.x1,&obj.y1,&obj.x2,&obj.y2,&obj.x3,&obj.y3);

    objects[objectCount++] = obj;
}

void deleteObject() {
    int idx; printf("Enter object index to delete: "); scanf("%d", &idx);
    if (idx >= 0 && idx < objectCount) objects[idx].active = 0;
}

void modifyObject() {
    int idx; printf("Enter object index to modify: "); scanf("%d", &idx);
    if (idx < 0 || idx >= objectCount || !objects[idx].active) return;
    Object *obj = &objects[idx];
    printf("Enter new character: "); scanf(" %c", &obj->ch);
    if (obj->type == LINE) scanf("%d %d %d %d", &obj->x1,&obj->y1,&obj->x2,&obj->y2);
    else if (obj->type == RECTANGLE) scanf("%d %d %d %d", &obj->x1,&obj->y1,&obj->x2,&obj->y2);
    else if (obj->type == CIRCLE) scanf("%d %d %d", &obj->x1,&obj->y1,&obj->radius);
    else if (obj->type == TRIANGLE) scanf("%d %d %d %d %d %d", &obj->x1,&obj->y1,&obj->x2,&obj->y2,&obj->x3,&obj->y3);
}

void listObjects() {
    for (int i = 0; i < objectCount; i++)
        if (objects[i].active) printf("Index %d Type %d Char %c\n", i, objects[i].type, objects[i].ch);
}

/* Main menu */
int main() {
    int choice;
    while (1) {
        printf("\n--- ASCII Graphics Editor ---\n");
        printf("1. Add object\n2. Delete object\n3. Modify object\n4. Display picture\n5. List objects\n0. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) addObject();
        else if (choice == 2) deleteObject();
        else if (choice == 3) modifyObject();
        else if (choice == 4) { renderObjects(); displayCanvas(); }
        else if (choice == 5) listObjects();
        else if (choice == 0) break;
    }
    return 0;
}
