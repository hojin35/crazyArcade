#include<SFML/Graphics.hpp>
#include<vector>
#include<chrono>
#include "unit.h"
#include<iostream>
namespace ch = std::chrono;

void CharacterMove(sf::Event& e, User& player);
int main()
{
	ch::time_point<ch::high_resolution_clock> start;

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "crazyArcade");
	User player1;
	sf::Texture t_bomb;
	sf::Texture t_character;
	sf::Texture t_box;
	t_bomb.loadFromFile("./images/waterbomb.png");
	t_character.loadFromFile("./images/player.png");
	t_box.loadFromFile("./images/box.png");
	
	sf::Sprite s_bomb(t_bomb);
	sf::Sprite s_character(t_character);
	sf::Sprite s_box(t_box);
	int gameMap[13][15];
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (rand() % 5 == 0)
				gameMap[i][j] = 0;
			else
				gameMap[i][j] = 1;
		}
	}

	std::vector<Bomb> bomb_array;
	while (window.isOpen())
	{
		// 이벤트 처리
		sf::Event e;
		while (window.pollEvent(e))
		{

			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Space)
				{
					//TODO : gameMap 0인 곳을 찾아서 물풍선 설치
					Point character_position = player1.GetPosition();
					int map_x = character_position.x / 80;
					int map_y = character_position.y / 80;
					if (gameMap[map_y][map_x] == 1)
					{
						continue;
					}
					gameMap[map_y][map_x] = 2;
					start = ch::high_resolution_clock::now();
					Bomb bomb1(map_x, map_y,start);
					bomb_array.push_back(bomb1);
				}
				CharacterMove(e, player1);
			}
		}

		window.clear(sf::Color::White);

		ch::time_point<ch::high_resolution_clock> end = ch::high_resolution_clock::now();
		for (int i = 0; i < bomb_array.size(); i++)
		{
			// 터질 떄가 되었다면... 터트리기
			if (bomb_array[i].GetPassedTime(end) > 3000)
			{
				// TODO : 연쇄 폭팔 && 박스 터트리기
				Point p = bomb_array[i].GetPosition();
				gameMap[p.y][p.x] = 0;
				bomb_array.erase(bomb_array.begin() + i);
				i--;				// 안해주면 vector 범위 벗어남.
			}
		}
		// 박스 그리기 && 폭탄 그리기
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (gameMap[i][j] == 1)
				{
					s_box.setPosition(j * 80, i * 80);
					window.draw(s_box);
				}
				if (gameMap[i][j] == 2)
				{
					s_bomb.setPosition(j * 80, i * 80);
					window.draw(s_bomb);
				}
			}
		}

		
		// 플레이어 그리기
		Point player_position = player1.GetPosition();
		s_character.setPosition(player_position.x, player_position.y);
		window.draw(s_character);
		window.display();
	}

}

void CharacterMove(sf::Event& e, User& player)
{
	if (e.key.code == sf::Keyboard::Up)
		player.UserMoveY(false);
	if (e.key.code == sf::Keyboard::Down)
		player.UserMoveY(true);
	if (e.key.code == sf::Keyboard::Left)
		player.UserMoveX(false);
	if (e.key.code == sf::Keyboard::Right)
		player.UserMoveX(true);
}