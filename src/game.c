/*******************************************************************************************
*
*   INSTRUCTIONS
*
*   In your main programming assignment for this semester you will create a quiz game
*   application in C in groups of threes (3). 
*   The submission date for this coursework is Friday 3rd May 2022 no later than 8pm. 
*   
*   Submission:
*   By the date/time specified above, you should upload a zip file. The name of the zip 
*   file must be in the form:
*   cse1201-groupname-project.zip
*   e.g. cse1201-group2-project.zip
*   The zip file must contain your source code, documentation and any resource files 
*   needed. 
*   Failure to follow the submission specifications, by providing non-needed files or not 
*   following the naming conventions and .zip structure specified, may result in penalised 
*   marks. 
*   Plagiarism cases will be handled as documented in the University regulations.
*   
*   Project Concept Specifications
*   You are to produce a quiz game application in C which is based on a simple question answer-based project. The quiz game application will enable users to answer a set of 
*   questions and get a score for each correct answer. The game should provide users with 
*   questions from various topics such as politics, history, geography, current affairs, 
*   sports, movies, arts, for example. A Guyanese flavoured game would be a nice touch.
*   The quiz game application is expected to maintain a leader board and keep track of an 
*   individual player’s five highest scores or most recent scores. 
*   You can enhance the game by adding rules, providing bonus questions, requiring a 
*   fixed time to answer each question, lifelines, different difficulty levels, etc. 
*   
*   Required features: 
*   - Minimum of 5 questions per game or round
*     - Questions can be all from one topic or randomly chosen from the 
*       available topics
*     - A minimum of 5 questions should be available for about five topic 
*       areas. 
*   - Informs user of correct or incorrect answer
*   - Keeps track of users individual scores (up to 5 scores, most recent or highest)
*   - Maintains a Leader board of 5-10 highest scores
*   
*   Optional features: 
*   - Bonus questions with extra points
*   - Timed answer
*     - only providing a specific amount of time to answer 1 question or for a 
*       round. For example, 5 secs for each question or 30 seconds for a round 
*       or game. 
*   - Lifelines
*     - Allowing a specific amount of incorrect answers, each incorrect answer 
*       costs one life. So if all lives used, game ends.
*     - Alternatively, just keep track of total correct answers. 
*   - Different difficulty levels
*   - Other
*
********************************************************************************************/

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "maze.h"
#include <direct.h>
#include <stdio.h>

#define MAZE_WIDTH 1600
#define MAZE_HEIGHT 800
#define CELL_SIZE 40
#define PLAYER_SIZE 20
#define NUM_MENU_OPTIONS 4
Rectangle menuOptionRecs[NUM_MENU_OPTIONS] = {
    (Rectangle){ 550, 335, 500, 50 },
    (Rectangle){ 550, 395, 500, 50 },
    (Rectangle){ 550, 455, 500, 50 },
    (Rectangle){ 550, 515, 500, 50 }
};

Rectangle intromenuOptionRecs[NUM_MENU_OPTIONS] = {
    (Rectangle){ 550, 135, 500, 100 },
    (Rectangle){ 550, 285, 500, 100 },
    (Rectangle){ 550, 435, 500, 100 },
    (Rectangle){ 550, 585, 500, 100 }
};

int (*levels[])[MAZE_LEVEL_HEIGHT][MAZE_LEVEL_WIDTH] = {&level_1, &level_2, &level_3, &level_4, &level_5, &level_6, &level_7, &level_8, &level_9, &level_10, &level_11, &level_12, &level_13, &level_14, &level_15, &level_16, &level_17, &level_18, &level_19, &level_20};
int current_level = 16;
int game_state = 0; // 0 = intro, 1 = playing, 2 = instructions, 3 = win, 4 = asking_question, 5 = instructions, 6 = leaderboard, 7 = settings, 8 = help 9 = next level
int question_number = 0;
int lives = 5;
int points = 0;
int IsClickedOption;
int IsHoveredOption;

// Define the player structure
typedef struct Player {
    Vector2 position;
    float speed;
} Player;


// Function to check collision between player and walls
bool CheckCollisionWithWalls(Vector2 position) {

    // Calculate the vertices of the player's bounding box
    int topX = position.x;
    int topY = position.y;
    int bottomX = position.x + PLAYER_SIZE;
    int bottomY = position.y + PLAYER_SIZE;

    // Check each corner of the player's bounding box for collision
    bool topLeftCollision = (*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] == 1;
    bool topRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] == 1;
    bool bottomLeftCollision = (*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] == 1;
    bool bottomRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] == 1;

    // If any corner collides with a wall, return true
    return topLeftCollision || topRightCollision || bottomLeftCollision || bottomRightCollision;
}

void CheckCollisionWithQuestions(Vector2 position) {

    // Calculate the vertices of the player's bounding box
    int topX = position.x;
    int topY = position.y;
    int bottomX = position.x + PLAYER_SIZE;
    int bottomY = position.y + PLAYER_SIZE;

    // Check each corner of the player's bounding box for collision
    bool topLeftCollision = (*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] == 2;
    bool topRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] == 2;
    bool bottomLeftCollision = (*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] == 2;
    bool bottomRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] == 2;

    // Increment question number if any corner collides with a question and set level map to 10 to prevent object from being rendered
    if(topLeftCollision || topRightCollision || bottomLeftCollision || bottomRightCollision){
        game_state = 4;
        question_number ++;
        // remove question from map
        if(topLeftCollision){(*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] = 10;}
        if(topRightCollision){(*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] = 10;}
        if(bottomLeftCollision){(*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] = 10;}
        if(bottomRightCollision){(*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] = 10;}
    }
}

bool IsAnswerButtonClicked(int mousex, int mousey, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return ((mousex >= buttonX && mousex <= buttonX + buttonWidth &&
            mousey >= buttonY && mousey <= buttonY + buttonHeight));
}

int GetClickedButton(int mousex, int mousey) {
    // Check if mouse click is within the bounds of each answer button
    if (IsAnswerButtonClicked(mousex, mousey, 500, 335, 500, 50)) {
        // First answer button clicked
        return 1;
    } else if (IsAnswerButtonClicked(mousex, mousey, 500, 395, 500, 50)) {
        // Second answer button clicked
        return 2;
    } else if (IsAnswerButtonClicked(mousex, mousey, 500, 455, 500, 50)) {
        // Third answer button clicked
        return 3;
    } else if (IsAnswerButtonClicked(mousex, mousey, 500, 515, 500, 50)) {
        // Fourth answer button clicked
        return 4;
    }

    // If no button is clicked, return 0 or any other value indicating no button clicked
    return 0;
}

int GetClickedOption(int mousex, int mousey, Rectangle* optionRecs, int numOptions) {
    for (int i = 0; i < numOptions; i++) {
        if (CheckCollisionPointRec((Vector2){ mousex, mousey }, optionRecs[i])) {
            return i;
        }
    }
    return -1; // No option clicked
}

/* Main function */
int main(void)
{
    // Initialization
    InitWindow(MAZE_WIDTH, MAZE_HEIGHT, "Twisted Trails");
    SetTargetFPS(60);
    InitAudioDevice();              // Initialize audio device  

    // Initialize player
    Player player;
    player.position = (Vector2){ 40, 40 };
    player.speed = 5;

    // Load assets
    Texture2D wall_block_texture = LoadTexture("./assets/block.png");
    Texture2D floor_grass_texture = LoadTexture("./assets/g2.png");
    Texture2D player_texture = LoadTexture("./assets/spider.png");
    Texture2D intro_screen = LoadTexture("./assets/intro_screen.png");
    Texture2D incorrect = LoadTexture("./assets/incorrect.png");
    Texture2D correct = LoadTexture("./assets/correct.png");
    Texture2D question_texture = LoadTexture("./assets/question.png");
    Texture2D level_completed = LoadTexture("./assets/level_completed.png");
    Image image = LoadImage("./assets/icon.png"); 
    Font introfont = LoadFont("./assets/font2.ttf");
    Music music = LoadMusicStream("./assets/ambient.ogg");

    // create rectangles
    Rectangle rec12 = {0, 0, 40, 40};
    Rectangle rec8 = {0, 0, 600, 500};
    Rectangle rec10 = {0, 0, 1600, 800};

    // set window icon
    SetWindowIcon(image);  
    // play music stream
    PlayMusicStream(music);

    // init level variables for control flow
    int correct_answers = 0;
    while (!WindowShouldClose())
    {
        // begin playing music
        UpdateMusicStream(music);      // Update music buffer with new stream data

        // start drawing
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Update player position if state is playing
        if (game_state == 1)
        {
            if ((IsKeyDown(KEY_RIGHT) && !CheckCollisionWithWalls((Vector2){ player.position.x + player.speed, player.position.y })) || (IsKeyDown(KEY_RIGHT) && (((player.position.x + 40) > 1600) || ((player.position.x - 40) < 0)) /*&& (correct_answers == 5)*/))
            {
                player.position.x += player.speed;
            }
            if ((IsKeyDown(KEY_LEFT) && !CheckCollisionWithWalls((Vector2){ player.position.x - player.speed, player.position.y })) || (IsKeyDown(KEY_LEFT) && (((player.position.x + 40) > 1600) || ((player.position.x - 40) < 0)) /*&& (correct_answers == 5)*/))
            {
                player.position.x -= player.speed;
            }
            if ((IsKeyDown(KEY_DOWN) && !CheckCollisionWithWalls((Vector2){ player.position.x, player.position.y + player.speed })) || (IsKeyDown(KEY_DOWN) && (((player.position.y + 40) > 800) || ((player.position.y - 40) < 0)) /*&& (correct_answers == 5)*/))
            {
                player.position.y += player.speed;
            }
            if ((IsKeyDown(KEY_UP) && !CheckCollisionWithWalls((Vector2){ player.position.x, player.position.y - player.speed })) || (IsKeyDown(KEY_UP) && (((player.position.y + 40) > 800) || ((player.position.y - 40) < 0)) /*&& (correct_answers == 5)*/))
            {
                player.position.y -= player.speed;
            }
        }
        

        // Check if player collides with a question
        CheckCollisionWithQuestions((Vector2){ player.position.x, player.position.y});

        if(game_state == 0){
            // Main Screen
            // Draw intro screen
            DrawTextureRec(intro_screen, rec10, (Vector2){ 0, 0 }, RAYWHITE);

            // create text for menu options
            char *menuOptionText[4] = { "Play", "Instructions", "Leaderboard", "Settings" };

            // Draw menu options
            for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
                DrawRectangleRec(intromenuOptionRecs[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                DrawRectangleLines(intromenuOptionRecs[i].x, intromenuOptionRecs[i].y, intromenuOptionRecs[i].width, intromenuOptionRecs[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                // DrawText(menuOptionText[i], (int)(intromenuOptionRecs[i].x + intromenuOptionRecs[i].width / 2 - MeasureText(menuOptionText[i], 50) / 2), (int)intromenuOptionRecs[i].y + 19, 50, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : DARKGRAY);
                DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(intromenuOptionRecs[i].x + intromenuOptionRecs[i].width / 2 - MeasureText(menuOptionText[i], 50) / 2), (int)intromenuOptionRecs[i].y + 19}, 50, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
            }


            // Check for mouse interaction with menu options
            int mousex = GetMouseX();
            int mousey = GetMouseY();
            IsClickedOption = GetClickedOption(mousex, mousey, intromenuOptionRecs, NUM_MENU_OPTIONS);
            IsHoveredOption = -1; // Reset hovered option

            // Handle menu option click events
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                // Perform action based on clicked option
                switch (IsClickedOption) {
                    case 0:
                        game_state = 1;
                        break;
                    case 1:
                        game_state = 2;
                        break;
                    case 2:
                        game_state = 6;
                        break;
                    case 3:
                        game_state = 7;
                        break;
                }
            };
        }

        // Instructions screen
        if(game_state == 2){
            // Draw intruction screen
            DrawTextureRec(intro_screen, rec10, (Vector2){ 0, 0 }, RAYWHITE);

            // create text for menu options
            char *menuOptionText[1] = { "Back"};

            Rectangle back_rec[1] = {(Rectangle){ 1380, 735, 200, 60 }};
            
            // Draw intructions and back button
            for (int i = 0; i < 1; i++) {
                DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 50) / 2), (int)back_rec[i].y + 7}, 50, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
            }

            // Check for mouse interaction with menu options
            int mousex = GetMouseX();
            int mousey = GetMouseY();
            IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 1);
            IsHoveredOption = -1; // Reset hovered option

            // Handle menu option click events
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                // return to main menu
                game_state = 0;
                
            };
        }

        if(game_state == 6){
            // Draw leaderboard screen
            DrawTextureRec(intro_screen, rec10, (Vector2){ 0, 0 }, RAYWHITE);

            // create text for menu options
            char *menuOptionText[1] = { "Back" };

            Rectangle back_rec[1] = {(Rectangle){ 1380, 735, 200, 60 }};
            
            // Draw intructions and back button
            for (int i = 0; i < 1; i++) {
                DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 50) / 2), (int)back_rec[i].y + 7}, 50, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
            }

            // Check for mouse interaction with menu options
            int mousex = GetMouseX();
            int mousey = GetMouseY();
            IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 1);
            IsHoveredOption = -1; // Reset hovered option

            // Handle menu option click events
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                // return to main menu
                game_state = 0;
                
            };
        }

        if(game_state == 7){
            // Draw settings screen
            DrawTextureRec(intro_screen, rec10, (Vector2){ 0, 0 }, RAYWHITE);

            // create text for menu options
            char *menuOptionText[1] = { "Back" };

            Rectangle back_rec[1] = {(Rectangle){ 1380, 735, 200, 60 }};
            
            // Draw intructions and back button
            for (int i = 0; i < 1; i++) {
                DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 50) / 2), (int)back_rec[i].y + 7}, 50, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
            }

            // Check for mouse interaction with menu options
            int mousex = GetMouseX();
            int mousey = GetMouseY();
            IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 1);
            IsHoveredOption = -1; // Reset hovered option

            // Handle menu option click events
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                // return to main menu
                game_state = 0;
                
            };
        }

        else if(game_state == 1 || game_state == 4 || game_state == 5 || game_state == 3 || game_state == 9){
            // redirect to game completed once level is completed. level is completed only if the player x and y are greater than the size of the maze
            if (player.position.x > MAZE_WIDTH || player.position.y > MAZE_HEIGHT || player.position.x < 0 || player.position.y < 0) {
                // show level completed screen
                game_state = 9;
            }

            // Draw maze
            for (int x = 0; x < MAZE_LEVEL_HEIGHT; x++)
            {
                for (int y = 0; y < MAZE_LEVEL_WIDTH; y++)
                {
                    // Create walls
                    if ((*levels[current_level])[x][y] == 1)
                    {
                        DrawTextureRec(wall_block_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                    }
                    else if (((*levels[current_level])[x][y] == 0) || ((*levels[current_level])[x][y] == 10))
                    {
                        DrawTextureRec(floor_grass_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                    }

                    // Create Question
                    if ((*levels[current_level])[x][y] == 2)
                    {
                        DrawTextureRec(wall_block_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                        DrawTextureRec(question_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                    }

                    // Create removable blocks
                    if ((*levels[current_level])[x][y] == 3)
                    {
                        if(correct_answers == 5){
                            // Draw wall block
                            DrawTextureRec(floor_grass_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                        }else{
                            // Draw floor block
                            DrawTextureRec(wall_block_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
                        }
                        
                    }
                }
            }
            
            // Draw player
            DrawRectangle(player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE, RED);
            // DrawTextureRec(player_texture, rec12, (Vector2){ player.position.x, player.position.y }, RAYWHITE);
            if(game_state == 4){
                // define menu options
                char *menuOptionText[4] = { level1_questions.level_questions[question_number-1].option1, level1_questions.level_questions[question_number-1].option2, level1_questions.level_questions[question_number-1].option3, level1_questions.level_questions[question_number-1].option4 };

                // draw question stuff outline
                DrawRectangle(500, 100, 600, 500, LIGHTGRAY);
                DrawText(TextFormat("Question %d", question_number), 520, 120, 40, BLACK);

                // Draw question
                DrawText(TextFormat(level1_questions.level_questions[question_number-1].line1, question_number), 520, 230, 30, BLACK);
                //DrawRectangleLines(500, 230, 600, 80, GRAY); Removing cause it looks weird gotta fix or find a better look before submission guys

                // Draw menu options
                for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
                    DrawRectangleRec(menuOptionRecs[i], (IsClickedOption == i || IsHoveredOption == i) ? SKYBLUE : LIGHTGRAY);
                    DrawRectangleLines(menuOptionRecs[i].x, menuOptionRecs[i].y, menuOptionRecs[i].width, menuOptionRecs[i].height, (IsClickedOption == i || IsHoveredOption == i) ? BLUE : GRAY);
                    DrawText(menuOptionText[i], (int)(menuOptionRecs[i].x + menuOptionRecs[i].width / 2 - MeasureText(menuOptionText[i], 25) / 2), (int)menuOptionRecs[i].y + 11, 25, (IsClickedOption == i || IsHoveredOption == i) ? DARKBLUE : DARKGRAY);
                }


                // Check for mouse interaction with menu options
                int mousex = GetMouseX();
                int mousey = GetMouseY();
                IsClickedOption = GetClickedOption(mousex, mousey, menuOptionRecs, NUM_MENU_OPTIONS);
                IsHoveredOption = -1; // Reset hovered option

                // Handle menu option click events
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                    // Perform action based on clicked option
                    printf("Clicked option %d\n", IsClickedOption);
                    if(IsClickedOption+1 == level1_questions.level_questions[question_number-1].answer){
                        // person selected the correct answer or cheated cause some of these questions tuff
                        game_state = 3;
                        points++;
                        correct_answers++;

                        // play some sound here
                        // could display a correct sign on screen here also
                    }else{
                        // person selected the wrong answer
                        lives--;
                        
                        game_state = 5;
                    }
                }
                    
            }

            if (game_state == 5) {
                // draw incorrect answer and wait for user to click retry button
                DrawTextureRec(incorrect, rec8, (Vector2){ 500, 100 }, RAYWHITE);
                
                // create text for menu options
                char *menuOptionText[1] = { "Retry" };

                Rectangle back_rec[1] = {(Rectangle){ 530, 550, 150, 40 }};
                
                // Draw intructions and back button
                for (int i = 0; i < 1; i++) {
                    DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                    DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                    DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 30) / 2), (int)back_rec[i].y + 7}, 30, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
                }

                // Check for mouse interaction with menu options
                int mousex = GetMouseX();
                int mousey = GetMouseY();
                IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 1);
                IsHoveredOption = -1; // Reset hovered option

                // Handle menu option click events
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                    // return to main menu
                    game_state = 4;
                    
                };
            }

            if (game_state == 3) {
                // draw incorrect answer and wait for user to click retry button
                DrawTextureRec(correct, rec8, (Vector2){ 500, 100 }, RAYWHITE);
                
                // create text for menu options
                char *menuOptionText[1] = { "Continue" };

                Rectangle back_rec[1] = {(Rectangle){ 530, 550, 230, 40 }};
                
                // Draw intructions and back button
                for (int i = 0; i < 1; i++) {
                    DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                    DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                    DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 30) / 2)-10, (int)back_rec[i].y + 7}, 30, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
                }

                // Check for mouse interaction with menu options
                int mousex = GetMouseX();
                int mousey = GetMouseY();
                IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 1);
                IsHoveredOption = -1; // Reset hovered option

                // Handle menu option click events
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                    // return to main menu
                    game_state = 1;
                    
                };
            }

            if (game_state == 9) {
                // draw incorrect answer and wait for user to click retry button
                DrawTextureRec(level_completed, rec8, (Vector2){ 500, 100 }, RAYWHITE);
                
                // create text for menu options
                char *menuOptionText[2] = { "Continue" , "Main Menu" };

                Rectangle back_rec[2] = {(Rectangle){ 560, 550, 230, 40 }, (Rectangle){ 810, 550, 230, 40 }};
                
                // Draw intructions and back button
                for (int i = 0; i < 2; i++) {
                    DrawRectangleRec(back_rec[i], (IsClickedOption == i || IsHoveredOption == i) ? DARKGRAY : YELLOW);
                    DrawRectangleLines(back_rec[i].x, back_rec[i].y, back_rec[i].width, back_rec[i].height, (IsClickedOption == i || IsHoveredOption == i) ? DARKBROWN : BEIGE);
                    DrawTextEx(introfont, menuOptionText[i], (Vector2){(int)(back_rec[i].x + back_rec[i].width / 2 - MeasureText(menuOptionText[i], 30) / 2)-10, (int)back_rec[i].y + 7}, 30, 5, (IsClickedOption == i || IsHoveredOption == i) ? YELLOW : BLACK);
                }

                // Check for mouse interaction with menu options
                int mousex = GetMouseX();
                int mousey = GetMouseY();
                IsClickedOption = GetClickedOption(mousex, mousey, back_rec, 2);
                IsHoveredOption = -1; // Reset hovered option

                // Handle menu option click events
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
                    // return to main menu
                    switch (IsClickedOption) {
                        case 0:
                            ClearBackground(RAYWHITE);
                            game_state = 1;
                            if(current_level >= 19){
                                current_level = 0;
                            }else {
                                current_level++;
                            }                            
                            correct_answers = 4;
                            question_number = 0;
                            player.position.x = 40;
                            player.position.y = 40;
                            break;
                        default:
                            ClearBackground(RAYWHITE);
                            game_state = 0;
                            if(current_level >= 19){
                                current_level = 0;
                            }else {
                                current_level++;
                            }
                            correct_answers = 4;
                            question_number = 0;
                            player.position.x = 40;
                            player.position.y = 40;
                            break;
                    }                    
                };
            }
        }
        
        EndDrawing();
    }

    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    CloseWindow();
    return 0;
}