#include "states.h"
#include "attributes.h"
// read 70 bytes -> write into 70 bytes

class ChessPosition : public ChessState{
    // generates moves given a chess state
    std::map<Position, std::set<Position>> PossibleMoves;
    std::string playedMove;
    Position BlackKingPosition;
    Position WhiteKingPosition;
    bool toMoveIsChecked;
    
    public:
    ChessPosition(std::string FEN): ChessState(FEN){
        PopulateMoves();
        filterInvalidMoves();
    }
    ChessPosition(const ChessState& state ): ChessState(state){
        PopulateMoves();
        filterInvalidMoves();
    }
    ChessPosition(const ChessPosition& other) : ChessState(other){
        this->PossibleMoves = other.PossibleMoves;
        this->BlackKingPosition = other.BlackKingPosition;
        this->WhiteKingPosition = other.WhiteKingPosition;
    }
    
    ChessPosition& operator=(const ChessPosition& other){
        if (this == &other) return *this;
        ChessState::operator=(other);
        this->PossibleMoves = other.PossibleMoves;
        this->BlackKingPosition = other.BlackKingPosition;
        this->WhiteKingPosition = other.WhiteKingPosition;
        return *this;
    }
    void PopulateMoves(){
        PossibleMoves.clear();
        this->BlackKingPosition = INVALID_POS;
        this->WhiteKingPosition = INVALID_POS;
        for (Position pos = 0; pos < 64; pos++){
            if (isValidPiece(getPiece(pos)))
                populatePieceMoves(pos);
        }
    }
    const std::set<Position>& getMoves(Position pos){
        return PossibleMoves[pos];
    }
    private:
    inline Position move(Position from, int dx, int dy) const{
        int x = dx + (from % 8);
        if (x >= 8 || x < 0) return INVALID_POS;
        int y = dy + (from / 8);
        if (y >= 8 || y < 0) return INVALID_POS;
        return static_cast<int>(x + y * 8);
    }
    inline int getRelativeRow(Position from) const{
        if (isWhitePiece(from)){
            return 7 - getRow(from);
        }
        return getRow(from);
    }
    inline int getRelativeDy(Position from) const{
        if (isWhitePiece(from)){
            return -1;
        }
        return 1;
    }
    inline void addIfEmpty(Position from, Position to){
        if (to != INVALID_POS && getPiece(to) == ' ') PossibleMoves[from].emplace(to);
    }
    inline void addIfOpposite(Position from, Position to){
        if (to != INVALID_POS && isOppositeColor(getPiece(from), getPiece(to))) PossibleMoves[from].emplace(to);
    }
    inline void addIfOppositeOrEmpty(Position from, Position to){
        if (to != INVALID_POS && (getPiece(to) == ' ' || isOppositeColor(getPiece(from), getPiece(to)))) PossibleMoves[from].emplace(to);
    }
    inline void addThroughLine(Position from, int dx, int dy){
        Piece fromPiece = getPiece(from);
        Piece toPiece;
        Position curSquare;
        for (int i = 1; i < 8; i++){
            curSquare = move(from, dx*i, dy*i);
            toPiece = getPiece(curSquare);
            if (!isValidPosition(curSquare) || isSameColor(fromPiece, toPiece)) return;
            PossibleMoves[from].emplace(curSquare); // its empty or black so add it
            if(isOppositeColor(fromPiece, toPiece)){
                return; // cant go further than what is taken
            }
        }
    }
    
    void populatePawnMoves(Position pawnPos){
        // enpassant and promotion not yet implemented
        int relRow = getRelativeRow(pawnPos);
        int dy =  getRelativeDy(pawnPos); 
        int dx = 1;
        int row = getRow(pawnPos), col = getCol(pawnPos);

        if      (relRow == 1 // double-forward
                 && isEmptySpot(move(pawnPos, 0, dy))){ // next piece must be open
            addIfEmpty(pawnPos, move(pawnPos, 0, dy+dy)); // add if 2 forward is open
        }
        else if (relRow == 4 // enpassant
                 && isValidPosition(getEnpassantPosition())
                 &&    (getRow(getEnpassantPosition()) - row == dy)   // must be 1 forward
                 && (abs(getCol(getEnpassantPosition()) )- col == 1) 
                ){ // must be 1 left or 1 right
            PossibleMoves[pawnPos].emplace(getEnpassantPosition());
        }
        addIfEmpty   (pawnPos, move(pawnPos,   0, dy)); 
        addIfOpposite(pawnPos, move(pawnPos,  dx, dy));
        addIfOpposite(pawnPos, move(pawnPos, -dx, dy));
    }
    void populateRookMoves  (Position rookPos){
        for (int i = -1; i <= 1; i+=2){
            addThroughLine(rookPos, i, 0);
            addThroughLine(rookPos, 0, i);
        }
    }
    void populateBishopMoves(Position bishopPos){
        for (int i = -1; i <= 1; i+=2){
            for (int j = -1; j <= 1; j+=2){
                addThroughLine(bishopPos, i, j);
            }
        }
    }
    void populateQueenMoves (Position queenPos){
        Moves res;
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                if (i == 0 && j == 0) continue;
                addThroughLine(queenPos, i, j);
            }
        }
    }
    void populateKingMoves  (Position kingPos){
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                if (i == 0 && j == 0) continue;
                addIfOppositeOrEmpty(kingPos, move(kingPos, i, j));
            }
        }
        bool isWhite = isWhitePiece(kingPos);
        if (canCastleKingside(isWhite) && isEmptySpot(move(kingPos, 1, 0))){
            addIfEmpty(kingPos, move(kingPos, 2, 0));
        }
        else if (canCastleQueenside(isWhite) && isEmptySpot(move(kingPos, -1, 0)) && isEmptySpot(move(kingPos, -3, 0))){
            addIfEmpty(kingPos, move(kingPos, -2, 0));
        }
        if (isWhite) this->WhiteKingPosition = kingPos;
        else this->BlackKingPosition = kingPos;
    }
    void populateKnightMoves(Position knightPos){
        for (int i = -1; i <= 1; i+=2){
            for (int j = -1; j <= 1; j+=2){
                addIfOppositeOrEmpty(knightPos, move(knightPos, i*2, j));
                addIfOppositeOrEmpty(knightPos, move(knightPos, i, j*2));
            }
        }
    }
    void populatePieceMoves (Position pos){
        switch (getUncoloredPiece(getPiece(pos))){
            case ' ':  return;
            case 'p':  return populatePawnMoves  (pos);
            case 'n':  return populateKnightMoves(pos);
            case 'b':  return populateBishopMoves(pos);
            case 'r':  return populateRookMoves  (pos);
            case 'q':  return populateQueenMoves (pos);
            case 'k':  return populateKingMoves  (pos);
            default: throw std::runtime_error("Invalid Piece");
        }
    }
    
    bool isPieceBeingAttacked(Position piece) const {
        bool pieceColor = isWhitePiece(piece);
        for (const auto& movePair: this->PossibleMoves){
            if (isWhitePiece(movePair.first) == pieceColor) continue;
            else if (movePair.second.find(piece) != movePair.second.end()) return true;
        }
        return false;
    }
    bool isInCheck(bool isWhite) const{
        return isPieceBeingAttacked(isWhite? this->WhiteKingPosition:this->BlackKingPosition);
    }
    void filterInvalidCastling(){
        Position kingPos = isWhiteToMove()? this->WhiteKingPosition: this->BlackKingPosition;
        int relRow = getRelativeRow(kingPos);
        std::set<Position>& moves = this->PossibleMoves[kingPos];
        // check kingside
        for (int i = -1; i <= 1; i+=2){
            Position adjacentMove = move(kingPos, 1*i, 0);
            Position castlingMove = move(kingPos, 2*i, 0);
            if (moves.find(castlingMove) == moves.end()) continue; // no castling so skip
            else if ( (moves.find(adjacentMove) == moves.end()) // in between covered
                   || (this->toMoveIsChecked) // cant castle if checked
            ) moves.erase(castlingMove); 
            
        }
        // if adjacent move is filtered out, filter out castling too
    }
    void filterInvalidMoves(){
        std::map<Position, std::set<Position>> toRemove;
        std::vector<Position> toRemovePiece;
        bool color = isWhiteToMove();
        // castling into check is automatically filtered
        // however cannot castle when inbetween position is being attacked
        for (const auto& movePair: this->PossibleMoves){
            if (isWhitePiece(movePair.first) != color){ // cannot move this piece as it is not the correct color
                toRemovePiece.push_back(movePair.first);
                continue;
            }
            for (Position moveTo: movePair.second){
                if (getNextPosition(movePair.first, moveTo).isInCheck(color)){ // if this move causes a check in the next turn
                    toRemove[movePair.first].emplace(moveTo);
                }
            }
        }
        for (Position p: toRemovePiece){
            this->PossibleMoves.erase(p);
        }
        for (const auto& movePair: toRemove){
            for (Position move: movePair.second){
                this->PossibleMoves[movePair.first].erase(move);
            }
            if (this->PossibleMoves[movePair.first].size() == 0){
                this->PossibleMoves.erase(movePair.first);
            }
        }
    }
    ChessPosition getNextPosition(Position from, Position to) const{
        ChessPosition nextPosition(*this);

        Piece pieceFrom = getPiece(from);
        Piece pieceType = getUncoloredPiece(pieceFrom);
        Position rowFrom = getRow(from);
        Position colFrom = getCol(from);
        Position rowTo = getRow(to);
        Position colTo = getCol(to);
        Position relRowFrom = getRelativeRow(from);

        bool pieceIsWhite = isWhiteColor(pieceFrom);
        const std::set<Position>& validMoves = PossibleMoves.at(from);

        bool addNewEnpassantPosition = false;
        int dy = getRelativeDy(from);
        // start playing move
        if (pieceType == 'p'){
            if (to == getEnpassantPosition()){ // take enpassant
                nextPosition.killPiece(move(to, 0, pieceIsWhite? 1: -1)); // the only usage of deleting a piece
            }
            else if (rowTo-rowFrom == dy+dy){ // double move
                addNewEnpassantPosition = true;
            }
            nextPosition.resetLastPawnMove();
        }
        else{
            nextPosition.incrementLastPawnMove();
        }

        if (pieceType == 'k'){ // any king move results in castling being disabled
            // move rooks 
            if (colTo-colFrom == 2 && nextPosition.canCastleKingside(pieceIsWhite)){
                nextPosition.MovePieceRaw(getPosition(7, rowFrom), getPosition(5, rowFrom));
            }
            else if (colTo-colFrom == -2 && nextPosition.canCastleQueenside(pieceIsWhite)){
                nextPosition.MovePieceRaw(getPosition(0, rowFrom), getPosition(3, rowFrom));
            }
            nextPosition.disableQueensideCastle(pieceIsWhite);
            nextPosition.disableKingsideCastle(pieceIsWhite);
            
        }
        else if (pieceType == 'r'){ // any rook move disables castling for its side
            if (colFrom == 7 && relRowFrom == 0){
                nextPosition.disableKingsideCastle(pieceIsWhite);
            }
            else if (colFrom == 1 && relRowFrom == 0) {
                nextPosition.disableQueensideCastle(pieceIsWhite);
            }
        }
        nextPosition.clearEnpassantPosition();// reset after each move as you can only enpassant as soon as you get the opportunity

        if (addNewEnpassantPosition)
            nextPosition.setEnpassantPosition(move(from, 0, dy));

        nextPosition.MovePieceRaw(from, to);
        nextPosition.incrementHalfMove();
        nextPosition.PopulateMoves();
        return nextPosition;
    }
    public:
    bool getIsInCheck() const{
        return this->toMoveIsChecked;
    }
    Position getKing() const{
        return isWhiteToMove()? this->WhiteKingPosition: this->BlackKingPosition; 
    }
    std::string MovesToStr(Position pos) const{
        if (PossibleMoves.find(pos) == PossibleMoves.end()) return "[]"; 
        std::ostringstream oss;
        oss << "{";
        int i = 0;
        for (Position p: PossibleMoves.at(pos)){
            if (i++ > 0) oss << ", ";
            oss << getChessPosition(p);
        }
        oss << "}";
        return oss.str();
    }
    void print(Position pos) const{
        std::cout
        << "-----------------\n"
        << "piece        :" << getPiece(pos & 63) << '\n'
        << "moves        :" << MovesToStr(pos) << '\n'
    //    << "isvalidpiece :" << (isValidPiece(getPiece(pos)) ? "True": "False") << '\n' 
    //    << "getrow       :" << (int)getRow(pos) << '\n'
    //    << "getcol       :" << (int)getCol(pos) << '\n' 
        << "getchesspos  :" << getChessPosition(pos) << '\n' 
        << "blackKingPos :" << getChessPosition(this->BlackKingPosition) << '\n' 
        << "whiteKingPos :" << getChessPosition(this->WhiteKingPosition) << '\n' 
    //    << "isvalidpos   :" << (isValidPosition(pos) ? "True": "False") << '\n'
        << "isCheck      :" << (getIsInCheck() ? "True": "False") << '\n'
        << "enpassant    :" << getChessPosition(getEnpassantPosition()) << '\n'
        << "toMove       :" << (isWhiteToMove() ? "White": "Black") << '\n';
    }
    bool update(Position from, Position to){
        if ((PossibleMoves.find(from) == PossibleMoves.end()) ||
            (PossibleMoves[from].find(to) == PossibleMoves[from].end())) return false;
        std::ostringstream moveOss;
        *this = getNextPosition(from, to);
        this->toMoveIsChecked = isInCheck(isWhiteToMove());
        filterInvalidMoves();
        filterInvalidCastling();
        return true;
    }

};