#pragma once

#include <string>
#include <array>
#include <iostream>
#include <memory>
#include <map>
#include <raylib.h>

#include "attributes.h"

class ChessTexture{
    std::string piecesFilePath;

    std::array<Texture2D, 12> pieceTextures;
    Texture2D boardTexture;
    Texture2D selectedTexture;
    Texture2D movedTexture;
    Texture2D checkTexture;
    Texture2D spotTexture;
    Texture2D promotionTexture;
    
    float boardSize;
    float boardGridSize;

    bool isLoaded;

    static void _resizeImage(Image* img, float scale) {
        if (img->width > img->height){
            ImageResize(img, scale, static_cast<float>(img->height)/img->width * scale);
        }
        else {
            ImageResize(img, static_cast<float>(img->width)/img->height * scale, scale);
        }
    }

    void _readPiecesImage(){
        // column ->[R, B, Q, K, N, P] row -> [black, white]
        Image chesspiecesImg = LoadImage(piecesFilePath.c_str());
        if (!IsImageValid(chesspiecesImg)) throw std::runtime_error("Invalid Pieces Image: " + piecesFilePath);
        float dx = chesspiecesImg.width/6.0f; // num pieces
        float dy = chesspiecesImg.height/2.0f; // num colors
        Image pieceImg;
        Piece piece;
        for (int y = 0; y < 2; y++){
            for (int x = 0; x < 6; x++){
                pieceImg = ImageFromImage(chesspiecesImg, {dx*x,dy*y, dx, dy});
                piece = getPieceFromTextureIndex(x+y*6);

                ImageAlphaCrop(&pieceImg, PieceTextureAttribute::ALPHA_MIN);
                _resizeImage(&pieceImg, PiecesAttributes.at(piece).textureScale * boardGridSize);
                
                if (!IsImageValid(pieceImg)) throw std::runtime_error("Invalid Piece Image");
                this->pieceTextures[x+y*6] = LoadTextureFromImage(pieceImg);
                
                UnloadImage(pieceImg);
            }
        }
    }
    void _readBoardImage(){

        Image chessboardImg = GenImagePerlinNoise(boardSize, boardSize, 0, 0, 4.0f);
        Image whiteImg = GenImagePerlinNoise(boardSize, boardSize, boardSize*2, boardSize*2, 4.0f);

        ImageColorBrightness(&chessboardImg, BoardTextureAttribute::BRIGHTNESS);   
        ImageBlurGaussian(&chessboardImg, BoardTextureAttribute::BLUR);  
        ImageColorTint(&chessboardImg, BoardTextureAttribute::DARK_COLOR);    

        ImageColorBrightness(&whiteImg, BoardTextureAttribute::BRIGHTNESS);   
        ImageBlurGaussian(&whiteImg, BoardTextureAttribute::BLUR);    
        ImageColorTint(&whiteImg, BoardTextureAttribute::LIGHT_COLOR);    
        
        _resizeImage(&chessboardImg, boardSize);
        _resizeImage(&whiteImg, boardSize); 
        
        float ds = this->boardGridSize;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if ((i + j) % 2) continue; // skip odd
                ImageDrawImageRec(&chessboardImg, whiteImg, {ds*i, ds*j, ds, ds}, {ds*i, ds*j}, WHITE);
            }
        }

        if (!IsImageValid(chessboardImg)) throw std::runtime_error("Invalid Board Image");
        this->boardTexture = LoadTextureFromImage(chessboardImg);
        
        UnloadImage(chessboardImg);
        UnloadImage(whiteImg);
    }
    void _createOtherTextures(){
        Image movedImage =     GenImageColor(this->boardGridSize, this->boardGridSize, OtherTexturesAttribute::MOVED_COLOR   );
        Image selectedImage =  GenImageColor(this->boardGridSize, this->boardGridSize, OtherTexturesAttribute::SELECTED_COLOR);
        Image checkImage =     GenImageColor(this->boardGridSize, this->boardGridSize, {0x0, 0x0, 0x0, 0x0}); //OtherTexturesAttribute::CHECK_COLOR   );
        Image spotImage =      GenImageColor(this->boardGridSize, this->boardGridSize, {0x0, 0x0, 0x0, 0x0});
        Image promotionImage = GenImageColor(this->boardGridSize, this->boardGridSize*4, OtherTexturesAttribute::PROMOTION_COLOR);
        
        // draw circle spot
        ImageDrawCircle(&spotImage,
             this->boardGridSize/2, this->boardGridSize/2, 
             this->boardGridSize*SpotTextureAttribute::RAD_FRACTION/2, SpotTextureAttribute::COLOR);
        
        ImageDrawCircleGradient(&checkImage,
            {this->boardGridSize/2, this->boardGridSize/2},
             this->boardGridSize/2, 
             OtherTexturesAttribute::CHECK_COLOR, 
             ColorLerp({0x0, 0x0, 0x0, 0x0}, OtherTexturesAttribute::CHECK_COLOR, OtherTexturesAttribute::CHECK_OPACITY));

        this->movedTexture     = LoadTextureFromImage(movedImage    );  
        this->selectedTexture  = LoadTextureFromImage(selectedImage );  
        this->checkTexture     = LoadTextureFromImage(checkImage    );  
        this->spotTexture      = LoadTextureFromImage(spotImage     );  
        this->promotionTexture = LoadTextureFromImage(promotionImage);

        UnloadImage(movedImage    );
        UnloadImage(selectedImage );
        UnloadImage(checkImage    );
        UnloadImage(spotImage     );
        UnloadImage(promotionImage);
    }
    public:
    ChessTexture(std::string piecesFilePath, float boardSize)
     : piecesFilePath(piecesFilePath), boardSize(boardSize){
        this->boardGridSize = this->boardSize/8;
        _readPiecesImage();
        _readBoardImage();
        _createOtherTextures();
        this->isLoaded = true;
    }
    const Texture2D& getPieceTexture(int i){
        return this->pieceTextures[i];
    }
    const Texture2D& getPieceTexture(Piece p){
        return this->pieceTextures[PiecesAttributes.at(p).textureIndex];
    }
    const Texture2D& getBoardTexture(){
        return this->boardTexture;
    }
    const Texture2D& getSelectedTexture(){
        return this->selectedTexture;
    }
    const Texture2D& getMovedTexture(){
        return this->movedTexture;
    }
    const Texture2D& getCheckTexture(){
        return this->checkTexture;
    }
    const Texture2D& getSpotTexture(){
        return this->spotTexture;
    }
    const Texture2D& getPromotionTexture(){
        return this->promotionTexture;
    }
    const std::array<Texture2D, 12>& getAllPiecesTexture(){
        return this->pieceTextures;
    }
    const float getGridSize(){
        return this->boardGridSize;
    }

};