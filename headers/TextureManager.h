#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class TextureManager{
    TextureManager() = default;
    TextureManager(TextureManager const&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager const&) = delete;
    TextureManager& operator=(TextureManager &&) = delete;

    std::vector<sf::Texture> textures;
    std::map<std::string,int> mapping;

public:
   static TextureManager& instance() {
        static TextureManager myInstance;
        return myInstance;
    }

    void load(const std::string name, const std::string filename, const sf::IntRect area = sf::IntRect()){
        textures.push_back(sf::Texture());
        textures.back().loadFromFile(filename,area);
        mapping[name] = textures.size()-1;
    }
    void loadAll();

    sf::Texture& get(const std::string& name){
        return textures[mapping[name]];
    }
    sf::Texture& operator[](const std::string&name){
        return get(name);
    }

};

#endif // TEXTUREMANAGER_H
