#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

// Constants
#define FPS 60
#define PADDLE_WIDTH 200
#define PADDLE_HEIGHT 20
#define BALL_RADIUS 15  
#define BRICK_WIDTH 100 
#define BRICK_HEIGHT 40 
#define PADDLE_SPEED 20
#define TOP_PADDING 50 

// Variables for screen size
int screenWidth;
int screenHeight;
int numBricksRows;
int numBricksCols;

// Game variables
float paddleX;
float ballX;
float ballY;
float ballSpeedX = 6.0f;
float ballSpeedY = -6.0f;
float ballSpeedMultiplier = 1.0f;
int lives = 3;
int score = 0;
bool gameRunning = false;
bool gamePaused = false;

struct Brick {
    float x, y;
    bool active;
    float r, g, b;
};
std::vector<Brick> bricks;

void saveScore();
int getHighestScore();

// Initialize bricks dynamically based on screen size
void initBricks() {
    bricks.clear();
    numBricksCols = screenWidth / (BRICK_WIDTH + 10);
    numBricksRows = (screenHeight / 3) / (BRICK_HEIGHT + 10);

    for (int i = 0; i < numBricksRows; i++) {
        for (int j = 0; j < numBricksCols; j++) {
            Brick brick;
            brick.x = j * (BRICK_WIDTH + 10) + 10;
            brick.y = screenHeight - TOP_PADDING - (i + 1) * (BRICK_HEIGHT + 10);
            brick.active = true;

            brick.r = 0.5f + static_cast<float>(rand()) / (2.0f * static_cast<float>(RAND_MAX));
            brick.g = 0.5f + static_cast<float>(rand()) / (2.0f * static_cast<float>(RAND_MAX));
            brick.b = 0.5f + static_cast<float>(rand()) / (2.0f * static_cast<float>(RAND_MAX));
            bricks.push_back(brick);
        }
    }
}

// Start game
void startGame() {
    paddleX = screenWidth / 2 - PADDLE_WIDTH / 2;
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;
    ballSpeedX = (rand() % 2 == 0 ? 6.0f : -6.0f);
    ballSpeedY = 6.0f; 
    ballSpeedMultiplier = 1.0f;
    lives = 3;
    score = 0;
    gameRunning = true;
    gamePaused = false;
    initBricks();
}

// Update game state
void updateGame() {
    if (!gameRunning || gamePaused) return;

    ballSpeedMultiplier += 0.001f;
    ballX += ballSpeedX * ballSpeedMultiplier;
    ballY += ballSpeedY * ballSpeedMultiplier;

    // Ball collision with walls
    if (ballX - BALL_RADIUS <= 0 || ballX + BALL_RADIUS >= screenWidth) {
        ballSpeedX = -ballSpeedX;
    }
    if (ballY + BALL_RADIUS >= screenHeight) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with paddle
    if (ballY - BALL_RADIUS <= PADDLE_HEIGHT && ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }
    
    // Ball collision with bricks
    for (auto& brick : bricks) {
        if (brick.active && ballX + BALL_RADIUS > brick.x && ballX - BALL_RADIUS < brick.x + BRICK_WIDTH &&
            ballY + BALL_RADIUS > brick.y && ballY - BALL_RADIUS < brick.y + BRICK_HEIGHT) {
            brick.active = false;
            ballSpeedY = -ballSpeedY;
            score += 10;
        }
    }

    // Ball out of bounds
    if (ballY - BALL_RADIUS <= 0) {
        lives--;
        if (lives > 0) {
            ballX = screenWidth / 2;
            ballY = screenHeight / 2;
            ballSpeedMultiplier = 1.0f;
        } else {
            gameRunning = false;
            saveScore();
        }
    }
}

// Draw paddle
void drawPaddle() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(paddleX, 10);
    glVertex2f(paddleX + PADDLE_WIDTH, 10);
    glVertex2f(paddleX + PADDLE_WIDTH, 10 + PADDLE_HEIGHT);
    glVertex2f(paddleX, 10 + PADDLE_HEIGHT);
    glEnd();
}

// Draw ball
void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(ballX + cos(angle) * BALL_RADIUS, ballY + sin(angle) * BALL_RADIUS);
    }
    glEnd();
}

// Draw bricks
void drawBricks() {
    for (const auto& brick : bricks) {
        if (brick.active) {
            glColor3f(brick.r, brick.g, brick.b);
            glBegin(GL_QUADS);
            glVertex2f(brick.x, brick.y);
            glVertex2f(brick.x + BRICK_WIDTH, brick.y);
            glVertex2f(brick.x + BRICK_WIDTH, brick.y + BRICK_HEIGHT);
            glVertex2f(brick.x, brick.y + BRICK_HEIGHT);
            glEnd();
        }
    }
}

// Draw the current score and highest score
void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Display current score in the top-right corner
    glRasterPos2f(screenWidth - 200, screenHeight - 30);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Display highest score at the top-center
    int highestScore = getHighestScore();
    std::string highScoreText = "High Score: " + std::to_string(highestScore);
    
    int textWidth = highScoreText.length() * 9;
    float xPosition = (screenWidth - textWidth) / 2.0f;

    glRasterPos2f(xPosition, screenHeight - 30);
    for (char c : highScoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Draw lives
void drawLives() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(10, screenHeight - 30);
    std::string livesText = "Lives: " + std::to_string(lives);
    for (char c : livesText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Display routine
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawPaddle();
    drawBall();
    drawBricks();
    drawLives();
    drawScore();

    glutSwapBuffers();
}

// Save score to file
void saveScore() {
    std::ofstream file("score.txt", std::ios::app);
    if (file.is_open()) {
        file << "Score: " << score << "\n";
        file.close();
    }
}

// Read highest score from file
int getHighestScore() {
    int highestScore = 0;
    std::ifstream file("score.txt");
    std::string line;

    while (std::getline(file, line)) {
        int score;
        if (sscanf(line.c_str(), "Score: %d", &score) == 1) {
            if (score > highestScore) {
                highestScore = score;
            }
        }
    }

    return highestScore;
}

// Mouse input for paddle
void mouseMotion(int x, int y) {
    if (gameRunning && !gamePaused) {
        paddleX = x - PADDLE_WIDTH / 2;
        if (paddleX < 0) paddleX = 0;
        if (paddleX + PADDLE_WIDTH > screenWidth) paddleX = screenWidth - PADDLE_WIDTH;
    }
}

// Keyboard input for other actions
void keyboardNormal(unsigned char key, int x, int y) {
    // Pause and resume game
    if (key == 27) {  // ESC key
        if (gameRunning) {
            gamePaused = !gamePaused;
        }
    }
    // Start the game
    else if (key == 13) {  // Enter key
        if (!gameRunning) {
            startGame();
        }
    }
    // Quit the game
    else if (key == 'q' || key == 'Q') {
        exit(0);
    }
}

// Timer function
void timer(int value) {
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

// Initialization
void setup() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
    glMatrixMode(GL_MODELVIEW);

    srand(static_cast<unsigned int>(time(0)));
    initBricks();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Breakout Game");

    setup();
    glutDisplayFunc(display);
    glutTimerFunc(1000 / FPS, timer, 0);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboardNormal);

    glutMainLoop();
    return 0;
}
