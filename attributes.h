#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <raylib.h>

#define DEBUG_MODE 

namespace PieceTextureAttribute{
    const float ALPHA_MIN = 0.5f;
}

namespace BoardTextureAttrubute{
    const Color DARK_COLOR = {0x79, 0x41, 0x19, 0xFF};
    const Color LIGHT_COLOR = {0xF2,0xEE,0xE2,0xFF};
    const int BRIGHTNESS = 125;
    const int BLUR = 2;
}

typedef uint8_t Position;
typedef char Piece;

typedef std::pair<Position, Position> Move;

// each piece has each unique:
// identifier (something like 'q')
// index
// scale
// value
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

struct PieceAttribute{
    short value;
    unsigned int textureIndex;
    float textureScale;
};
const std::string PieceTextureOrder = "rbqknpRBQKNP";
const std::map<Piece, PieceAttribute> PiecesAttributes
{
  {'r', (PieceAttribute){-5    , 0 , 0.90f}},
  {'b', (PieceAttribute){-3    , 1 , 0.90f}},
  {'q', (PieceAttribute){-8    , 2 , 0.90f}},
  {'k', (PieceAttribute){-32768, 3 , 0.90f}},
  {'n', (PieceAttribute){-3    , 4 , 0.85f}},
  {'p', (PieceAttribute){-1    , 5 , 0.80f}},
  {'R', (PieceAttribute){5     , 6 , 0.90f}},
  {'B', (PieceAttribute){3     , 7 , 0.90f}},
  {'Q', (PieceAttribute){8     , 8 , 0.90f}},
  {'K', (PieceAttribute){32767 , 9 , 0.90f}},
  {'N', (PieceAttribute){3     , 10, 0.85f}},
  {'P', (PieceAttribute){1     , 11, 0.80f}}
};

bool isValidPiece(char c){
    return (PiecesAttributes.find(c) != PiecesAttributes.end());
}

Piece getPieceFromTextureIndex(int index){
    return PieceTextureOrder[index];
}
