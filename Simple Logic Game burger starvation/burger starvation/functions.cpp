#include "functions.h"

void vInitializeHud(vector<sf::Text*> &hud, sf::Font &font, vector<string> svector, unsigned uelements_amount, unsigned hud_x_space = 0, unsigned hud_y_space = 0,
	unsigned hud_position_x = 1600 / 2, unsigned hud_position_y = 900 / 3)
{
	for (int i = 0; i != uelements_amount; ++i)
	{
		hud.push_back(new sf::Text);
		hud[i]->setFont(font);
		hud[i]->setCharacterSize(75);
		hud[i]->setFillColor(sf::Color::White);
		hud[i]->setPosition(hud_position_x + (i*hud_x_space), hud_position_y + (i*hud_y_space));
		hud[i]->setString(svector[i]);
	}
}

void vDrawHud(sf::RenderWindow *window, std::vector<sf::Text*> hud)
{
	int ihud_size = hud.size();

	for (int i = 0; i != ihud_size; ++i)
	{
		window->draw(*hud[i]);
	}
}

void vHudDelete(vector<sf::Text*> hud)
{
	for (int i = 0; i != hud.size(); ++i)
	{
		delete hud[i];
	}
}
void vTextureDelete(vector<sf::Texture*> texture)
{
	for (int i = 0; i != texture.size(); ++i)
	{
		delete texture[i];
	}
}
void vSpriteDelete(vector<sf::Sprite*> sprite)
{
	for (int i = 0; i != sprite.size(); ++i)
	{
		delete sprite[i];
	}
}


void vSetSprite(sf::Texture &texture, sf::Sprite &sprite, int x_start = 0, int y_start = 0)
{
	sf::Vector2u texture_size(texture.getSize());

	texture.setSmooth(true);
	texture.setRepeated(true);

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(x_start, y_start, texture_size.x, texture_size.y));
}
void vSetTotalSprite(string path, sf::Texture &texture, sf::Sprite &sprite, int x_start = 0, int y_start = 0)
{
	if (!texture.loadFromFile(path))
	{
		std::cout << "texture: " << path << " failed to load.\n";
	}
	else
	{
		vSetSprite(texture, sprite, x_start, y_start);
	}
}