#ifndef CIRCLE_H
#define CIRCLE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cmath>

enum class adenin_thymin{
    blue = 0,
    red
};
enum class guanin_cytosin{
    green = 0,
    yellow
};
enum class acid_pair{
    adenin_thymin = 0,
    guanin_cytosin = 1
};

enum class Aminoacid{
    adenin = 0,
    thymin,
    guanin,
    cytosin
};
//circle -> sinus und cosinus Kurve 
class circle{
public:
    explicit circle(float r, const sf::Font& font){
        shape.setRadius(r);
        shape.setPosition(10.f, 10.f);
        shape.setOutlineThickness(1.5f);
        shape.setOutlineColor(sf::Color::Black);
        
        acid_str.setFont(font);
        acid_str.setCharacterSize(20);
        acid_str.setFillColor(sf::Color::Black);
        acid_str.setOutlineThickness(1.f);
        acid_str.setOutlineColor(sf::Color::Yellow);
    };
    ~circle(){};
    circle(const circle&) = default;
    circle& operator=(const circle&) = default;

    sf::RectangleShape bindung;
    
    sf::Text acid_str;

    sf::CircleShape shape;

    float dy;

    void change_Y_position_SINUS(const float& win_size_y, const float& time, const float& curve_scale);
    void change_Y_position_COSINUS(const float& win_size_y, const float& time, const float& curve_scale);

    void change_Y_position(const float& win_size_y, const float& AREA_MAX_Y, const float& AREA_MIN_Y);
    void set_phase_offset(const float& amount){
        phase_offset = amount;
    }

    //Base Color für Acid
    void set_acid_type(const Aminoacid& type);
    Aminoacid check_acid_type()const { return acid_type;};
    sf::Color get_color_with_acid_type();

    //gegenüberliegende Farbe / TextStr / Aminoacid AcidType setzen
    sf::Color get_opposite_color_with_acid_type();
    Aminoacid get_opposite_Aminoacid_with_acid_type();
    std::string get_opposite_txt_with_acid_type();
    
private:
    Aminoacid acid_type;
    float step = 0.1f;
    float phase_offset;
    float amplitude = 100.f;
    float frequency = 1.f;
    float offset = 150.f;
};//circle

#endif // CIRCE_H