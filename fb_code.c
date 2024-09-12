#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int birdY = 300; // Initial bird position
int birdVelocity = 0; // Vertical velocity of the bird
int pipeX = 200;
int pipeY = 0;
int score = 0;
int gameState = 0; // 0: Start, 1: Playing, 2: End
int gameSpeed = 30; // Initial game speed (lower value means faster updates)
int pipeGap = 150; // Height of the gap in the pipes

void init() {
  glClearColor(0.53, 0.81, 0.92, 1); // Sky blue background
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 400, 0, 600);
}

void drawBird() {
  glPushMatrix();
  glTranslatef(50, birdY, 0);
  glColor3f(1.0, 1.0, 0.0); // Yellow color for the bird
  glBegin(GL_POLYGON);
  glVertex2f(-20, -20);
  glVertex2f(20, -20);
  glVertex2f(0, 20);
  glEnd();
  glPopMatrix();
}

void drawPipe() {
  glPushMatrix();
  glTranslatef(pipeX, 0, 0);
  glColor3f(0.0, 1.0, 0.0); // Green color for the pipe

  // Lower pipe
  glBegin(GL_POLYGON);
  glVertex2f(-20, 0);
  glVertex2f(20, 0);
  glVertex2f(20, pipeY - pipeGap / 2);
  glVertex2f(-20, pipeY - pipeGap / 2);
  glEnd();

  // Upper pipe
  glBegin(GL_POLYGON);
  glVertex2f(-20, pipeY + pipeGap / 2);
  glVertex2f(20, pipeY + pipeGap / 2);
  glVertex2f(20, 600);
  glVertex2f(-20, 600);
  glEnd();

  glPopMatrix();
}

void drawText(const char* text, int x, int y) {
  glRasterPos2i(x, y);
  glColor3f(1.0, 1.0, 1.0); // White color for the text
  for (const char* c = text; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  }
}

void drawStartScreen() {
  drawText("Press SPACE to Start", 120, 300);
}

void drawEndScreen() {
  char scoreText[20];
  sprintf(scoreText, "Final Score: %d", score);
  drawText(scoreText, 150, 320);
  drawText("Game Over! Press R to Restart", 100, 300);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  if (gameState == 0) { // Start screen
    drawStartScreen();
  } else if (gameState == 1) { // Playing
    drawBird();
    drawPipe();
    // Draw the score
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(scoreText, 10, 570);
  } else if (gameState == 2) { // End screen
    drawEndScreen();
  }

  glutSwapBuffers();
}

void update(int value) {
  if (gameState == 1) {
    // Update bird position with gravity
    birdY += birdVelocity;
    birdVelocity -= 1; // Apply gravity (adjust as needed)

    // Ensure bird doesn't go below ground or above screen
    if (birdY < 20) {
      birdY = 20;
      birdVelocity = 0; // Reset velocity if bird hits ground (adjust as needed)
    } else if (birdY > 580) {
      birdY = 580;
      birdVelocity = 0; // Reset velocity if bird hits top (adjust as needed)
    }

    // Update pipe position
    pipeX -= (5 + score / 5); // Increase pipe speed as score increases
    if (pipeX < -50) {
      pipeX = 400; // Reset pipe position
      pipeY = rand() % (600 - pipeGap) + pipeGap / 2; // Randomize pipe gap position
      score++;
      gameSpeed = 30 - score / 5; // Increase game speed as score increases
      if (gameSpeed < 10) gameSpeed = 10; // Cap the speed at a minimum value
    }

    // Check collision with pipe
    if ((birdY < pipeY - pipeGap / 2 || birdY > pipeY + pipeGap / 2) && pipeX < 70 && pipeX > 30) {
      gameState = 2; // End game
    }

    glutPostRedisplay();
    glutTimerFunc(gameSpeed, update, 0); // Adjust update interval based on game speed
  }
}

void keyboard(unsigned char key, int x, int y) {
  if (gameState == 0) {
    if (key == ' ') {
      gameState = 1;
      birdY = 300;
      birdVelocity = 0;
      pipeX = 400;
      pipeY = rand() % (600 - pipeGap) + pipeGap / 2;
      score = 0;
      gameSpeed = 30;
      glutTimerFunc(0, update, 0);
    }
  } else if (gameState == 1) {
    if (key == ' ') {
      birdVelocity = 10; // Flap the bird by setting a positive velocity (adjust as needed)
    }
  } else if (gameState == 2) {
    if (key == 'r' || key == 'R') {
      gameState = 0;
      birdY = 300;
      birdVelocity = 0;
      pipeX = 400;
      pipeY = rand() % (600 - pipeGap) + pipeGap / 2;
      score = 0;
      gameSpeed = 30;
      glutPostRedisplay();
    }
  }
}

int main(int argc, char **argv) {
  srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(400, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Flappy Bird");
  init();
  glutDisplayFunc(draw);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
