#include "circle.h"

/* CIRCLE   */
void circle::change_Y_position_COSINUS(const float& win_size_y, const float& time, const float& curve_scale){
    sf::Vector2f new_pos = shape.getPosition();

    //Cosinusförmige Bewegung: Amplitude * cos(Frequenz * Zeit) + Verschiebung
    new_pos.y = curve_scale * (amplitude * std::cos(frequency * time + phase_offset) + offset);
    
    shape.setPosition(new_pos);
}

void circle::change_Y_position_SINUS(const float& win_size_y, const float& time, const float& curve_scale){
    sf::Vector2f new_pos = shape.getPosition();

    //Sinusförmige Bewegung: Amplitude * sin(Frequenz * Zeit + phase_offset) + Verschiebung
    new_pos.y = curve_scale * (amplitude * std::sin(frequency * time + phase_offset) + offset);
    
    shape.setPosition(new_pos);
}

void circle::change_Y_position(const float& win_size_y, const float& AREA_MAX_Y, const float& AREA_MIN_Y){
    sf::Vector2f new_pos = shape.getPosition();

    new_pos.y += step;

    if(new_pos.y >= AREA_MAX_Y){
        step = -0.1f;
        
    }else if(new_pos.y <= AREA_MIN_Y){
        step = 0.1f;
    }

    dy = shape.getPosition().y + step;
    shape.setPosition(new_pos);
}

void circle::set_acid_type(const Aminoacid& type){
    switch (type){
    case Aminoacid::adenin:
        acid_type = Aminoacid::adenin;
        acid_str.setString("A");
        break;
    case Aminoacid::thymin:
        acid_type = Aminoacid::thymin;
        acid_str.setString("T");
        break;
    case Aminoacid::guanin:
        acid_type = Aminoacid::guanin;
        acid_str.setString("G");
        break;
    case Aminoacid::cytosin:
        acid_type = Aminoacid::cytosin;
        acid_str.setString("C");
        break;
    
    default:
        break;
    }
    acid_type = type;
}

sf::Color circle::get_color_with_acid_type(){
    sf::Color color_temp;
    sf::Color color = shape.getFillColor();

    switch (acid_type){
    case Aminoacid::adenin:
        color_temp = sf::Color::Blue;
        break;
    case Aminoacid::thymin:
        
        color_temp = sf::Color::Red;
        break;
    case Aminoacid::guanin:
        color_temp = sf::Color::Green;
        break;
    case Aminoacid::cytosin:
        color_temp = sf::Color::Yellow;
        break;
    
    default:
        break;
    }
    
    color.r = color_temp.r;
    color.g = color_temp.g;
    color.b = color_temp.b;
    color.a = 255;
    return color;
}

//gegenüberliegende Farbe für die Aminoacid
sf::Color circle::get_opposite_color_with_acid_type(){
    sf::Color color_temp;
    sf::Color color;
    switch (acid_type){
    case Aminoacid::adenin:
        color_temp = sf::Color::Red;
        break;
    case Aminoacid::thymin:
        color_temp = sf::Color::Blue;
        break;
    case Aminoacid::guanin:
        color_temp = sf::Color::Yellow;
        break;
    case Aminoacid::cytosin:
        color_temp = sf::Color::Green;
        break;
    default:
        break;
    }
    color.r = color_temp.r;
    color.g = color_temp.g;
    color.b = color_temp.b;
    color.a = 255;
    return color;
}

//gegenüberliegende Farbe für die Aminoacid
Aminoacid circle::get_opposite_Aminoacid_with_acid_type(){
    
    Aminoacid opposite_type;
    switch (acid_type){
    case Aminoacid::adenin:
        opposite_type = Aminoacid::thymin;
        break;
    case Aminoacid::thymin:
        opposite_type = Aminoacid::adenin;
        break;
    case Aminoacid::guanin:
        opposite_type = Aminoacid::cytosin;
        break;
    case Aminoacid::cytosin:
        opposite_type = Aminoacid::guanin;
        break;
    default:
        break;
    }
    
    return opposite_type;
}
std::string circle::get_opposite_txt_with_acid_type(){
    std::string txt_;
    switch (acid_type){
    case Aminoacid::adenin:
        txt_ = "T";
        break;
    case Aminoacid::thymin:
        txt_ = "A";
        break;
    case Aminoacid::guanin:
        txt_ = "C";
        break;
    case Aminoacid::cytosin:
        txt_ = "G";
        break;
    default:
        break;
    }
    return txt_;
}