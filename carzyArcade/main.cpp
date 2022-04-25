#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<chrono>
#include "unit.h"
#include "effect.h"
namespace ch = std::chrono;
class GameInfo
{
public:
	std::vector<Bomb> bomb_array_;
	std::vector<Wave> wave_array_;
	std::vector<Boom> boom_array_;
private:

};
void CharacterMove(sf::Event& e, User& player, int game_map[][15]);
void BombExplosion(int game_map[][15], const int kMapSize[],
	sf::RenderWindow& window, GameInfo &game_info,Bomb& bomb);
int WhatExploded(int game_map[][15], const int kMapSize[],
	sf::RenderWindow& window,GameInfo &game_info, Bomb& bomb, Point p);
/* TODO LIST:
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
	// Texture
	sf::Texture t_bomb;
	sf::Texture t_character;
	sf::Texture t_box;
	sf::Texture t_wave;
	sf::Texture t_portion;
	sf::Texture t_boom;
	t_bomb.loadFromFile("./images/waterbomb.png");
	t_character.loadFromFile("./images/player.png");
	t_box.loadFromFile("./images/box.png");
	t_wave.loadFromFile("./images/wave.png");
	t_portion.loadFromFile("./images/portion.png");
	t_boom.loadFromFile("./images/boom.png");

	sf::Sprite s_bomb(t_bomb);
	sf::Sprite s_character(t_character);
	sf::Sprite s_box(t_box);
	sf::Sprite s_wave(t_wave);
	sf::Sprite s_portion(t_portion);
	sf::Sprite s_boom(t_boom);
	// png 크기 조절 언젠가는 해야됨 하고나서 삭제
	s_bomb.setScale(1.25f, 1.25f);
	s_character.setScale(1.25f, 1.25f);
	s_character.setOrigin(40, 40);
	s_boom.setScale(0.3125f, 0.3125f);

	GameInfo game_info;
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
					game_info.bomb_array_.push_back(bomb1);
				}
				CharacterMove(e, player1, game_map);
			}
		}

		window.clear(sf::Color(129, 193, 71));
		ch::time_point<ch::high_resolution_clock> end = ch::high_resolution_clock::now();
		for (int i = 0; i < (int)game_info.bomb_array_.size(); i++)
		{
			// 터질 떄가 되었다면... 터트리기
			if (game_info.bomb_array_[i].GetPassedTime(end) > 3000)
			{

				Point p = game_info.bomb_array_[i].GetPosition();
				if (game_map[p.y][p.x] != 0)
				{
					BombExplosion(game_map, kMapSize, window, game_info,game_info.bomb_array_[i]);
				}
				game_info.bomb_array_.erase(game_info.bomb_array_.begin() + i);
				i--;				// 안해주면 vector 범위 벗어남.
			}
		}
		// 폭팔 범위 그리기
		for (int i=0; i<(int)game_info.wave_array_.size();i++)
		{
			if (game_info.wave_array_[i].GetPassedTime(end) < 500)
			{
				
				Point p = game_info.wave_array_[i].GetPosition();
				s_wave.setPosition(p.x * 80, p.y * 80);
				window.draw(s_wave);
			}
			else
			{
				game_info.wave_array_.erase(game_info.wave_array_.begin() + i);
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

		// 상자 터지는 이펙트
		for (int i = 0; i < (int)game_info.boom_array_.size(); i++)
		{
			if (game_info.boom_array_[i].GetPassedTime(end) < 500)
			{

				Point p = game_info.boom_array_[i].GetPosition();
				s_boom.setPosition(p.x * 80, p.y * 80);
				window.draw(s_boom);
			}
			else
			{
				game_info.boom_array_.erase(game_info.boom_array_.begin() + i);
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
	if (game_map[map_y][map_x] > 2)
	{
		player.GainItems(game_map[map_y][map_x]);
		game_map[map_y][map_x] = 0;
	}
}
void BombExplosion(int game_map[][15], const int kMapSize[],
	sf::RenderWindow& window,GameInfo &game_info, Bomb& bomb)
{
	//std::vector<wave> wave_array; // wave class 만들어서 문양 넣기
	// 폭팔 이펙트 구현 몰?루 하겠다..

	Point p = bomb.GetPosition();
	game_map[p.y][p.x] = 0;
	Wave wave(p, ch::high_resolution_clock::now());
	game_info.wave_array_.push_back(wave);

	bool is_boom[4] = { false, };
	int explosion_range = bomb.GetExplosionRange();
	for (int i = 1; i <= explosion_range; i++)
	{
		if (is_boom[0] == false)
		{
			Point copy_p = p;
			copy_p.y += i;
			if (WhatExploded(game_map, kMapSize, window, game_info, bomb, copy_p) != 0)
				is_boom[0] = true;
		}
		if (is_boom[1] == false)
		{
			Point copy_p = p;
			copy_p.y -= i;
			if (WhatExploded(game_map, kMapSize, window, game_info, bomb, copy_p) != 0)
				is_boom[1] = true;
		}
		if (is_boom[2] == false)
		{
			Point copy_p = p;
			copy_p.x += i;
			if (WhatExploded(game_map, kMapSize, window, game_info, bomb, copy_p) != 0)
				is_boom[2] = true;
		}
		if (is_boom[3] == false)
		{
			Point copy_p = p;
			copy_p.x -= i;
			if (WhatExploded(game_map, kMapSize, window, game_info, bomb, copy_p) != 0)
				is_boom[3] = true;
		}
	}
}

int WhatExploded(int game_map[][15], const int kMapSize[],
	sf::RenderWindow& window,GameInfo &game_info, Bomb& bomb, Point p)
{
	if (kMapSize[0] <= p.x || 0 > p.x || kMapSize[1] <= p.y || 0 > p.y)
		return -1;

	if (game_map[p.y][p.x] == 0)
	{
		Wave wave(p,ch::high_resolution_clock::now());
		game_info.wave_array_.push_back(wave);
		return 0;
	}
	else if (game_map[p.y][p.x] == 1)
	{
		Boom boom(p, ch::high_resolution_clock::now());
		game_info.boom_array_.push_back(boom);

		if (rand() % 2)
			game_map[p.y][p.x] = 3;
		else
			game_map[p.y][p.x] = 0;
		return 1;
	}
	else if (game_map[p.y][p.x] == 2)
	{
		Wave wave(p, ch::high_resolution_clock::now());
		game_info.wave_array_.push_back(wave);
		for (Bomb& b : game_info.bomb_array_)
		{
			if (p.operator==(b.GetPosition()))
			{
				BombExplosion(game_map, kMapSize, window,game_info, b);
				break;
			}
		}
		return 2;
	}
	else
	{
		Wave wave(p, ch::high_resolution_clock::now());
		game_info.wave_array_.push_back(wave);
		game_map[p.y][p.x] = 0;
		return 0;
	}
}