#include <iostream>
#include <raylib.h>

#include "attributes.h"


class ChessPlayerInteraction{
    Vector2 topLeftOffset;
    size_t boardSize;
    
    Position selectedFrom;
    Position selectedTo;
    Position promotionSelection;

    bool hasSelectedSquare;
    bool hasCompletedMove;
    bool mouseIsDownPreviously;

    public:
    ChessPlayerInteraction(Vector2 topLeftOffset, size_t boardSize) 
    : topLeftOffset(topLeftOffset), boardSize(boardSize) {
        this->selectedFrom = -1;
        this->selectedTo = -1;
        this->promotionSelection = -1;

        this->hasSelectedSquare = false;
        this->hasCompletedMove = false;
        this->mouseIsDownPreviously = false;
    }

    bool populatePlayerInput(){
        if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)){
            this->mouseIsDownPreviously = false;    
            return false;
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseIsDownPreviously) return false; // hold click, noclick
        this->mouseIsDownPreviously = true;

        Vector2 mousePosRaw = GetMousePosition();
        Vector2 mousePosRel = {mousePosRaw.x - topLeftOffset.x, mousePosRaw.y - topLeftOffset.y};
        // check bounds
        if ((mousePosRel.x < 0 || mousePosRel.x >= boardSize) ||
            (mousePosRel.y < 0 || mousePosRel.y >= boardSize)) {
            this->hasSelectedSquare = false; // if clicked out can keep making moves
            this->hasCompletedMove = false;
            return true;
        } 
        
        int index = static_cast<int>((mousePosRel.y/boardSize)*8)*8 + static_cast<int>((mousePosRel.x/boardSize)*8);
        
        if (hasSelectedSquare){
            if (this->selectedFrom == index){ // clicked on same spot again
                this->hasSelectedSquare = false; // if clicked out can keep making moves
                this->hasCompletedMove = false;
                return true;
            } 
            this->selectedTo = static_cast<Position>(index);
            this->hasCompletedMove = true;
        }
        else {
            this->selectedFrom = static_cast<Position>(index);
            this->hasCompletedMove = false;
        }
        this->hasSelectedSquare = !this->hasSelectedSquare;
        return true;
    }
    bool promptPromotion(int row, bool isWhite){
        return true;
    }
    bool HasSelectedSquare(){
        return this->hasSelectedSquare;
    }
    bool HasCompletedMove(){
        return this->hasCompletedMove;
    }
    Move getLastMove(){
        if (!this->hasCompletedMove) return {-1, -1};
        return {this->selectedFrom, this->selectedTo};
    }
    Position getHighlightedPosition(){
        if (!this->hasSelectedSquare) return -1;
        return this->selectedFrom;
    }
    void resetMove(){
        this->hasSelectedSquare = false;
        this->hasCompletedMove = false;
    }
    void print(){
        std::cout << 
        "topLeftOffset    : " << this->topLeftOffset.x << ","
                              << this->topLeftOffset.y << "\n"
        "boardSize        : " << this->boardSize << "\n"
        "selectedFrom     : " << getChessPosition(this->selectedFrom)<<"-"
                              << (int)this->selectedFrom << "\n"
        "selectedTo       : " << getChessPosition(this->selectedTo)  <<"-"
                              <<  (int)this->selectedTo  << "\n"
        "hasSelectedSquare: " << this->hasSelectedSquare << "\n"
        "hasCompletedMove : " << this->hasCompletedMove  << "\n"
        << std::endl;
    }
};