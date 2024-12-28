
# Breakout Game

A simple 2D Breakout game implemented using OpenGL and FreeGLUT. The goal of the game is to control a paddle and break all the bricks by bouncing a ball off them. The game includes features such as a score system, multiple lives, and dynamic brick placement. The game can be paused and resumed using the keyboard.

## Features

- **Breakout Gameplay**: Control the paddle and break all the bricks.
- **Dynamic Brick Layout**: Bricks are generated dynamically based on the screen size.
- **Scoring**: Keep track of your score and the highest score.
- **Lives**: You start with three lives, and you lose one each time the ball falls off the screen.
- **Paddle Control**: Use the mouse to move the paddle.
- **Pause/Resume**: Pause and resume the game with the ESC key.
- **Start/Restart**: Start a new game by pressing Enter.
- **Quit**: Quit the game by pressing the 'Q' or 'q' key.

## Requirements

- **OpenGL**: The game uses OpenGL for rendering.
- **FreeGLUT**: The game uses FreeGLUT for window management and input handling.

To build the game, you need to have the following installed:
- [OpenGL](https://www.opengl.org/)
- [FreeGLUT](http://freeglut.sourceforge.net/)
- A C++ compiler (such as GCC or MSVC)

## Building the Game

1. Clone or download the repository.
2. Ensure that you have OpenGL and FreeGLUT installed on your system.
3. Compile the game with the following command (you may need to link OpenGL and FreeGLUT libraries):
   ```bash
   g++ -o breakout breakout.cpp -lGL -lGLU -lglut
   ```
4. Run the executable:
   ```bash
   ./breakout
   ```

## Game Controls

- **Mouse**: Move the paddle left and right.
- **ESC**: Pause or resume the game.
- **Enter**: Start a new game or restart after game over.
- **Q/q**: Quit the game.

## Scoring and High Scores

- **Score**: You gain points by breaking bricks. Each brick is worth 10 points.
- **Lives**: You start with 3 lives. Losing all lives will end the game.
- **High Score**: The highest score is saved in a file called `score.txt` and is displayed on the screen.

## File Structure

- `breakout.cpp`: Main source file for the game logic and rendering.
- `score.txt`: Stores the highest score (appended after each game).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
