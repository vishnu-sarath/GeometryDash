#include "main.cpp"
#include <SFML/Graphics.hpp>

class Animation{
public:
    Animation(sf::Texture *texture, sf::Vector2u imagecount, float switchtime){
        this->imageCount = imagecount;
        this->switchTime = switchtime;
        totalTime = 0.0f;
        currentImage.x =  0;

        uvRect.width = texture->getsize().x / (float)imageCount.x;
        uvRect.height = texture->getsize().y / (float)imageCount.y;
    }
    ~Animation(){}

    void update(int row, float deltaTime){
        currentImage.y = row;
        totalTime += deltaTime;

        if(totalTime >= switchTime){
            totaltime -=switchTime;
            currentImage.x ++;

            if(currentImage.x >= imageCount.x){
                currentImage.x = 0;
            }
        }

        uvRect.left = currentImage.x * uvRect.width;
        uvRect.top = currentImage.y * uvRect.height;

    }

public:
    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;
    
    float switchTime;
    float totalTime;
};

class Player{
public:
    Player(sf::Texture *texture, sf::Vector2u imagecount, float switchtime, float Speed):
    animation(texture, imagecount, switchtime)
    {
        Speed = speed;
        row = 0;
        faceRight = true;

        body.setSize({100,150});
    sf::Texture playerTexture("Idle.png");
    body.setPosition({400 , 300});
    body.setTexture(texture);
    }
    void update(float deltaTime){
        sf::Vector2f movement(0.0f, 0.0f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
            movement.x += speed * deltaTime;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            movement.x -= speed * deltaTime;}

        if(movement.x ==0.0f){
            row = 0;
        else{
            row = 1;
            if(movement.x > 0){
                faceRight = true;}
            else{
                faceRight = false;}
        }

        animation.update(row, deltaTime, faceRight);
        body.setTextureRect(animation.uvRect);
        body.move(movement);
    }


    void draw(sf::RenderWindow window){
        window.draw(body);
    }
private:
    sf::RectangleShape body;
    Animation animation;
    unsigned int row;
    float speed;
    bool faceRight;

};



class Collider{
    public:
    Collider(sf::RectangleShape& body):
    body(body)
    {
    }
    void move(float dx, float dy){
        body.move({dx,dy});
    }
    bool checkCollision( Collider& other, float push){
        sf::Vector2f otherPosition = other.Getposition();
        sf::Vector2f otherHalfSize = other.GetHalfSize();
        sf::Vector2f thisPosition = Getposition();
        sf::Vector2f thisHalfSize = GetHalfSize();
        float deltax = otherPosition.x-thisPosition.x;
        float deltay = otherPosition.y-thisPosition.y;
        
        float intersectx = abs(deltax) - (otherPosition.x + thisPosition.x);
        float intersecty = abs(deltay) - (otherPosition.y + thisPosition.y);
        if(intersectx<0.0f && intersecty<0.0f){
            push = std::min(std::max(push,0.0f),1.0f);
            if(intersectx > intersecty){
                if(deltax>0.0f){
                    move(intersectx*(1.0f - push), 0.0f);
                    other.move(-intersectx * push, 0.0f);
                }
                else{
                    move(-intersectx*(1.0f - push), 0.0f);
                    other.move(intersectx * push, 0.0f);
                }
                
            }
            else{
                if(deltay>0.0f){
                    move(0.0f, intersecty*(1.0f - push));
                    other.move(0.0f, -intersecty * push);
                }
                else{
                    move( 0.0f, -intersecty*(1.0f - push));
                    other.move( 0.0f, intersecty * push);
                }
                
            }
            return true;
        }
        return false;
    }
    sf::Vector2f Getposition(){
        return body.getPosition();
    }
    sf::Vector2f GetHalfSize(){
        return body.getSize() / 2.0f;
    }
    
    private:
        sf::RectangleShape& body;
    
};