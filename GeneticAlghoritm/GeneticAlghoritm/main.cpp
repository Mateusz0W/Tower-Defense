#include <SFML/Graphics.hpp>
#include<ctime>

#include "Bullet.h"
#include"GEN.h"

int main() {
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Tower Defense");
    Tower tower("Base.png", "Gun.png", sf::Vector2f(800.f, 900.f), sf::Vector2f(850.f, 900.f));
    Airplane airplane("Airplane.png", sf::Vector2f(-250.f, 100.f), 200.f);
    Bullet bullet(500.f);

    //loading font
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }
    // Create text objects for displaying information
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(25);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10.f, window.getSize().y - 172.f);

    //Genetic Alghoritm
    std::vector<GEN::Individual> population = GEN::GeneratePopulation();
    std::vector<GEN::Individual> Best_Solutions;

    do {

        for (auto& individual : population)
            individual.fit = GEN::fitness(bullet, tower, airplane, individual);

        std::sort(population.begin(), population.end(), [](GEN::Individual a, GEN::Individual b) {return (a.fit + a.time_to_shoot) < (b.fit + b.time_to_shoot); });
        Best_Solutions.push_back(population[0]);//Best solution in each generation

        std::vector<GEN::Individual> new_population;

        float sum = 0.f;

        //Setting up probality for Roulette wheel
        std::for_each(population.begin(), population.end(), [&sum](GEN::Individual individual) {sum += individual.fit+individual.time_to_shoot; });//without including time_to_shoot alghoritm will not favour shorter time
        std::for_each(population.begin(), population.end(), [sum](GEN::Individual& individual) {individual.probability = sum - individual.fit-individual.time_to_shoot; }); // same as above
        std::for_each(population.begin(), population.end(), [sum](GEN::Individual& individual) {individual.probability /= sum; });

        for (int i = 0; i < POPULATION_SIZE / 2; i++) {
            std::pair<GEN::Individual, GEN::Individual>children = GEN::crossover(GEN::Selection(population), GEN::Selection(population));
            GEN::mutation(children.first);
            GEN::mutation(children.second);
            new_population.push_back(children.first);
            new_population.push_back(children.second);
        }

        population = new_population;
        new_population.clear();

    } while (!GEN::found(Best_Solutions.back()));

    std::cout << Best_Solutions.size() << std::endl;
    for (const auto i : Best_Solutions)
        std::cout << i.angle << "\t" << i.fit << "\t" << i.time_to_shoot << std::endl;

    int itteration = 0;
    bool flag = true;//is used to set bullet starting position

    sf::Clock clock;//is used to measure time between frames
    sf::Clock scene;//is used to measure time duration of scene

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();//time between frames. is used for animation
        sf::Time time = scene.getElapsedTime();

        if (itteration < Best_Solutions.size()) {

            double angle = Best_Solutions[itteration].angle;
            float time_to_shoot = Best_Solutions[itteration].time_to_shoot;

            tower.Set(angle, time_to_shoot);
            tower.GunSprite.setRotation(angle);

            window.clear(sf::Color::White);

            if (time.asSeconds() >= time_to_shoot) {
                tower.Fire();
                if (flag)
                    bullet.bullet.setPosition(bullet.setBulletStartingPosition(tower, angle));
                flag = false;
                bullet.fly(deltaTime, angle);
                window.draw(bullet.bullet);
            }

            airplane.fly(deltaTime);

            window.draw(tower.BaseSprite);
            window.draw(tower.GunSprite);
            window.draw(airplane.AirplaneSprite);

            text.setString(
                "GENERATION: " + std::to_string(itteration+1) +
                "\nNumber of all generations: " + std::to_string(Best_Solutions.size()) +
                "\nAngle: " + std::to_string(Best_Solutions[itteration].angle) +
                "\nFit: " + std::to_string(Best_Solutions[itteration].fit) +
                "\nTime to shoot: " + std::to_string(Best_Solutions[itteration].time_to_shoot) +
                "\nTime in seconds: " + std::to_string(time.asSeconds())
            );

            window.draw(text);
            window.display();

            //Restart scene
            if (time.asSeconds() > 9.5f) {
                airplane.Restart();
                itteration++;
                flag = true;
                scene.restart();
            }
        }
    }

    return 0;
}