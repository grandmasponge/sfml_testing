#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

//not using this yet more proof of concept

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
        sf::Int32 prev = sf::Time::Zero.asMilliseconds();
        int damage = 30;
        float max_vel = 10.f;
};

class Zombie {
    public:
     sf::Vector2f velocity;
     int health;
     sf::RectangleShape mesh = sf::RectangleShape(sf::Vector2f(30, 30));
     float speed = 3.f;
     Zombie()
     {
        this->mesh.setFillColor(sf::Color::Green);
        this->mesh.setPosition(Spawn());
     }
     void update(sf::Vector2f playerpos)
     {
        sf::Vector2f playercenter = sf::Vector2f(playerpos.x, playerpos.y);
        sf::Vector2f zombiedir = sf::Vector2f(playercenter - this->mesh.getPosition());
        sf::Vector2f zombiedirnorm = sf::Vector2f(zombiedir.x / sqrt(pow(zombiedir.x, 2) + pow(zombiedir.y, 2)), zombiedir.y / sqrt(pow(zombiedir.x, 2) + pow(zombiedir.y, 2)));
        this->mesh.move(this->speed * zombiedirnorm);
     }
     bool bullet_collision(Bullet *bullet)
     {
        if(this->mesh.getGlobalBounds().intersects(bullet->mesh.getGlobalBounds()))
        {
            std::cout << "bullet collision" << std::endl;
            return true;
        }
        return false;
     }
    void take_damage(int damage)
     {
        std::cout << "zombie took damage" << std::endl;
        this->health -= damage;
     }
    
     bool player_collision(Player *player)
     {
        if(this->mesh.getGlobalBounds().intersects(player->mesh.getGlobalBounds()))
        {
            std::cout << "player collision" << std::endl;
            return true;
        }
        return false;
     }
    sf::Vector2f Spawn()
    {
        //spawn off screen
        sf::Vector2f spawn;
        spawn.x = rand() % 50 + 800;
        spawn.y = rand() % 50 + 600;
        return spawn;
    } 
    
    
};

Player player(sf::Vector2f(100, 100), sf::Vector2f(0, 0), sf::Vector2f(0, 0));
std::vector<Bullet> bullets;
std::vector<Zombie> Zombies;
Bullet bullet;
sf::Clock delta_clock;
sf::Clock zombie_clock;
Zombie zombie = Zombie();

void shooting()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && delta_clock.getElapsedTime().asMilliseconds() - bullet.prev > 250)
    {
        bullet.mesh.setPosition(player.mesh.getPosition());
        bullet.current_vel = player.Aimdir * bullet.max_vel;
        bullets.push_back(bullet);
        bullet.prev = delta_clock.getElapsedTime().asMilliseconds();
    }
}

void zombie_spawn()
{
    if (zombie_clock.getElapsedTime().asSeconds() > 5)
    {
        zombie.health = 100;
        zombie.mesh.setPosition(zombie.Spawn());
        Zombies.push_back(zombie);
        zombie_clock.restart();
    }
}

void zombie_death()
{
    for(size_t i = 0; i < Zombies.size(); i++)
    {
        if(Zombies[i].health <= 0)
        {
            std::cout << "zombie died" << std::endl;
            Zombies.erase(Zombies.begin() + i);
        }
    }
}



void cleanup()
{
    for(size_t i = 0; i < bullets.size(); i++)
    {
        if(bullets[i].mesh.getPosition().x > 800 || bullets[i].mesh.getPosition().x < 0 || bullets[i].mesh.getPosition().y > 600 || bullets[i].mesh.getPosition().y < 0)
        {
            bullets.erase(bullets.begin() + i);
        }
    }
}

void remove_from_vector(size_t index)
{
    bullets.erase(bullets.begin() + index);
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    
    while (window.isOpen())
    {
        window.clear();

        zombie_spawn();
        zombie_death();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        player.update(sf::Vector2f(sf::Mouse::getPosition(window)));
        
        window.draw(player.mesh);
    //collision and drawing bullets
        for (size_t i = 0; i < bullets.size(); i++)
        {
            bullets[i].mesh.move(bullets[i].current_vel);
            window.draw(bullets[i].mesh);
        }
        //zombies stuff
        for(size_t i = 0; i < Zombies.size(); i++)
        {
            for(size_t j = 0; j < bullets.size(); j++)
            {
                if(Zombies[i].bullet_collision(&bullets[j]))
                {
                    Zombies[i].take_damage(bullets[j].damage);
                    remove_from_vector(j);
                }
            }
        }
        for (size_t i = 0; i < Zombies.size(); i++)
        {
            Zombies[i].update(player.mesh.getPosition());
            window.draw(Zombies[i].mesh);
        }
        shooting();
        cleanup();
        window.display();
    }

    return 0;
}