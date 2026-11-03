#pragma once
#include <set>
#include <sstream>
#include <bitset>
#include <iostream>
#include <array>
#include "attributes.h"


class ChessState{
    // class for storing chess states
    private:
    std::array<Piece, 64> stateArray; // a8 = 0 h1 = 63
    uint16_t numHalfMoves; // max number of FULL moves is 2 ^ 15 = 32768, parity decides who to move
    Position enpassantMove; // position behind enpassantable pawn (0-64)
    uint8_t castlingMove; // who can castle 0000
    uint8_t lastPawnMove; // 50 move rule // small counter

    public:
    void populateWithFEN(std::string FEN){
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
            else if (isValidPiece(c)){
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
            this->enpassantMove = getIndexPosition(enpassantMoveString.substr(0, 2));
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
    std::string getFEN(){
        std::ostringstream oss;
        int spaceCount;
        char curPiece;
        // piece array
        for (int i = 0; i < 64; i+=8){
            if (i > 0) oss << "/";
            spaceCount = 0;
            for (int j = 0; j < 8; j++){
                curPiece = this->stateArray[j+i];
                if (curPiece == ' '){
                    spaceCount++;
                }
                else{
                    if (spaceCount > 0){
                        oss << spaceCount;
                    }
                    spaceCount = 0;
                    oss << curPiece;
                }
            }
            if (spaceCount > 0){
                oss << spaceCount;
            }
        }
        // to move
        oss << " ";
        if (isWhiteToMove()){
            oss << "w";
        }
        else {
            oss << "b";
        }
        
        // castling
        oss << " ";
        if (castlingMove){
            if ((castlingMove & 0b0001) == 0b0001) oss << "K";
            if ((castlingMove & 0b0010) == 0b0010) oss << "Q";
            if ((castlingMove & 0b0100) == 0b0100) oss << "k";
            if ((castlingMove & 0b1000) == 0b1000) oss << "q";
        }
        else{
            oss << "-";
        }
        // enpassant
        oss << " ";
        if (isValidPosition(this->enpassantMove)){
            oss << getChessPosition(this->enpassantMove);
        }
        else {
            oss << "-";
        }
        // num pawn moves
        oss << " ";
        oss << (int)this->lastPawnMove;

        // num full moves
        oss << " ";
        oss << (int)(this->numHalfMoves)/2+1;

        return oss.str();
    }
    ChessState(std::string FEN){
        populateWithFEN(FEN);
    };
    ChessState(const ChessState& other){
        for (int i = 0; i < 64; i++) this->stateArray[i] = other.stateArray[i];
        this->numHalfMoves = other.numHalfMoves;
        this->enpassantMove = other.enpassantMove;
        this->castlingMove = other.castlingMove;
        this->lastPawnMove = other.lastPawnMove;
    }
    ChessState& operator=(const ChessState& other){
        if (this == &other) return *this;
        for (int i = 0; i < 64; i++) this->stateArray[i] = other.stateArray[i];
        this->numHalfMoves =  other.numHalfMoves;
        this->enpassantMove = other.enpassantMove;
        this->castlingMove =  other.castlingMove;
        this->lastPawnMove =  other.lastPawnMove;
        return *this;
    }
    void printRawBoard(){
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
    }
    void print() const{
        //printRawBoard();
        std::cout 
        << "enpassantMove: " << getChessPosition(enpassantMove) << '\n'// position behind enpassantable pawn (0-64)
        << " castlingMove: " << std::bitset<4>{castlingMove} << '\n' // who can castle 0000
        << "lastPawnMove : " << (int)lastPawnMove << '\n'// 50 move rule
        << "numHalfMoves : " << (int)numHalfMoves << '\n' // max number of FULL moves is 2 ^ 15 = 32768, parity is who is to move
        << std::endl;
    }
    inline Piece getPiece(Position indexPosition) const{
        return this->stateArray[indexPosition];
    }
    inline bool isEmptySpot(Position indexPosition) const{
        return getPiece(indexPosition) == ' ';
    }
    inline bool isWhitePiece(Position indexPosition) const{
        return isWhiteColor(getPiece(indexPosition));
    }
    inline bool isBlackPiece(Position indexPosition) const{
        return isBlackColor(getPiece(indexPosition));
    }
    inline bool isWhiteToMove() const{
        return this->numHalfMoves % 2 == 0;
    }
    inline bool isPieceToMove(Position indexPosition) const{
        return (isWhiteToMove() && isWhitePiece(indexPosition)) || (!isWhiteToMove() && isBlackPiece(indexPosition));
    }
    inline bool canCastleQueenside(bool isWhite) const{
        if (isWhite) return castlingMove & 0b0010;
        else return castlingMove & 0b1000;
    }
    inline bool canCastleKingside(bool isWhite) const{
        if (isWhite) return castlingMove & 0b0001;
        else return castlingMove & 0b0100;
    }
    inline Position getEnpassantPosition() const {
        return this->enpassantMove; 
    }
    inline int getLastPawnMove() const{
        return this->lastPawnMove;
    }
    protected:
    inline void clearEnpassantPosition() {
        this->enpassantMove = INVALID_POS; 
    }
    inline void setEnpassantPosition(Position newPos) {
        this->enpassantMove = newPos; 
    }
    inline void incrementHalfMove() {
        this->numHalfMoves++;
    }
    inline void incrementLastPawnMove(){
        this->lastPawnMove++;
    }
    inline void resetLastPawnMove(){
        this->lastPawnMove = 0;
    }
    inline void disableQueensideCastle(bool isWhite){
        if (isWhite) castlingMove &= 0b1101;
        else castlingMove &= 0b0111;
    }
    inline void disableKingsideCastle(bool isWhite){
        if (isWhite) castlingMove &= 0b1110;
        else castlingMove &= 0b1011;
    }
    inline void killPiece(Position pos){
        this->stateArray[pos] = ' ';
    }
    inline void MovePieceRaw(Position from, Position to){
        this->stateArray[to] = this->stateArray[from];
        this->stateArray[from] = ' ';
    }
    inline void PromotePiece(Position from, Position to, Promotion::choice choice){
        switch(choice){
            case Promotion::queen:  this->stateArray[to] = isWhitePiece(from)? 'Q':'q'; break;
            case Promotion::rook:   this->stateArray[to] = isWhitePiece(from)? 'R':'r'; break;
            case Promotion::bishop: this->stateArray[to] = isWhitePiece(from)? 'B':'b'; break;
            case Promotion::knight: this->stateArray[to] = isWhitePiece(from)? 'N':'n'; break;
            default: throw std::runtime_error("Invalid Promotion Choice");
        }
        this->stateArray[from] = ' ';
    }
};
