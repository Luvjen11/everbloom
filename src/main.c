#include "raylib.h"

// --------
// ENUMS
// -------- 

// screentype
typedef enum {
    SCREEN_TITLE = 0,
    SCREEN_DIALOGUE,
    SCREEN_FARM,
    SCREEN_CAULDRON
} ScreenState;

// seedtype
typedef enum {
    SEED_NONE = 0,
    SEED_RED,
    SEED_BLUE,
    SEED_YELLOW
} SeedType;

// flowerstage
typedef enum {
    FLOWER_NONE = 0,
    FLOWER_BUD,
    FLOWER_MID,
    FLOWER_BLOOM
} FlowerStage;

// brewpotion
typedef enum {
    BREW_NONE = -1,
    BREW_GREEN = 0,
    BREW_PURPLE = 1,
    BREW_ORANGE = 2
} BrewResult;


// --------
// VARIABLES
// --------

FlowerStage flowerStages[2] = { FLOWER_NONE, FLOWER_NONE };
float growthTimer = 0.0f;
bool readyToHarvest = false;
bool harvested = false;
bool planted = false;

// brewing 
BrewResult brewResult = BREW_NONE;
// tracks final brew dialogue
bool brewed = false;
int brewDialogueIndex = 0;

// player picked seed
SeedType selectedSeeds[2] = { SEED_NONE, SEED_NONE };
// player planted seeds
SeedType plantedSeeds[2] = { SEED_NONE, SEED_NONE };
// player harvested seeds
SeedType harvestedSeeds[2] = { SEED_NONE, SEED_NONE };

// --------
// FUNCTIONS
// --------

// to check if 2 seeds have been selected
bool HasTwoSeeds(SeedType seeds[2]) {
    return seeds[0] != SEED_NONE && seeds[1] != SEED_NONE;
}

// add seeds
void AddSeed(SeedType selectedSeeds[2], SeedType seed) {

    // to not allow more than two seeds 
    if (HasTwoSeeds(selectedSeeds))
        return;

    if (selectedSeeds[0] == SEED_NONE) {
        selectedSeeds[0] = seed;
    }
    else if (selectedSeeds[1] == SEED_NONE) {
        selectedSeeds[1] = seed;
    }
}

// based on primary colors 
BrewResult GetBrewResult(SeedType seeds[2]) {
    bool hasRed = (seeds[0] == SEED_RED || seeds[1] == SEED_RED);
    bool hasBlue = (seeds[0] == SEED_BLUE || seeds[1] == SEED_BLUE);
    bool hasYellow = (seeds[0] == SEED_YELLOW || seeds[1] == SEED_YELLOW);

    if (hasBlue && hasYellow) return BREW_GREEN;
    if (hasRed && hasBlue) return BREW_PURPLE;
    if (hasRed && hasYellow) return BREW_ORANGE;

    return BREW_NONE;
}

// --------
// MAIN
// --------

int main(void) {

    InitWindow(1280, 512, "Everbloom");

    // TEXTURES 

    Texture2D background = LoadTexture("resources/background.png");
    Texture2D dialogueScreen = LoadTexture("resources/dialogue.png");
    Texture2D farm = LoadTexture("resources/farm.png");
    Texture2D inventory = LoadTexture("resources/inventory-icon.png");
    Texture2D inventoryScreen = LoadTexture("resources/inventory-screen.png"); 
    Texture2D startButton = LoadTexture("resources/start-button.png");
    Texture2D nextButton = LoadTexture("resources/next-button.png");
    Texture2D redSeed = LoadTexture("resources/red-seed.png");
    Texture2D yellowSeed = LoadTexture("resources/yellow-seed.png");
    Texture2D blueSeed  = LoadTexture("resources/blue-seed.png");
    Texture2D slots = LoadTexture("resources/slots.png");
    Texture2D redSeedSmall = LoadTexture("resources/red-seed-small.png");
    Texture2D yellowSeedSmall = LoadTexture("resources/yellow-seed-small.png");
    Texture2D blueSeedSmall = LoadTexture("resources/blue-seed-small.png");
    Texture2D cauldronScreen = LoadTexture("resources/cauldron-screen.png");
    Texture2D bud = LoadTexture("resources/bud.png");
    Texture2D redMid = LoadTexture("resources/red-mid.png");
    Texture2D redBloom = LoadTexture("resources/red-bloom.png");
    Texture2D blueMid = LoadTexture("resources/blue-mid.png");
    Texture2D blueBloom = LoadTexture("resources/blue-bloom.png");
    Texture2D yellowMid = LoadTexture("resources/yellow-mid.png");
    Texture2D yellowBloom = LoadTexture("resources/yellow-bloom.png"); 
    Texture2D cauldronOrange = LoadTexture("resources/cauldron-orange.png"); 
    Texture2D cauldronPurple = LoadTexture("resources/cauldron-purple.png"); 
    Texture2D cauldronGreen = LoadTexture("resources/cauldron-green.png"); 
        
    //screen one start screen
    ScreenState currentScreen = SCREEN_TITLE;

    // dialouge switching
    int dialogueIndex = 0;
    // total dialogue lines
    int dialogueCount = 8; 

    // array of dialogue text
    const char *dialogueLines[] = {
        "Client: They say you grow things that should not exist.",
        "You: People say many things when they want something.",
        "Client: I want the Everbloom.",
        "You: Then you want trouble dressed as a flower.",
        "Client: I can afford trouble.",
        "You: Money does not impress the garden.",
        "Client: I did not come here for warnings.",
        "You: Then hope it blooms in your favour."
    };

    const char *brewDialogues[3][3] = {
        {
            // GREEN
            "The brew glows green",
            "You: Curious. This one feels unstable.",
            "The client drinks. Their smile fades first.\nThe potion ended up killing the customer...\nCongratulations?!"
        },
        {
            // PURPLE
            "The brew glows purple",
            "You: Beautiful. That usually means trouble.",
            "The client transforms, though not in any way either of you had hoped.\nThe client is not happy, but at least they are alive... I guess?"
        },
        {
            // ORANGE
            "The brew glows orange",
            "You: Warm. Sharp. Ambitious.",
            "The potion takes hold. The Everbloom answers. Forever young I guess!"
        },
    };

    bool inventoryOpen = false;

    // --------
    // LOOP
    // --------

    while (!WindowShouldClose()) {

        // --------
        // POSITIONS OF ASSETS
        // --------

        // track mouse position
        Vector2 mouse = GetMousePosition();

        // start button position 
        Vector2 startPos = { 440, 270 };
        Rectangle startRect = { startPos.x, startPos.y, startButton.width, startButton.height };
        // hover effect start button
        bool startHover = CheckCollisionPointRec(mouse, startRect);
    
        // next button position
        Vector2 nextPos = { 1280 - nextButton.width - 60, 512 - nextButton.height - 30 };
        Rectangle nextRect = { nextPos.x, nextPos.y, nextButton.width, nextButton.height };
        // hover effect next button 
        bool nextHover = CheckCollisionPointRec(mouse, nextRect);

        // plot position
        Rectangle plotArea = { 880, 85, 260, 160 };
        // hover effect plot
        bool plotHover = CheckCollisionPointRec(mouse, plotArea);

        // seed box position
        Rectangle redSeedRect    = { 300,  70, 150, 150 };
        Rectangle yellowSeedRect = { 560,  70, 150, 150 };
        Rectangle blueSeedRect   = { 815,  70, 150, 150 };

        // plant position -> where to plant flowers
        Vector2 plantPos1 = { 940, 120 };
        Vector2 plantPos2 = { 1000, 160 };


        // --------
        // UPDATE
        // --------

        if (currentScreen == SCREEN_TITLE) {
            
            // if click start button move to dialogue screen
            if (startHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                currentScreen = SCREEN_DIALOGUE;
                dialogueIndex = 0;
            }

        }
        else if (currentScreen == SCREEN_DIALOGUE) {
                        
            // dialogue handling 
            // when click next button change text
            if (nextHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                dialogueIndex++;
            }

            // when dialox is done move to farm screen
            if (dialogueIndex >= dialogueCount) {
                currentScreen = SCREEN_FARM;
            }

        }
        else if (currentScreen == SCREEN_FARM) {

            // INVENTORY AREA

            // click I inventory to open
            if (IsKeyPressed(KEY_I)) {
                inventoryOpen = !inventoryOpen;
            }

            // close inventory with ESC
            if (inventoryOpen && IsKeyPressed(KEY_ESCAPE)) {
                inventoryOpen = false;
            }

            // to clear the slots and allow to reselect seeds
            if (inventoryOpen && IsKeyPressed(KEY_C)) {
                selectedSeeds[0] = SEED_NONE;
                selectedSeeds[1] = SEED_NONE;
            }

            if (inventoryOpen) {

                // inventory seed selection 

                // add the seeds that are selected
                if (CheckCollisionPointRec(mouse, redSeedRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    AddSeed(selectedSeeds, SEED_RED);
                }

                if (CheckCollisionPointRec(mouse, yellowSeedRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    AddSeed(selectedSeeds, SEED_YELLOW);
                }

                if (CheckCollisionPointRec(mouse, blueSeedRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    AddSeed(selectedSeeds, SEED_BLUE);
                }


            }

            // planting by clicking plot 
            if (plotHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (!planted && HasTwoSeeds(selectedSeeds)) {

                    // plant seeds
                    plantedSeeds[0] = selectedSeeds[0];
                    plantedSeeds[1] = selectedSeeds[1];

                    // bud stage
                    flowerStages[0] = FLOWER_BUD;
                    flowerStages[1] = FLOWER_BUD;
                    
                    //set planted to true and start timer
                    planted = true;
                    growthTimer = 0.0f;
                    readyToHarvest = false;

                    // then empty slots
                    selectedSeeds[0] = SEED_NONE;
                    selectedSeeds[1] = SEED_NONE;
                }
            }

            // grow flower by time for now
            if (planted && !readyToHarvest) {

                growthTimer += GetFrameTime();
                
                if (growthTimer >= 3.0f) {
                    flowerStages[0] = FLOWER_MID;
                    flowerStages[1] = FLOWER_MID;
                }

                if (growthTimer >= 6.0f) {
                    flowerStages[0] = FLOWER_BLOOM;
                    flowerStages[1] = FLOWER_BLOOM;
                    readyToHarvest = true;
                }

            }

            // harvest flower
            if (plotHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                
                if (readyToHarvest) {
                    harvestedSeeds[0] = plantedSeeds[0];
                    harvestedSeeds[1] = plantedSeeds[1];

                    // reset everything
                    plantedSeeds[0] = SEED_NONE;
                    plantedSeeds[1] = SEED_NONE;

                    // reset planted seeds
                    flowerStages[0] = FLOWER_NONE;
                    flowerStages[1] = FLOWER_NONE;

                    planted = false;
                    harvested = true;
                    readyToHarvest = false;
                }
            }

            // move to next screen by pressing B
            if (harvested && IsKeyPressed(KEY_B)) {
                brewResult = GetBrewResult(harvestedSeeds);
                brewed = false;
                brewDialogueIndex = 0;
                currentScreen = SCREEN_CAULDRON;
            }

        } else if (currentScreen == SCREEN_CAULDRON) {

            // change dialouge
            if (!brewed && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                brewDialogueIndex++;

                // stop after dialouge is done
                if (brewDialogueIndex >= 3) {
                    brewDialogueIndex = 2;
                    brewed = true;
                }
            }

            // restart the game by setting initial states
            if (brewed && IsKeyPressed(KEY_R)) {
                currentScreen = SCREEN_TITLE;

                selectedSeeds[0] = SEED_NONE;
                selectedSeeds[1] = SEED_NONE;

                plantedSeeds[0] = SEED_NONE;
                plantedSeeds[1] = SEED_NONE;

                harvestedSeeds[0] = SEED_NONE;
                harvestedSeeds[1] = SEED_NONE;

                flowerStages[0] = FLOWER_NONE;
                flowerStages[1] = FLOWER_NONE;

                growthTimer = 0.0f;
                readyToHarvest = false;
                harvested = false;
                planted = false;

                brewResult = BREW_NONE;
                brewed = false;
                brewDialogueIndex = 0;

                inventoryOpen = false;
                dialogueIndex = 0;
            }
    
        
        }


        // --------
        // DRAWING
        // --------

        BeginDrawing();
        ClearBackground(BLACK);

                
        if (currentScreen == SCREEN_TITLE) {

            // start background
            DrawTexture(background, 0, 0, WHITE);

            // fade when hover
            Color tint = startHover ? Fade(WHITE, 0.8f) : WHITE;

            // draw button
            DrawTexture(startButton, startPos.x, startPos.y, tint);

        }
        else if (currentScreen == SCREEN_DIALOGUE) {

            // dialogue background
            DrawTexture(dialogueScreen, 0, 0, WHITE);

            // dialogue box
            Rectangle dialogueBox = { 80, 320, 1120, 150 };
            DrawRectangleRounded(dialogueBox, 0.2f, 10, Fade(BLACK, 0.45f));
            DrawText(dialogueLines[dialogueIndex], 110, 360, 26, RAYWHITE);


            Color nextTint = nextHover ? Fade(WHITE, 0.8f) : WHITE;
            DrawTexture(nextButton, nextPos.x, nextPos.y, nextTint);
            
        }
        else if (currentScreen == SCREEN_FARM) {

            ClearBackground((Color){200, 220, 200, 255});
            DrawTexture(farm, 0, 0, WHITE);

            SetMouseCursor(plotHover ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);


            Rectangle invRect = { 40, 30, (float)inventory.width, (float)inventory.height };
            DrawTexture(inventory, (int)invRect.x, (int)invRect.y, WHITE);

            // show text when on plot
                
            if (!planted && plotHover && HasTwoSeeds(selectedSeeds)) {
                DrawText("Click to plant", 880, 85, 20, Fade(DARKGREEN, 0.55f));
            }
            else if (planted && !readyToHarvest) {
                DrawText("The flowers are growing...", 850, 60, 20, DARKGREEN);
            }
            else if (readyToHarvest && plotHover) {
                DrawText("Click to harvest", 880, 85, 20, Fade(DARKGREEN, 0.55f));
            }

            // inventory screen
            if (inventoryOpen) {
                DrawTexture(inventoryScreen, 0, 0, WHITE);

                // draw slots and seeds background
                DrawTexture(redSeed, 0, 0, WHITE);
                DrawTexture(yellowSeed, 0, 0, WHITE);
                DrawTexture(blueSeed, 0, 0, WHITE);
                DrawTexture(slots, 0, 0, WHITE);
                        
                
                // positions of slots
                Vector2 slot1Pos = { 322, 335 };
                Vector2 slot2Pos = { 442, 335 };


                // draw selected seed in slot 1
                if (selectedSeeds[0] == SEED_RED)
                    DrawTexture(redSeedSmall, (int)slot1Pos.x, (int)slot1Pos.y, WHITE);

                if (selectedSeeds[0] == SEED_YELLOW)
                    DrawTexture(yellowSeedSmall, (int)slot1Pos.x, (int)slot1Pos.y, WHITE);

                if (selectedSeeds[0] == SEED_BLUE)
                    DrawTexture(blueSeedSmall, (int)slot1Pos.x, (int)slot1Pos.y, WHITE);

                // draw selected seed in slot 2
                if (selectedSeeds[1] == SEED_RED)
                    DrawTexture(redSeedSmall, (int)slot2Pos.x, (int)slot2Pos.y, WHITE);

                if (selectedSeeds[1] == SEED_YELLOW)
                    DrawTexture(yellowSeedSmall, (int)slot2Pos.x, (int)slot2Pos.y, WHITE);

                if (selectedSeeds[1] == SEED_BLUE)
                    DrawTexture(blueSeedSmall, (int)slot2Pos.x, (int)slot2Pos.y, WHITE);

                DrawText("Press I or ESC to close", 30, 480, 20, RAYWHITE);
            }

            // Draw flower 1
            if (plantedSeeds[0] == SEED_RED) {
                if (flowerStages[0] == FLOWER_BUD) DrawTexture(bud, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_MID) DrawTexture(redMid, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_BLOOM) DrawTexture(redBloom, (int)plantPos1.x, (int)plantPos1.y, WHITE);
            }

            if (plantedSeeds[0] == SEED_BLUE) {
                if (flowerStages[0] == FLOWER_BUD) DrawTexture(bud, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_MID) DrawTexture(blueMid, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_BLOOM) DrawTexture(blueBloom, (int)plantPos1.x, (int)plantPos1.y, WHITE);
            }

            if (plantedSeeds[0] == SEED_YELLOW) {
                if (flowerStages[0] == FLOWER_BUD) DrawTexture(bud, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_MID) DrawTexture(yellowMid, (int)plantPos1.x, (int)plantPos1.y, WHITE);
                if (flowerStages[0] == FLOWER_BLOOM) DrawTexture(yellowBloom, (int)plantPos1.x, (int)plantPos1.y, WHITE);
            }

            // Draw flower 2
            if (plantedSeeds[1] == SEED_RED) {
                if (flowerStages[1] == FLOWER_BUD) DrawTexture(bud, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_MID) DrawTexture(redMid, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_BLOOM) DrawTexture(redBloom, (int)plantPos2.x, (int)plantPos2.y, WHITE);
            }

            if (plantedSeeds[1] == SEED_BLUE) {
                if (flowerStages[1] == FLOWER_BUD) DrawTexture(bud, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_MID) DrawTexture(blueMid, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_BLOOM) DrawTexture(blueBloom, (int)plantPos2.x, (int)plantPos2.y, WHITE);
            }

            if (plantedSeeds[1] == SEED_YELLOW) {
                if (flowerStages[1] == FLOWER_BUD) DrawTexture(bud, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_MID) DrawTexture(yellowMid, (int)plantPos2.x, (int)plantPos2.y, WHITE);
                if (flowerStages[1] == FLOWER_BLOOM) DrawTexture(yellowBloom, (int)plantPos2.x, (int)plantPos2.y, WHITE);
            }

            if (harvested) {
                DrawText("Press B to brew", 850, 60, 20, DARKGREEN);
            }
            

        }
        else if (currentScreen == SCREEN_CAULDRON) {
            DrawTexture(cauldronScreen, 0, 0, WHITE);

            if (brewResult == BREW_GREEN) {
                DrawTexture(cauldronGreen, 0, 0, WHITE);
            }
            else if (brewResult == BREW_PURPLE) {
                DrawTexture(cauldronPurple, 0, 0, WHITE);
            }
            else if (brewResult == BREW_ORANGE) {
                DrawTexture(cauldronOrange, 0, 0, WHITE);
            }
            else {
                DrawTexture(cauldronScreen, 0, 0, WHITE);
            }

            Rectangle dialogueBox = { 80, 320, 1120, 150 };
            DrawRectangleRounded(dialogueBox, 0.2f, 10, Fade(BLACK, 0.45f));

            if (brewResult != BREW_NONE) {
                DrawText(brewDialogues[brewResult][brewDialogueIndex], 110, 360, 26, RAYWHITE);
            }

            if (!brewed) {
                Color nextTint = nextHover ? Fade(WHITE, 0.8f) : WHITE;
                DrawTexture(nextButton, nextPos.x, nextPos.y, nextTint);
            }
            else {
                DrawText("Press R to restart", 30, 480, 20, RAYWHITE);
            }


            
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(dialogueScreen);
    UnloadTexture(startButton);
    UnloadTexture(nextButton);
    UnloadTexture(farm);
    UnloadTexture(inventory);
    UnloadTexture(inventoryScreen);
    UnloadTexture(redSeed);
    UnloadTexture(yellowSeed);
    UnloadTexture(blueSeed);
    UnloadTexture(slots);
    UnloadTexture(redSeedSmall);
    UnloadTexture(yellowSeedSmall);
    UnloadTexture(blueSeedSmall);
    UnloadTexture(cauldronScreen);
    UnloadTexture(bud);
    UnloadTexture(redMid);
    UnloadTexture(yellowMid);
    UnloadTexture(blueMid);
    UnloadTexture(redBloom);
    UnloadTexture(yellowBloom);
    UnloadTexture(blueBloom);
    UnloadTexture(cauldronOrange);
    UnloadTexture(cauldronPurple);
    UnloadTexture(cauldronGreen);
    CloseWindow();
    return 0;
}