#ifndef ANIMATOR_H
#define ANIMATOR_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cmath>

#include <chrono>
#include <ctime>

#include <string>
#include <sstream>
#include <iomanip>

#include <random>
#include <vector>
#include <bitset>
#include <map>

#include "circle.h"
#include "airplane.h"

constexpr float Y_START = 60.f;
constexpr float adj_size = 1.f; //adj_size * radius
constexpr float curve_scale = 1.2f;

constexpr float AREA_MIN_Y = 50.f;
constexpr float AREA_MAX_Y = 250.f * curve_scale;

enum class bits{
    show_acid_type = 0,
    show_overlay,
    show_statistics,
    show_white_screen,
    is_Paused,
    is_Running,
    standard_animation,
    special_animation
};

enum class animations{
    DNA = 0,
    fly_plane,
    show_sinus,
    show_cosinus,
    show_bindungen,
    sin2t1_cos_t2_plus_2t1,
    cos_2t2,
    restart,
    begin_destroy
};

struct statistics{
    //Statistics
    int adenin = 0;
    int thymin = 0;
    int guanin = 0;
    int cytosin = 0;
};

class randomizer{
public:
    randomizer(){};
    ~randomizer(){};

    int random_int(int bottom, int top);
private:
    std::random_device rd;
    
};//randomizer

struct DNA{
    //Spiralen - im Funktionsparameter als added_circle benannt
    std::vector<circle> spiral_one;
    std::vector<circle> spiral_second;
};//DNA


struct _time_stamps{
    std::chrono::time_point<std::chrono::steady_clock> get_current_time_point();

    std::chrono::milliseconds get_milliseconds_between_time_points(std::chrono::time_point<std::chrono::steady_clock> t1
        , std::chrono::time_point<std::chrono::steady_clock> t0);
    
    //Laufzeit Uhr
    std::chrono::system_clock::time_point count;

    std::chrono::milliseconds cooldown_plane{10};
    std::chrono::time_point<std::chrono::steady_clock> last_time_plane_moveX;

    std::chrono::milliseconds cooldown_stats{1200};
    std::chrono::time_point<std::chrono::steady_clock> last_time_stats;

    std::chrono::time_point<std::chrono::steady_clock> now;
    std::chrono::time_point<std::chrono::steady_clock> last_time_rectBuilder;
    
    //Special Animation

    std::chrono::milliseconds cooldown_restart_destroy{640};
    std::chrono::time_point<std::chrono::steady_clock> last_time_restarted_destroy;
    //DNA Uhren
    sf::Clock clock_1;
    sf::Clock clock_2;
    
    std::chrono::milliseconds cooldown{320};
    //Erste Kurve
    std::chrono::time_point<std::chrono::steady_clock> last_time;
    //Zweite Kurve
    std::chrono::time_point<std::chrono::steady_clock> last_time_second;

};
class Animator{
public:
    Animator(const sf::Font& _font) : font(_font){
        status_bits.reset();
        w_animation.reset();
        _clock.now = _clock.get_current_time_point();
        _clock.last_time = _clock.now;
        _clock.last_time_plane_moveX = _clock.now;
        _clock.last_time_rectBuilder = _clock.now;
        _clock.last_time_second = _clock.now;
        _clock.last_time_stats = _clock.now;
        _clock.last_time_restarted_destroy = _clock.now;
    };
    ~Animator(){};
    Animator(const Animator&) = default;
    Animator& operator=(const Animator&) = default;

    std::bitset<16> status_bits;
    std::bitset<16> w_animation;
    statistics stats;

    airplane plane{60.f, 60.f, 20.f, 10.f};
    DNA _DNA;
    
    std::vector<circle> rect_builder;
    
    std::vector<sf::RectangleShape> rect_connections;

    void initialize_start();

    void set_system_clock_now(){
        _clock.count = std::chrono::system_clock::now();
    };

    std::string get_time_stream_count(){
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - _clock.count);
        int minutes = elapsed.count() / 60;
        int seconds = elapsed.count() % 60;

        std::ostringstream time_stream_count;
        time_stream_count << std::setfill('0') << std::setw(2) << minutes
                    << ":" << std::setfill('0') << std::setw(2) << seconds;
        
        return time_stream_count.str();
    };

    void initialize_time_points();

    void start_by_create_new_ball(std::vector<circle>& added_circle);
    void process_Animation_DNA_Flow(const float& win_size_x, const float& win_size_y);
    void processSPECIAL_Animation_DNA_Flow(const float& win_size_x, const float& win_size_y);
    void fly_plane(const float& win_size_x);

    

    std::chrono::milliseconds::rep get_cooldown()const{ return _clock.cooldown.count();}
     
    void reset_dy(){ dy = 0.f; }
private:
    randomizer rand;
    sf::Font font;
    float radius = 5.f;
    float dy; 

    /*==================*/
    /* Zeit Steuerung   */
    _time_stamps _clock;

    void create_new_ball_by_AddedCircle_size_SINUS(std::vector<circle>& added_circle);
    void create_new_ball_by_AddedCircle_size_COSINUS(std::vector<circle>& added_circle, sf::Color color);
    void create_new_ball_by_AddedCircle_size_RECT_BUILDER(std::vector<circle>& added_circle, sf::Color color);

    void process_animation_SINUS(float time, std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y);
    void process_animation_COSINUS(float time, std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y);
    void connect_curves();
    void process_animation_rect_builder(std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y);

    void create_rectangle(std::vector<circle>& added_circle);

    void collect_statistik(const std::vector<circle>& spiral_for_stats);
    //lineare Interpolation bzw. lineare Skalierung -- Flexible Skalierung und Transparenzformel
    //Wertebereich normalisieren und auf Ausgabeskala abbilden
    float map(float value, float in_min, float in_max, float out_min, float out_max);
    void adjust_scale(sf::CircleShape& shape, const sf::Vector2f& pos_sin);
    void adjust_color_alpha(sf::CircleShape& shape, const sf::Vector2f& pos_sin);

    void adjust_scale_colorAlpha();
    void adjust_color_alpha_forStr(sf::Text& txt, const sf::Vector2f& pos_sin);
};

#endif // ANIMATOR_H