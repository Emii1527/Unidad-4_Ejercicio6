#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


class Juego
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event evt;

	//Mouse 
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	
	//Feedback
	sf::Font font;
	sf::Text uiText;
	 
	
	bool endGame;
	unsigned points;
	int health;
	int enemigosMatados;
	float ETSpawnTimer; //Tiempo de referencia para manejar máximos y mínimos de los spawns
	float ETSpawnTimerMax; //Tiempo límite máximo entre un spawn y otro (distancia de sprites)
	int maxET; //Cantidad de enemigos por tiempo de spawn 
	bool mouseHeld;
	bool spawnActivo;
	bool limiteAlcanzado;
	

	//Game objects
	sf::FloatRect bounds;

	std::vector<sf::Sprite> ETs;

	sf::Texture texture_et, t_mira, t_gameOver, t_win; 
	sf::Sprite sprite_et, s_mira, s_gameOver, s_win;

	//Funciones privadas
	void initVariables();
	void initWindow();
	void initEt();
	void initMira();
	void initText();
	void initFont();
	void initGameOver();
	void initWin();

public:

	//Constructores / Destructores
	Juego();
	virtual ~Juego();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Funciones
	// 
	void spawnET();
	void pollEvents();

	void updateET();
	void updateMousePosition();
	void updateText();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderMira(sf::RenderTarget& target);
	void renderET(sf::RenderTarget& target);
	void renderGameOver();
	void renderWin();
	void render();

};

