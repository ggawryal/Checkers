#include "ResourceManager.h"

void TextureManager::loadAll(){
    TextureManager::load("blackCell 1","data/graph/board.png",sf::IntRect(0,0,89,89));
    TextureManager::load("whiteCell 1","data/graph/board.png",sf::IntRect(89,0,89,89));

    TextureManager::load("blackPawn 1","data/graph/checkers.png",sf::IntRect(0,0,89,89));
    TextureManager::load("whitePawn 1","data/graph/checkers.png",sf::IntRect(89,0,89,89));

    TextureManager::load("blackQueen 1","data/graph/checkers.png",sf::IntRect(0,89,89,89));
    TextureManager::load("whiteQueen 1","data/graph/checkers.png",sf::IntRect(89,89,89,89));
}

void FontManager::loadAll(){
    FontManager::load("arial","data/fonts/arial.ttf");
}
