#include "Juego.h"

//Funciones privadas
void Juego::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->limiteAlcanzado = false;
	this->points = 0;
	this->health = 15;
	this->enemigosMatados = 0;
	this->ETSpawnTimer = this->ETSpawnTimerMax;
	this->ETSpawnTimerMax = 10.f;
	this->spawnActivo = true;
	this->maxET = 6;
	this->mouseHeld = false;
}

void Juego::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Videojuego - E6", sf::Style::Titlebar | sf::Style::Close);
	this->window->setMouseCursorVisible(false);

	this->window->setFramerateLimit(60);

}

//Mira
void Juego::initMira()
{
	this->t_mira.loadFromFile("crosshair.png");
	this->s_mira.setTexture(t_mira);

	this->s_mira.setScale(sf::Vector2f(0.2f, 0.2f));
	this->s_mira.setPosition(0, 0);
}

void Juego::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(20);
	this->uiText.setFillColor(sf::Color::Green);
	this->uiText.setString("NONE");
}



void Juego::initFont()
{
	if(this->font.loadFromFile("Fonts/RubikMoonrocks-Regular.ttf"))
	{
	std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" <<  "\n" ;
	}
}

void Juego::initGameOver()
{
	this->t_gameOver.loadFromFile("GameOver.png");
	this->s_gameOver.setTexture(t_gameOver);
	this->s_gameOver.setPosition(240, 125);
}

void Juego::initWin()
{
	this->t_win.loadFromFile("YouWin.png");
	this->s_win.setTexture(t_win);
	this->s_win.setPosition(240, 125);
}

//Enemigos
void Juego::initEt()
{
	this->texture_et.loadFromFile("et.png");
	this->sprite_et.setTexture(texture_et);

	this->sprite_et.setScale(sf::Vector2f(0.06f,0.06f)); 
}


//Constructores / Destructores
Juego::Juego()
{
	this->initVariables();
	this->initWindow();
	this->initFont();
	this->initText();
	this->initEt();
	this->initMira();
	this->initGameOver();
	this->initWin();
}

Juego::~Juego()
{

	delete this->window;

}

//Accesors
const bool Juego::running() const
{
	return this->window->isOpen();
}

const bool Juego::getEndGame() const
{
	return this->endGame;
}


//Funciones

void Juego::spawnET()
{
	if (!this->spawnActivo)
		return;

	this->sprite_et.getGlobalBounds();

	float xPos = (static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - bounds.width)));

	this->sprite_et.setPosition(xPos, 0.f);

	//Spawn the enemy
	this->ETs.push_back(this->sprite_et);


}

void Juego::pollEvents()
{
	while (this->window->pollEvent(this->evt))
	{
		switch (this->evt.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->evt.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}

	}
}


void Juego::updateET()
{
	//Timer para el spawn de los enemigos
	if (this->ETs.size() < this->maxET) { //"size" es como "la cantidad", es un parametro que tomando datos dados como maximos y minimos estima una "longitud" 
	
		if (this->ETSpawnTimer >= this->ETSpawnTimerMax) 
		{

			//Spawn de los ET y reset del timer
			this->spawnET();
			this->ETSpawnTimer = 0.f;
		}
		else {
			this->ETSpawnTimer += 0.5f;
		}
	}

	//Movimiento de los enemigos de manera vertical 
	for (int i = 0; i < this->ETs.size(); i++)
	{
		bool deleted = false;

		this->ETs[i].move(0.f, 5.f); //Velocidad de movimiento en "y"

		if (this->ETs[i].getPosition().y > this->window->getSize().y)
		{
			if (!this->limiteAlcanzado)
			{
				this->ETs.erase(this->ETs.begin() + i);

				if (this->enemigosMatados < 5){
					this->health -= 1;

						if(this->health <= 0)
						{
						this->limiteAlcanzado = true;
						this->spawnActivo = false;
						}
					}
				}
			}
		}



	//Chequear si se cliqueo 
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->ETs.size() && deleted == false; i++)
			{
				if (this->ETs[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Eliminar enemigos
					deleted = true;
					this->ETs.erase(this->ETs.begin() + i);

					//Dar puntos
					if (!this->limiteAlcanzado)
					{
						this->enemigosMatados++;
						this->points += 1;
					}

					//Limitar máximo de enemigos a eliminar
					if (this->enemigosMatados >= 5)
					{
						this->limiteAlcanzado = true;
						this->spawnActivo = false;
					}
				}
				
			}
		}
		else
		{
			this->mouseHeld = false;
		}
	}
}


void Juego::updateMousePosition()
{

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

}

void Juego::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n";

	ss << "Health: " << this->health<< "\n";

	this->uiText.setString(ss.str());
}

void Juego::update()
{

	this->pollEvents();

	if(this->endGame == false)
	{
	this->updateMousePosition();

	this->updateText();

	this->updateET();
	}

	//Condición de fin de juego
	//if (this->health <= 0)
		//this->endGame = true;

	//if (this->points == 5)
		//this->endGame = true;
}

void Juego::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Juego::renderMira(sf::RenderTarget& target)
{
	// Obtener la posición actual del ratón en la ventana
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	// Actualizar la posición de la mira
	this->s_mira.setPosition(static_cast<float>(mousePosWindow.x), static_cast<float>(mousePosWindow.y));

	// Dibujar la mira
	this->window->draw(this->s_mira);

}

void Juego::renderET(sf::RenderTarget& target)
{
	for (auto& e : this->ETs)
	{
		target.draw(e);
	}
}

void Juego::renderGameOver()
{
	if (this->health <= 0)
	{
		this->spawnActivo = false;
		this->window->draw(this->s_gameOver);
	}
}

void Juego::renderWin()
{
	if (this->points == 5)
	{
		this->spawnActivo = false;
		this->window->draw(this->s_win);
	}
}


void Juego::render()
{

	this->window->clear(sf::Color::Black);

	//Dibujar los objetos del juego

	this->renderET(*this->window);

	this->renderMira(*this->window);

	this->renderText(*this->window);

	this->renderGameOver();

	this->renderWin();

	this->window->display();

}
