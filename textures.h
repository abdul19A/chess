#pragma once

#include <string>
#include <array>
#include <iostream>
#include <memory>

#include "raylib.h"
#include "states.h"

const float ALPHA_MIN = 0.01f;
const Color DARK_COLOR = {0x79, 0x41, 0x19, 0xFF};
const Color LIGHT_COLOR = {0xF2,0xEE,0xE2,0xFF};
const int BRIGHTNESS = 125;
const int BLUR = 2;

class ChessTexture{
    std::string piecesFilePath;

    std::array<Texture2D, 12> pieceTextures;
    Texture2D boardTexture;
    size_t pieceScale;
    size_t boardScale;

    size_t boardGridSize;

    bool isLoaded;

    static void _resizeImage(Image* img, size_t scale) {
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
        if (!IsImageValid(chesspiecesImg)) throw std::runtime_error("Invalid Pieces Image");
        float dx = chesspiecesImg.width/6.0f; // num pieces
        float dy = chesspiecesImg.height/2.0f; // num colors
        Image pieceImg;
        for (int y = 0; y < 2; y++){
            for (int x = 0; x < 6; x++){
                pieceImg = ImageFromImage(chesspiecesImg, {dx*x,dy*y,dx, dy});
                
                ImageAlphaCrop(&pieceImg, ALPHA_MIN);
                _resizeImage(&pieceImg, pieceScale);
                
                if (!IsImageValid(pieceImg)) throw std::runtime_error("Invalid Piece Image");
                this->pieceTextures[x+y*6] = LoadTextureFromImage(pieceImg);
                
                UnloadImage(pieceImg);
            }
        }
    }
    void _readBoardImage(){
        Image chessboardImg = GenImagePerlinNoise(boardScale, boardScale, 0, 0, 4.0f);
        Image whiteImg = GenImagePerlinNoise(boardScale, boardScale, boardScale*2, boardScale*2, 4.0f);

        ImageColorBrightness(&chessboardImg, BRIGHTNESS);   
        ImageBlurGaussian(&chessboardImg, BLUR);  
        ImageColorTint(&chessboardImg, DARK_COLOR);    

        ImageColorBrightness(&whiteImg, BRIGHTNESS);   
        ImageBlurGaussian(&whiteImg, BLUR);    
        ImageColorTint(&whiteImg, LIGHT_COLOR);    
        
        _resizeImage(&chessboardImg, boardScale);
        _resizeImage(&whiteImg, boardScale); 
        
        this->boardGridSize = this->boardScale/8;
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

    public:
    ChessTexture(std::string piecesFilePath, size_t pieceScale, size_t boardScale)
     : piecesFilePath(piecesFilePath), pieceScale(pieceScale), boardScale(boardScale){
        _readPiecesImage();
        _readBoardImage();
        this->isLoaded = true;
    }
    Texture2D& getPieceTexture(int i){
        if (!isLoaded) throw std::runtime_error("Textures not initialized");
        return this->pieceTextures[i];
    }
    Texture2D& getBoardTexture(){
        if (!isLoaded) throw std::runtime_error("Textures not initialized");
        return this->boardTexture;
    }
    
};