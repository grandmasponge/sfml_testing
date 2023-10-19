#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

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
        this->Aimdir = aimdirnorm;

        //movment

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            this->velocity.y -= this->speed;
            if(this->velocity.y < -10)
            {
                this->velocity.y = -10;
            }
            this->mesh.move(0.f, this->velocity.y);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            this->velocity.y += this->speed;
            if(this->velocity.y > 10)
            {
                this->velocity.y = 10;
            }
            this->mesh.move(0.f, this->velocity.y);
        }
        else{
            this->velocity.y = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            this->velocity.x -= this->speed;
            if (this->velocity.x < -10)
            {
                this->velocity.x = -10;
            }
            this->mesh.move(this->velocity.x, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            this->velocity.x += this->speed;
            if(this->velocity.x > 10)
            {
                this->velocity.x = 10;
            }
            this->mesh.move(this->velocity.x, 0);
        }
        else {
            
            this->velocity.x = 0;
        }

    };
    sf::Vector2f position, velocity;
    sf::Vector2f Aimdir;
    float speed = 1.f;
    float max_speed = 5.f;
    sf::RectangleShape mesh;  
};

class Bullet {
    public:
        sf::Vector2f current_vel;
        Bullet(): current_vel(0.0f, 0.0f) {
            this->mesh.setRadius(10.f);
            this->mesh.setFillColor(sf::Color::Blue);
        }
        sf::CircleShape mesh;
        float max_vel = 5.f;
};


std::vector<Bullet> bullets;

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    
    Player player(sf::Vector2f(100, 100), sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    
    while (window.isOpen())
    {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        player.update(sf::Vector2f(sf::Mouse::getPosition(window)));
        
        //shooting
        Bullet bullet;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            
            bullet.mesh.setPosition(player.position);
            bullet.current_vel =  player.Aimdir * bullet.max_vel;
            std::cout << bullet.current_vel.x << " " << bullet.current_vel.y << std::endl;
            bullets.push_back(Bullet(bullet));
        }

        // window.draw(bullet.mesh)
        
        window.draw(player.mesh);

        for (size_t i = 0; i < bullets.size(); i++)
        {
            bullets[i].mesh.move(bullets[i].current_vel);
            std::cout << bullets.size() << std::endl;
            window.draw(bullets[i].mesh);
            bullets[i].mesh.move(bullets[i].current_vel);
        }

        //undrawbullets
        for (size_t i = 0; i < bullets.size(); i++)
        {
            
        }
        window.display();
    }

    return 0;
}