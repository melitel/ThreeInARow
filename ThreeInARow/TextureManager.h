#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

class TextureManager
{
public:

    static sf::Texture& GetTexture(const std::string& filename) {
        auto it = m_textures.find(filename);

        if (it != m_textures.end()) {
            // Texture already loaded, return it
            return *it->second;
        }

        // Texture not found, load it
        sf::Texture* texture = new sf::Texture();
        if (texture->loadFromFile(filename)) {
            // Successfully loaded, add it to the cache
            m_textures[filename] = texture;
            return *texture;
        }

        // Failed to load texture, return a default texture or handle the error
        // For simplicity, we'll return a placeholder texture here
        static sf::Texture placeholderTexture;
        return placeholderTexture;
    }

private:

    static std::unordered_map<std::string, sf::Texture*> m_textures;
};

