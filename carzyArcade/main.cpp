#include<SFML/Graphics.hpp>
#include<vector>
#include<chrono>
#include "unit.h"
#include<iostream>

namespace ch = std::chrono;

void CharacterMove(sf::Event& e, User& player,int game_map[][15]);
void BombExplosion(int game_map[][15], const int kMapSize[], sf::RenderWindow& window, sf::Sprite& s_wave, Bomb& bomb, std::vector<Bomb>& bomb_array);
/* TODO LIST:
*   ������ �߰�
*	���� ���� ǥ��(���� ��) - 0.5�ʸ� �����ұ�?
*	2p ���� ���� ����� �Ǳ� ��
*	�׷��� �ٽ� �׸��� ( �ļ���)
*	�ڽ� ���� �̵� �Ұ�
*	��ǳ�� ���� �� Ÿ��..
*/
// �������� �� ���� �ͳ� ������


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
	t_bomb.loadFromFile("./images/waterbomb.png");
	t_character.loadFromFile("./images/player.png");
	t_box.loadFromFile("./images/box.png");
	t_wave.loadFromFile("./images/wave.png");
	sf::Sprite s_bomb(t_bomb);
	sf::Sprite s_character(t_character);
	// png ũ�� ���� �������� �ؾߵ� �ϰ��� ����
	s_bomb.setScale(1.25f, 1.25f);
	s_character.setScale(1.25f, 1.25f);

	sf::Sprite s_box(t_box);
	sf::Sprite s_wave(t_wave);

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
		// �̺�Ʈ ó��
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
					//TODO : gameMap 0�� ���� ã�Ƽ� ��ǳ�� ��ġ
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
			// ���� ���� �Ǿ��ٸ�... ��Ʈ����
			if (bomb_array[i].GetPassedTime(end) > 3000)
			{

				Point p = bomb_array[i].GetPosition();
				if (game_map[p.y][p.x] != 0)
				{
					BombExplosion(game_map, kMapSize, window, s_wave, bomb_array[i], bomb_array);
				}
				bomb_array.erase(bomb_array.begin() + i);
				i--;				// �����ָ� vector ���� ���.
			}
		}
		// �ڽ� �׸��� && ��ź �׸���
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
			}
		}


		// �÷��̾� �׸���
		Point player_position = player1.GetPosition();
		s_character.setPosition(player_position.x, player_position.y);
		window.draw(s_character);
		window.display();
	}

}
void CharacterMove(sf::Event& e, User& player,int game_map[][15])
{
	Point p = player.GetPosition();
	int map_x = p.x / 80;
	int map_y = p.y / 80;
	if (e.key.code == sf::Keyboard::Up && game_map[map_y-1][map_x] == 0)
		player.UserMoveY(false);
	if (e.key.code == sf::Keyboard::Down && game_map[map_y + 1][map_x] == 0)
		player.UserMoveY(true);
	if (e.key.code == sf::Keyboard::Left && game_map[map_y][map_x-1] == 0)
		player.UserMoveX(false);
	if (e.key.code == sf::Keyboard::Right && game_map[map_y][map_x+1] == 0)
		player.UserMoveX(true);
}
// ��� ���߿� ����� ����Ĩ�ô�. �迭�̶� 0 bomb�� -1��... 15��° �迭�� bomb�ȴ�...
void BombExplosion(int game_map[][15], const int kMapSize[], sf::RenderWindow& window, sf::Sprite& s_wave, Bomb& bomb, std::vector<Bomb>& bomb_array)
{
	// ���� ����Ʈ ���� ��?�� �ϰڴ�..
	Point p = bomb.GetPosition();
	game_map[p.y][p.x] = 0;

	int explosion_range = bomb.GetExplosionRange();
	for (int i = 1; i <= explosion_range; i++)
	{
		if (game_map[p.y + i][p.x] == 0)
			continue;
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
			break;
		}
		else
			game_map[p.y + i][p.x] = 0;
		break;
	}
	
	for (int i = 1; i <= explosion_range; i++)
	{
		if (game_map[p.y - i][p.x] == 0)
			continue;
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
			break;
		}
		else
			game_map[p.y - i][p.x] = 0;
		break;
	}
	for (int i = 1; i <= explosion_range; i++)
	{
		if (game_map[p.y][p.x + i] == 0)
			continue;
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
			break;
		}
		else
			game_map[p.y][p.x+i] = 0;
		break;
	}
	for (int i = 1; i <= explosion_range; i++)
	{
		if (game_map[p.y][p.x-i] == 0)
			continue;
		if (game_map[p.y][p.x-i] == 2)
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
			break;
		}
		else
			game_map[p.y][p.x-i] = 0;
		break;
	}
}