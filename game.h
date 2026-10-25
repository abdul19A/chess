#include "textures.h"
#include "states.h"
#include "interactions.h"

std::string DEFAULT_GAME_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string DEFAULT_PIECES_PNG = "resources/chesspieces.png";

class ChessGame{
  ChessState states;
  ChessTexture texture;
  ChessPlayerInteraction interaction;

  Vector2 topLeftOffset;
  public:
  ChessGame(Vector2 offset, float boardScale)
   : topLeftOffset(offset),
     texture(DEFAULT_PIECES_PNG, boardScale),
     states(DEFAULT_GAME_FEN),
     interaction(offset, boardScale){}

  void Draw(){
    const auto& piecesTexture = texture.getAllPiecesTexture();
    const auto& boardTexture = texture.getBoardTexture();
    DrawTexture(boardTexture, this->topLeftOffset.x, this->topLeftOffset.y, WHITE);
    
    int x, y, order;
    int gridSize = texture.getGridSize();

    if (interaction.HasSelectedSquare()){
      Position highlightedSpot = interaction.getHighlightedPosition();
      x = (highlightedSpot % 8) * gridSize + this->topLeftOffset.x;
      y = (highlightedSpot / 8) * gridSize + this->topLeftOffset.y;
      DrawTexture(texture.getSelectedTexture(), x, y, WHITE);
    }
    if (interaction.HasCompletedMove()){
      Move doneMove = interaction.getLastMove();
      x = (doneMove.first % 8) * gridSize + this->topLeftOffset.x;
      y = (doneMove.first / 8) * gridSize + this->topLeftOffset.y;
      DrawTexture(texture.getMovedTexture(), x, y, WHITE);
      x = (doneMove.second % 8) * gridSize + this->topLeftOffset.x;
      y = (doneMove.second / 8) * gridSize + this->topLeftOffset.y;
      DrawTexture(texture.getMovedTexture(), x, y, WHITE);
    }
   
    
    for (int i = 0; i < 64; i++){

      x = (i % 8) * gridSize + this->topLeftOffset.x;
      y = (i / 8) * gridSize + this->topLeftOffset.y;

      Piece p = states.getPiece(i);
      if (p == ' ') continue;

      const PieceAttribute& p_attrib = PiecesAttributes.at(p);
      const Texture2D pieceTexture = piecesTexture[p_attrib.textureIndex];

      x += (gridSize - pieceTexture.width)/2;
      y += (gridSize - pieceTexture.height)/2;
      DrawTexture(pieceTexture, x, y, WHITE);
      #ifdef DEBUG_MODE
        DrawRectangleLines(x, y, pieceTexture.width, pieceTexture.height, RED);
      #endif
    }
  }
  void update(){
    if (interaction.populatePlayerInput()){
      #ifdef DEBUG_MODE
      std::cout << "return true  ---------------" << std::endl;
      interaction.print();
      #endif
    }
  } 
};