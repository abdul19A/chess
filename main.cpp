#include <raylib.h>
#include "game.h"

int main(int argc, char** argv)
{  
    const int screenWidth = 800;
    const int screenHeight = 800;
    SetTraceLogLevel(LOG_WARNING); 
    InitWindow(screenWidth, screenHeight, "chess");
    SetTargetFPS(60);
    ChessGame game({100, 100}, 600);
    std::cout << __FILE__ << std::endl;
    #ifdef DEBUG_MODE
        std::cout << "RUNNING: DEBUG MODE" << std::endl;
    #else
        std::cout << "RUNNING: RELEASE MODE" << std::endl;
    #endif

    while (!WindowShouldClose()){
        game.update();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}