#include "Game.h"

//Static Functions

//Initializer functions
void Game::initWindow()
{
    /* Creates a SFML window using option from a window.ini file. */

    std::ifstream ifs("Config/window.ini");

    std::string title = "None";
    sf::VideoMode window_bounds(800, 600);
    unsigned framerate_limit = 60;
    bool veritical_sync_enabled = false;

    if (ifs.is_open())
    {
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs >> framerate_limit;
        ifs >> veritical_sync_enabled;
    }

    ifs.close();

	this->window = new sf::RenderWindow(sf::VideoMode(window_bounds), title);
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(veritical_sync_enabled);
}

void Game::initStates()
{
    this->states.push(new GameState(this->window));
}

//Constructors/Destructors
Game::Game()
{
    this->initWindow();
    this->initStates();
}

Game::~Game()
{
	delete this->window;

    while (!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
}

//Functions
void Game::endApplication()
{
    std::cout << "Ending application!" << "\n";
}

void Game::updateDt()
{
    /* Updates the dt variable with the time it takes to update and render one frame. */

    this->dt = this->dtClock.restart().asSeconds();

    //Displays the dt time
    system("cls");
    std::cout << this->dt << "\n";
}

void Game::updateSFMLEvents()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}

void Game::update()
{
    this->updateSFMLEvents();

    if (!this->states.empty())
    {
        this->states.top()->update(this->dt);

        if (this->states.top()->getQuit())
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    //Application end
    else
    {
        this->endApplication();
        this->window->close();
    }
}

void Game::render()
{
    this->window->clear();

    //Render items
    if (!this->states.empty())
        this->states.top()->render();

    this->window->display();
}

void Game::run()
{
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}
