#include <textures.h>
#include <states.h>

string DEFAULT_GAME_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string DEFAULT_PIECES_PNG = "chesspieces.png";

class ChessGame{
  ChessState states;
  ChessTexture texture;
  ChessGame(){
    
  }
};