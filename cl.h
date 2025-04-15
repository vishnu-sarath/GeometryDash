#include "main.h"
#include <SFML/Graphics.hpp>

class Animation{
public:
    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime){
        this->imageCount = imageCount;
        

    }
    ~Animation(){}

public:
    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;
    
    float switchTime;
    float totalTime;
};