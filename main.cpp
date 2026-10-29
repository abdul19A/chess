#include <raylib.h>
#include "game.h"

std::string ENPASSANT_TEST = "rnbq1bn1/1pp1p2r/4k1Pp/3pPp2/pP1K4/5N2/P1PP1PP1/RNBQ1B1R b - b3 0 10";
int main(int argc, char** argv)
{  
    const int screenWidth = 800;
    const int screenHeight = 800;
    SetTraceLogLevel(LOG_WARNING); 
    InitWindow(screenWidth, screenHeight, "Chess");
    SetTargetFPS(60);
    ChessGame game({100, 100}, 600, ENPASSANT_TEST);
    std::cout << __FILE__ << std::endl;

    #ifdef DEBUG_MODE
        std::cout << "RUNNING: DEBUG MODE" << std::endl;
    #else
        std::cout << "RUNNING: RELEASE MODE" << std::endl;
    #endif

    while (!WindowShouldClose()){
        game.update();
        BeginDrawing();
            ClearBackground({0x60, 0x40, 0x35, 0xFF});
            game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}