#include "collider.h"
    Collider::Collider(sf::RectangleShape& body):
    body(body)
    {
    }

    bool Collider::checkCollision( Collider& other, float push){
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
    
