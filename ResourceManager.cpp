#include "ResourceManager.h"

void TextureManager::loadAll(){
    TextureManager::load("blackCell 1","data/graph/board.png",sf::IntRect(0,0,89,89));
    TextureManager::load("whiteCell 1","data/graph/board.png",sf::IntRect(89,0,89,89));
    TextureManager::load("cells 1","data/graph/board.png",sf::IntRect(0,0,178,89));


    TextureManager::load("blackCell 2","data/graph/board.png",sf::IntRect(0,89,89,89));
    TextureManager::load("whiteCell 2","data/graph/board.png",sf::IntRect(89,89,89,89));
    TextureManager::load("cells 2","data/graph/board.png",sf::IntRect(0,89,178,89));

    TextureManager::load("blackCell 3","data/graph/board.png",sf::IntRect(0,178,89,89));
    TextureManager::load("whiteCell 3","data/graph/board.png",sf::IntRect(89,178,89,89));
    TextureManager::load("cells 3","data/graph/board.png",sf::IntRect(0,178,178,89));

    TextureManager::load("blackCell 4","data/graph/board.png",sf::IntRect(0,267,89,89));
    TextureManager::load("whiteCell 4","data/graph/board.png",sf::IntRect(89,267,89,89));
    TextureManager::load("cells 4","data/graph/board.png",sf::IntRect(0,267,178,89));

    TextureManager::load("blackCell 5","data/graph/board.png",sf::IntRect(0,356,89,89));
    TextureManager::load("whiteCell 5","data/graph/board.png",sf::IntRect(89,356,89,89));
    TextureManager::load("cells 5","data/graph/board.png",sf::IntRect(0,356,178,89));


    TextureManager::load("blackPawn 1","data/graph/checkers.png",sf::IntRect(0,0,89,89));
    TextureManager::load("whitePawn 1","data/graph/checkers.png",sf::IntRect(89,0,89,89));
    TextureManager::load("blackQueen 1","data/graph/checkers.png",sf::IntRect(0,89,89,89));
    TextureManager::load("whiteQueen 1","data/graph/checkers.png",sf::IntRect(89,89,89,89));

    TextureManager::load("blackPawn 2","data/graph/checkers.png",sf::IntRect(0,178,89,89));
    TextureManager::load("whitePawn 2","data/graph/checkers.png",sf::IntRect(89,178,89,89));
    TextureManager::load("blackQueen 2","data/graph/checkers.png",sf::IntRect(0,267,89,89));
    TextureManager::load("whiteQueen 2","data/graph/checkers.png",sf::IntRect(89,267,89,89));

    TextureManager::load("blackPawn 3","data/graph/checkers.png",sf::IntRect(0,356,89,89));
    TextureManager::load("whitePawn 3","data/graph/checkers.png",sf::IntRect(89,356,89,89));
    TextureManager::load("blackQueen 3","data/graph/checkers.png",sf::IntRect(0,445,89,89));
    TextureManager::load("whiteQueen 3","data/graph/checkers.png",sf::IntRect(89,445,89,89));

}

void FontManager::loadAll(){
    FontManager::load("arial","data/fonts/arial.ttf");
}
