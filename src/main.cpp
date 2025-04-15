#include <SFML/Graphics.hpp>


class Animation;
class Player;
class Collider;

class Collider{
    public:
    Collider(sf::RectangleShape& Body):
        body(Body)
    {}
    ~Collider(){}
    void move(float dx, float dy){
        body.move({dx,dy});
    }
    bool checkCollision(Collider& other, float push){
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


class Animation{
    public:
        sf::IntRect uvRect;

    private:
        sf::Vector2u imageCount;
        sf::Vector2u currentImage;
        
        float switchTime;
        float totalTime;
    
    public:
        Animation(sf::Texture *texture, sf::Vector2u imagecount, float switchtime){
            this->imageCount = imagecount;
            this->switchTime = switchtime;
            totalTime = 0.0f;
            currentImage.x =  0;
    
            uvRect.size.x = texture->getSize().x / (float)imagecount.x;
            uvRect.size.y = texture->getSize().y / (float)imagecount.y;
        }
        ~Animation(){}
    
        void update(int row, float deltaTime, bool faceRight){
            currentImage.y = row;
            totalTime += deltaTime;
    
            if(totalTime >= switchTime){
                totalTime -=switchTime;
                currentImage.x ++;
    
                if(currentImage.x >= imageCount.x){
                    currentImage.x = 0;
                }
            }
    
            uvRect.position.y = currentImage.y * uvRect.size.y;
            if(faceRight){
                uvRect.position.x = currentImage.x * uvRect.size.x;
                uvRect.size.x = abs(uvRect.size.x);
            }
            else{
                uvRect.position.x = (currentImage.x+1) * abs(uvRect.size.x);
                uvRect.size.x = -abs(uvRect.size.x);
            }
    
        }
        
    };


class Platform{
private:
    sf::RectangleShape body;

public:
    Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position){
        body.setSize(size);
        body.setOrigin(size / 2.0f);
        body.setTexture(texture);
        body.setPosition(position);

    }
    void draw(sf::RenderWindow& window){
        window.draw(body);
    }
    Collider getCollider(){
        return Collider(body);
    }

};
    
class Player{
    private:
            sf::RectangleShape body;
            Animation animation;
            unsigned int row;
            float speed;
            bool faceRight;
    public:
    Player(sf::Texture *texture, sf::Vector2u imagecount, float switchtime, float Speed):
    animation(texture, imagecount, switchtime)
    {
        this->speed = Speed;
        row = 0;
        faceRight = true;
        body.setSize({100,100});
        body.setOrigin(body.getSize()/ 2.0f);
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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
            movement.y -= speed * deltaTime;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
            movement.y += speed * deltaTime;}
    
        if(movement.x ==0.0f){
            row = 0;}
        else{
            row = 1;
            if(movement.x > 0){
                faceRight = true;
            }
            else{
                faceRight = false;
            }
        }
    
        animation.update(row, deltaTime, faceRight);
        body.setTextureRect(animation.uvRect);
        body.move(movement);
    }
    
    
    void draw(sf::RenderWindow& window){
        window.draw(body);
    }
    sf::Vector2f getposition(){
        return body.getPosition();
    }
    Collider getCollider(){
        return Collider(body);       
    }
    
};







static const float VIEW_HEIGHT = 512.0f;

void ResizeView(const sf::RenderWindow& window, sf::View& view ){
    float aspectratio =float(window.getSize().x ) / float(window.getSize().y);
    view.setSize({VIEW_HEIGHT* aspectratio, VIEW_HEIGHT});
}
int main()
{
    sf::RenderWindow window(sf::VideoMode({800,600}), "My Window", sf::Style::Close   | sf::Style::Resize);
    sf::View view({0.0f, 0.0f},{512.0f, 512.0f});
    sf::Texture playerTexture("sprite.png");
    //window.setFramerateLimit(24);
    Player player(&playerTexture,{8, 4}, 0.12f, 100.0f);
    //playerTexture.setRepeated(true);
    Platform platform1(nullptr, {100.0f, 100.0f},{50.0f,0.0f});
    //Platform platform2(nullptr, {200.0f, 400.0f},{0.0f,300.0f});

    float deltaTime = 0.0f;
    sf::Clock clock;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
            window.close();
            }
            if (event->is<sf::Event::Resized>()){
                ResizeView(window, view);
            }  
        }

        player.update(deltaTime);
        Collider* col = player.getCollider(); 

    platform1.getCollider().checkCollision(col, 0.5f);
        //platform2.getCollider().checkCollision(player.getCollider(), 1.0f);
        
        view.setCenter(player.getposition());

        window.clear(sf::Color::Blue);
        window.setView(view);
        platform1.draw(window);
        //platform2.draw(window);
        player.draw(window);
        window.display();
    
    }
}

