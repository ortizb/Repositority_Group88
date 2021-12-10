#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Header.h"
using namespace std;

class TextureManager {
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string fileName);
public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear();
};
unordered_map<string, sf::Texture> TextureManager::textures;
void TextureManager::LoadTexture(string fileName) {
	string path = "assets/";
	path += fileName;
	textures[fileName].loadFromFile(path);
}
sf::Texture& TextureManager::GetTexture(string textureName) {
	if (textures.find(textureName) == textures.end()) {
		LoadTexture(textureName);
	}
	return textures[textureName];
}
void TextureManager::Clear() {
	textures.clear();
}

int main() {
	int mode = 0;

	sf::RenderWindow menuWindow(sf::VideoMode(1280, 835), "Birthday Clusters!");

	sf::Sprite background(TextureManager::GetTexture("soccerfield.png"));

	sf::Font font1;
	if (!font1.loadFromFile("assets/Soccerleague.ttf")) {
		cout << "Error loading" << endl;
		system("pause");
	}
	sf::Font font2;
	if (!font2.loadFromFile("assets/arial.ttf")) {
		cout << "Error loading" << endl;
		system("pause");
	}
	sf::Text myText;
	myText.setFont(font1);
	myText.setString("Birthdays in Sports");
	myText.setOutlineThickness(4);
	myText.setCharacterSize(100);
	myText.setPosition(200, 50);

	sf::Sprite instructions;
	instructions.setTexture(TextureManager::GetTexture("instructions.png"));
	instructions.setPosition(140, 200);

	sf::Sprite button0;
	button0.setTexture(TextureManager::GetTexture("button0.png"));
	button0.setPosition(540, 550);

	sf::Sprite button1;
	button1.setTexture(TextureManager::GetTexture("button1.png"));
	button1.setPosition(540, 550);
	
	bool filePresent = false; 
	ifstream loadedFile;
	while (menuWindow.isOpen()) {
		menuWindow.draw(background);
		menuWindow.draw(myText);

		sf::Event event;

		//check if the file is ready
		loadedFile.close();
		loadedFile.open("input.csv");
		if (loadedFile) {
			filePresent = true;
		}
		else {
			filePresent = false;
		}

		if (mode == 0) {
			while (menuWindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					menuWindow.close();
				}
				else if (event.type == sf::Event::MouseButtonPressed) {
					if (filePresent) {
						sf::Vector2i position = sf::Mouse::getPosition(menuWindow);
						if (position.x < button1.getPosition().x + 200 && position.x > button1.getPosition().x && position.x < button1.getPosition().y + 200 && position.x > button1.getPosition().y) {
							mode = 1;
						}
					}
				}
			}
			menuWindow.draw(instructions);
			if (filePresent) {
				menuWindow.draw(button1);
			}
			else {
				menuWindow.draw(button0);
			}
			menuWindow.display();

		}
		if (mode == 1) {
			//get results and post results
			vector<pair<pair<string, int>, vector<pair<string, int>>> > playerList;
			vector<pair<pair<string, int>, vector<pair<string, int>>>> ourGraph = graphGenerator(loadedFile, playerList);

			menuWindow.display();
			while (true) {

			}


		}

	}
	TextureManager::Clear();
	return 0;
}
