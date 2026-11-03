#pragma once

#include <map>
#include <set>
#include <memory>
#include <string>
#include <utility>
#include <raylib.h>
#include <vector>
#include <cmath>

#define DEBUG_MODE
namespace PieceTextureAttribute{
    const float ALPHA_MIN = 0.1f;
}
namespace BoardTextureAttribute{
    const Color DARK_COLOR = {0x41, 0x79, 0x19, 0xFF};
    const Color LIGHT_COLOR = {0xF2,0xEE,0xE2,0xFF};
    const int BRIGHTNESS = 115;
    const int BLUR = 5;
}
namespace SpotTextureAttribute{
    const float RAD_FRACTION = 0.5;
    const Color COLOR = {0x70, 0x70, 0x70, 0xFF};
}
namespace OtherTexturesAttribute{
    const Color MOVED_COLOR    = {0xFF, 0x10, 0x10, 0x90};
    const Color SELECTED_COLOR = {0x10, 0xFF, 0x10, 0x90};
    const Color CHECK_COLOR    = {0x10, 0x10, 0xFF, 0x90}; 

}

typedef char Piece;
typedef uint8_t Position;
typedef std::pair<Position, Position> Move;
typedef std::pair<Position, std::set<Position>> Moves; 

const Position INVALID_POS = 0xFF;

namespace Promote{
    enum choice{
        none,
        queen,
        rook,
        bishop,
        knight
    };
}

inline bool isValidPositionString(std::string s){
    // checks if a position string (like a8) is valid
    if (s.size() != 2) return false;
    return (s[0] <= 'h' && s[0] >= 'a' && s[1] <= '8' && s[1] >= '1');
}
inline bool isValidPosition(Position s){
    // checks if the position is within range
    return s < 64;
}
inline Position getIndexPosition(std::string s){
    // converts chess notation a8-h1 to a number 0-63
    return ((s[0]-'a')) + ((7-(s[1]-'1'))*8);
}
inline std::string getChessPosition(Position p){
    std::string s = "??"; 
    if (p >= 64) return s;
    s[0] = 'a' + (p % 8);
    s[1] = '1' + (7-(p / 8));
    return s;
}

struct PieceAttribute{
    short value;
    unsigned int textureIndex;
    float textureScale;
    PieceAttribute(short value, unsigned int textureIndex, float textureScale) : value(value), textureIndex(textureIndex), textureScale(textureScale) {};
};
const std::string PieceTextureOrder = "rbqknpRBQKNP";
const std::map<Piece, PieceAttribute> PiecesAttributes
{
  {'r', {-5    , 0 , 0.80f}},
  {'b', {-3    , 1 , 0.85f}},
  {'q', {-8    , 2 , 0.90f}},
  {'k', {-32768, 3 , 0.90f}},
  {'n', {-3    , 4 , 0.85f}},
  {'p', {-1    , 5 , 0.80f}},
  {'R', {5     , 6 , 0.80f}},
  {'B', {3     , 7 , 0.85f}},
  {'Q', {8     , 8 , 0.90f}},
  {'K', {32767 , 9 , 0.90f}},
  {'N', {3     , 10, 0.85f}},
  {'P', {1     , 11, 0.80f}}
};
inline bool isNumeric(std::string s){
    if (s == "") return false;
    for (char c: s){
        if (c > '9' || c < '0') return false;
    }
    return true;
}
inline bool isValidPiece(Piece c){
    return (PiecesAttributes.find(c) != PiecesAttributes.end());
}
inline Position getRow(Position p){return p / 8; };
inline Position getCol(Position p){return p % 8; };
inline bool isOppositeColor(Piece left, Piece right){
    return ((left < 'a') ^ (right < 'a')) && left != ' ' && right != ' '; // both lower = false both upper = false
}
inline bool isSameColor(Piece left, Piece right) {
    return !((left < 'a') ^ (right < 'a')) && left != ' ' && right != ' ';
}
inline bool isWhiteColor(Piece p){
    return 'A' < p && 'Z' > p;
}
inline bool isBlackColor(Piece p){
    return 'a' < p && 'z' > p;
}
inline Piece getPieceFromTextureIndex(int index){
    return PieceTextureOrder[index];
}
inline Piece getUncoloredPiece(Piece c){
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}
inline Position getPosition(Position x, Position y){
    return y * 8 + x;
}
