// /*******************************************************************************************
// *
// *   INSTRUCTIONS
// *
// *   In your main programming assignment for this semester you will create a quiz game
// *   application in C in groups of threes (3). 
// *   The submission date for this coursework is Friday 3rd May 2022 no later than 8pm. 
// *   
// *   Submission:
// *   By the date/time specified above, you should upload a zip file. The name of the zip 
// *   file must be in the form:
// *   cse1201-groupname-project.zip
// *   e.g. cse1201-group2-project.zip
// *   The zip file must contain your source code, documentation and any resource files 
// *   needed. 
// *   Failure to follow the submission specifications, by providing non-needed files or not 
// *   following the naming conventions and .zip structure specified, may result in penalised 
// *   marks. 
// *   Plagiarism cases will be handled as documented in the University regulations.
// *   
// *   Project Concept Specifications
// *   You are to produce a quiz game application in C which is based on a simple question answer-based project. The quiz game application will enable users to answer a set of 
// *   questions and get a score for each correct answer. The game should provide users with 
// *   questions from various topics such as politics, history, geography, current affairs, 
// *   sports, movies, arts, for example. A Guyanese flavoured game would be a nice touch.
// *   The quiz game application is expected to maintain a leader board and keep track of an 
// *   individual player’s five highest scores or most recent scores. 
// *   You can enhance the game by adding rules, providing bonus questions, requiring a 
// *   fixed time to answer each question, lifelines, different difficulty levels, etc. 
// *   
// *   Required features: 
// *   - Minimum of 5 questions per game or round
// *     - Questions can be all from one topic or randomly chosen from the 
// *       available topics
// *     - A minimum of 5 questions should be available for about five topic 
// *       areas. 
// *   - Informs user of correct or incorrect answer
// *   - Keeps track of users individual scores (up to 5 scores, most recent or highest)
// *   - Maintains a Leader board of 5-10 highest scores
// *   
// *   Optional features: 
// *   - Bonus questions with extra points
// *   - Timed answer
// *     - only providing a specific amount of time to answer 1 question or for a 
// *       round. For example, 5 secs for each question or 30 seconds for a round 
// *       or game. 
// *   - Lifelines
// *     - Allowing a specific amount of incorrect answers, each incorrect answer 
// *       costs one life. So if all lives used, game ends.
// *     - Alternatively, just keep track of total correct answers. 
// *   - Different difficulty levels
// *   - Other
// *
// ********************************************************************************************/

// #include "raylib.h"
// #include "stdio.h"
// #include "stdlib.h"
// #include "string.h"
// #include "maze.h"
// #include <direct.h>
// #include <stdio.h>

// #define MAZE_WIDTH 1600
// #define MAZE_HEIGHT 800
// #define CELL_SIZE 40
// #define PLAYER_SIZE 20
// #define NUM_MENU_OPTIONS 4
// Rectangle menuOptionRecs[NUM_MENU_OPTIONS] = {
//     (Rectangle){ 550, 335, 500, 50 },
//     (Rectangle){ 550, 395, 500, 50 },
//     (Rectangle){ 550, 455, 500, 50 },
//     (Rectangle){ 550, 515, 500, 50 }
// };

// int (*levels[])[MAZE_LEVEL_HEIGHT][MAZE_LEVEL_WIDTH] = {&level_1, &level_2, &level_3, &level_4, &level_5, &level_6, &level_7, &level_8, &level_9, &level_10, &level_11, &level_12, &level_13, &level_14, &level_15, &level_16, &level_17, &level_18, &level_19, &level_20};
// int current_level = 0;
// int game_state = 1; // 0 = intro, 1 = playing, 2 = game over, 3 = win, 4 = asking_question, 5 = instructions, 6 = leaderboard, 7 = settings, 8 = help
// int question_number = 0;
// int lives = 5;
// int points = 0;
// int IsClickedOption;
// int IsHoveredOption;

// // Define the player structure
// typedef struct Player {
//     Vector2 position;
//     float speed;
// } Player;


// // Function to check collision between player and walls
// bool CheckCollisionWithWalls(Vector2 position) {
//     int playerCellX = position.x / CELL_SIZE;
//     int playerCellY = position.y / CELL_SIZE;

//     // Calculate the vertices of the player's bounding box
//     int topX = position.x;
//     int topY = position.y;
//     int bottomX = position.x + PLAYER_SIZE;
//     int bottomY = position.y + PLAYER_SIZE;

//     // Check each corner of the player's bounding box for collision
//     bool topLeftCollision = (*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] == 1;
//     bool topRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] == 1;
//     bool bottomLeftCollision = (*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] == 1;
//     bool bottomRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] == 1;

//     // If any corner collides with a wall, return true
//     return topLeftCollision || topRightCollision || bottomLeftCollision || bottomRightCollision;
// }

// void CheckCollisionWithQuestions(Vector2 position) {
//     int playerCellX = position.x / CELL_SIZE;
//     int playerCellY = position.y / CELL_SIZE;

//     // Calculate the vertices of the player's bounding box
//     int topX = position.x;
//     int topY = position.y;
//     int bottomX = position.x + PLAYER_SIZE;
//     int bottomY = position.y + PLAYER_SIZE;

//     // Check each corner of the player's bounding box for collision
//     bool topLeftCollision = (*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] == 2;
//     bool topRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] == 2;
//     bool bottomLeftCollision = (*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] == 2;
//     bool bottomRightCollision = (*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] == 2;

//     // Increment question number if any corner collides with a question and set level map to 10 to prevent object from being rendered
//     if(topLeftCollision || topRightCollision || bottomLeftCollision || bottomRightCollision){
//         game_state = 4;
//         question_number ++;
//         // remove question from map
//         if(topLeftCollision){(*levels[current_level])[topX / CELL_SIZE][topY / CELL_SIZE] = 10;}
//         if(topRightCollision){(*levels[current_level])[bottomX / CELL_SIZE][topY / CELL_SIZE] = 10;}
//         if(bottomLeftCollision){(*levels[current_level])[topX / CELL_SIZE][bottomY / CELL_SIZE] = 10;}
//         if(bottomRightCollision){(*levels[current_level])[bottomX / CELL_SIZE][bottomY / CELL_SIZE] = 10;}
//     }
// }

// bool IsAnswerButtonClicked(int mousex, int mousey, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
//     return ((mousex >= buttonX && mousex <= buttonX + buttonWidth &&
//             mousey >= buttonY && mousey <= buttonY + buttonHeight));
// }

// int GetClickedButton(int mousex, int mousey) {
//     // Check if mouse click is within the bounds of each answer button
//     if (IsAnswerButtonClicked(mousex, mousey, 500, 335, 500, 50)) {
//         // First answer button clicked
//         return 1;
//     } else if (IsAnswerButtonClicked(mousex, mousey, 500, 395, 500, 50)) {
//         // Second answer button clicked
//         return 2;
//     } else if (IsAnswerButtonClicked(mousex, mousey, 500, 455, 500, 50)) {
//         // Third answer button clicked
//         return 3;
//     } else if (IsAnswerButtonClicked(mousex, mousey, 500, 515, 500, 50)) {
//         // Fourth answer button clicked
//         return 4;
//     }

//     // If no button is clicked, return 0 or any other value indicating no button clicked
//     return 0;
// }

// int GetClickedOption(int mousex, int mousey, Rectangle* optionRecs, int numOptions) {
//     for (int i = 0; i < numOptions; i++) {
//         if (CheckCollisionPointRec((Vector2){ mousex, mousey }, optionRecs[i])) {
//             return i;
//         }
//     }
//     return -1; // No option clicked
// }

// /* Main function */
// int main(void)
// {
//     // Initialization
//     InitWindow(MAZE_WIDTH, MAZE_HEIGHT, "Maze Run");
//     SetTargetFPS(60);
//     InitAudioDevice();              // Initialize audio device

//     Music music = LoadMusicStream("./ambient.ogg");

//     PlayMusicStream(music);

//     float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
//     bool pause = false;    

//     // Initialize player
//     Player player;
//     player.position = (Vector2){ 40, 40 };
//     player.speed = 5;

//     // Load images
//     Texture2D wall_block_texture = LoadTexture("./block.png");
//     Texture2D floor_grass_texture = LoadTexture("./g2.png");
//     Texture2D player_texture = LoadTexture("./spider.png");
//     Texture2D lose_screen = LoadTexture("./lose.png");
//     Rectangle rec12 = {0, 0, 40, 40};
//     Rectangle rec7 = {0, 0, 25, 25};
//     Rectangle rec8 = {0, 0, 600, 500};

//     while (!WindowShouldClose())
//     {
//         BeginDrawing();

//         ClearBackground(RAYWHITE);

//         // Update player position if state is playing
//         if (game_state == 1)
//         {
//             if (IsKeyDown(KEY_RIGHT) && !CheckCollisionWithWalls((Vector2){ player.position.x + player.speed, player.position.y }))
//             {
//                 player.position.x += player.speed;
//             }
//             if (IsKeyDown(KEY_LEFT) && !CheckCollisionWithWalls((Vector2){ player.position.x - player.speed, player.position.y }))
//             {
//                 player.position.x -= player.speed;
//             }
//             if (IsKeyDown(KEY_DOWN) && !CheckCollisionWithWalls((Vector2){ player.position.x, player.position.y + player.speed }))
//             {
//                 player.position.y += player.speed;
//             }
//             if (IsKeyDown(KEY_UP) && !CheckCollisionWithWalls((Vector2){ player.position.x, player.position.y - player.speed }))
//             {
//                 player.position.y -= player.speed;
//             }
//         }
        

//         // Check if player collides with a question
//         CheckCollisionWithQuestions((Vector2){ player.position.x, player.position.y});


//         if(game_state == 1 || game_state == 4 || game_state == 5){
//             // Draw maze
//             for (int x = 0; x < MAZE_LEVEL_HEIGHT; x++)
//             {
//                 for (int y = 0; y < MAZE_LEVEL_WIDTH; y++)
//                 {
//                     // Create walls
//                     if ((*levels[current_level])[x][y] == 1)
//                     {
//                         DrawTextureRec(wall_block_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
//                     }
//                     else if (((*levels[current_level])[x][y] == 0) || ((*levels[current_level])[x][y] == 10))
//                     {
//                         DrawTextureRec(floor_grass_texture, rec12, (Vector2){ x * CELL_SIZE, y * CELL_SIZE }, RAYWHITE);
//                     }

//                     // Create Question
//                     if ((*levels[current_level])[x][y] == 2)
//                     {
//                         DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
//                     }
//                 }
//             }
            
//             // Draw player
//             DrawRectangle(player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE, RED);

//             if(game_state == 4){
//                 // define menu options
//                 char *menuOptionText[4] = { level1_questions.level_questions[question_number-1].option1, level1_questions.level_questions[question_number-1].option2, level1_questions.level_questions[question_number-1].option3, level1_questions.level_questions[question_number-1].option4 };

//                 // draw question stuff outline
//                 DrawRectangle(500, 100, 600, 500, LIGHTGRAY);
//                 DrawText(TextFormat("Question %d", question_number), 520, 120, 40, BLACK);

//                 // Draw question
//                 DrawText(TextFormat(level1_questions.level_questions[question_number-1].line1, question_number), 520, 230, 30, BLACK);
//                 //DrawRectangleLines(500, 230, 600, 80, GRAY); Removing cause it looks weird gotta fix or find a better look before submission guys

//                 // Draw menu options
//                 for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
//                     DrawRectangleRec(menuOptionRecs[i], (IsClickedOption == i || IsHoveredOption == i) ? SKYBLUE : LIGHTGRAY);
//                     DrawRectangleLines(menuOptionRecs[i].x, menuOptionRecs[i].y, menuOptionRecs[i].width, menuOptionRecs[i].height, (IsClickedOption == i || IsHoveredOption == i) ? BLUE : GRAY);
//                     DrawText(menuOptionText[i], (int)(menuOptionRecs[i].x + menuOptionRecs[i].width / 2 - MeasureText(menuOptionText[i], 25) / 2), (int)menuOptionRecs[i].y + 11, 25, (IsClickedOption == i || IsHoveredOption == i) ? DARKBLUE : DARKGRAY);
//                 }


//                 // Check for mouse interaction with menu options
//                 int mousex = GetMouseX();
//                 int mousey = GetMouseY();
//                 IsClickedOption = GetClickedOption(mousex, mousey, menuOptionRecs, NUM_MENU_OPTIONS);
//                 IsHoveredOption = -1; // Reset hovered option

//                 // Handle menu option click events
//                 if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && IsClickedOption != -1) {
//                     // Perform action based on clicked option
//                     printf("Clicked option %d\n", IsClickedOption);
//                     if(IsClickedOption+1 == level1_questions.level_questions[question_number-1].answer){
//                         // person selected the correct answer or cheated cause some of these questions tuff
//                         game_state = 1;
//                         points++;

//                         // play some sound here
//                         // could display a correct sign on screen here also
//                     }else{
//                         // person selected the wrong answer
//                         double current_time = GetTime() + 3;
//                         lives--;
//                         printf("Wrong answer, you lose a life\n %f", current_time);
                        
//                         game_state = 5;
//                         // draw lose screen
//                         // printf("draw lose screen and waiting 3 seconds\n");
//                         // DrawTextureRec(lose_screen, rec8, (Vector2){ 0, 0 }, RAYWHITE);
//                         // play some sound here
//                         // could display a wrong sign on screen here also
                        
//                         // WaitTime(3);
//                     }
//                 }

//                 // only check if the user clicked the button
//                 // if(IsMouseButtonDown(0)){
//                 //     // get the button the user clicked
//                 //     selected_answer = GetClickedButton(mousex, mousey);
//                 // }
//                 // check if the use clicked the correct answer
//                 // if(selected_answer == level1_questions.level_questions[question_number-1].answer){
//                 //     game_state = 1;
//                 //     question_number++;
//                 //     points++;
//                 // } 
//                 // else{
//                 //     // check if the user clicked the wrong answer
//                 //     if((selected_answer != level1_questions.level_questions[question_number-1].answer) && (selected_answer != 0)){
//                 //         lives--;
//                 //         // DrawTextureRec(lose_screen, rec8, (Vector2){ 500, 100 }, RAYWHITE);
//                 //         // WaitTime(3);
//                 //         printf("Wrong answer, you lose a life\n");
//                 //     }

//                 //     // Asking question
//                     // DrawRectangle(500, 100, 600, 500, LIGHTGRAY);
//                     // DrawText(TextFormat("Question %d %d", question_number, selected_answer), 520, 120, 40, BLACK);

//                     // // Draw question
//                     // DrawText(TextFormat(level1_questions.level_questions[question_number-1].line1, question_number), 520, 230, 20, BLACK);

//                 //     // Draw button clickable area
//                 //     DrawRectangle(500, 335, 500, 50, GREEN);
//                 //     DrawRectangle(500, 395, 500, 50, BLUE);
//                 //     DrawRectangle(500, 455, 500, 50, RED);
//                 //     DrawRectangle(500, 515, 500, 50, PINK);

                    
//                 //     // Draw answer buttons
//                 //     DrawText(TextFormat(level1_questions.level_questions[question_number-1].option1), 530, 350, 20, BLACK);
//                 //     DrawText(TextFormat(level1_questions.level_questions[question_number-1].option2), 530, 410, 20, BLACK);
//                 //     DrawText(TextFormat(level1_questions.level_questions[question_number-1].option3), 530, 470, 20, BLACK);
//                 //     DrawText(TextFormat(level1_questions.level_questions[question_number-1].option4), 530, 530, 20, BLACK);
//                 // }
                    
//             }

//             if(game_state == 5){
//                 // draw lose screen
//                 // printf("draw lose screen and waiting 3 seconds\n");
//                 DrawRectangle(500, 100, 600, 500, BLACK);
//                 DrawTextureRec(lose_screen, rec8, (Vector2){ 0, 0 }, RAYWHITE);
//                 // WaitTime(3);
//                 // game_state = 4;
//             }
//         }
        
//         EndDrawing();
//     }

//     UnloadMusicStream(music);   // Unload music stream buffers from RAM
//     CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
//     CloseWindow();
//     return 0;
// }