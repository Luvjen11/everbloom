#include "raylib.h"


int main(void) {
   InitWindow(1280, 512, "Everbloom");

    Texture2D background = LoadTexture("resources/background.png");
    Texture2D dialogueScreen = LoadTexture("resources/dialogue.png");
    Texture2D startButton = LoadTexture("resources/start-button.png");
    Texture2D nextButton = LoadTexture("resources/next-button.png");
    
    //screen one start screen
    int currentScreen = 0;
    // dialouge switching
    int dialogueIndex = 0;

    // array of dialogue text

    const char *dialogueLines[] = {
        "Client: They say you cultivate the impossible.",
        "You: They exaggerate.",
        "Client: I require the Everbloom.",
        "You: That flower has a temperament.",
        "Client: So do I.",
        "You: It withers for the impatient.",
        "Client: I am not impatient.",
        "You: Then pray it chooses to bloom for you."
    };

    int dialogueCount = 8;

    while (!WindowShouldClose()) {

        // track mouse position
        Vector2 mouse = GetMousePosition();
        

        BeginDrawing();
        ClearBackground(BLACK);
        if (currentScreen == 0) {

            // start background
            DrawTexture(background, 0, 0, WHITE);

            // start button
            Vector2 startPos = { 440, 270 };
            Rectangle startRect = { startPos.x, startPos.y, startButton.width, startButton.height };

            // hover effect
            bool startHover = CheckCollisionPointRec(mouse, startRect);

            if (startHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                currentScreen = 1;
                dialogueIndex = 0;
            }

            Color tint = startHover ? Fade(WHITE, 0.8f) : WHITE;
            DrawTexture(startButton, startPos.x, startPos.y, tint);

        } else if (currentScreen == 1) {

            // dialogue background
            DrawTexture(dialogueScreen, 0, 0, WHITE);

            // dialogue box
            Rectangle dialogueBox = { 80, 320, 1120, 150 };
            DrawRectangleRounded(dialogueBox, 0.2f, 10, Fade(BLACK, 0.45f));
            DrawText(dialogueLines[dialogueIndex], 110, 360, 26, RAYWHITE);

            // next button
            Vector2 nextPos = { 1280 - nextButton.width - 60, 512 - nextButton.height - 30 };
            Rectangle nextRect = { nextPos.x, nextPos.y, nextButton.width, nextButton.height };

            // hover effect
            bool nextHover = CheckCollisionPointRec(mouse, nextRect);
            
            // dialogue handling 
            if (nextHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                dialogueIndex++;
            }

            if (dialogueIndex >= dialogueCount) {
                currentScreen = 2;
            }

            Color nextTint = nextHover ? Fade(WHITE, 0.8f) : WHITE;
            DrawTexture(nextButton, nextPos.x, nextPos.y, nextTint);

        } else if (currentScreen == 2) {
            ClearBackground((Color){200, 220, 200, 255});
            DrawText("A GIRL STILL NEEDS TO GET TO WORK...", 400, 200, 40, DARKGREEN);
        }
        

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(dialogueScreen);
    UnloadTexture(startButton);
    UnloadTexture(nextButton);
    CloseWindow();
    return 0;
}