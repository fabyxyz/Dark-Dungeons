#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;

int main() {
	//Load Settings
	string flickering;
	ifstream flick("Settings/flickering.ini");
	while (flick >> flickering) {}
	int musicVolume;
	ifstream musicVol("Settings/musicVolume.ini");
	while (musicVol >> musicVolume) {}
	int soundVolume;
	ifstream soundVol("Settings/soundVolume.ini");
	while (soundVol >> soundVolume) {}

	//Window
	const int framerate = 60;
	RenderWindow window(VideoMode(1200, 800), "Dark Dungeons", Style::Titlebar | Style::Close);
	const float windowX = window.getSize().x;
	const float windowY = window.getSize().y;
	window.setFramerateLimit(framerate);

	//Cursors
	Cursor arrow, hand;
	arrow.loadFromSystem(Cursor::Arrow);
	hand.loadFromSystem(Cursor::Hand);

	//App Data
	srand((unsigned)time(0));
	bool focused = true;

	//VNG
	RectangleShape vng(Vector2f(windowX, windowY));
	vng.setPosition(Vector2f(0.f, 0.f));
	Texture vngTex;
	vngTex.loadFromFile("Content/Effects/vng.png");
	vng.setTexture(&vngTex);

	//Font
	Font defaultFont;
	defaultFont.loadFromFile("Content/Fonts/defaultFont.ttf");

	//Scenes
	double scene = 0.1;

	//Main Menu
	RectangleShape mainMenu(Vector2f(windowX, windowY));
	mainMenu.setPosition(Vector2f(0.f, 0.f));
	Texture mainMenuTex;
	mainMenuTex.loadFromFile("Content/Scenes/mainMenu.png");
	mainMenu.setTexture(&mainMenuTex);
	const int mainMenuButtons = 3;
	RectangleShape mainMenuButton[mainMenuButtons];
	Text mainMenuText[mainMenuButtons];
	for (int i = 0; i < mainMenuButtons; i++) {
		//Buttons
		mainMenuButton[i].setSize(Vector2f(200.f, 100.f));
		mainMenuButton[i].setOrigin(Vector2f(mainMenuButton[i].getSize().x / 2, mainMenuButton[i].getSize().y / 2));
		mainMenuButton[i].setFillColor(Color(50, 50, 50));
		//Text
		mainMenuText[i].setFillColor(Color(150, 150, 150));
		mainMenuText[i].setCharacterSize(36);
		mainMenuText[i].setFont(defaultFont);
	}
	mainMenuButton[0].setPosition(Vector2f(windowX / 6, 200.f));
	mainMenuButton[1].setPosition(Vector2f(windowX / 6, 400.f));
	mainMenuButton[2].setPosition(Vector2f(windowX / 6, 600.f));
	mainMenuText[0].setString("Joaca");
	mainMenuText[0].setPosition(Vector2f(mainMenuButton[0].getPosition().x - 50.f, mainMenuButton[0].getPosition().y - 20.f));
	mainMenuText[1].setString("Setari");
	mainMenuText[1].setPosition(Vector2f(mainMenuButton[1].getPosition().x - 50.f, mainMenuButton[1].getPosition().y - 20.f));
	mainMenuText[2].setString("Iesi");
	mainMenuText[2].setPosition(Vector2f(mainMenuButton[2].getPosition().x - 50.f, mainMenuButton[2].getPosition().y - 20.f));
	Music mainMenuMusic;
	mainMenuMusic.openFromFile("Content/Music/mainMenu.ogg");
	mainMenuMusic.setVolume(musicVolume);
	mainMenuMusic.setLoop(true);
	bool mainMenuMusicPlayed = false;

	//Settings
	RectangleShape settings(Vector2f(windowX, windowY));
	settings.setPosition(Vector2f(0.f, 0.f));
	Texture settingsTex;
	settingsTex.loadFromFile("Content/Scenes/settings.png");
	settings.setTexture(&settingsTex);
	//Buttons
	RectangleShape goBack(Vector2f(100.f, 50.f));
	goBack.setPosition(Vector2f(20.f, 20.f));
	goBack.setFillColor(Color(50, 50, 50));
	Text goBackText("Inapoi", defaultFont, 26);
	goBackText.setPosition(Vector2f(25.f, 25.f));
	goBackText.setFillColor(Color(150, 150, 150));
	//Volume Buttons
	RectangleShape volumeButton[2][6];
	float volumeButtonSize = 80.f;
	float volumeButtonPos = 100.f;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			volumeButton[i][j].setSize(Vector2f(volumeButtonSize, 50.f));
			volumeButton[i][j].setFillColor(Color(50, 50, 50));
			//Button Position
			if (i == 0) {
				volumeButton[i][j].setPosition(Vector2f(volumeButtonPos, 200.f));
			}
			else {
				volumeButton[i][j].setPosition(Vector2f(volumeButtonPos, 380.f));
			}
			if (j == 5) {
				volumeButtonPos = 0.f;
			}
			volumeButtonPos += volumeButtonSize + 20.f;
		}
	}
	Text musicText("Volum muzica", defaultFont, 36);
	Text soundText("Volum sunete", defaultFont, 36);
	musicText.setPosition(Vector2f(100.f, 120.f));
	musicText.setFillColor(Color(150, 150, 150));
	musicText.setLetterSpacing(2.f);
	soundText.setPosition(Vector2f(100.f, 300.f));
	soundText.setFillColor(Color(150, 150, 150));
	soundText.setLetterSpacing(2.f);

	//Loading Screen
	Text loading("Loading...", defaultFont, 56);
	loading.setPosition(Vector2f(450.f, 350.f));
	loading.setFillColor(Color(150, 150, 150));
	loading.setLetterSpacing(2.f);
	int loadReach = framerate * 5;
	int loadTime = 0;

	//Map
	RectangleShape map(Vector2f(2400.f, 1600.f));
	map.setPosition(Vector2f(0.f, 0.f));
	Texture mapTex;
	mapTex.loadFromFile("Content/Region/map.png");
	map.setTexture(&mapTex);
	Music gameplayMusic;
	gameplayMusic.openFromFile("Content/Music/gameplay.ogg");
	gameplayMusic.setVolume(musicVolume);
	gameplayMusic.setLoop(true);
	bool gameplayMusicPlayed = false;

	//Pause Menu
	bool showPauseMenu = false;
	RectangleShape pauseMenu(Vector2f(windowX / 3, windowY));
	pauseMenu.setPosition(Vector2f(0.f, 0.f));
	pauseMenu.setFillColor(Color(255, 255, 255, 128));

	//Player
	class Player {
		public:
			RectangleShape body;
			float speed = 0.5f;
	};
	Player player;
	player.body.setSize(Vector2f(42.f, 54.f)); //3:1 Ratio
	player.body.setPosition(Vector2f(windowX / 2, windowY / 2));
	double texDelay = 0.5;
	int texTime = 0;
	//Up Textures
	Texture u1;
	u1.loadFromFile("Content/Entities/Player/up1.png");
	Texture u2;
	u2.loadFromFile("Content/Entities/Player/up2.png");
	Texture u3;
	u3.loadFromFile("Content/Entities/Player/up3.png");
	//Down Textures
	Texture d1;
	d1.loadFromFile("Content/Entities/Player/down1.png");
	Texture d2;
	d2.loadFromFile("Content/Entities/Player/down2.png");
	Texture d3;
	d3.loadFromFile("Content/Entities/Player/down3.png");
	//Left Textures
	Texture l1;
	l1.loadFromFile("Content/Entities/Player/left1.png");
	Texture l2;
	l2.loadFromFile("Content/Entities/Player/left2.png");
	Texture l3;
	l3.loadFromFile("Content/Entities/Player/left3.png");
	//Right Textures
	Texture r1;
	r1.loadFromFile("Content/Entities/Player/right1.png");
	Texture r2;
	r2.loadFromFile("Content/Entities/Player/right2.png");
	Texture r3;
	r3.loadFromFile("Content/Entities/Player/right3.png");
	//Default Texture
	player.body.setTexture(&d2);


	while (window.isOpen()) {
		Vector2f pos = (Vector2f)sf::Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			else if (event.type == Event::GainedFocus) {
				focused = true;
			}
			else if (event.type == Event::LostFocus) {
				focused = false;
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					if (scene == 1.0) {
						if (showPauseMenu == false) {
							showPauseMenu = true;
							focused = false;
							gameplayMusic.pause();
						}
						else {
							showPauseMenu = false;
							focused = true;
							gameplayMusic.play();
						}
					}
				}

			}
		}

		//Update Frame
		window.clear();

		if (scene == -1.0) {
			window.setMouseCursor(arrow);
			if (loadTime <= loadReach) {
				window.draw(loading);
				loadTime += 1;
			}
			else {
				scene = 1.0;
			}
		}
		else if (scene == 0) {
			scene = 1.0;
		}
		else if (scene == 0.1) {
			//Play Music
			if (mainMenuMusicPlayed == false) {
				mainMenuMusicPlayed = true;
				mainMenuMusic.play();
			}
			//Hover Effect
			if (mainMenu.getGlobalBounds().contains(pos)) {
				window.setMouseCursor(arrow);
			}
			for (int i = 0; i < mainMenuButtons; i++) {
				if (mainMenuButton[i].getGlobalBounds().contains(pos)) {
					window.setMouseCursor(hand);
					mainMenuButton[i].setFillColor(Color(80, 80, 80));
				}
				else {
					mainMenuButton[i].setFillColor(Color(50, 50, 50));
				}
			}
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (mainMenuButton[0].getGlobalBounds().contains(pos)) {
					mainMenuMusic.stop();
					scene = -1.0;
				}
				if (mainMenuButton[1].getGlobalBounds().contains(pos)) {
					scene = 0.2;
				}
				if (mainMenuButton[2].getGlobalBounds().contains(pos)) {
					window.close();
				}
			}

			//Draw Frame
			window.draw(mainMenu);
			for (int i = 0; i < mainMenuButtons; i++) {
				window.draw(mainMenuButton[i]);
				window.draw(mainMenuText[i]);
			}
		}
		else if (scene == 0.2) {
			//Hover Effect
			if (settings.getGlobalBounds().contains(pos)) {
				window.setMouseCursor(arrow);
			}
			if (goBack.getGlobalBounds().contains(pos)) {
				window.setMouseCursor(hand);
				goBack.setFillColor(Color(80, 80, 80));
			}
			else {
				goBack.setFillColor(Color(50, 50, 50));
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 6; j++) {
					if (volumeButton[i][j].getGlobalBounds().contains(pos)) {
						volumeButton[i][j].setFillColor(Color(80, 80, 80));
					}
					else {
						volumeButton[i][j].setFillColor(Color(50, 50, 50));
					}
				}
			}
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (goBack.getGlobalBounds().contains(pos)) {
					scene = 0.1;
				}
				if (volumeButton[0][0].getGlobalBounds().contains(pos)) {

				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape) && focused == true) {
				scene = 0.1;
			}

			//Draw Frame
			window.draw(settings);
			window.draw(goBack);
			window.draw(goBackText);
			window.draw(musicText);
			window.draw(soundText);
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 6; j++) {
					window.draw(volumeButton[i][j]);
				}
			}
		}
		else if (scene == 1.0) {
			//Play Music
			if (gameplayMusicPlayed == false) {
				gameplayMusicPlayed = true;
				gameplayMusic.play();
			}
			//Hover Effect
			if (map.getGlobalBounds().contains(pos)) {
				window.setMouseCursor(arrow);
			}
			//Flickering Effect
			if (flickering == "true") {
				int flick = rand() % 50 + 1;
				if (flick == 1) {
					scene = 0;
				}
			}
			//Player Movement
			if (focused == true) {
				if (map.getPosition().y < windowY / 2) {
					if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
						map.move(Vector2f(0.f, player.speed));
						texTime += 1;
						if (texTime > 0 && texTime < framerate * texDelay) {
							player.body.setTexture(&u1);
						}
						else if (texTime > framerate * texDelay && texTime < (framerate * texDelay) * 2) {
							player.body.setTexture(&u2);
						}
						else if (texTime > (framerate * texDelay) * 2 && texTime < (framerate * texDelay) * 3) {
							player.body.setTexture(&u3);
						}
						else if (texTime > (framerate * texDelay) * 3 && texTime < (framerate * texDelay) * 4) {
							player.body.setTexture(&u2);
						}
						else if (texTime > (framerate * texDelay) * 4) {
							texTime = 0;
						}
					}
				}
				if (map.getPosition().y > -1152) {
					if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
						map.move(Vector2f(0.f, -player.speed));
						texTime += 1;
						if (texTime > 0 && texTime < framerate * texDelay) {
							player.body.setTexture(&d1);
						}
						else if (texTime > framerate * texDelay && texTime < (framerate * texDelay) * 2) {
							player.body.setTexture(&d2);
						}
						else if (texTime > (framerate * texDelay) * 2 && texTime < (framerate * texDelay) * 3) {
							player.body.setTexture(&d3);
						}
						else if (texTime > (framerate * texDelay) * 3 && texTime < (framerate * texDelay) * 4) {
							player.body.setTexture(&d2);
						}
						else if (texTime > (framerate * texDelay) * 4) {
							texTime = 0;
						}
					}

				}
				if (map.getPosition().x < windowX / 2) {
					if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
						map.move(Vector2f(player.speed, 0.f));
						texTime += 1;
						if (texTime > 0 && texTime < framerate * texDelay) {
							player.body.setTexture(&l1);
						}
						else if (texTime > framerate * texDelay && texTime < (framerate * texDelay) * 2) {
							player.body.setTexture(&l2);
						}
						else if (texTime > (framerate * texDelay) * 2 && texTime < (framerate * texDelay) * 3) {
							player.body.setTexture(&l3);
						}
						else if (texTime > (framerate * texDelay) * 3 && texTime < (framerate * texDelay) * 4) {
							player.body.setTexture(&l2);
						}
						else if (texTime > (framerate * texDelay) * 4) {
							texTime = 0;
						}
					}
				}
				if (map.getPosition().x > -1776) {
					if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
						map.move(Vector2f(-player.speed, 0.f));
						texTime += 1;
						if (texTime > 0 && texTime < framerate * texDelay) {
							player.body.setTexture(&r1);
						}
						else if (texTime > framerate * texDelay && texTime < (framerate * texDelay) * 2) {
							player.body.setTexture(&r2);
						}
						else if (texTime > (framerate * texDelay) * 2 && texTime < (framerate * texDelay) * 3) {
							player.body.setTexture(&r3);
						}
						else if (texTime > (framerate * texDelay) * 3 && texTime < (framerate * texDelay) * 4) {
							player.body.setTexture(&r2);
						}
						else if (texTime > (framerate * texDelay) * 4) {
							texTime = 0;
						}
					}
				}
			}
			window.draw(map);
			window.draw(player.body);
			window.draw(vng);
		}

		if (showPauseMenu == true) {
			window.draw(pauseMenu);
		}

		window.display();

	}

	return 0;
}
