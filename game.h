#include "textures.h"
#include "states.h"
#include "interactions.h"
#include "position.h"
#include "debugdefiner.h"

//std::string DEFAULT_GAME_FEN = "8/pppppppp/rnbqkbnr/8/8/RNBQKBNR/PPPPPPPP/8 w KQkq - 0 1";
std::string DEFAULT_GAME_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string DEFAULT_PIECES_PNG = "resources/chesspieces.png";

class ChessGame{
  ChessPosition currentPosition;
  ChessTexture texture;
  ChessPlayerInteraction interaction;
  
  Vector2 topLeftOffset;
  float gridSize;
  public:
  ChessGame(Vector2 offset, float boardScale, std::string FEN = DEFAULT_GAME_FEN)
   : topLeftOffset(offset),
     texture(DEFAULT_PIECES_PNG, boardScale),
     currentPosition(FEN),
     interaction(offset, boardScale),
     gridSize(texture.getGridSize())
     {}

  inline Vector2 getGridPosition(Position pos){
    return Vector2{
      (pos % 8) * gridSize + this->topLeftOffset.x,
      (pos / 8) * gridSize + this->topLeftOffset.y
    };
  }
  inline Vector2 getGridCenter(Position pos){
    return Vector2{
      (pos % 8) * gridSize + this->topLeftOffset.x + gridSize/2,
      (pos / 8) * gridSize + this->topLeftOffset.y + gridSize/2
    };
  }
  void DrawSelected(){
    if (interaction.HasSelectedSquare()){
      DrawTextureV(texture.getSelectedTexture(), 
                   getGridPosition(interaction.getHighlightedPosition()), 
                   WHITE);
    }
  }
  void DrawSpot(){
    if (interaction.HasSelectedSquare()){
      Position highlightedSpot = interaction.getHighlightedPosition();
      for (Position move: currentPosition.getMoves(highlightedSpot)){
        DrawTextureV(texture.getSpotTexture(), getGridPosition(move), WHITE);
      }
    }
  }

  void DrawMoved(){
    if (interaction.HasCompletedMove()){
      Move doneMove = interaction.getLastMove();
      DrawTextureV(texture.getMovedTexture(), getGridPosition(doneMove.first), WHITE);
      DrawTextureV(texture.getMovedTexture(), getGridPosition(doneMove.second), WHITE);
    }
  }
  void DrawPieces(){
    const auto& piecesTexture = texture.getAllPiecesTexture();
    Vector2 tl;
    Piece p;
    for (int i = 0; i < 64; i++){
      p = currentPosition.getPiece(i);
      if (!isValidPiece(p)) continue;
      tl = getGridPosition(i);

      const PieceAttribute& p_attrib = PiecesAttributes.at(p);
      const Texture2D& pieceTexture = piecesTexture[p_attrib.textureIndex];
      
      tl.x += (gridSize - pieceTexture.width)/2;
      tl.y += (gridSize - pieceTexture.height)/2;

      DrawTextureV(pieceTexture, tl, WHITE);

      #ifdef DEBUG_MODE
        DrawRectangleLines(tl.x, tl.y, pieceTexture.width, pieceTexture.height, RED);
      #endif
    }
  }
  void DrawBoard(){
    const auto& boardTexture = texture.getBoardTexture();
    DrawTexture(boardTexture, this->topLeftOffset.x, this->topLeftOffset.y, WHITE);
  }
  void Draw(){
    // order matters
    DrawBoard();
    DrawSelected();
    DrawMoved();
    DrawPieces();
    DrawSpot();
  }
  void update(){
    if (interaction.populatePlayerInput()){
      #ifdef DEBUG_MODE
      std::cout << "return true  ---------------" << std::endl;
      interaction.print();
      if (interaction.HasSelectedSquare()){
        currentPosition.print(interaction.getHighlightedPosition());
      }
      if (interaction.HasCompletedMove()){
        Move playerMove = interaction.getLastMove();
      #endif
      }

    }
  } 
};