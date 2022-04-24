#include<SFML/Graphics.hpp>
#include<vector>
#include<chrono>
#include "unit.h"
#include<iostream>

namespace ch = std::chrono;

void CharacterMove(sf::Event& e, User& player, int game_map[][15]);
void BombExplosion(int game_map[][15], const int kMapSize[], sf::RenderWindow& window, sf::Sprite& s_wave, Bomb& bomb, std::vector<Bomb>& bomb_array);
/* TODO LIST:
*   아이템 추가
*	폭팔 범위 표시(터질 떄) - 0.5초면 적당할까?
*	2p 모드로 갈아 엎어야 되긴 함
*	그래픽 다시 그리기 ( 후순위)
*	박스 위로 이동 불가
*	물풍선 맞을 시 타격..
*/
// 어지럽다 다 엎고 싶네 ㅋㅋㅋ


int main()
{
	const int kMapSize[2] = { 15,13 };

	ch::time_point<ch::high_resolution_clock> start;

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "crazyArcade");
	User player1;
	sf::Texture t_bomb;
	sf::Texture t_character;
	sf::Texture t_box;
	sf::Texture t_wave;
	sf::Texture t_portion;
	t_bomb.loadFromFile("./images/waterbomb.png");
	t_character.loadFromFile("./images/player.png");
	t_box.loadFromFile("./images/box.png");
	t_wave.loadFromFile("./images/wave.png");
	t_portion.loadFromFile("./images/portion.png");
	sf::Sprite s_bomb(t_bomb);
	sf::Sprite s_character(t_character);
	// png 크기 조절 언젠가는 해야됨 하고나서 삭제
	s_bomb.setScale(1.25f, 1.25f);
	s_character.setScale(1.25f, 1.25f);
	s_character.setOrigin(40, 40);
	sf::Sprite s_box(t_box);
	sf::Sprite s_wave(t_wave);
	sf::Sprite s_portion(t_portion);
	int game_map[13][15] =
	{
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1}
	};

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
					if (game_map[map_y][map_x] == 1)
					{
						continue;
					}
					game_map[map_y][map_x] = 2;
					start = ch::high_resolution_clock::now();
					Bomb bomb1(map_x, map_y, start, player1.GetExplosionRange());
					bomb_array.push_back(bomb1);
				}
				CharacterMove(e, player1, game_map);
			}
		}

		window.clear(sf::Color(129, 193, 71));
		ch::time_point<ch::high_resolution_clock> end = ch::high_resolution_clock::now();
		for (int i = 0; i < (int)bomb_array.size(); i++)
		{
			// 터질 떄가 되었다면... 터트리기
			if (bomb_array[i].GetPassedTime(end) > 3000)
			{

				Point p = bomb_array[i].GetPosition();
				if (game_map[p.y][p.x] != 0)
				{
					BombExplosion(game_map, kMapSize, window, s_wave, bomb_array[i], bomb_array);
				}
				bomb_array.erase(bomb_array.begin() + i);
				i--;				// 안해주면 vector 범위 벗어남.
			}
		}
		// 박스 그리기 && 폭탄 그리기
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (game_map[i][j] == 1)
				{
					s_box.setPosition(j * 80, i * 80);
					window.draw(s_box);
				}
				if (game_map[i][j] == 2)
				{
					s_bomb.setPosition(j * 80, i * 80);
					window.draw(s_bomb);
				}
				if (game_map[i][j] == 3)
				{
					s_portion.setPosition(j * 80, i * 80);
					window.draw(s_portion);
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
void CharacterMove(sf::Event& e, User& player, int game_map[][15])
{
	Point temp_p = player.GetPosition();
	if (e.key.code == sf::Keyboard::Up)
		player.UserMoveY(false);
	if (e.key.code == sf::Keyboard::Down)
		player.UserMoveY(true);
	if (e.key.code == sf::Keyboard::Left)
		player.UserMoveX(false);
	if (e.key.code == sf::Keyboard::Right)
		player.UserMoveX(true);

	Point p = player.GetPosition();
	int map_x = p.x / 80;
	int map_y = p.y / 80;
	if (game_map[map_y][map_x] == 1 || game_map[map_y][map_x] == 2)
	{
		player.SetPosition(temp_p);
	}
}
// 얘는 나중엔 절대로 뜯어고칩시다. 배열이라 0 bomb면 -1인... 15번째 배열이 bomb된다...
void BombExplosion(int game_map[][15], const int kMapSize[], sf::RenderWindow& window, sf::Sprite& s_wave, Bomb& bomb, std::vector<Bomb>& bomb_array)
{
	//std::vector<wave> wave_array; // wave class 만들어서 문양 넣기
	// 폭팔 이펙트 구현 몰?루 하겠다..

	Point p = bomb.GetPosition();
	game_map[p.y][p.x] = 0;
	bool is_boom[4] = { false, };
	int explosion_range = bomb.GetExplosionRange();
	for (int i = 1; i <= explosion_range; i++)
	{
		if (is_boom[0] == false)
		{
			if (game_map[p.y + i][p.x] == 0)
			{
				//new wave(x,y,time);
				// wave_array.push_back(wave);
			}
			if (game_map[p.y + i][p.x] == 1)
			{
				if (rand() % 2)
					game_map[p.y + i][p.x] = 3;
				else
					game_map[p.y + i][p.x] = 0;
			}
			if (game_map[p.y + i][p.x] == 2)
			{
				Point copy_p = p;
				copy_p.y += i;
				for (Bomb& b : bomb_array)
				{
					if (copy_p.operator==(b.GetPosition()))
					{
						BombExplosion(game_map, kMapSize, window, s_wave, b, bomb_array);
						break;
					}
				}
			}
			else
			{
				game_map[p.y + i][p.x] = 0;
			}
			is_boom[0] = true;
		}
		if (is_boom[1] == false)
		{
			if (game_map[p.y - i][p.x] == 0)
			{
				//new wave(x,y,time);
				// wave_array.push_back(wave);
			}
			if (game_map[p.y - i][p.x] == 1)
			{
				if (rand() % 2)
					game_map[p.y - i][p.x] = 3;
				else
					game_map[p.y - i][p.x] = 0;
			}
			if (game_map[p.y - i][p.x] == 2)
			{
				Point copy_p = p;
				copy_p.y -= i;
				for (Bomb& b : bomb_array)
				{
					if (copy_p.operator==(b.GetPosition()))
					{
						BombExplosion(game_map, kMapSize, window, s_wave, b, bomb_array);
						break;
					}
				}
			}
			else
			{
				game_map[p.y + i][p.x] = 0;
			}
			is_boom[1] = true;
		}
		if (is_boom[2] == false)
		{
			if (game_map[p.y][p.x + i] == 0)
			{
				//new wave(x,y,time);
				// wave_array.push_back(wave);
			}
			if (game_map[p.y][p.x + i] == 1)
			{
				if (rand() % 2)
					game_map[p.y][p.x + i] = 3;
				else
					game_map[p.y][p.x + i] = 0;
			}
			if (game_map[p.y][p.x + i] == 2)
			{
				Point copy_p = p;
				copy_p.x += i;
				for (Bomb& b : bomb_array)
				{
					if (copy_p.operator==(b.GetPosition()))
					{
						BombExplosion(game_map, kMapSize, window, s_wave, b, bomb_array);
						break;
					}
				}
			}
			else
			{
				game_map[p.y][p.x + i] = 0;
			}
			is_boom[2] = true;
		}
		if (is_boom[3] == false)
		{
			if (game_map[p.y][p.x - i] == 0)
			{
				//new wave(x,y,time);
				// wave_array.push_back(wave);
			}
			if (game_map[p.y][p.x - i] == 1)
			{
				if (rand() % 2)
					game_map[p.y][p.x - i] = 3;
				else
					game_map[p.y][p.x - i] = 0;
			}
			if (game_map[p.y][p.x - i] == 2)
			{
				Point copy_p = p;
				copy_p.x -= i;
				for (Bomb& b : bomb_array)
				{
					if (copy_p.operator==(b.GetPosition()))
					{
						BombExplosion(game_map, kMapSize, window, s_wave, b, bomb_array);
						break;
					}
				}
			}
			else
			{
				game_map[p.y][p.x + i] = 0;
			}
			is_boom[3] = true;
		}
	}
}

void WhatExploded(int game_map[][15], const int kMapSize[], sf::RenderWindow& window, sf::Sprite& s_wave, Bomb& bomb, std::vector<Bomb>& bomb_array,Point p)
{
	if (game_map[p.y][p.x] == 0)
	{
		//new wave(x,y,time);
		// wave_array.push_back(wave);
	}
	else if (game_map[p.y][p.x] == 1)
	{
		if (rand() % 2)
			game_map[p.y][p.x] = 3;
		else
			game_map[p.y][p.x] = 0;
	}
	else if (game_map[p.y][p.x] == 2)
	{
		for (Bomb& b : bomb_array)
		{
			if (p.operator==(b.GetPosition()))
			{
				BombExplosion(game_map, kMapSize, window, s_wave, b, bomb_array);
				break;
			}
		}
	}
	else
	{
		game_map[p.y][p.x] = 0;
	}
}