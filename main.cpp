#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>


//not using this yet more proof of concept
 const int map[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

void draw_map(sf::RenderWindow *window);


class Player {
 public:
    Player(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f dir) {

            this->position = position;
            this->velocity = velocity;
            this->Aimdir = dir;
            this->mesh.setSize(sf::Vector2f(30, 30));
            this->mesh.setFillColor(sf::Color::Red);
    };

    void update(sf::Vector2f mousepos) {
        this->position = this->mesh.getPosition();
        sf::Vector2f playercenter = sf::Vector2f(this->position.x + 15, this->position.y + 15);
        sf::Vector2f aimdir = sf::Vector2f(mousepos - playercenter);
        sf::Vector2f aimdirnorm = sf::Vector2f(aimdir.x / sqrt(pow(aimdir.x, 2) + pow(aimdir.y, 2)), aimdir.y / sqrt(pow(aimdir.x, 2) + pow(aimdir.y, 2)));
        
        std::cout << aimdirnorm.x << " " << aimdirnorm.y << std::endl;
        //movment
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            this->velocity.y -= this->speed;
            if(this->velocity.y < -3)
            {
                this->velocity.y = -3;
            }
            this->mesh.move(0.f, this->velocity.y);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            this->velocity.y += this->speed;
            if(this->velocity.y > 3)
            {
                this->velocity.y = 3;
            }
            this->mesh.move(0.f, this->velocity.y);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            this->velocity.x -= this->speed;
            if (this->velocity.x < -3)
            {
                this->velocity.x = -3;
            }
            this->mesh.move(this->velocity.x, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            this->velocity.x += this->speed;
            if(this->velocity.x > 3)
            {
                this->velocity.x = 3;
            }
            this->mesh.move(this->velocity.x, 0);
        }
        else {
            this->velocity.x = 0;
            this->velocity.y = 0;
        }
        //shooting
        
        
    };
    sf::Vector2f position, velocity;
    sf::Vector2f Aimdir;
    float speed = 0.5   ;
    float max_speed = 3;
    sf::RectangleShape mesh;  
};

class Bullet {
    public:
        sf::CircleShape mesh;
        sf::Vector2f current_vel;
        float max_vel;
};



int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    
    Player player(sf::Vector2f(100, 100), sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        player.update(sf::Vector2f(sf::Mouse::getPosition(window)));

        window.clear();
        window.draw(player.mesh);
        window.display();
    }

    return 0;
}