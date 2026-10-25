#pragma once
#include <map>
#include "textures.h"
#include <sstream>
#include <bitset>

// only one pawn is enpassantable per position so 8 bits for which col, one for pawn color, 4 for castling rights, 1 for moving
// total 16 bits;
const std::map<char, size_t> PieceOrder{
  {'r', 0},
  {'b', 1},
  {'q', 2},
  {'k', 3},
  {'n', 4},
  {'p', 5},
  {'R', 6},
  {'B', 7},
  {'Q', 8},
  {'K', 9},
  {'N', 10},
  {'P', 11}
};

typedef uint8_t Position;
typedef char Piece;


// general use helpers
inline bool isNumeric(std::string s){
    if (s == "") return false;
    for (char c: s){
        if (c > '9' || c < '0') return false;
    }
    return true;
}
inline bool isValidPositionString(std::string s){
    if (s.size() != 2) return false;
    return (s[0] <= 'h' && s[0] >= 'a' && s[1] <= '8' && s[1] >= '1');
}
inline Position getIndexPosition(char col, char num){
    // converts a number 0-63 to chess notation a8-h1
    return ((col-'a')) + (((num-'1'))*8);
}
inline std::string getChessPosition(Position p){
    std::string s = "??";

    if (p == 0xFF) return "NA"; // not any
    if (p >= 64) return s; // invalid 
   
    int rem = p % 8;
    int div = p / 8;
    s[0] = 'a' + rem;
    s[1] = '1' + div;
    return s;
}

class ChessState{
    public:
    Piece* stateArray; // a8 = 0 h1 = 63
    uint16_t numHalfMoves; // max number of FULL moves is 2 ^ 15 = 32768, parity decides who to move
    Position enpassantMove; // position behind enpassantable pawn (0-64)
    uint8_t castlingMove; // who can castle 0000
    uint8_t lastPawnMove; // 50 move rule
    // 64 bytes + 2 bytes + 1 byte + 1 byte + 1 byte = 69 + pad = 70 bytes, I can afford to waste 1 byte.

    void _populateWithFEN(std::string FEN){
        // inputs a valid FEN string
        std::istringstream iss(FEN);

        std::string stateArrayString;    iss >> stateArrayString;
        std::string toMoveString;        iss >> toMoveString;
        std::string castlingMoveString;  iss >> castlingMoveString;
        std::string enpassantMoveString; iss >> enpassantMoveString;
        std::string lastPawnMoveString;  iss >> lastPawnMoveString;
        std::string numFullMovesString;  iss >> numFullMovesString;
        
        // fill state array
        int pieceCounter = 0;
        for (const char c: stateArrayString){
            if ('1' <= c && c <= '8'){
                int numSkips =  c - '0';
                // fill skips with spaces
                for (int i = 0; i < numSkips; i++) stateArray[pieceCounter+i] = ' ';
                pieceCounter += numSkips;
            }
            else if (PieceOrder.find(c) != PieceOrder.end()){
                stateArray[pieceCounter] = c;
                pieceCounter++;
            }
            else if (c != '/')
                throw std::runtime_error("Invalid Character in State Array String");
            // ignore '/'
        }
        // fill num half moves
        if (toMoveString == "b"){
            this->numHalfMoves = 1;
        }
        else if (toMoveString == "w"){
            this->numHalfMoves = 0;
        }
        else {
            throw std::runtime_error("Invalid character in To Move string");
        }
        
        if (!isNumeric(numFullMovesString))
            throw std::runtime_error("Non-Numeric value in Num of full moves");
        else 
            this->numHalfMoves += (stoi(numFullMovesString)-1) * 2; // starts at 1 idk why
        
        // fill enpassant move
        if (isValidPositionString(enpassantMoveString)){
            this->enpassantMove = getIndexPosition(enpassantMoveString[0], enpassantMoveString[1]);
        }
        else if(enpassantMoveString == "-") 
            this->enpassantMove = -1; // aka invalid move;
        else
            throw std::runtime_error("Invalid character in enpassant string");
        
        // fill castling rights 
        this->castlingMove = 0;
        if (castlingMoveString != "-"){
            for (const char c: castlingMoveString){
                switch (c){
                    case 'K': castlingMove |= 0b0001; break;
                    case 'Q': castlingMove |= 0b0010; break;
                    case 'k': castlingMove |= 0b0100; break;
                    case 'q': castlingMove |= 0b1000; break;
                    default: throw std::runtime_error("Invalid character in castling string");
                }
            }
        }
        // fill last pawn move
        if (!isNumeric(lastPawnMoveString))
            throw std::runtime_error("Non-Numeric value in Num of full moves");
        else 
            this->lastPawnMove = stoi(lastPawnMoveString); // starts at 1 idk why
    }
    
    ChessState(std::string FEN){
        _populateWithFEN(FEN);
    };

    ChessState(const ChessState& other){
        for (int i = 0; i < 64; i++) this->stateArray[i] = other.stateArray[i];
        this->numHalfMoves = other.numHalfMoves;
        this->enpassantMove = other.enpassantMove;
        this->castlingMove = other.castlingMove;
        this->lastPawnMove = other.lastPawnMove;
    }

    void operator=(const ChessState& other){
        for (int i = 0; i < 64; i++) this->stateArray[i] = other.stateArray[i];
        this->numHalfMoves = other.numHalfMoves;
        this->enpassantMove = other.enpassantMove;
        this->castlingMove = other.castlingMove;
        this->lastPawnMove = other.lastPawnMove;
    } 
    void print(){
        for (int j = 0; j < 8; j++){
            for (int i = 0; i < 8; i++) std::cout << "+--";
            std::cout << "+" << std::endl;
            for (int i = 0; i < 8; i++){
                std::cout << "| " << stateArray[j*8+i];
            }
            std::cout << "|" << std::endl;
        }
        for (int i = 0; i < 8; i++) std::cout << "+--";
        std::cout << "+" << std::endl;;
        std::cout << "enpassantMove: " << getChessPosition(enpassantMove) << std::endl; // position behind enpassantable pawn (0-64)
        std::cout << " castlingMove: " << std::bitset<4>{castlingMove} << std::endl; // who can castle 0000
        std::cout << "lastPawnMove : " << (int)lastPawnMove  << std::endl; // 50 move rule
        std::cout << "numHalfMoves : " << (int)numHalfMoves  << std::endl; // max number of FULL moves is 2 ^ 15 = 32768, parity is who is to move
    }
};
