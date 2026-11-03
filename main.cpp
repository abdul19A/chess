#include <raylib.h>
#include "game.h"

std::string ENPASSANT_TEST = "rnbq1bn1/1pp1p2r/4k1Pp/3pPp2/pP1K4/5N2/P1PP1PP1/RNBQ1B1R b - b3 0 10";
std::string CASTLING_TEST = "rnbqk2r/p1pp1pp1/1pNn4/2b4p/3Pp3/1QP2N2/PP2B2P/R1B1K2R w KQq - 0 19";
std::string FILTER_TEST = "rnb1k2r/p1ppqpp1/1pNnQ3/2b4p/3Pp3/2P2N2/PP2B2P/R1B1K2R b KQq - 2 20";
std::string CHECK_TEST = "rnb1k1Rr/p1pp1p2/1pNn4/2b4p/3Pp3/2P2N1q/PP2B2P/R1B1K3 b Qq - 2 22";
std::string NO_CHECK_TEST = "rnb1k1r1/p1pp1p2/1pNn4/2b4p/3Pp3/2P2N1q/PP2B2P/R1B1K3 w Qq - 0 23";
std::string DEFEND_CHECK_TEST = "rnb1kb1r/ppppqQpp/7n/4p3/4P3/3P4/PPP2PPP/RNB1KBNR b KQkq - 0 4";
std::string ENPASSANT_CHECKMATE = "rn1q1b1r/pp1bkB1p/1n3pp1/1Pp1p3/8/BQ2P3/P2P1PPP/RN2K1NR w KQ c6 0 11";
std::string ENPASSANT_DEFENSE = "rn1q1b1r/pp1bkB1p/1n3p2/1Pp5/3KpPp1/1Q2P3/PB1P2PP/RN4NR w - c6 0 14";
std::string PROMOTION_TEST = "rnbqkbnr/pPpppppp/8/8/8/8/PPPPPPpP/RNBQKBNR w KQkq - 0 1";
int main(int argc, char** argv)
{  
    const int screenWidth = 800;
    const int screenHeight = 800;
    SetTraceLogLevel(LOG_WARNING); 
    InitWindow(screenWidth, screenHeight, "Chess");
    SetTargetFPS(60);
    ChessGame game({100, 100}, 600, PROMOTION_TEST);
    std::cout << __FILE__ << std::endl;

    #ifdef DEBUG_MODE
        std::cout << "RUNNING: DEBUG MODE" << std::endl;
    #else
        std::cout << "RUNNING: RELEASE MODE" << std::endl;
    #endif

    while (!WindowShouldClose()){
        game.update();
        BeginDrawing();
            ClearBackground({0x0, 0x0, 0x0, 0xFF});
            game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}