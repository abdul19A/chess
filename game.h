#include "textures.h"
#include "states.h"
#include "interactions.h"
#include "position.h"

std::string DEFAULT_GAME_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string DEFAULT_PIECES_PNG = "resources/chesspieces.png";

class ChessGame{
  ChessPosition currentPosition;
  ChessTexture texture;
  ChessPlayerInteraction interaction;
  
  Vector2 topLeftOffset;
  float gridSize;
  Move lastMove;
  bool isPromptingPromotion;
  Move promotionMove;

  public:
  ChessGame(Vector2 offset, float boardScale, std::string FEN = DEFAULT_GAME_FEN)
   : topLeftOffset(offset),
     texture(DEFAULT_PIECES_PNG, boardScale),
     currentPosition(FEN),
     interaction(offset, boardScale),
     gridSize(texture.getGridSize()),
     lastMove(INVALID_MOVE),
     isPromptingPromotion(false),
     promotionMove(INVALID_MOVE)
     {}
  private:
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
    if (lastMove != INVALID_MOVE){
      DrawTextureV(texture.getMovedTexture(), getGridPosition(lastMove.first), WHITE);
      DrawTextureV(texture.getMovedTexture(), getGridPosition(lastMove.second), WHITE);
    }
  }
  void DrawCheck(){
    if (currentPosition.getIsInCheck()){
      DrawTextureV(texture.getCheckTexture(), getGridPosition(currentPosition.getKingPosition()), WHITE);
    }
  }
  void DrawPiece(Piece piece, Position pos){
    const auto& pieceTexture = texture.getPieceTexture(piece);
    Vector2 tl = getGridPosition(pos);
    const PieceAttribute& p_attrib = PiecesAttributes.at(piece);
    tl.x += (gridSize - pieceTexture.width)/2;
    tl.y += (gridSize - pieceTexture.height)/2;
    //DrawTextureV(pieceTexture, tl, WHITE); <- weird clipping behavior due to float size
    DrawTextureV(pieceTexture, {std::floor(tl.x), std::floor(tl.y)}, WHITE);

    #ifdef DEBUG_MODE
      DrawRectangleLines(tl.x, tl.y, pieceTexture.width, pieceTexture.height, RED);
    #endif
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

      //DrawTextureV(pieceTexture, tl, WHITE); <- weird clipping behavior due to float size
      DrawTextureV(pieceTexture, {std::floor(tl.x), std::floor(tl.y)}, WHITE);

      #ifdef DEBUG_MODE
        DrawRectangleLines(tl.x, tl.y, pieceTexture.width, pieceTexture.height, RED);
      #endif
    }
  }
  void DrawBoard(){
    const auto& boardTexture = texture.getBoardTexture();
    DrawTexture(boardTexture, this->topLeftOffset.x, this->topLeftOffset.y, WHITE);
  }
  void DrawPromotion(){
    if (this->isPromptingPromotion){
      int i = 0;
      if (currentPosition.isWhiteToMove()){
        DrawTextureV(texture.getPromotionTexture(), getGridPosition(this->promotionMove.second), WHITE);
        for (Piece p: Promotion::order) DrawPiece(uncoloredToWhite(p), movePosition(this->promotionMove.second, 0, i++));
      }
      else {
        DrawTextureV(texture.getPromotionTexture(), getGridPosition(movePosition(this->promotionMove.second, 0, -4)), WHITE);
        for (Piece p: Promotion::order) DrawPiece(p, movePosition(this->promotionMove.second, 0, i--));
      }
    }
  } 
  bool promptPromotion(Move playedMove){ 
    if (currentPosition.isPromotionMove(playedMove.first, playedMove.second)){
      this->isPromptingPromotion = true;
      this->promotionMove = playedMove;
      return false;
    }
    else {
      return true;
    }
  }
  public:
  void Draw(){
    // order matters
    DrawBoard();
    DrawSelected();
    DrawMoved();
    DrawCheck();
    DrawPieces();
    DrawSpot();
    DrawPromotion();
  }
  void update(){
    if (interaction.populatePlayerInput()){
      #ifdef DEBUG_MODE
      std::cout << "---------------" << std::endl;
      interaction.print();
      if (interaction.HasSelectedSquare()){
        currentPosition.print(interaction.getHighlightedPosition());
      }
      #endif
    }
    else {
      return; // early out
    }
    // handle selecting option for promotion
    if (interaction.HasSelectedSquare() && this->isPromptingPromotion){
      Position selectedSquare = interaction.getHighlightedPosition();
      Promotion::choice chosen = currentPosition.getPromotion(selectedSquare, this->promotionMove.second);
      
      if (chosen == Promotion::none){
        // we did not play this move so let the player pick another one
        interaction.resetMove();
      }
      else {
        // play this move
        currentPosition.update(this->promotionMove.first, this->promotionMove.second, chosen);
        lastMove = this->promotionMove;
      }
      // resets
      this->promotionMove = INVALID_MOVE;
      this->isPromptingPromotion = false;
    }

    else if (interaction.HasCompletedMove()){
      Move move = interaction.getLastMove();
      
      if (currentPosition.isValidMove(move.first, move.second) && promptPromotion(move)){
          currentPosition.update(move.first, move.second);
          lastMove = move;
      } 
      else {
          interaction.resetMove();
      }
    }  
  } 
};