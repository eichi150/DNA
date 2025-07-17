#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <SFML/System.hpp>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <bitset>

#include "animator.h"
#include "circle.h"
#include "airplane.h"

sf::Text write_small_Text(const sf::Color& color, float posX, float posY, const sf::Font& font, const std::string& prompt){
    sf::Text txt;
    txt.setFont(font);
    txt.setCharacterSize(20);
    txt.setFillColor(color);
    txt.setPosition(posX, posY);
    txt.setString(prompt);
    return txt;
}  

sf::Text write_Text(sf::Color color, float posX, float posY, const sf::Font& font, const std::string& prompt){
    sf::Text txt;
    txt.setFont(font);
    txt.setCharacterSize(20);
    txt.setFillColor(color);
    txt.setPosition(posX, posY);
    txt.setString(prompt);
    return txt;
}   


int main(void){
    
    sf::Font font;
     
    try{
       font.loadFromFile("./ARIAL.TTF");
    }catch(std::runtime_error& er){
        sf::RenderWindow error{sf::VideoMode{300, 300}, "Error"};
        sf::Text error_msg;
        error_msg.setCharacterSize(20);
        error_msg.setFillColor(sf::Color::Black);
        error_msg.setPosition(10.f, 10.f);
        error_msg.setString(er.what());

        while(error.isOpen()){
            sf::Event event;
            while(error.pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    error.close();
                }
            }
            error.clear(sf::Color::White);

            error.draw(error_msg);
            error.display();
        }
    }
    
    sf::Text _clock_count;
    _clock_count.setFont(font);
    _clock_count.setCharacterSize(30);
    _clock_count.setFillColor(sf::Color::White);
    _clock_count.setPosition(150.f, 0.f);
    

    sf::String steuerung_L_str = L"W = White/Black Hintergrundfarbe\nA = Animation starten\nT = Zeige Aminosäure an\nS = Sinus ein-/ ausblenden\nC = Cosinus ein-/ausblenden\nB = Bindungen ein-/ ausblenden\nI = Statistik\nR = Reset\nLeertaste = Pause // Weiter\nO = Overlay ein-/ ausblenden\nEscape = Programm schließen";

    const float win_size_x = 1100.f;
    const float win_size_y = 800.f;

    sf::RenderWindow window{sf::VideoMode{static_cast<int>(win_size_x), static_cast<int>(win_size_y)}, "DNA Sequenz", sf::Style::Default};
    window.setFramerateLimit(120);

    Animator ani{font};
    ani.initialize_start();
    ani.initialize_time_points();

    while(window.isOpen()){
        
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape){
                    window.close();
                }
                
                if(event.key.code == sf::Keyboard::R){
                    ani.initialize_start();
                    ani.initialize_time_points();
                }
                if(event.key.code == sf::Keyboard::U
                    && !ani.status_bits.test(static_cast<int>(bits::is_Running)))
                {
                    ani.status_bits.set(static_cast<int>(bits::special_animation));
                    ani.status_bits.reset(static_cast<int>(bits::standard_animation));

                    ani.status_bits.set(static_cast<int>(bits::is_Running));
                    //Sinus Kurve anstoßen, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani._DNA.spiral_one);
                    //Cosinus Kurve anstoßen, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani._DNA.spiral_second);
                    //Weißer Kreis unterhalb, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani.rect_builder);
                    //Counter Uhr starten
                    ani.set_system_clock_now();// = std::chrono::system_clock::now();
                    //Status Bits aktivieren
                    ani.w_animation.set(static_cast<int>(animations::show_cosinus));
                    ani.w_animation.set(static_cast<int>(animations::show_sinus));
                    ani.w_animation.set(static_cast<int>(animations::show_bindungen));

                    ani.w_animation.set(static_cast<int>(animations::DNA));
                }
                //MAIN ANIMATION
                if(event.key.code == sf::Keyboard::A
                    && !ani.status_bits.test(static_cast<int>(bits::is_Running))
                    && !ani.status_bits.test(static_cast<int>(bits::is_Paused)))
                {
                    ani.status_bits.set(static_cast<int>(bits::is_Running));
                    //Sinus Kurve anstoßen, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani._DNA.spiral_one);
                    //Cosinus Kurve anstoßen, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani._DNA.spiral_second);
                    //Weißer Kreis unterhalb, ersten Kreis erstellen
                    ani.start_by_create_new_ball(ani.rect_builder);
                    //Counter Uhr starten
                    ani.set_system_clock_now();//count = std::chrono::system_clock::now();
                    //Status Bits aktivieren
                    ani.w_animation.set(static_cast<int>(animations::show_cosinus));
                    ani.w_animation.set(static_cast<int>(animations::show_sinus));
                    ani.w_animation.set(static_cast<int>(animations::show_bindungen));

                    ani.w_animation.set(static_cast<int>(animations::DNA));
                    ani.status_bits.set(static_cast<int>(bits::standard_animation));
                }
                if(event.key.code == sf::Keyboard::P){
                    if(ani.w_animation.test(static_cast<int>(animations::fly_plane))){
                        ani.w_animation.reset(static_cast<int>(animations::fly_plane));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::fly_plane));
                    }
                }

                if(event.key.code == sf::Keyboard::Space){
                    if(ani.status_bits.test(static_cast<int>(bits::is_Paused))){
                        ani.status_bits.reset(static_cast<int>(bits::is_Paused));
                    }else{
                        ani.status_bits.set(static_cast<int>(bits::is_Paused));
                    }
                }
                
                if(event.key.code == sf::Keyboard::O){
                    if(ani.status_bits.test(static_cast<int>(bits::show_overlay))){
                        ani.status_bits.reset(static_cast<int>(bits::show_overlay));
                    }else{
                        ani.status_bits.set(static_cast<int>(bits::show_overlay));
                    } 
                }
                if(event.key.code == sf::Keyboard::W){
                    
                    if(ani.status_bits.test(static_cast<int>(bits::show_white_screen))){
                        ani.status_bits.reset(static_cast<int>(bits::show_white_screen));
                    }else{
                        ani.status_bits.set(static_cast<int>(bits::show_white_screen));
                    }
                }
                if(event.key.code == sf::Keyboard::Num0){
                    ani.w_animation.reset(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1));
                    ani.w_animation.reset(static_cast<int>(animations::cos_2t2));
                }
                if(event.key.code == sf::Keyboard::Num1){
                    if(ani.w_animation.test(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1))){
                        ani.w_animation.reset(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::sin2t1_cos_t2_plus_2t1));
                    }
                    
                }
                if(event.key.code == sf::Keyboard::Num2){
                    if(ani.w_animation.test(static_cast<int>(animations::cos_2t2))){
                        ani.w_animation.reset(static_cast<int>(animations::cos_2t2));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::cos_2t2));
                    }
                    
                }
                if(event.key.code == sf::Keyboard::C){
                    if(ani.w_animation.test(static_cast<int>(animations::show_cosinus))){
                        ani.w_animation.reset(static_cast<int>(animations::show_cosinus));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::show_cosinus));
                    }
                    
                }
                if(event.key.code == sf::Keyboard::S){
                    if(ani.w_animation.test(static_cast<int>(animations::show_sinus))){
                        ani.w_animation.reset(static_cast<int>(animations::show_sinus));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::show_sinus));
                    }
                    
                }
                if(event.key.code == sf::Keyboard::B){
                    if(ani.w_animation.test(static_cast<int>(animations::show_bindungen))){
                        ani.w_animation.reset(static_cast<int>(animations::show_bindungen));
                    }else{
                        ani.w_animation.set(static_cast<int>(animations::show_bindungen));
                    }
                }
                if(event.key.code == sf::Keyboard::T){
                    if(ani.status_bits.test(static_cast<int>(bits::show_acid_type))){
                        ani.status_bits.reset(static_cast<int>(bits::show_acid_type));
                    }else{
                        ani.status_bits.set(static_cast<int>(bits::show_acid_type));
                    }
                }
                if(event.key.code == sf::Keyboard::I){
                    if(ani.status_bits.test(static_cast<int>(bits::show_statistics))){
                        ani.status_bits.reset(static_cast<int>(bits::show_statistics));
                    }else{
                        ani.status_bits.set(static_cast<int>(bits::show_statistics));
                    }
                }
            }
        }
        
        if(ani.status_bits.test(static_cast<int>(bits::is_Running))){
            //Counter Uhr
            _clock_count.setString(ani.get_time_stream_count());
        }
        //? While Animation Running
        if(ani.status_bits.test(static_cast<int>(bits::is_Running))){
            //! While Nicht Pausiert
            if(!ani.status_bits.test(static_cast<int>(bits::is_Paused))){
                if(ani.w_animation.test(static_cast<int>(animations::fly_plane))){
                    ani.fly_plane(win_size_x);
                }
                //  HAUPTANIMATION  DNA  //
                if(ani.w_animation.test(static_cast<int>(animations::DNA))){
                    if(ani.status_bits.test(static_cast<int>(bits::special_animation))){
                        
                        ani.processSPECIAL_Animation_DNA_Flow(win_size_x, win_size_y);

                        if(ani._DNA.spiral_one.back().shape.getPosition().x >= 400.f){
                            ani.w_animation.set(static_cast<int>(animations::restart));
                        }
                    }
                    if(ani.status_bits.test(static_cast<int>(bits::standard_animation))){
                        ani.process_Animation_DNA_Flow(win_size_x, win_size_y);
                    }
                    
                }
            }
        }
        //GUI
        bool which_color = ani.status_bits.test(static_cast<int>(bits::show_white_screen));
        if(which_color){//White
            window.clear(sf::Color::White);
        }else{//Black
            window.clear(sf::Color::Black);
        }
        if(ani.status_bits.test(static_cast<int>(bits::is_Running))){
            //Kreis der Rahmen begleitet
            for(const auto& rectBuilder : ani.rect_builder){
                window.draw(rectBuilder.shape);
            }
            //Rahmen zeichnen
            for(const auto& rect : ani.rect_connections){
                window.draw(rect);
            }
            if(ani.w_animation.test(static_cast<int>(animations::fly_plane))){
                window.draw(ani.plane.sprite_plane);
            }

            //??? cos, sin, bindungen anzeigen
            if(ani.w_animation.test(static_cast<int>(animations::show_cosinus))
                && ani.w_animation.test(static_cast<int>(animations::show_sinus))
                && ani.w_animation.test(static_cast<int>(animations::show_bindungen)))
            {
                //Positionsabhängig entweder erst Sinus vor Cosinus ODER Cosinus vor Sin zeichnen
                for(int i{1}; i < ani._DNA.spiral_one.size(); ++i){
                    sf::Vector2f pos_sin = ani._DNA.spiral_one[i].shape.getPosition();
                    sf::Vector2f pos_cos = ani._DNA.spiral_second[i].shape.getPosition();

                    //Sinus vor Cosinus
                    bool y_almost_equal = std::abs(pos_sin.y - pos_cos.y) < 0.1f;
                    //Bindung zeichnen
                    window.draw(ani._DNA.spiral_one[i].bindung);
                    
                    if(pos_sin.y < ( AREA_MAX_Y - AREA_MIN_Y) / 1.5f && y_almost_equal){
                        //Sinus vor Cosinus
                        window.draw(ani._DNA.spiral_one[i].shape);
                        
                        if(i < ani._DNA.spiral_second.size()){
                            window.draw(ani._DNA.spiral_second[i].shape);
                        }
                    }else {
                        //Cosinus vor Sinus
                        if(i < ani._DNA.spiral_second.size()){
                            window.draw(ani._DNA.spiral_second[i].shape);
                        }
                        window.draw(ani._DNA.spiral_one[i].shape);
                    }
                }
            //?? cos, sin anzeigen
            }else if(ani.w_animation.test(static_cast<int>(animations::show_cosinus))
                && ani.w_animation.test(static_cast<int>(animations::show_sinus)))
            {
                for(int i{1}; i < ani._DNA.spiral_one.size(); ++i){
                    sf::Vector2f pos_sin = ani._DNA.spiral_one[i].shape.getPosition();
                    sf::Vector2f pos_cos = ani._DNA.spiral_second[i].shape.getPosition();
                    
                    //Sinus vor Cosinus
                    if(pos_sin.y > pos_cos.y && pos_sin.y < 150.f){

                        window.draw(ani._DNA.spiral_one[i].shape);
                        
                        if(i < ani._DNA.spiral_second.size()){
                            window.draw(ani._DNA.spiral_second[i].shape);
                        }
                    }else{
                    //Cosinus vor Sinus
                        if(i < ani._DNA.spiral_second.size()){
                            window.draw(ani._DNA.spiral_second[i].shape);
                        }
                        window.draw(ani._DNA.spiral_one[i].shape);
                    }
                }
            }
            //?! sin anzeigen
            else if(ani.w_animation.test(static_cast<int>(animations::show_sinus)) 
                && !ani.w_animation.test(static_cast<int>(animations::show_cosinus)))
            {
                for(int i{1}; i < ani._DNA.spiral_one.size(); ++i){
                    sf::Vector2f pos_sin = ani._DNA.spiral_one[i].shape.getPosition();

                    if(ani.w_animation.test(static_cast<int>(animations::show_bindungen))){
                        window.draw(ani._DNA.spiral_one[i].bindung);
                    }
                    window.draw(ani._DNA.spiral_one[i].shape);
                }
            //?! cos anzeigen
            }else if(ani.w_animation.test(static_cast<int>(animations::show_cosinus))
                && !ani.w_animation.test(static_cast<int>(animations::show_sinus)))
            {
                for(int i{1}; i < ani._DNA.spiral_second.size(); ++i){
                    sf::Vector2f pos_sin = ani._DNA.spiral_second[i].shape.getPosition();

                    if(ani.w_animation.test(static_cast<int>(animations::show_bindungen))
                        && i < ani._DNA.spiral_one.size())
                    {
                        window.draw(ani._DNA.spiral_one[i].bindung);
                    }
                    window.draw(ani._DNA.spiral_second[i].shape);
                }
            }
            //? Acid Type anzeigen
            if(ani.status_bits.test(static_cast<int>(bits::show_acid_type))){
                for(int i{1}; i < ani._DNA.spiral_one.size(); ++i){
                    //Cosinus Kurve beschriften
                    if(i < ani._DNA.spiral_second.size() && ani.w_animation.test(static_cast<int>(animations::show_cosinus))){

                        window.draw(ani._DNA.spiral_second[i].acid_str);
                    }
                    //Sinus Kurve beschriften
                    if(ani.w_animation.test(static_cast<int>(animations::show_sinus))){

                        window.draw(ani._DNA.spiral_one[i].acid_str);
                    }
                }
            }

            if(ani.status_bits.test(static_cast<int>(bits::show_overlay))){
                window.draw(_clock_count);
            }
        }
        if(ani.status_bits.test(static_cast<int>(bits::show_statistics))
            && ani.status_bits.test(static_cast<int>(bits::is_Running))){

            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 250.f, 0.f, font, "Cooldown: " + std::to_string(ani.get_cooldown())));
            if(ani._DNA.spiral_one.size() > 1 && ani._DNA.spiral_second.size() > 1){
                window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 550.f, 0.f, font, "Sinus: " + std::to_string(ani._DNA.spiral_one.size() -1)));
                window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 800.f, 0.f, font, "Cosinus: " + std::to_string(ani._DNA.spiral_second.size() -1)));
            }
            float y = 350.f;
            float y_offset = 20.f;
            window.draw(write_small_Text(sf::Color::Blue,   0.f, y + (0.f * y_offset), font, "Adenin: " + std::to_string(ani.stats.adenin)));
            window.draw(write_small_Text(sf::Color::Red,    0.f, y + (1.f * y_offset), font, "Thymin: " + std::to_string(ani.stats.thymin)));
            window.draw(write_small_Text(sf::Color::Green,  0.f, y + (2.f * y_offset), font, "Guanin: " + std::to_string(ani.stats.guanin)));
            window.draw(write_small_Text(sf::Color::Yellow, 0.f, y + (3.f * y_offset), font, "Cytosin: " + std::to_string(ani.stats.cytosin)));
        }
        
        if(ani.status_bits.test(static_cast<int>(bits::is_Paused))){
            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 10.f, 0.f, font, "Paused" ));

        }else if(ani.status_bits.test(static_cast<int>(bits::is_Running))){
           window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 10.f, 0.f, font, "Running" )); 
        }

        if(ani.status_bits.test(static_cast<int>(bits::show_overlay))){
        //Sinusförmige Bewegung: Amplitude * sin(Frequenz * Zeit + phase_offset) + Verschiebung
            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 0.f, 440.f, font, "Taste 0: (Default) sin= A * sin(f * t_1 + phase_offset) UND cos= A * cos(f * t_2 + phase_offset)" )); 
            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 0.f, 480.f, font, "Taste 1: sin= A * sin(f * 2t_1 + phase_offset) UND cos= A * cos(f * (2t_1 + t_2) + phase_offset)" ));
            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 0.f, 500.f, font, "Taste 2: sin= A * sin(f * 2t_1 + phase_offset) UND cos= A * cos(f * (2t_2) + phase_offset)" )); 
            window.draw(write_Text((which_color ? sf::Color::Black : sf::Color::White), 0.f, 520.f, font, steuerung_L_str.toAnsiString()));
        }
        window.display();
    }
}
