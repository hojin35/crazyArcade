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
	int gameMap[15][13];
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 13; j++)
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
					start = ch::high_resolution_clock::now();
					Bomb bomb1(player1.GetPosition(),start);
					bomb_array.push_back(bomb1);
				}
				CharacterMove(e, player1);
			}
		}

		window.clear(sf::Color::White);
		// 박스 그리기
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				if (gameMap[i][j] == 1)
				{
					s_box.setPosition(i * 80, j * 80);
					window.draw(s_box);
				}
			}
		}

		ch::time_point<ch::high_resolution_clock> end = ch::high_resolution_clock::now();
		// 폭탄 그리기
		for (int i=0;i<bomb_array.size();i++)
		{
			// 터질 떄가 되었다면... 터트리기
			if (bomb_array[i].GetPassedTime(end) > 3000)
			{
				// TODO : 연쇄 폭팔 && 박스 터트리기
				bomb_array.erase(bomb_array.begin() + i);
				i--;
				continue;
			}

			Point bomb_rocation = bomb_array[i].GetPosition();
			s_bomb.setPosition(bomb_rocation.x, bomb_rocation.y);
			window.draw(s_bomb);
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
void TimerInit()
{
	
}