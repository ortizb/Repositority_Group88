#pragma once
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

#define deleteK 8
#define enterK 13
#define escapeK 27

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

class Textbox {
private:
	sf::Text thisTextbox;
	ostringstream thisText;
	bool isSelected = false;
	bool hasLimit = false;
	int limit = 999;
	void deleter() {
		string myText = thisText.str();
		myText = myText.substr(0, myText.size()-1);
		thisText.str("");
		thisText << myText;
	}
	void input(int typed) {
		if (typed != deleteK && typed != escapeK && typed != enterK) {
			thisText << static_cast<char>(typed);
		}
		else if (typed == deleteK) {
			if (thisText.str().length() > 0) {
				deleter();
			}
		}
		thisTextbox.setString(thisText.str() + "_");
	}
public:
	Textbox() {

	}
	Textbox(bool selected, int textSize) {
		isSelected = selected;
		thisTextbox.setCharacterSize(textSize);
		if (selected) {
			thisTextbox.setString("_");
		}
		else {
			thisTextbox.setString("");
		}
	}
	void setPosition(sf::Vector2f position) {
		thisTextbox.setPosition(position);
	}
	void setSelect(bool setter) {
		isSelected = setter;
		if (!setter) {
			string myText = thisText.str();
			myText = myText.substr(0, myText.size());
			thisTextbox.setString(myText);
		}
	}
	void setFont(sf::Font& font) {
		thisTextbox.setFont(font);
	}
	void setLimit(int theLim, bool setter) {
		limit = theLim;
		hasLimit = setter;
	}
	void typer(sf::Event theEvent) {
		if (isSelected) {
			int typedChar = theEvent.text.unicode;
			if (typedChar < 128) {
				if (hasLimit) {
					if (thisText.str().length() < limit) {
						input(typedChar);
					}
					else if (thisText.str().length() >= limit && typedChar == deleteK) {
						deleter();
					}
				}
				else {
					input(typedChar);
				}
			}
		}
	}
	string getText() {
		return thisText.str();
	}
	void drawTo(sf::RenderWindow& theWin) {
		theWin.draw(thisTextbox);
	}
};

bool isValidDate(string givenDate, int& DOYindex) {
	if (givenDate.length() != 5) {
		return false;
	}
	string monthS = givenDate.substr(0, 2);
	string dayS = givenDate.substr(3);
	int indexVal = 0;
	int month;
	if (isdigit(monthS[0]) && isdigit(monthS[1])) {
		month = stoi(monthS);
		if (month > 0 && month < 13) {
			switch (month) {
			case 1:
				indexVal = 0;
				break;
			case 2:
				indexVal = 31;
				break;
			case 3:
				indexVal = 59;
				break;
			case 4:
				indexVal = 90;
				break;
			case 5:
				indexVal = 120;
				break;
			case 6:
				indexVal = 151;
				break;
			case 7:
				indexVal = 181;
				break;
			case 8:
				indexVal = 212;
				break;
			case 9:
				indexVal = 243;
				break;
			case 10:
				indexVal = 273;
				break;
			case 11:
				indexVal = 304;
				break;
			case 12:
				indexVal = 334;
				break;
			}
		}
		else { return false; }
	}
	else { return false; }
	if (isdigit(dayS[0]) && isdigit(dayS[1])) {
		int day = stoi(dayS);
		if (day < 1) {
			return false;
		}
		if (month == 2) {
			if (day > 28) {
				return false;
			}
		}
		else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
			if (day > 31) {
				return false;
			}
		}
		else if (month == 4 || month == 6 || month == 9 || month == 11) {
			if (day > 30) {
				return false;
			}
		}
		indexVal += (day - 1);
		DOYindex = indexVal;
		return true;
	}
	else { return false; }
}

int main() {
	bool readyForTest = false;
	bool validBirthday = false;
	int DOYindex = 0;
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
	myText.setString("birthday stats!");
	myText.setOutlineThickness(4);
	myText.setCharacterSize(100);
	myText.setPosition(240, 50);
	
	sf::Text invalidEntry;
	invalidEntry.setFont(font2);
	invalidEntry.setString("INVALID ENTRY");
	invalidEntry.setFillColor(sf::Color::Red);
	invalidEntry.setCharacterSize(15);
	invalidEntry.setPosition(580, 380);

	sf::Text validEntry;
	validEntry.setFont(font2);
	validEntry.setString("VALID ENTRY!!");
	validEntry.setFillColor(sf::Color::Green);
	validEntry.setCharacterSize(15);
	validEntry.setPosition(580, 380);

	Textbox myTextbox(true, 30);
	myTextbox.setPosition({ 600,305 });
	myTextbox.setFont(font2);
	myTextbox.setLimit(5, true);

	sf::Sprite textbox1;
	textbox1.setTexture(TextureManager::GetTexture("textbox1.png"));
	textbox1.setPosition(485, 200);


	while (menuWindow.isOpen()) {
		sf::Event event;


		while (menuWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				menuWindow.close();
			}
			else if (event.type == sf::Event::TextEntered) {
				myTextbox.typer(event);
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2i position = sf::Mouse::getPosition(menuWindow);
				//go through relevant bounds, seeing if any of the following were clicked:
				//the key birthdate box
				if (position.x < textbox1.getPosition().x + 300 && position.x > textbox1.getPosition().x && position.y < textbox1.getPosition().y + 200 && position.x) {
					myTextbox.setSelect(true);

				}
				else {
					myTextbox.setSelect(false);
				}
			}
		}
		string birthdayText = myTextbox.getText();
		int& DOYindexR = DOYindex;
		validBirthday = isValidDate(birthdayText, DOYindexR);
		menuWindow.clear();
		menuWindow.draw(background);
		menuWindow.draw(textbox1);
		menuWindow.draw(myText);
		if (validBirthday) {
			menuWindow.draw(validEntry);
		}
		else {
			menuWindow.draw(invalidEntry);
		}
		myTextbox.drawTo(menuWindow);

		menuWindow.display();
	}
	TextureManager::Clear();


	return 0;
}