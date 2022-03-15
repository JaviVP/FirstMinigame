#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include "SDL_Mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_Mixer/libx86/SDL2_mixer.lib" )

#include "Entity.h"

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_SHOTS		128
#define MAX_ENEMIES		257 //This number must be one higher of the desired enemies
#define BULLET_DELAY	10
#define ENEMY_DELAY 	120
#define MAX_TRACKS		10
#define MAX_HEARTS		3


class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool LoadAudios();
	bool LoadImages();
	void Release();
	bool Dead(int _num);
	bool Input();
	bool Update();
	void Draw();

private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Texture* img_background, * img_player_N, * img_player_NE, * img_player_NW, * img_player_W, * img_player_SW, * img_player_S, * img_player_SE, * img_player_E, * img_shot, * img_Heart, * img_EmptyHeart, * img_WhiteHeart, * img_GreyHeart,
		* img_GreenEnemy_N, * img_GreenEnemy_S, * img_GreenEnemy_E, * img_GreenEnemy_W, * img_GreenEnemy_NW, * img_GreenEnemy_NE, * img_GreenEnemy_SW, * img_GreenEnemy_SE,
		* img_GreenEnemyGroup_N, * img_GreenEnemyGroup_S, * img_GreenEnemyGroup_E, * img_GreenEnemyGroup_W, * img_GreenEnemyGroup_NW, * img_GreenEnemyGroup_NE, * img_GreenEnemyGroup_SW, * img_GreenEnemyGroup_SE,
		* img_BlueEnemy_N, * img_BlueEnemy_S, * img_BlueEnemy_E, * img_BlueEnemy_W, * img_BlueEnemy_NW, * img_BlueEnemy_NE, * img_BlueEnemy_SW, * img_BlueEnemy_SE,
		* img_GameOver, * img_One, * img_Zero, * img_Two, * img_Three, * img_Five, * img_Four, * img_Six, * img_Seven, * img_Eight, * img_Nine;

	Uint32 buttons; 

	Entity Player, Shots[MAX_SHOTS], Scene, Enemy[MAX_ENEMIES], Heart[MAX_HEARTS],Score1,Score2;
	int idx_shot;
	int idx_Enemy;
	int idx_Heart;

	int mouseX, mouseY;

	int enemyDead[31]{ 31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31 };
	int scoreDead = 0;
	int score = 0;

	bool god_mode;

	bool toggle_enemies = true;

	int bullet_delay_c = 0;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 

	Mix_Music* tracks[MAX_TRACKS];
	int num_tracks;

	Mix_Chunk* sfxs[MAX_SHOTS];
	int num_sfxs;
};
