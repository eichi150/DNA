#ifndef AIRPLANE_H
#define AIRPLANE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class airplane{
public:
    airplane(const float& pos_x, const float& pos_y, const float& size_x, const float& size_y){

        body.setSize({size_x, size_y});
        body.setFillColor(sf::Color::Magenta);
        body.setPosition(pos_x, pos_y);
        
        wing_1.setPointCount(3);
        wing_1.setPoint(0, {pos_x + (size_x / 2.f), pos_y - (2.f * size_y)});
        wing_1.setPoint(1, {pos_x, pos_y});
        wing_1.setPoint(2, {pos_x + size_x, pos_y});
        wing_1.setFillColor(sf::Color::Red);

        wing_2.setPointCount(3);
        wing_2.setPoint(0, {pos_x + (size_x / 2.f), pos_y + (3.f * size_y)});
        wing_2.setPoint(1, {pos_x, pos_y + size_y});
        wing_2.setPoint(2, {pos_x + size_x, pos_y + size_y});
        wing_2.setFillColor(sf::Color::Red);
        
        rendtex.create(140, 100);
        rendtex.clear(sf::Color::Transparent);
        rendtex.draw(wing_1);
        rendtex.draw(wing_2);
        rendtex.draw(body);
        rendtex.display();

        sprite_plane.setTexture(rendtex.getTexture());
        sprite_plane.setPosition(pos_x, pos_y);
    };
    ~airplane(){};
    airplane(const airplane&) = default;
    airplane& operator=(const airplane&) = default;
    
    sf::Sprite sprite_plane;

    void change_Plane_X_position(const float& win_size_x);

private:
    sf::RenderTexture rendtex;
    sf::RectangleShape body;
    sf::ConvexShape wing_1;
    sf::ConvexShape wing_2;
    float dx;
    float step = 0.1f;
};//airplane

#endif // AIRPLANE_H