#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>

#define SIZE 20

float angle = 0.0f;
int maze[SIZE][SIZE];
int goalX = SIZE - 3, goalY = SIZE - 3;
int xPos = 1;
int yPos = 1;
int isWin = 0;
int ip = 0;

void menu(int num){
    if(num==3){
        exit(0);
    }
    glutPostRedisplay();
}

void generateMaze() {
    srand(time(NULL));

    // Initialize the maze with walls
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            maze[i][j] = 1;
        }
    }

    // Generate the maze with multiple paths
    int stack[SIZE * SIZE];
    int top = 0;

    int visited[SIZE][SIZE] = { 0 };
    int directions[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    int currentX = 1;
    int currentY = 1;
    visited[currentY][currentX] = 1;
    maze[currentY][currentX] = 0;

    while (currentX != goalX || currentY != goalY) {
        int validNeighbors[4][2];
        int validCount = 0;

        for (int i = 0; i < 4; i++) {
            int neighborX = currentX + directions[i][0] * 2;
            int neighborY = currentY + directions[i][1] * 2;

            if (neighborX >= 1 && neighborX < SIZE - 1 && neighborY >= 1 && neighborY < SIZE - 1 && visited[neighborY][neighborX] == 0) {
                validNeighbors[validCount][0] = neighborX;
                validNeighbors[validCount][1] = neighborY;
                validCount++;
            }
        }

        if (validCount > 0) {
            int randomIndex = rand() % validCount;
            int nextX = validNeighbors[randomIndex][0];
            int nextY = validNeighbors[randomIndex][1];

            maze[nextY][nextX] = 0;
            visited[nextY][nextX] = 1;

            maze[currentY + (nextY - currentY) / 2][currentX + (nextX - currentX) / 2] = 0;
            visited[currentY + (nextY - currentY) / 2][currentX + (nextX - currentX) / 2] = 1;

            stack[top++] = currentX;
            stack[top++] = currentY;

            currentX = nextX;
            currentY = nextY;
        }
        else if (top > 0) {
            currentY = stack[--top];
            currentX = stack[--top];
        }
        else {
            // Randomly choose a new starting position
            currentX = rand() % (SIZE - 2) + 1;
            currentY = rand() % (SIZE - 2) + 1;

            // Reset the visited array
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    visited[i][j] = 0;
                }
            }

            visited[currentY][currentX] = 1;
        }
    }
}

void drawMaze() {
    int i, j;
    float x, y;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (maze[i][j] == 1) {
                glColor3f(0.0f, 0.8f, 0.3f);  // Walls are green
            } else if (i == goalY && j == goalX) {
                glColor3f(1.0f, 0.0f, 0.0f);  // Goal is red
            } else {
                glColor3f(0.7f, 0.7f, 0.7f);  // Paths are white
            }

            x = j * 2.0f;
            y = i * 2.0f;

            glBegin(GL_QUADS);
            glVertex3f(x, 0.0f, y);
            glVertex3f(x + 2.0f, 0.0f, y);
            glVertex3f(x + 2.0f, 0.0f, y + 2.0f);
            glVertex3f(x, 0.0f, y + 2.0f);
            glEnd();
        }
    }
}
/*
void drawPlayer() {  //when not using metallic cube as player
    float x = xPos * 2.0f + 1.0f;
    float y = yPos * 2.0f + 1.0f;

    glColor3f(0.0f, 0.0f, 1.0f);  // Player is blue

    glPushMatrix();
    glTranslatef(x, 0.0f, y);
    glutSolidCube(1.0f);
    glPopMatrix();
}  */

void drawPlayer() {  //when using metallic cube as player
    float x = xPos * 2.0f + 1.0f;
    float y = yPos * 2.0f + 1.0f;

    //glColor3f(0.0f, 0.5f, 0.8f);  // Player is blue

    glPushMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat lightPosition[]={5.0f,15.0f,5.0f,1.0f};
    GLfloat lightAmbient[]={0.2f,0.2f,0.2f,1.0f};
    GLfloat lightDiffuse[]={0.8f,0.8f,0.8f,1.0f};
    GLfloat lightSpecular[]={1.0f,1.0f,1.0f,1.0f};
    
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    
    GLfloat cubeAmbient[]={0.25f,0.25f,0.25f,1.0f};
    GLfloat cubeDiffuse[]={0.4f,0.4f,0.4f,1.0f};
    GLfloat cubeSpecular[]={0.8f,0.8f,0.8f,1.0f};
    GLfloat cubeShininess = 100.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,cubeAmbient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,cubeDiffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,cubeSpecular);
    glMaterialfv(GL_FRONT,GL_SHININESS,&cubeShininess);
    
    glTranslatef(x, 0.0f, y);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glutSolidCube(1.5f);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glPopMatrix();
}

void display() {
     
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float mazeSize = SIZE * 2.0f;
    gluLookAt(mazeSize / 2.0f, mazeSize * 1.5f, mazeSize * 2.0f, mazeSize / 2.0f, 0.0f, mazeSize / 2.0f, 0.0f, 1.0f, 0.0f);
    
    if(ip==1){
    	glColor3f(1.0f, 1.0f, 1.0f);  // message 
        glRasterPos3f(0.0f, SIZE+5.0f, mazeSize / 2.0f - 0.5f);
        const char* message = "Currently playing manually. Use W-A-S-D controls.";
        for (const char* c = message; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    
    if(ip==2){
    	glColor3f(1.0f, 1.0f, 1.0f);  // message 
        glRasterPos3f(3.0f, SIZE+5.0f, mazeSize / 2.0f - 0.5f);
        char* message = "Solving Automatically using D.F.S.";
        for (const char* c = message; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }

    if (isWin) {
        glColor3f(1.0f, 0.0f, 0.0f);  // message 
        glRasterPos3f(0.0f, 1.0f, mazeSize / 2.0f - 0.5f);
        const char* winMessage = "Finish!! Press ESC to exit the Simulation";
        for (const char* c = winMessage; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }

    drawMaze();
    drawPlayer();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)width / (float)height;
    float mazeSize = SIZE * 2.0f;
    gluPerspective(45.0, aspectRatio, 1.0, mazeSize * 5.0f);
    glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y) {  //this was for w-a-s-d controls
    if (isWin) {
    	if(key==27)
    	    exit(0);
        return;  // Ignore key inputs after winning
    }

    int nextX = xPos;
    int nextY = yPos;

    switch (key) {
        //case GLUT_KEY_UP:
        case 'w':
            nextY = yPos - 1;
            break;
        //case GLUT_KEY_DOWN:
        case 's':
            nextY = yPos + 1;
            break;
        //case GLUT_KEY_LEFT:
        case 'a':
            nextX = xPos - 1;
            break;
        //case GLUT_KEY_RIGHT:
        case 'd':
            nextX = xPos + 1;
            break;
        case 27:
            exit(0);
    }

    // Check if the next position is a valid move
    if (maze[nextY][nextX] != 1) {
        xPos = nextX;
        yPos = nextY;
    }

    if (xPos == goalX && yPos == goalY) {
        isWin = 1;
    }

    glutPostRedisplay();
}




void idle() {      //this update was for rotating the player at it's place
    angle += 2.0f;

    if (angle > 360.0f) {
        angle -= 360.0f;
    }

    glutPostRedisplay();
}


void update(int value) {      //this update is for moving the player automatically till goal
    
    // Perform Depth First Search (DFS)
    static int visited[SIZE][SIZE] = {0};
    static int stack[SIZE * SIZE * 2];
    static int top = 0;

    if (isWin) {
        return;  // Stop updating if the goal is reached
    }

    if (top == 0) {
        // Push the starting position onto the stack
        stack[top++] = xPos;
        stack[top++] = yPos;
        visited[yPos][xPos] = 1;
    }

    if (top > 0) {
        // Pop the current position from the stack
        int currentY = stack[--top];
        int currentX = stack[--top];

        // Check if the current position is the goal
        if (currentX == goalX && currentY == goalY) {
            isWin = 1;
            glutPostRedisplay();
            return;
        }

        // Push the next positions onto the stack
        int directions[4][2] = { {-1, 0},{0, -1}, {1, 0}, {0,1}};
        for (int i = 0; i < 4; i++) {
            int nextX = currentX + directions[i][0];
            int nextY = currentY + directions[i][1];

            // Check if the next position is a valid move and has not been visited
            if (maze[nextY][nextX] != 1 && visited[nextY][nextX] == 0) {
                stack[top++] = nextX;
                stack[top++] = nextY;
                visited[nextY][nextX] = 1;
            }
        }

       
        xPos = stack[top - 2];
        yPos = stack[top - 1];
    }

    glutPostRedisplay();
    glutTimerFunc(300, update, 0);  // animation delay
    
}


int main(int argc, char** argv) {
    printf("Select your option \n");
    printf("1.Play the game (Use W-A-S-D as controls)\n2.Solve automatically using dfs\n");
    scanf("%d",&ip);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Maze Solver");
    glEnable(GL_DEPTH_TEST);
    
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    
    glutCreateMenu(menu);
    
    glutAddMenuEntry("Exit",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    generateMaze();
    
    
    
    if(ip==1){
    	glutKeyboardFunc(keyboard);  //this was for implementing the keyboard callback
    }
    else if(ip==2){
    	glutTimerFunc(300, update, 0);
    	glutKeyboardFunc(keyboard);
    }

    generateMaze();
    
    glutMainLoop();

    return 0;
}

