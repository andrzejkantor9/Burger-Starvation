#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <chrono>
#include <thread>

#include <iostream>
#include <ctime>
#include <sstream>
#include <cmath>

#include "ingredients.h"

using std::vector;
using std::string;

#define EQUALS_MOUSE_POSITION ->getGlobalBounds().contains(fmouse_position)
#define LEFT_BUTTON_PRESSED sf::Mouse::isButtonPressed(sf::Mouse::Left)

int i_inside_burger_counter{ 0 };//static wanna be
float f_ingredient_move{ 0.f };

void vSetSingleTxt(sf::Text &txt_ingredients, sf::Font &font, sf::Color color= sf::Color::White, sf::Vector2f v2f_position = sf::Vector2f(0.f, 0.f));
void vInitializeHud(vector<sf::Text*> &hud, sf::Font &font, vector<string> svector, unsigned uelements_amount, unsigned hud_x_space = 0, unsigned hud_y_space = 0,
	unsigned hud_position_x = 1600 / 2, unsigned hud_position_y = 900 / 3);
void vInitializeDescisionMenu(vector<sf::Text*> &hud, sf::Font &font, vector<string> s_vector, unsigned u_elements_amount = 3);
void vSetOneTxt(vector<sf::Text*> &txt, sf::Font &font, string s_ingredient_name, sf::Color color = sf::Color::White);
void vSetOneSpr(vector <sf::Sprite*> &sprite, sf::Texture &texture, string path, float scale = 1.f, int which_ingredient = 0);
void vSetBool(vector<bool> &bingredients, int);

void vDrawHud(sf::RenderWindow *window, std::vector<sf::Text*> hud);
bool bHandlePlayAgain(sf::RenderWindow &window, vector<sf::Text*> txr, bool &b_main_menu, bool &b_pause_menu, bool &b_play_again,
	float &f_poison_count, float &f_ingredient_move, float &f_feed_count, vector<float> &f_poison_value, vector<float> &f_ingredient_feed_value,
	sf::RectangleShape &rts_poison_hud, sf::RectangleShape &rts_feed_hud,
	sf::Text &txt_poison_count, sf::Text &txt_feed_count,
	vector<sf::Sprite*> &spr_burger_bread,
	int &i_inside_burger_counter, bool b_totally_new_game,
	int *i_turns_left, sf::RectangleShape &rts_turns_left, sf::Text &txt_turns_left);
void vDrawCircle(sf::RenderWindow *window, std::vector<sf::CircleShape*> circle, vector<bool> vb_bool);

void vHudDelete(vector<sf::Text*> hud);
void vTextureDelete(vector<sf::Texture*> texture);
void vSpriteDelete(vector<sf::Sprite*> sprite);
void vClearContinuousSpr(vector <sf::Sprite*> &sprite);

void vSetSprite(sf::Texture &texture, sf::Sprite &sprite, int x_start = 0, int y_start = 0, float scale =1.f);
void vSetTotalSprite(string path, sf::Texture &texture, sf::Sprite &sprite, int x_start = 0, int y_start = 0, float scale =1.f);

string sGetFloatPrecision(float value, int precision);
void vResetGame(float &f_poison_count, float &f_ingredient_move, float &f_feed_count, vector<float> &f_poison_value, vector<float> &f_ingredient_feed_value,
	sf::RectangleShape &rts_poison_hud, sf::RectangleShape &rts_feed_hud,
	sf::Text &txt_poison_count, sf::Text &txt_feed_count,
	vector<sf::Sprite*> &spr_burger_bread, 
	int &i_inside_burger_counter, bool b_totally_new_game,
	int *i_turns_left = nullptr, sf::RectangleShape &rts_turns_left = sf::RectangleShape(), sf::Text &txt_turns_left = sf::Text());

int main()
{
	FreeConsole();

	srand(time(0));

	int screen_width{ 1600 };
	int screen_height{ 900 };
	int i_turns_left{ 20 }, i_poisoned_ingredients{ 0 };
	float f_feed_count{ 7.5f}, f_poison_count{ 2.f };
	bool was_clicked{ false }, b_write_poisoned{ false };
	bool b_main_menu{ false }, boptions_menu{ false }, b_exit_menu{ false }, bcredits_menu{ false }, btexture_load{ true }, bpause_menu{ false }, back_to_pause_menu{ false };
	bool bstart_game{ true }, bingredients_txt{ false }, b_inside_burger{ false }, b_turns_left{ false }, b_txt_turns_left{ false }, b_ingredient_name{ false };
	bool b_poison_meter{ false }, b_reset_visible{ false }, b_circle_visible{ false }, b_feed_visible{ false };
	bool b_is_window_focused{ true }, b_pause_menu{ false }, b_esc_up{ false }, b_play_again_confirm{ false }, b_game_finished{ false };

	vector<string> s_ingredient{ "beef", "cucumber", "ketchup", "lettuce", "onion", "tomato" };
	vector<bool> bingredients;
	vector<bool> bburger_bread, vb_is_poisoned, vb_circle_visible;

	vSetBool(vb_circle_visible, s_ingredient.size());
	vSetBool(vb_is_poisoned, s_ingredient.size());
	for (int i = 0; i != s_ingredient.size() /2; ++i)
	{
		int i_poison_temp = rand() % s_ingredient.size();
		if (vb_is_poisoned[i_poison_temp] == true)	--i;
		else vb_is_poisoned[i_poison_temp] = true;
	}

	vSetBool(bburger_bread, 2);
	vSetBool(bingredients, s_ingredient.size());

	int hud_position_x, hud_position_y;

	hud_position_x = screen_width / 2.f, hud_position_y = screen_height / 3.f;
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Burger Starvation.");
	window.setFramerateLimit(60);		
	sf::Vector2u vu_screen_size(screen_width, screen_height);
	sf::Vector2f vu_position(hud_position_x, hud_position_y - 200);

	std::vector<sf::Text*> hud;
	std::vector<sf::Text*> hud_pause;
	std::vector<sf::Text*> txr_exit_confirm;
	std::vector<sf::Text*> txr_play_again_confirm;

	sf::Text hud_fps_counter, hud_clock;
	sf::Font font;
	font.loadFromFile("fonts/DS-DIGIT.ttf");

	std::vector<std::string> menu_1s{ "Resume", "Play Again", "Exit" };
	vector<string> s_exit{ "Yes", "No", "Do you really want to exit?" };
	vector<string> s_play_again{ "Yes", "No", "Do you want to play again?" };
	vector<string> spause{ "Powrot", "Options", "Main Menu", "Exit" };

	vInitializeHud(hud, font, menu_1s, 3, 0, 80);
	vInitializeDescisionMenu(txr_exit_confirm, font, s_exit);
	vInitializeDescisionMenu(txr_play_again_confirm, font, s_play_again);
	vInitializeHud(hud_pause, font, spause, 4, 0, 80, 100, 100);

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f fmouse_position;
	vector<sf::Texture*> txr_ingredients;
	vector<sf::Sprite*> spr_ingredients;

	sf::Texture txr_pause;
	sf::Sprite spr_pause;
	vSetTotalSprite("sprites/black.png", txr_pause, spr_pause);
	spr_pause.setColor(sf::Color(255, 255, 255, 155));

	vector<sf::Texture*> txr_burger_bread;
	vector<sf::Sprite*> spr_burger_bread;
	sf::Text txt_ingredients;
	vSetSingleTxt(txt_ingredients, font, sf::Color::Black);
	
	vector<sf::Texture*> txr_inside_burger;
	vector<sf::Text*> txt_inside_burger;
	vector<sf::Sprite*> spr_inside_burger;
	vector<string> s_inside_burger{ "sprites/beef.png", "sprites/cucumber inside.png", "sprites/ketchup inside.png", "sprites/lettuce.png", 
		"sprites/onion.png", "sprites/tomato inside.png" };
	vector<float> f_inside_burger_scale{ 2.f, .8f, 2.5f, 1.8f, 0.8f, .65f };
	vector<float> f_ingredient_feed_value{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
	vector<float> f_poison_value{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, };

	vector<float> f_bread_move{30.f, 30.f, 35.f , 30.f , 30.f , 25.f };

	for (int i = 0; i != 2; ++i)
	{
		txr_burger_bread.push_back(new sf::Texture);
		spr_burger_bread.push_back(new sf::Sprite);
	}
	for (int i = 0; i != s_ingredient.size(); ++i)
	{
		txr_ingredients.push_back(new sf::Texture);
		spr_ingredients.push_back(new sf::Sprite);
	}
	for (int i = 0; i != s_ingredient.size(); ++i)
	{
		txr_inside_burger.push_back(new sf::Texture);
		if (!txr_inside_burger[i]->loadFromFile(s_inside_burger[i]))
		{
			std::cout << "texture: " << s_inside_burger[i] << " failed to load.\n";
		}
		else
		{
			txr_inside_burger[i]->setSmooth(true);
			txr_inside_burger[i]->setRepeated(true);
		}
	}

	vSetTotalSprite("sprites/greenbox.jpg", texture, sprite, 0, 0, 1.125f);
	
	vSetTotalSprite("sprites/beef.png", *txr_ingredients[0], *spr_ingredients[0], 10, 10);
	vSetTotalSprite("sprites/cucumber.png", *txr_ingredients[1], *spr_ingredients[1], 10, 200, 0.5f);
	vSetTotalSprite("sprites/ketchup.png", *txr_ingredients[2], *spr_ingredients[2], 10, 400);
	vSetTotalSprite("sprites/lettuce.png", *txr_ingredients[3], *spr_ingredients[3], 10, 600);
	vSetTotalSprite("sprites/onion.png", *txr_ingredients[4], *spr_ingredients[4], 300, 10, 0.45f);
	vSetTotalSprite("sprites/tomato.png", *txr_ingredients[5], *spr_ingredients[5], 300, 200, 0.5f);	
	for (int i = 0; i != s_ingredient.size(); ++i)
		spr_ingredients[i]->setColor(sf::Color(255, 255, 255, 180));

	vSetTotalSprite("sprites/burger bread up.png", *txr_burger_bread[1], *spr_burger_bread[1], 1600/3, 900/3, 1.f);
	vSetTotalSprite("sprites/burger bread down.png", *txr_burger_bread[0], *spr_burger_bread[0], 1600 / 3, 900 / 1.8f, 1.f);

	sf::SoundBuffer sdb_ingredient_confirm;
	sf::Sound snd_ingredient_confirm;
	snd_ingredient_confirm.setVolume(25);
	sf::SoundBuffer sdb_poisoned_ingredient;
	sf::Sound snd_poisoned_ingredient;
	snd_poisoned_ingredient.setVolume(50);

	if (!sdb_ingredient_confirm.loadFromFile("music/explosion.wav"))
	{
		std::cout << "Failed to load ingredient confirm sound.\n";
	}
	else
	{
		snd_ingredient_confirm.setBuffer(sdb_ingredient_confirm);
	}
	if (!sdb_poisoned_ingredient.loadFromFile("music/cough.wav"))
		std::cout << "Failed to load poisoned ingredient sound.\n";
	else	snd_poisoned_ingredient.setBuffer(sdb_poisoned_ingredient);

	sf::Music mus_background;
	sf::Music mus_won;
	sf::Music mus_lost;
	if (!mus_background.openFromFile("music/background - adrian von ziegler - dance with trees.ogg"))
	{
		std::cout << "Failed to load background music.\n";
	}
	else
	{
		mus_background.play();
		mus_background.setLoop(true);
		mus_background.setVolume(35.f);
	}

	sf::RectangleShape rts_turns_left(sf::Vector2f(15, i_turns_left * 15));
	rts_turns_left.setPosition(sf::Vector2f(1400, 70));
	rts_turns_left.setFillColor(sf::Color(207, 244, 255));
	sf::Text txt_turns_left;
	vSetSingleTxt(txt_turns_left, font, sf::Color(207, 244, 255), sf::Vector2f(1450, 70));
	txt_turns_left.setString(std::to_string(i_turns_left));

	sf::RectangleShape rts_poison_hud(sf::Vector2f(15, f_poison_count * 15.f));
	rts_poison_hud.setPosition(sf::Vector2f(1300, 70));
	rts_poison_hud.setFillColor(sf::Color(192, 174, 222)); 
	sf::Text txt_poison_count;
	vSetSingleTxt(txt_poison_count, font, sf::Color(192, 174, 222), sf::Vector2f(1350, 70));
	txt_poison_count.setString(sGetFloatPrecision(f_poison_count, 2)); 

	sf::RectangleShape rts_feed_hud(sf::Vector2f(15, f_feed_count * 15));
	rts_feed_hud.setPosition(sf::Vector2f(1500, 70));
	rts_feed_hud.setFillColor(sf::Color(24, 188, 63));
	sf::Text txt_feed_count;
	vSetSingleTxt(txt_feed_count, font, sf::Color(24, 188, 63), sf::Vector2f(1520, 70));
	txt_feed_count.setString(sGetFloatPrecision(f_feed_count, 2));

	sf::Texture txr_reset;
	sf::Sprite spr_reset;
	vSetTotalSprite("sprites/reset.png", txr_reset, spr_reset, 1420, 700, 0.55f);
	spr_reset.setColor(sf::Color(255, 255, 255, 180));

	vector<sf::CircleShape*> crs_poison_dots;
	for (int i = 0; i != s_ingredient.size(); ++i)
	{
		crs_poison_dots.push_back(new sf::CircleShape);
		crs_poison_dots[i]->setRadius(15.f);
		crs_poison_dots[i]->setPosition(spr_ingredients[i]->getPosition());
	}

	vector<string> s_game_finished = { "You have won!", "You have lost." };
	sf::Text txt_game_finished(s_game_finished[0], font, 75);
	txt_game_finished.setFillColor(sf::Color::White);
	txt_game_finished.setPosition(550, 100);

	while (window.isOpen())
	{
		fmouse_position.x = static_cast<float>(sf::Mouse::getPosition(window).x), fmouse_position.y = static_cast<float>(sf::Mouse::getPosition(window).y);
		sf::Event event;
		window.pollEvent(event);
		if (event.type == sf::Event::LostFocus)
			b_is_window_focused = false;
		else if (event.type == sf::Event::GainedFocus)
			b_is_window_focused = true;

		if (b_is_window_focused)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				b_pause_menu = !b_pause_menu;
				//while (event.type != sf::Event::KeyReleased && event.key.code != sf::Keyboard::Escape)
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !b_write_poisoned)
			{
				b_write_poisoned = true;
				for (int i = 0; i != s_ingredient.size(); ++i) if (vb_is_poisoned[i]) std::cout << i << " - is poisoned, ";
			}
			else if (bstart_game && !b_pause_menu)
			{
				b_main_menu = false;
				b_turns_left = true;
				b_txt_turns_left = true;
				b_poison_meter = true;
				b_reset_visible = true;
				b_feed_visible = true;
				for (int i = 0; i != bingredients.size(); ++i)
				{
					bingredients[i] = true;
					if (spr_ingredients[i]EQUALS_MOUSE_POSITION)
					{
						txt_ingredients.setString(s_ingredient[i]);
						txt_ingredients.setPosition(spr_ingredients[i]->getPosition());
						bingredients_txt = true;
						spr_ingredients[i]->setColor(sf::Color(255, 255, 255, 255));
						if (LEFT_BUTTON_PRESSED)
						{
							sf::Vector2f v2f_upper_bread_position(spr_burger_bread[1]->getPosition().x, spr_burger_bread[1]->getPosition().y - f_bread_move[i]);
							spr_burger_bread[1]->setPosition(v2f_upper_bread_position);
							vSetOneSpr(spr_inside_burger, *txr_inside_burger[i], s_inside_burger[i], f_inside_burger_scale[i], i);
							b_inside_burger = true;

							--i_turns_left;
							if (i_turns_left == 0)
							{
								b_pause_menu = true;
								b_play_again_confirm = true;
								b_game_finished = true;
								txt_game_finished.setString(s_game_finished[1]);

								if (!mus_background.openFromFile("music/lost - adrian von ziegler - passing.ogg"))
								{
									std::cout << "Failed to load background music.\n";
								}
								else
								{
									mus_background.play();
									mus_background.setLoop(true);
									mus_background.setVolume(25.f);
								}
							}

							rts_turns_left.setSize(sf::Vector2f(15.f, static_cast<float>(i_turns_left) * 15.f));
							rts_turns_left.setPosition(sf::Vector2f(1400, 70));		/*+ ((50 - i_turns_left)*15.f)*/
							txt_turns_left.setString(std::to_string(i_turns_left));

							f_feed_count -= f_ingredient_feed_value[i];
							f_ingredient_feed_value[i] /= 2;
							if (f_feed_count <= 0)
							{
								b_pause_menu = true;
								b_play_again_confirm = true;
								b_game_finished = true;
								txt_game_finished.setString(s_game_finished[0]);

								if (!mus_background.openFromFile("music/won - adrian von ziegler - where i belong.ogg"))
								{
									std::cout << "Failed to load background music.\n";
								}
								else
								{
									mus_background.play();
									mus_background.setLoop(true);
									mus_background.setVolume(25.f);
								}
							}
							rts_feed_hud.setSize(sf::Vector2f(15.f, f_feed_count * 15.f));
							rts_feed_hud.setPosition(sf::Vector2f(1500, 70));
							txt_feed_count.setString(sGetFloatPrecision(f_feed_count, 2));

							if (vb_is_poisoned[i])
							{
								snd_poisoned_ingredient.play();
								f_poison_count -= f_poison_value[i];
								f_poison_value[i] /= 2;
								if (f_poison_count <= 0)
								{
									b_pause_menu = true;
									b_play_again_confirm = true;
									b_game_finished = true;
									txt_game_finished.setString(s_game_finished[1]);

									if (!mus_background.openFromFile("music/lost - adrian von ziegler - passing.ogg"))
									{
										std::cout << "Failed to load background music.\n";
									}
									else
									{
										mus_background.play();
										mus_background.setLoop(true);
										mus_background.setVolume(25.f);
									}
								}
								rts_poison_hud.setSize(sf::Vector2f(15.f, static_cast<float>(f_poison_count) * 15.f));
								rts_poison_hud.setPosition(sf::Vector2f(1300, 70));
								txt_poison_count.setString(sGetFloatPrecision(f_poison_count, 2));

								crs_poison_dots[i]->setFillColor(sf::Color(189, 46, 230));
								vb_circle_visible[i] = true;
							}
							else
							{
								snd_ingredient_confirm.play();

								crs_poison_dots[i]->setFillColor(sf::Color(207, 244, 255));
								vb_circle_visible[i] = true;
							}
						}
					}
					else
					{
						spr_ingredients[i]->setColor(sf::Color(255, 255, 255, 180));
					}

				}
				for (int i = 0; i != bburger_bread.size(); ++i) bburger_bread[i] = true;
				if (spr_reset.getGlobalBounds().contains(fmouse_position))
				{
					spr_reset.setColor(sf::Color(255, 255, 255, 255));
					if (LEFT_BUTTON_PRESSED)
					{
						vResetGame(f_poison_count, f_ingredient_move, f_feed_count, f_poison_value, f_ingredient_feed_value,
							rts_poison_hud, rts_feed_hud, txt_poison_count, txt_feed_count, spr_burger_bread, 
							i_inside_burger_counter, false);

						vClearContinuousSpr(spr_inside_burger);
					}
				}
				else
				{
					spr_reset.setColor(sf::Color(255, 255, 255, 180));
				}

			}
			else if (b_pause_menu)
			{
				b_main_menu = true;

				for (int i = 0; i != 3; ++i)
				{
					if (hud[i]EQUALS_MOUSE_POSITION)
					{
						hud[i]->setFillColor(sf::Color::White);

						if (LEFT_BUTTON_PRESSED)
						{
							switch (i)
							{
							case 0:	b_pause_menu = false; break;
							case 1: b_play_again_confirm = true; break;
							case 2: 
								b_exit_menu = true; 
								break;
							}
						}
					}
					else
					{
						hud[i]->setFillColor(sf::Color(130, 130, 130));
					}
				}
			}
			if (b_exit_menu)
			{
				b_main_menu = false;

				for (int i = 0; i != 2; ++i)
				{
					if (txr_exit_confirm[i]EQUALS_MOUSE_POSITION)
					{
						txr_exit_confirm[i]->setFillColor(sf::Color::White);

						if (LEFT_BUTTON_PRESSED)
						{
							switch (i)
							{
							case 0: window.close(); break;
							case 1:
								b_pause_menu = true;
								b_exit_menu = false;
								break;
							}
						}
					}
					else
					{
						txr_exit_confirm[i]->setFillColor(sf::Color(130, 130, 130));
					}
				}
			}
			else if (b_play_again_confirm)
			{
				if (
					bHandlePlayAgain(window, txr_play_again_confirm, b_main_menu, b_pause_menu, b_play_again_confirm,
						f_poison_count, f_ingredient_move, f_feed_count, f_poison_value, f_ingredient_feed_value,
						rts_poison_hud, rts_feed_hud, txt_poison_count, txt_feed_count, spr_burger_bread,
						i_inside_burger_counter, true, &i_turns_left, rts_turns_left, txt_turns_left)
				)
				{
					for (int i = 0; i != vb_circle_visible.size(); ++i)
					{
						vb_circle_visible[i] = false;
					}
					vb_is_poisoned = { 0, 0, 0, 0, 0, 0 };
					for (int i = 0; i != 3; ++i)
					{
						int i_poison_temp = rand() % s_ingredient.size();
						if (vb_is_poisoned[i_poison_temp] == true)	--i;
						else vb_is_poisoned[i_poison_temp] = true;
					}

					vClearContinuousSpr(spr_inside_burger);
					b_play_again_confirm = false;
					b_game_finished = false;

					if (!mus_background.openFromFile("music/background - adrian von ziegler - dance with trees.ogg"))
					{
						std::cout << "Failed to load background music.\n";
					}
					else
					{
						mus_background.play();
						mus_background.setLoop(true);
						mus_background.setVolume(35.f);
					}
				}
				else if (b_game_finished && txr_play_again_confirm[1]EQUALS_MOUSE_POSITION && LEFT_BUTTON_PRESSED)
					window.close();
			}
		}

		if (event.type == sf::Event::Closed)
			window.close();

		if (btexture_load && bstart_game)
			window.draw(sprite);

		if (bpause_menu)
			vDrawHud(&window, hud_pause);

		for (int i = 0; i != bburger_bread.size(); ++i) if (bburger_bread[i]) window.draw(*spr_burger_bread[i]);
		for (int i = 0; i != bingredients.size(); ++i)
		{
			if (bingredients[i])
			{
				window.draw(*spr_ingredients[i]);
			}
		}
		if (b_inside_burger)
		{
			for (int i = 0; i != spr_inside_burger.size(); ++i)
			{
				window.draw(*spr_inside_burger[i]);
			}
		}
		if (bburger_bread[1]) window.draw(*spr_burger_bread[1]);

		if(b_turns_left) window.draw(rts_turns_left);
		if (b_txt_turns_left) window.draw(txt_turns_left);
		if (b_poison_meter) { window.draw(txt_poison_count), window.draw(rts_poison_hud); }
		if (b_reset_visible) window.draw(spr_reset);
		vDrawCircle(&window, crs_poison_dots, vb_circle_visible);
		if (b_feed_visible) { window.draw(txt_feed_count), window.draw(rts_feed_hud); }
		if (b_pause_menu)
			window.draw(spr_pause);
		if (b_main_menu)
			vDrawHud(&window, hud);
		if (b_exit_menu)
			vDrawHud(&window, txr_exit_confirm);
		if (b_play_again_confirm)
			vDrawHud(&window, txr_play_again_confirm);
		if (b_game_finished)
			window.draw(txt_game_finished);
			
		window.display();
	}

	vHudDelete(hud);
	vHudDelete(txr_exit_confirm);
	vHudDelete(txr_play_again_confirm);

	vTextureDelete(txr_ingredients), vSpriteDelete(spr_ingredients);
	vTextureDelete(txr_burger_bread), vSpriteDelete(spr_burger_bread);

	vSpriteDelete(spr_inside_burger), vTextureDelete(txr_inside_burger);

	return 0;
}

void vSetSingleTxt(sf::Text &txt_ingredients, sf::Font &font, sf::Color color, sf::Vector2f v2f_position)
{
	txt_ingredients.setFont(font);
	txt_ingredients.setCharacterSize(30);
	txt_ingredients.setFillColor(color);
	txt_ingredients.setPosition(v2f_position);
}
void vInitializeHud(vector<sf::Text*> &hud, sf::Font &font, vector<string> svector, unsigned uelements_amount, unsigned hud_x_space, unsigned hud_y_space,
	unsigned hud_position_x, unsigned hud_position_y)
{
	for (int i = 0; i != uelements_amount; ++i)
	{
		hud.push_back(new sf::Text);
		hud[i]->setFont(font);
		hud[i]->setCharacterSize(75);
		hud[i]->setFillColor(sf::Color(130, 130, 130));
		hud[i]->setPosition(hud_position_x + (i*hud_x_space), hud_position_y + (i*hud_y_space));
		hud[i]->setString(svector[i]);
	}
}
void vInitializeDescisionMenu(vector<sf::Text*> &hud, sf::Font &font, vector<string> s_vector, unsigned u_elements_amount)
{
	for (int i = 0; i != u_elements_amount; ++i)
	{
		hud.push_back(new sf::Text);
		hud[i]->setFont(font);
		hud[i]->setCharacterSize(75);
		hud[i]->setFillColor(sf::Color(130, 130, 130));
		hud[i]->setString(s_vector[i]);
	}

	hud[0]->setPosition(700, 500);
	hud[1]->setPosition(1100, 500);
	hud[2]->setPosition(550, 300);
	hud[2]->setFillColor(sf::Color::White);
}
 
void vSetOneTxt(vector<sf::Text*> &txt, sf::Font &font, string s_ingredient_name, sf::Color color)
{
	txt.push_back(new sf::Text);
	txt[i_inside_burger_counter]->setFont(font);
	txt[i_inside_burger_counter]->setCharacterSize(30);
	txt[i_inside_burger_counter]->setFillColor(color);
	txt[i_inside_burger_counter]->setPosition((i_inside_burger_counter %2 == 0 ? 1600 / 2.5 : 1600 / 2.1), 900 / 1.5f - (25*i_inside_burger_counter));
	txt[i_inside_burger_counter]->setString(s_ingredient_name);
	++i_inside_burger_counter;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void vSetOneSpr(vector <sf::Sprite*> &sprite, sf::Texture &texture, string path, float scale, int which_ingredient)
{
	float f_lettuce_move = 0.f;
	vector<float> f_move{ 30.f, 30.f, 35.f , 30.f , 30.f , 25.f };
	vector<vector<float>> f_ingredient_positions{ {2.8f, 2.3f}, { 2.6f, 1.9f }, { 2.7f, 2.1f },{ 2.7f, 2.0f },{ 2.6f, 2.1f },{ 2.6f, 2.0f } };
	//beef, cucmber, ketchup, lettuce, onion, tomato
	f_ingredient_move += f_move[which_ingredient];

	if (which_ingredient == 3)
		f_lettuce_move = 20.f;
	else
		f_lettuce_move == 0.f;

	sprite.push_back(new sf::Sprite);
	sprite[i_inside_burger_counter]->setScale(sf::Vector2f(scale, scale));
	sprite[i_inside_burger_counter]->setPosition((i_inside_burger_counter % 2 == 0 ? 1600 / f_ingredient_positions[which_ingredient][0] 
		: 1600 / f_ingredient_positions[which_ingredient][1]), 900 / 1.85f - f_ingredient_move - f_lettuce_move);
	sprite[i_inside_burger_counter]->setTexture(texture);
	sprite[i_inside_burger_counter]->setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
	++i_inside_burger_counter;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void vSetBool(vector<bool> &bingredients, int inumber_of_ingredients)
{
	for (int i = 0; i != inumber_of_ingredients; ++i)
	{
		bingredients.push_back(false);
	}
}
void vSetSprite(sf::Texture &texture, sf::Sprite &sprite, int x_start, int y_start, float scale)
{
	sf::Vector2u texture_size(texture.getSize());
	sf::Vector2f vf_sprite_position(x_start, y_start);
	sf::Vector2f vf_sprite_scale(scale, scale);

	texture.setSmooth(true);
	texture.setRepeated(true);

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, texture_size.x, texture_size.y));
	sprite.setPosition(vf_sprite_position);
	sprite.setScale(vf_sprite_scale);
}
void vSetTotalSprite(string path, sf::Texture &texture, sf::Sprite &sprite, int x_start, int y_start, float scale)
{
	if (!texture.loadFromFile(path))
	{
		std::cout << "texture: " << path << " failed to load.\n";
	}
	else
	{
		vSetSprite(texture, sprite, x_start, y_start, scale);
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

bool bHandlePlayAgain(sf::RenderWindow &window, vector<sf::Text*> txr, bool &b_main_menu, bool &b_pause_menu, bool &b_play_again, 
	float &f_poison_count, float &f_ingredient_move, float &f_feed_count, vector<float> &f_poison_value, vector<float> &f_ingredient_feed_value,
	sf::RectangleShape &rts_poison_hud, sf::RectangleShape &rts_feed_hud,
	sf::Text &txt_poison_count, sf::Text &txt_feed_count,
	vector<sf::Sprite*> &spr_burger_bread,
	int &i_inside_burger_counter, bool b_totally_new_game,
	int *i_turns_left, sf::RectangleShape &rts_turns_left, sf::Text &txt_turns_left)
{
	sf::Vector2f fmouse_position;

	b_main_menu = false;
	fmouse_position.x = static_cast<float>(sf::Mouse::getPosition(window).x), fmouse_position.y = static_cast<float>(sf::Mouse::getPosition(window).y);

	for (int i = 0; i != 2; ++i)
	{
		if (txr[i]EQUALS_MOUSE_POSITION)
		{
			txr[i]->setFillColor(sf::Color::White);

			if (LEFT_BUTTON_PRESSED)
			{
				switch (i)
				{
				case 0: 
					vResetGame(f_poison_count, f_ingredient_move, f_feed_count, f_poison_value, f_ingredient_feed_value,
						rts_poison_hud, rts_feed_hud, txt_poison_count, txt_feed_count, spr_burger_bread,
						i_inside_burger_counter, true, i_turns_left, rts_turns_left, txt_turns_left);

					b_pause_menu = false;
					return true;
				case 1:
					b_pause_menu = true;
					b_play_again = false;
					break;
				}
			}
		}
		else
		{
			txr[i]->setFillColor(sf::Color(130, 130, 130));
		}
	}

	return false;
}

void vDrawCircle(sf::RenderWindow *window, std::vector<sf::CircleShape*> circle, vector<bool> vb_bool)
{
	for (int i = 0; i != circle.size(); ++i)
	{
		if(vb_bool[i])
			window->draw(*circle[i]);
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
void vClearContinuousSpr(vector <sf::Sprite*> &sprite)
{
	for (int i = sprite.size() - 1; i != -1; --i)
	{
		delete sprite.back();
		sprite.pop_back();
	}
}

string sGetFloatPrecision(float value, int precision)
{
	std::ostringstream ss;
	ss << value;
	string s_two_decimals(ss.str());

	for (int i = 0; i != s_two_decimals.length(); ++i)
	{
		if (s_two_decimals[i] == '.')
		{
			s_two_decimals.erase(s_two_decimals.begin()+i+precision, s_two_decimals.end());
			break;
		}
	}

	return s_two_decimals;
}


void vResetGame(float &f_poison_count, float &f_ingredient_move, float &f_feed_count, vector<float> &f_poison_value, vector<float> &f_ingredient_feed_value, 
	sf::RectangleShape &rts_poison_hud, sf::RectangleShape &rts_feed_hud,
	sf::Text &txt_poison_count, sf::Text &txt_feed_count, 
	vector<sf::Sprite*> &spr_burger_bread,
	int &i_inside_burger_counter, bool b_totally_new_game, 
	int *i_turns_left, sf::RectangleShape &rts_turns_left, sf::Text &txt_turns_left)
{
	f_poison_count = 2.f;
	f_ingredient_move = 0.f;
	rts_poison_hud.setSize(sf::Vector2f(15.f, static_cast<float>(f_poison_count) * 15.f));
	rts_poison_hud.setPosition(sf::Vector2f(1300, 70 + ((2 - f_poison_count)*15.f)));
	txt_poison_count.setString(sGetFloatPrecision(f_poison_count, 2));
	spr_burger_bread[1]->setPosition(1600.f / 3.f, 900.f / 3.f);
	f_feed_count = 7.5f;
	rts_feed_hud.setSize(sf::Vector2f(15, f_feed_count * 15));
	rts_feed_hud.setPosition(sf::Vector2f(1500, 70));
	txt_feed_count.setString(sGetFloatPrecision(f_feed_count, 2));
	f_ingredient_feed_value = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
	f_poison_value = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
	
	i_inside_burger_counter = 0;
	
	if (b_totally_new_game)
	{
		*i_turns_left = 20;

		rts_turns_left.setSize(sf::Vector2f(15, *i_turns_left * 15));
		rts_turns_left.setPosition(sf::Vector2f(1400, 70));
		rts_turns_left.setFillColor(sf::Color(207, 244, 255));
	
		txt_turns_left.setString(std::to_string(*i_turns_left));
	}
}

/*
void vSetLightenFont(std::vector<sf::Text*> text, int i_text_to_lighten, int i_text_elements = 3)
{
	for (int i = 0; i != i_text_elements; ++i)
	{
		text[i]->setFillColor(sf::Color(130, 130, 130));
	}
	text[i_text_to_lighten]->setFillColor(sf::Color::White);
}
*/
