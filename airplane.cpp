#include "airplane.h"

void airplane::change_Plane_X_position(const float& win_size_x){
    sf::Vector2f new_pos = sprite_plane.getPosition();

    new_pos.x += step;

    if(new_pos.x >= 500.f){
        step = -0.1f;
        
    }else if(new_pos.x <= 60.f){
        step = 0.1f;
    }

    dx = sprite_plane.getPosition().x + step;
    sprite_plane.setPosition(new_pos);
}