/*
    This is a simulation of a virus infection of a population.
    After a distinct time, the virus cannot anymore propagate.
    
    What we can see is, that some individuals get protected by
    others which are still immune.
    
    Copyleft license, guaranteed by nico.schumann@startmail.com
    
    Compile with:
       c++ -o <exe_file> corona.cpp -lsfml-graphics -lsfml-system -lsfml-window
*/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

const int WIN_HEIGHT   = 200;
const int WIN_WIDTH    = 200;
const int NO_OF_AGENTS =  40;
const int FRAME_RATE   =  60;
const int ILL_TIME     =  14

class Agent : public sf::RectangleShape
{
    int m_infectedDays;
    bool m_immune;
public:
    Agent() 
    : sf::RectangleShape(sf::Vector2f(10, 10))
    , m_infectedDays{0}, m_immune{false}
    { setFillColor(sf::Color::Green); }
    ~Agent() {}
    
    bool isInfected() const { return static_cast<bool>(m_infectedDays); }
    bool isImmune() const { return m_immune; }
    void infect() { 
        if( false == m_immune) {
            m_infectedDays = ILL_TIME * FRAME_RATE;
            m_immune = true;
            setFillColor(sf::Color::Red);
        }
    }
    void update() { 
        if (m_infectedDays) {
            if(m_infectedDays == 1 ) setFillColor(sf::Color::Cyan);
            --m_infectedDays;
        }
    }
};

class Game
{
public:
    Game() 
    : m_win( sf::VideoMode( WIN_WIDTH, WIN_HEIGHT ), "Corona")
    , m_gameOver{ false }
    {
        std::srand(std::time(0));
        for( int i = 0; i < NO_OF_AGENTS; ++i)
        {
            Agent agent;
            agent.setPosition( 
                std::rand() % WIN_WIDTH, std::rand() % WIN_HEIGHT 
            );
            m_agents.push_back( agent );
        }
        m_agents[0].infect();
    }
    
    ~Game() { m_win.close(); }
    
    void run() {
        while( true )
        {
            input();
            if ( m_gameOver ) return;
            update();
            draw();
            sf::sleep( sf::seconds( 1.0f / FRAME_RATE ) );
        }
    }
    
    
private:
    void update() {
    
        // moving the agents
        for( auto & agent : m_agents )
        {
            int x = agent.getPosition().x;
            int y = agent.getPosition().y;
            int dx = std::rand() % 3 - 1;
            int dy = std::rand() % 3 - 1;
            if( x + dx <= 0 ) dx = 1;
            if( y + dy <= 0 ) dy = 1;
            if( x + dx + agent.getSize().x >= WIN_WIDTH ) dx = -1;
            if( y + dy + agent.getSize().y >= WIN_HEIGHT) dy = -1;
            
            agent.move( dx, dy );
            agent.update();
        }
        
        // handling intersections
        for( int i = 0; i < m_agents.size(); ++i )
        {
            for( int k = 0; k < m_agents.size(); ++k )
            {
                if ( i != k
                    && ( m_agents[i].isInfected() || m_agents[k].isInfected())
                    && m_agents[i].getGlobalBounds().intersects(
                            m_agents[k].getGlobalBounds()
                       )
                )
                {
                    m_agents[i].infect();
                    m_agents[k].infect();
                }
            }
        }
    }
    
    void draw() {
        m_win.clear();
        for ( const auto & agent : m_agents )
        {
            m_win.draw( agent );
        }
        m_win.display();
    }
    
    void input() {
        while( m_win.pollEvent( m_event ) )
        {
            if( m_event.type == sf::Event::Closed )
                m_gameOver = true;
        }
    }
                    
private:
    bool m_gameOver;
    sf::Event m_event;
    sf::RenderWindow m_win;
    std::vector<Agent> m_agents;
};
int main()
{
    Game game;
    game.run();
}
