#include <iostream>
#include <ctime>
#include <conio.h>      // For capturing user key presses
#include <cstdlib>
#include <thread>       // For the sleep function (used in Sleep())
#include <ctime>
#include <windows.h>    // For system calls and key input functions
using namespace std;

// Game board (12x18 grid)
char ar1[12][18] = {
    {' ','_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_' , '_','_', '_' , '_',' '},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ',' ', ' ' , ' ','|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ',' ', ' ' , ' ','|'},        
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ',' ', ' ' , ' ','|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ',' ', ' ' , ' ',' ', ' ' , ' ', '|'},
    {' ','"', '"', '"', '"', '"', '"', '"', '"', '"', '"', '"', '"' , '"','"', '"' , '"', ' '},
};

// Structure to hold snake's position and previous position
struct snake {
    int row;     // Current row
    int col;     // Current column
    int pr_row;  // Previous row
    int pr_col;  // Previous column
    // Constructor for initializing positions
    snake(int r = 0, int c = 0, int pr = 0, int pc = 0)
        : row(r), col(c), pr_row(pr), pr_col(pc) {}
};

// Pointer to hold the snake parts
snake* snake_parts3 = new snake[1];

class game {
public:
    bool stop = true;    // Flag to stop the game
    int counter = 0;     // Counter to track the length of the snake

    // Check if the snake collides with walls or itself
    bool check_lose(snake parts[], int size, int row, int col) {
        if (ar1[row][col] == '@') { // If snake collides with its own body
            for (int i = 1; i < size; i++) {
                if (parts[i].col == col && parts[i].row == row) {
                    stop = false;
                    cout << "         You Ate Your Tail" << '\n';  // Game over message
                    return stop;
                }
            }
        }
        
        // Check if snake hits a wall (either the boundary or a part of the frame)
        int row_saver = parts[0].row;
        int col_saver = parts[0].col;
        parts[0].row = row;
        parts[0].col = col;
        if (ar1[parts[0].row][parts[0].col] == '"' || ar1[parts[0].row][parts[0].col] == '|' || ar1[parts[0].row][parts[0].col] == '_') {
            stop = false;
            cout << "         You Hit The Wall" << '\n';  // Game over message
        }
        parts[0].row = row_saver;
        parts[0].col = col_saver;
        return stop;
    }

    // Function to move the snake and adjust its body parts
    void reast_snake(snake parts[], int size, int* size2) {
        for (int i = 1; i < size; i++) {
            if (ar1[parts[i - 1].pr_row][parts[i - 1].pr_col] == '@') {
                break;  // Stop if snake collides with its own tail
            }
            if (i == size - 1) {  // If it's the last part of the snake, move it to the previous part's location
                snake_parts3[0] = parts[i];
                ar1[parts[i].row][parts[i].col] = ' ';
                parts[i].pr_col = parts[i].col;
                parts[i].pr_row = parts[i].row;
                parts[i].col = parts[i - 1].pr_col;
                parts[i].row = parts[i - 1].pr_row;
                ar1[parts[i].row][parts[i].col] = '@';
            } else {  // Move each snake part to the previous part's position
                ar1[parts[i].row][parts[i].col] = ' ';
                parts[i].pr_col = parts[i].col;
                parts[i].pr_row = parts[i].row;
                parts[i].col = parts[i - 1].pr_col;
                parts[i].row = parts[i - 1].pr_row;
                ar1[parts[i].row][parts[i].col] = '@';
            }
        }
    }

    // Print the game platform (board) to the console
    void print_platform() {
        system("cls");  // Clear screen
        cout << "******* Press ESC to Exit ********" << '\n';
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 18; j++) {
                cout << ar1[i][j];  // Print the game board
            }
            cout << '\n';
        }
        cout << "Snake Added Length: " << counter << '\n';
        cout << "Snake Full Length: " << counter + 3 << '\n'; // Display the full length of the snake
        cout << "Take (*) To Make Your Snake Taller" << '\n';
        cout << '\n';
    }

    // Generate a new food (represented by `*`) at a random location
    void random_snake(bool* food) {
        if (*food) {
            while (true) {
                srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
                int row = rand() % 10 + 1;  // Random row position (1 to 10)
                int col = rand() % 16 + 1;  // Random column position (1 to 16)
                if (ar1[row][col] == ' ') {  // If the position is empty
                    ar1[row][col] = '*';  // Place the food
                    *food = false;  // Food is no longer available
                    break;
                }
            }
        }
    }

    // Function to handle movement of the snake based on user input (Arrow keys)
    void run(snake parts[], int number, int size, int* i, int* size2, bool* food, int save, int* i2) {
        // This method handles the snake's movement and checks for collisions or food consumption.
    
        // Check if the direction is UP (72) and if the save variable is 0 or opposite of the current move direction (e.g., UP followed by DOWN)
        if(number == 72 && save == 0 || save == 72 && number == 0){
            // Check if moving upwards leads to a collision or hitting the wall.
            if(!check_lose(parts , size, (parts[0].row) - 1, parts[0].col )) {
                return;
            }
            
            // Check if the new position has food (indicated by "*").
            if(ar1[(parts[0].row) - 1][parts[0].col] == '*'){
                counter++;  // Increase score when food is eaten.
                *size2 += 1;  // Increase snake size.
                *food = true;  // Mark that the food has been eaten.
            }
    
            // Update the snake's head position.
            parts[0].pr_row = parts[0].row;
            parts[0].pr_col = parts[0].col;
            ar1[parts[0].row][parts[0].col] = ' ';  // Clear the current position of the head.
            parts[0].row--;  // Move the head upwards.
            
            if(parts[0].row < 1){  // Prevent the head from moving off the top of the screen.
                parts[0].row = 1;
            }
            ar1[parts[0].row][parts[0].col] = '@';  // Place the head in the new position.
            
            // Update the rest of the snake's body based on the new head position.
            reast_snake(parts, size, size2);
            
            // Every 3 steps or after 30 iterations, place a new food item.
            if(*i == 3 || *i2 == 30) {
                random_snake(food);
                *i = 0;
                *i2 = 0;
            }
            
            // Print the updated game board.
            print_platform();
        }
    
        // Check if the direction is DOWN (80) and if the save variable is 0 or opposite of the current move direction.
        if(number == 80 && save == 0 || save == 80 && number == 0){
            // Check if moving down leads to a collision or hitting the wall.
            if(!check_lose(parts , size, (parts[0].row) + 1, parts[0].col )) {
                return;
            }
    
            // Check if the new position has food.
            if(ar1[(parts[0].row) + 1][parts[0].col] == '*'){
                counter++;  // Increase score when food is eaten.
                *size2 += 1;  // Increase snake size.
                *food = true;  // Mark that the food has been eaten.
            }
    
            // Update the snake's head position.
            parts[0].pr_row = parts[0].row;
            parts[0].pr_col = parts[0].col;
            ar1[parts[0].row][parts[0].col] = ' ';  // Clear the current position of the head.
            parts[0].row++;  // Move the head downwards.
    
            if(parts[0].row > 10){  // Prevent the head from moving off the bottom of the screen.
                parts[0].row = 10;
            }
            ar1[parts[0].row][parts[0].col] = '@';  // Place the head in the new position.
    
            // Update the rest of the snake's body based on the new head position.
            reast_snake(parts , size , size2);
    
            // Every 3 steps or after 30 iterations, place a new food item.
            if(*i == 3 || *i2 == 30) {
                random_snake(food);
                *i = 0;
                *i2 = 0;
            }
    
            // Print the updated game board.
            print_platform();
        }
    
        // Check if the direction is RIGHT (77) and if the save variable is 0 or opposite of the current move direction.
        if(number == 77 && save == 0 || save == 77 && number == 0){
            // Check if moving right leads to a collision or hitting the wall.
            if(!check_lose(parts , size, (parts[0].row) , parts[0].col + 1 )) {
                return;
            }
    
            // Check if the new position has food.
            if(ar1[(parts[0].row)][parts[0].col + 1] == '*'){
                counter++;  // Increase score when food is eaten.
                *size2 += 1;  // Increase snake size.
                *food = true;  // Mark that the food has been eaten.
            }
    
            // Update the snake's head position.
            parts[0].pr_col = parts[0].col;
            parts[0].pr_row = parts[0].row;
            ar1[parts[0].row][parts[0].col] = ' ';  // Clear the current position of the head.
            parts[0].col++;  // Move the head to the right.
    
            if(parts[0].col > 16){  // Prevent the head from moving off the right side of the screen.
                parts[0].col = 16;
            }
            ar1[parts[0].row][parts[0].col] = '@';  // Place the head in the new position.
    
            // Update the rest of the snake's body based on the new head position.
            reast_snake(parts , size, size2);
    
            // Every 3 steps or after 30 iterations, place a new food item.
            if(*i == 3 || *i2 == 30) {
                random_snake(food);
                *i = 0;
                *i2 = 0;
            }
    
            // Print the updated game board.
            print_platform();
        }
    
        // Check if the direction is LEFT (75) and if the save variable is 0 or opposite of the current move direction.
        if(number == 75 && save == 0 || save == 75 && number == 0){
            // Check if moving left leads to a collision or hitting the wall.
            if(!check_lose(parts , size, (parts[0].row) , parts[0].col - 1 )) {
                return;
            }
    
            // Check if the new position has food.
            if(ar1[(parts[0].row)][parts[0].col - 1] == '*'){
                counter++;  // Increase score when food is eaten.
                *size2 += 1;  // Increase snake size.
                *food = true;  // Mark that the food has been eaten.
            }
    
            // Update the snake's head position.
            parts[0].pr_col = parts[0].col;
            parts[0].pr_row = parts[0].row;
            ar1[parts[0].row][parts[0].col] = ' ';  // Clear the current position of the head.
            parts[0].col--;  // Move the head to the left.
    
            if(parts[0].col < 1){  // Prevent the head from moving off the left side of the screen.
                parts[0].col = 1;
            }
            ar1[parts[0].row][parts[0].col] = '@';  // Place the head in the new position.
    
            // Update the rest of the snake's body based on the new head position.
            reast_snake(parts , size, size2);
    
            // Every 3 steps or after 30 iterations, place a new food item.
            if(*i == 3 || *i2 == 30) {
                random_snake(food);
                *i = 0;
                *i2 = 0;
            }
    
            // Print the updated game board.
            print_platform();
        }
    }
    
};

// Resize the snake array when it grows
snake* resizeSnakeArray(snake* oldArray, int oldSize, int newSize) {
    snake* newArray = new snake[newSize];
    for (int i = 0; i < oldSize; ++i) {
        newArray[i] = oldArray[i];  // Copy the old array's data
    }
    delete[] oldArray;  // Free the memory of the old array
    return newArray;  // Return the new array
}

int main(){
    cout << "loading..." << '\n';  // Display a loading message on the screen.
    
    bool food = true;  // Variable to track whether food should be placed.
    int save = 72;  // Variable to store the last direction the snake moved in.
    int i = 0;  // Variable used for tracking the movement steps of the snake.
    int i2 = 0;  // Variable used to track the number of iterations.
    int i3 = 0;  // Another variable used to count the movement steps of the snake.
    
    bool key_released[8] = {false, false, false, false, false, false, false, false};  // An array to track if a key has been released or not.
    
    game main;  // Creating an instance of the game class.
    
    int size2 = 3;  // Initial snake size.
    int size3 = 3;  // Variable to track the snake's size as it grows.
    
    // Dynamically allocating memory for snake parts with initial size of 3.
    snake* snake_parts = new snake[3];
    
    // Initializing snake parts.
    snake_parts[0] = snake(7, 7, 7, 7);  
    snake_parts[1] = snake(8, 7, 7, 7);
    snake_parts[2] = snake(9, 7, 8, 7);
    
    while (main.stop) {  // The game loop that runs while the game isn't stopped.
        Sleep(50);  // Adds a small delay for smoother gameplay.
        
        // If snake size increases, adjust the snake array size accordingly.
        if (size3 > size2) {
            for (int i = 0; i < size3; i++) {
                if (i < size2) {
                    continue;  // Skip the part if the size is already updated.
                } else {
                    snake_parts = resizeSnakeArray(snake_parts, size2, size3);  // Resize the snake array.
                    snake_parts[i] = snake_parts3[0];  // Assign the new snake part.
                }
            }
            ar1[snake_parts[size2].row][snake_parts[size2].col] = '@';  // Place new snake part on the game grid.
            size2 = size3;  // Update the current size of the snake.
            Sleep(10);  // Delay for smoother gameplay.
            main.print_platform();  // Print the game platform with the updated snake position.
        }
        
        if (GetAsyncKeyState(VK_ESCAPE)) {  // Check if the Escape key is pressed to exit the game.
            cout << "            You Exited" << '\n';  // Display message when player exits.
            break;  // Break the loop and exit the game.
        }

        // Check for key presses to move the snake and avoid multiple movements in the same direction.
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !key_released[1] && !key_released[2] && !key_released[3] && !key_released[4]) {
            i = 2;  // Set movement direction for right.
            i3++;  // Increment the step counter.
            key_released[5] = true;  // Mark the right key as released.
            key_released[0] = true;  // Prevent other conflicting keys from being active.
            key_released[6] = false;  // Allow down key to be pressed.
            key_released[7] = false;  // Allow up key to be pressed.
            main.run(snake_parts, 77, size2, &i2, &size3, &food, 0, &i3);  // Call the game run function for right movement.
            save = 77;  // Update the last movement direction to right.
        } else if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000) && key_released[0]) {  // If the right key is released.
            i = 2;  // Reset the movement counter.
            key_released[0] = false;  // Mark right key as no longer pressed.
        }

        // Repeat similar checks for left, up, and down movements.
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !key_released[0] && !key_released[2] && !key_released[3] && !key_released[5]) {
            i = 2;
            i3++;
            key_released[4] = true;
            key_released[1] = true;
            key_released[6] = false;
            key_released[7] = false;
            main.run(snake_parts, 75, size2, &i2, &size3, &food, 0, &i3);
            save = 75;
        } else if (!(GetAsyncKeyState(VK_LEFT) & 0x8000) && key_released[1]) {
            i = 2;
            key_released[1] = false;
        }
        
        else if (GetAsyncKeyState(VK_UP) & 0x8000 && !key_released[0] && !key_released[1] && !key_released[3] && !key_released[6]) {
            i = 2;
            i3++;
            key_released[4] = false;
            key_released[5] = false;
            key_released[7] = true;
            key_released[2] = true;
            main.run(snake_parts, 72, size2, &i2, &size3, &food, 0, &i3);
            save = 72;
        } else if (!(GetAsyncKeyState(VK_UP) & 0x8000) && key_released[2]) {
            i = 2;
            key_released[2] = false;
        }
        
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !key_released[0] && !key_released[1] && !key_released[2] && !key_released[7]) {
            i = 2;
            i3++;
            key_released[4] = false;
            key_released[5] = false;
            key_released[3] = true;
            key_released[6] = true;
            main.run(snake_parts, 80, size2, &i2, &size3, &food, 0, &i3);
            save = 80;
        } else if (!(GetAsyncKeyState(VK_DOWN) & 0x8000) && key_released[3]) {
            i = 2;
            key_released[3] = false;
        }
        
        else if (i == 10) {  // Check if it's time for snake to move and grow.
            main.run(snake_parts, 0, size2, &i2, &size3, &food, save, &i3);
            i2++;  // Increment iteration counter.
            i = 0;  // Reset step counter.
        }

        i++;  // Increment step counter to keep the movement going.
    }

    cout << "      Thank You For Playing" << '\n';  // Display thank you message after the game ends.
    cout << "**********************************" << '\n';  // Print a separator line.

    // Clean up dynamically allocated memory.
    delete[] snake_parts;  
    delete[] snake_parts3;  

    Sleep(3000);  // Pause before the program ends.
    return 0;  // Return from main function.
}
