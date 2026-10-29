#include "states.h"
#include "attributes.h"
// read 70 bytes -> write into 70 bytes

class ChessPosition : public ChessState{
    std::map<Position, std::set<Position>> PossibleMoves;

    public:
    ChessPosition(std::string FEN): ChessState(FEN){
        PopulateMoves();
    }
    void PopulateMoves(){
        PossibleMoves.clear();
        for (Position pos = 0; pos < 64; pos++){
            populatePieceMoves(pos);
        }
    }
    const std::set<Position>& getMoves(Position pos){
        return PossibleMoves[pos];
    }
    private:
    inline Position move(Position from, int dx, int dy){
        int x = dx + (from % 8);
        if (x >= 8 || x < 0) return INVALID_POS;
        int y = dy + (from / 8);
        if (y >= 8 || y < 0) return INVALID_POS;
        return static_cast<int>(x + y * 8);
    }
    inline int getRelativeRow(Position from){
        if (isWhitePiece(from)){
            return 7 - getRow(from);
        }
        return getRow(from);
    }
    inline int getRelativeDy(Position from){
        if (isWhitePiece(from)){
            return -1;
        }
        return 1;
    }
    inline void addIfEmpty(Position from, Position to){
        if (getPiece(to) == ' ') PossibleMoves[from].emplace(to);
    }
    inline void addIfOpposite(Position from, Position to){
        if (isOppositeColor(getPiece(from), getPiece(to))) PossibleMoves[from].emplace(to);
    }
    inline void addIfOppositeOrEmpty(Position from, Position to){
        if (getPiece(to) == ' ' || isOppositeColor(getPiece(from), getPiece(to))) PossibleMoves[from].emplace(to);
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
                 &&    (getRow(getEnpassantPosition()) - row == dy)   // must be 1 forward
                 && abs(getCol(getEnpassantPosition()) - col == 1 )){ // must be 1 left or 1 right
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
    }
    void populateKnightMoves(Position knightPos){
        Moves res;
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
    
    public:
    std::string MovesToStr(Position pos){
        std::ostringstream oss;
        oss << "{";
        for (Position p: PossibleMoves[pos]){
            oss << getChessPosition(p&63) << ", ";
        }
        oss << "}";
        return oss.str();
    }
    void print(Position pos){
        std::cout
        << "piece        :" << getPiece(pos & 63) << '\n'
        << "moves        :" << MovesToStr(pos) << '\n'
        << "isvalidpiece :" << (isValidPiece(getPiece(pos)) ? "True": "False") << '\n' 
        << "getrow       :" << (int)getRow(pos) << '\n'
        << "getcol       :" << (int)getCol(pos) << '\n' 
        << "getchesspos  :" << getChessPosition(pos) << '\n' 
        << "isvalidpos   :" << (isValidPosition(pos) ? "True": "False") << '\n';
        
    }
};