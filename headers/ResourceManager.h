#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <SFML/Graphics.hpp>,

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
        assert(mapping.count(name) > 0);
        return textures[mapping[name]];
    }
    sf::Texture& operator[](const std::string&name){
        return get(name);
    }

};

class FontManager{
    FontManager() = default;
    FontManager(FontManager const&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager const&) = delete;
    FontManager& operator=(FontManager &&) = delete;

    std::vector<sf::Font> fonts;
    std::map<std::string,int> mapping;

public:
   static FontManager& instance() {
        static FontManager myInstance;
        return myInstance;
    }

    void load(const std::string name, const std::string filename){
        fonts.push_back(sf::Font());
        fonts.back().loadFromFile(filename);
        mapping[name] = fonts.size()-1;
    }
    void loadAll();


    sf::Font& get(const std::string& name){
        return fonts[mapping[name]];
    }
    sf::Font& operator[](const std::string&name){
        return get(name);
    }

};

#endif // RESOURCEMANAGER_H
