#include "animator.h"

/* RANDOMIZER   */
int randomizer::random_int(int bottom, int top){
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(bottom, top);
    
    return dist(gen);
}

void Animator::processSPECIAL_Animation_DNA_Flow(const float& win_size_x, const float& win_size_y){
    
    if(w_animation.test(static_cast<int>(animations::restart))){   
            w_animation.set(static_cast<int>(animations::begin_destroy));

            /*set_system_clock_now();
            _DNA = {};
            rect_builder.clear();
            reset_dy();
            start_by_create_new_ball(_DNA.spiral_one);
            //Cosinus Kurve anstoßen, ersten Kreis erstellen
            start_by_create_new_ball(_DNA.spiral_second);*/
    }
    process_Animation_DNA_Flow(win_size_x, win_size_y);
}

/* ANIMATOR     */
void Animator::process_Animation_DNA_Flow(const float& win_size_x, const float& win_size_y){

    float time_1 = _clock.clock_1.getElapsedTime().asSeconds();
    if(w_animation.test(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1))){
        time_1 *= 2;
    }
    //Sinuskurve animieren
    process_animation_SINUS(time_1, _DNA.spiral_one, win_size_x, win_size_y);

    float time_2 = _clock.clock_2.getElapsedTime().asSeconds();
    if(w_animation.test(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1))){
        time_2 += time_1;
    }
    if(w_animation.test(static_cast<int>(animations::cos_2t2))){
        time_2 *= 2;
    }
    //Cosinuskurve animieren
    process_animation_COSINUS(time_2, _DNA.spiral_second, win_size_x, win_size_y);

    //Bindungen an Sinuskurve anlegen - optisch mit Cosinuskurve verbinden
    connect_curves();
    //weißer Kreis der unterhalb mitspringt
    process_animation_rect_builder(rect_builder, win_size_x, win_size_y);

    //Abfolge und Skalierung + Alpha Transparenz anpassen
    adjust_scale_colorAlpha();

    //Statistik
    if(status_bits.test(static_cast<int>(bits::show_statistics))
        && _clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_stats) >= _clock.cooldown_stats)
    {
        stats = {};
        collect_statistik(_DNA.spiral_one);
        collect_statistik(_DNA.spiral_second);
        _clock.last_time_stats = _clock.now;
    }
}

void Animator::fly_plane(const float& win_size_x){
    _clock.now = _clock.get_current_time_point();
    if(_clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_plane_moveX) >= _clock.cooldown_plane){
        plane.change_Plane_X_position(win_size_x);
        _clock.last_time_plane_moveX = _clock.now;
    }
}

std::chrono::time_point<std::chrono::steady_clock> _time_stamps::get_current_time_point(){
    return std::chrono::steady_clock::now();
}


std::chrono::milliseconds _time_stamps::get_milliseconds_between_time_points(
    std::chrono::time_point<std::chrono::steady_clock> t1
        , std::chrono::time_point<std::chrono::steady_clock> t0)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
}


void Animator::initialize_time_points(){
    _clock.now = _clock.get_current_time_point();
    _clock.last_time_stats = _clock.now;
}

void Animator::initialize_start(){
    //Leeren
    status_bits.reset();
    w_animation.reset();
    
    dy = 0.f;

    rect_builder.clear();
    rect_builder.shrink_to_fit();

    rect_connections.clear();
    rect_connections.shrink_to_fit();

    _DNA = {};

    //Startwerte
    status_bits.set(static_cast<int>(bits::show_overlay));

}
void Animator::create_rectangle(std::vector<circle>& added_circle){
    sf::RectangleShape rect{{5.f, 0.5f}};
    rect.setFillColor(sf::Color::White);
    rect.setPosition(added_circle.back().shape.getPosition().x, AREA_MIN_Y);
    rect_connections.push_back(rect);

    sf::RectangleShape rect_unten{{5.f, 0.5f}};
    rect_unten.setFillColor(sf::Color::White);
    rect_unten.setPosition(added_circle.back().shape.getPosition().x, AREA_MAX_Y + 20.f);
    rect_connections.push_back(rect_unten);
}

void Animator::process_animation_SINUS(float time, std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y){
    _clock.now = _clock.get_current_time_point();
    
    if(_clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time) >= _clock.cooldown){
        //Zeit gesteuert Bälle hinzufügen
        if(!added_circle.empty() && !status_bits.test(static_cast<int>(bits::is_Paused))){

            if(w_animation.test(static_cast<int>(animations::begin_destroy))){
                if(_clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_restarted_destroy) >= _clock.cooldown_restart_destroy){
                    added_circle.pop_back();
                }
                
            }else 
            if(added_circle.back().shape.getPosition().x <= win_size_x){
                
                create_new_ball_by_AddedCircle_size_SINUS(added_circle); 
                
                create_rectangle(added_circle);
                _clock.last_time = _clock.now;
            }
        }else if(w_animation.test(static_cast<int>(animations::begin_destroy))){
            //zerstören erledigt -> Neustart initieren HIER KEINE dy neuinitialisierung
            w_animation.reset(static_cast<int>(animations::begin_destroy));
            w_animation.reset(static_cast<int>(animations::restart));
            set_system_clock_now();//restart count

            start_by_create_new_ball(_DNA.spiral_one);
            //Cosinus Kurve anstoßen, ersten Kreis erstellen
            start_by_create_new_ball(_DNA.spiral_second);
        }
    }
    
    // Y Bewegung ändern
    for(auto& added : added_circle){
        
        added.change_Y_position_SINUS(win_size_y, time, curve_scale);
    }

}
void Animator::process_animation_COSINUS(float time, std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y){
    
    _clock.now = _clock.get_current_time_point();

    if(_clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_second) >= _clock.cooldown){
        //Zeit gesteuert Bälle hinzufügen
        if(!added_circle.empty() && !status_bits.test(static_cast<int>(bits::is_Paused))){

            if(w_animation.test(static_cast<int>(animations::begin_destroy))){

                if(_clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_restarted_destroy) >= _clock.cooldown_restart_destroy){
                    added_circle.pop_back();
                    _clock.last_time_restarted_destroy = _clock.now;
                }
                
            }else 
            if(added_circle.back().shape.getPosition().x <= win_size_x){

                create_new_ball_by_AddedCircle_size_COSINUS(added_circle, sf::Color::White);
                _clock.last_time_second = _clock.now;
            }
        }
    }
    
    // Y Bewegung ändern
    for(auto& added : added_circle){
        added.change_Y_position_COSINUS(win_size_y, time, curve_scale);
    }
}

void Animator::process_animation_rect_builder(std::vector<circle>& added_circle, const float& win_size_x, const float& win_size_y){
    

    if(_clock.get_milliseconds_between_time_points(_clock.get_current_time_point(), _clock.last_time_rectBuilder) >= _clock.cooldown){
        //Zeit gesteuert Bälle hinzufügen
        if(!added_circle.empty() && !status_bits.test(static_cast<int>(bits::is_Paused))){
            
            if(added_circle.back().shape.getPosition().x <= win_size_x){

                create_new_ball_by_AddedCircle_size_RECT_BUILDER(added_circle, sf::Color::White);
            }else{
                added_circle.pop_back();
            }

            _clock.last_time_rectBuilder = _clock.get_current_time_point();
        }
    }
    
    // Y Bewegung ändern
    for(auto& added : added_circle){
        added.change_Y_position(win_size_y, AREA_MAX_Y, AREA_MIN_Y);
    }
}

void Animator::create_new_ball_by_AddedCircle_size_RECT_BUILDER(std::vector<circle>& added_circle, sf::Color color){
    
    //Neuen Ball hinten hinzufügen
    circle new_ball{radius, font};
    new_ball.shape.setFillColor(color);
    new_ball.shape.setPosition({(1.f * radius) + added_circle.back().shape.getPosition().x, AREA_MAX_Y + 20.f});
    
    if(!added_circle.empty()){
       added_circle[0] = new_ball; 
    }else{
       added_circle.push_back(new_ball); 
    }
}

void Animator::start_by_create_new_ball(std::vector<circle>& added_circle){

    circle new_ball{radius, font};
    new_ball.shape.setFillColor(sf::Color::Red);
    new_ball.shape.setPosition({0.f, Y_START});
    
    added_circle.push_back(new_ball);

    _clock.now = _clock.get_current_time_point();
    _clock.last_time = _clock.now;
}

void Animator::create_new_ball_by_AddedCircle_size_SINUS(std::vector<circle>& added_circle){
    
    //Neuen Ball hinten hinzufügen
    circle new_ball{radius, font};
    int r = rand.random_int(static_cast<int>(Aminoacid::adenin), static_cast<int>(Aminoacid::cytosin));

    new_ball.set_acid_type(static_cast<Aminoacid>(r));
    new_ball.shape.setFillColor(new_ball.get_color_with_acid_type());
    
    new_ball.shape.setPosition({(adj_size * radius) + added_circle.back().shape.getPosition().x, Y_START});
    
    new_ball.set_phase_offset(added_circle.back().shape.getPosition().x * 0.05f);
    
    added_circle.push_back(new_ball);
    
}

void Animator::create_new_ball_by_AddedCircle_size_COSINUS(std::vector<circle>& added_circle, sf::Color color){
    
    //Neuen Ball hinten hinzufügen
    circle new_ball{radius, font};

    new_ball.shape.setFillColor(color);

    new_ball.shape.setPosition({(adj_size * radius) + added_circle.back().shape.getPosition().x, AREA_MAX_Y - 10.f});
    new_ball.set_phase_offset(added_circle.back().shape.getPosition().x * 0.05f);

    added_circle.push_back(new_ball);
    
}

void Animator::connect_curves(){
    
    for(int i{1}; i < _DNA.spiral_one.size(); ++i){
        if(i < _DNA.spiral_second.size()){
            /*  Bindung mit Rechteck darstellen */
            sf::Vector2f start = _DNA.spiral_one[i].shape.getPosition();
            sf::Vector2f end = _DNA.spiral_second[i].shape.getPosition();
            start.x += radius;
            start.y += radius;
            end.x += radius;
            end.y += radius;
            sf::Vector2f direction = end - start;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float angle = std::atan2(direction.y, direction.x) * 180.f/M_PI;

            float thickness = 2.f;
            sf::RectangleShape line(sf::Vector2f(length, thickness));
            //line.setOutlineColor(sf::Color::Black);
            //line.setOutlineThickness(0.3f);
            line.setFillColor(sf::Color::Cyan);
            sf::Color color = line.getFillColor();
            sf::Uint8 r = color.r;
            sf::Uint8 g = color.g;
            sf::Uint8 b = color.b;
            
            line.setFillColor(sf::Color(r, g, b, static_cast<sf::Uint8>(map(start.y, AREA_MIN_Y, AREA_MAX_Y, 125.f, 225.f))));

            line.setOrigin(0, thickness/2.f);
            line.setPosition(start);
            line.setRotation(angle);
            
            //spiral one hält die bindungen
            _DNA.spiral_one[i].bindung = line;
            //Gegenteilige AcidType, Farbe und Schrift anbringen
            _DNA.spiral_second[i].shape.setFillColor(_DNA.spiral_one[i].get_opposite_color_with_acid_type());
            _DNA.spiral_second[i].set_acid_type(_DNA.spiral_one[i].get_opposite_Aminoacid_with_acid_type());
            _DNA.spiral_second[i].acid_str.setString(_DNA.spiral_one[i].get_opposite_txt_with_acid_type());
        }
    }
}

float Animator::map(float value, float in_min, float in_max, float out_min, float out_max){
    if(std::abs(in_max - in_min) < 1e-6f){
        return (out_min + out_max) / 2.f;//vorläufige rückgabe bei disivion durch 0
    }
    float normalized = (value - in_min) / (in_max - in_min);
    return normalized * (out_max - out_min) + out_min;
}

void Animator::adjust_scale(sf::CircleShape& shape, const sf::Vector2f& pos_sin){
    float scale = map(pos_sin.y, AREA_MIN_Y, AREA_MAX_Y, 0.5f, 1.2f);
    shape.setScale(scale, scale);
};

void Animator::adjust_color_alpha(sf::CircleShape& shape, const sf::Vector2f& pos_sin){

    //int alpha = static_cast<int>(map(pos_sin.y, AREA_MIN_Y, AREA_MAX_Y, 100.f, 255.f));
    sf::Color color = shape.getFillColor();
    sf::Uint8 r = color.r;
    sf::Uint8 g = color.g;
    sf::Uint8 b = color.b;
    
    shape.setFillColor(sf::Color(r, g, b, static_cast<sf::Uint8>(map(pos_sin.y, AREA_MIN_Y, AREA_MAX_Y, 100.f, 255.f))));
};

void Animator::adjust_color_alpha_forStr(sf::Text& txt, const sf::Vector2f& pos_sin){

    //int alpha = static_cast<int>(map(pos_sin.y, AREA_MIN_Y, AREA_MAX_Y, 100.f, 255.f));
    sf::Color color = txt.getFillColor();
    sf::Uint8 r = color.r;
    sf::Uint8 g = color.g;
    sf::Uint8 b = color.b;
    
    txt.setFillColor(sf::Color(r, g, b, static_cast<sf::Uint8>(map(pos_sin.y, AREA_MIN_Y, AREA_MAX_Y, 100.f, 255.f))));
};


void Animator::adjust_scale_colorAlpha(){
    //Buchstaben auf die Kugeln setzen und Y korrigieren
    for(auto& cosinus : _DNA.spiral_second){
        sf::Vector2f pos = cosinus.shape.getPosition();
        //Buchstaben auf Cosinus 3.f nach oben korrigieren - sieht besser aus
        cosinus.acid_str.setPosition(pos.x, pos.y - 3.f);
    }
    for(auto& sinus : _DNA.spiral_one){
        sf::Vector2f pos = sinus.shape.getPosition();
        
        sinus.acid_str.setPosition(pos.x, pos.y - 3.f);
    }
//Ausgabe vorbereiten
    //?! sin anzeigen
    if(status_bits.test(static_cast<int>(animations::show_sinus))
        && !status_bits.test(static_cast<int>(animations::show_cosinus)))
    {
        for(int i{1}; i < _DNA.spiral_one.size(); ++i){
            sf::Vector2f pos_sin = _DNA.spiral_one[i].shape.getPosition();
            adjust_scale(_DNA.spiral_one[i].shape, pos_sin);
            adjust_color_alpha(_DNA.spiral_one[i].shape, pos_sin);
            
        }
    //?! cos anzeigen
    }else if(status_bits.test(static_cast<int>(animations::show_cosinus))
        && !status_bits.test(static_cast<int>(animations::show_sinus)))
    {
        for(int i{1}; i < _DNA.spiral_second.size(); ++i){
            sf::Vector2f pos_sin = _DNA.spiral_second[i].shape.getPosition();
            adjust_scale(_DNA.spiral_second[i].shape, pos_sin);
            adjust_color_alpha(_DNA.spiral_second[i].shape, pos_sin);
        
        }
    }else{ // beides anzeigen
        for(int i{1}; i < _DNA.spiral_one.size(); ++i){
            sf::Vector2f pos_sin = _DNA.spiral_one[i].shape.getPosition();
            sf::Vector2f pos_cos = _DNA.spiral_second[i].shape.getPosition();

            adjust_scale(_DNA.spiral_one[i].shape, pos_sin);
            adjust_color_alpha(_DNA.spiral_one[i].shape, pos_sin);

            adjust_scale(_DNA.spiral_second[i].shape, pos_sin);
            adjust_color_alpha(_DNA.spiral_second[i].shape, pos_sin);
        }
    }
    //? Acid Type anzeigen
    if(status_bits.test(static_cast<int>(bits::show_acid_type))){
        for(int i{1}; i < _DNA.spiral_one.size(); ++i){
            //Cosinus Kurve beschriften
            if(i < _DNA.spiral_second.size() && status_bits.test(static_cast<int>(animations::show_cosinus))){
                sf::Vector2f pos_sin = _DNA.spiral_second[i].shape.getPosition();
                adjust_color_alpha_forStr(_DNA.spiral_second[i].acid_str, pos_sin);
                
            }
            //Sinus Kurve beschriften
            if(status_bits.test(static_cast<int>(animations::show_sinus))){
                sf::Vector2f pos_sin = _DNA.spiral_one[i].shape.getPosition();
                adjust_color_alpha_forStr(_DNA.spiral_one[i].acid_str, pos_sin);

            }
        }
    }
}

void Animator::collect_statistik(const std::vector<circle>& spiral_for_stats){

    for(int i{1}; i < spiral_for_stats.size(); ++i){
        
        if(status_bits.test(static_cast<int>(bits::show_statistics))
            && _clock.get_milliseconds_between_time_points(_clock.now, _clock.last_time_stats) >= _clock.cooldown_stats)
        {
            const Aminoacid& check_for = spiral_for_stats[i].check_acid_type();
            if(check_for == Aminoacid::adenin){
                stats.adenin += 1;
            }else
            if(check_for == Aminoacid::thymin){
                stats.thymin += 1;
            }else
            if(check_for == Aminoacid::guanin){
                stats.guanin += 1;
            }else
            if(check_for == Aminoacid::cytosin){
                stats.cytosin += 1;
            }
        }
    }
}