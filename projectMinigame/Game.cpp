#include "Game.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

Game::Game() {}
Game::~Game(){}
int Enemy_delay = 0, counter = 0, HPcounter = 3, auxC = 0, counter1 = 0, MusicC = 0, SCORE = 0, STAGE = 0, StageC = 0, ENEMY_DELAY = 400, EnemyCounter = 0, DMG = 12;
int ScoreAux[200];
bool aux = true, Menu = false, X = false;

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("2D Shotter: wasd + left_click, F1: God Mode + Hitboxes, F2: Toggle enemies, F3: Toggle Oneshot, F4: Disabble Oneshot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	// Init SDL_Mixer
	int flags = MIX_INIT_OGG;
	if (Mix_Init(flags) != flags) {
		SDL_Log("Failed to init OGG module for SDL_Mixer!\n");
		SDL_Log("Mix_Init: %s\n", Mix_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		SDL_Log("Failed to init SDL_Mixer!\n");
		SDL_Log("Mix_OpenAudio: %s\n", Mix_GetError());
		return false;
	}
	if (!LoadAudios()) return false;
	//Init variables
	//size: 104x82
	Player.Init((WINDOW_WIDTH >> 1) - 16, (WINDOW_HEIGHT >> 1) -16, 64, 64, 3,NULL, NULL, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		Heart[i].Init(2 + counter, 700, 64, 64, 3, NULL, NULL, NULL, NULL);
		counter += 68;
	}
	idx_shot = 0;
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Score1.Init(WINDOW_WIDTH-80, WINDOW_HEIGHT-80, 64, 64, 0,NULL, NULL,NULL, NULL);
	Score2.Init(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 80, 64, 64, 0, NULL, NULL, NULL, NULL);
	Scene.Init(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,NULL, NULL, NULL, NULL);
	StartBack.Init(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, NULL, NULL, NULL, NULL);
	Text.Init((WINDOW_WIDTH / 2)-100, (WINDOW_HEIGHT / 2), 200, 64, 0, NULL, NULL, NULL, NULL);
	Stage.Init((WINDOW_WIDTH / 2) - 100, WINDOW_HEIGHT - 60, 200, 64, 0, NULL, NULL, NULL, NULL);
	god_mode = false;
	for (int i = 0; i < 200; i++) {
		ScoreAux[i] = -1;
	}
	return true;
}

bool Game::LoadAudios() {
	num_tracks = 0;
	tracks[num_tracks++] = Mix_LoadMUS("sample_ogg.ogg");

	Mix_PlayMusic(tracks[0], -1);

	// set channel 2 to half volume
	Mix_VolumeMusic(2);

	sfxs[num_sfxs++] = Mix_LoadWAV("sample_wav.wav");

	return true;
}

bool Game::LoadImages()
{
	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("background.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_S = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(South).png"));
	if (img_player_S == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_SE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(South-east).png"));
	if (img_player_SE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_SW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(South-west).png"));
	if (img_player_SW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_NW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(North-west).png"));
	if (img_player_NW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_NE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(North-east).png"));
	if (img_player_NE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_N = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(North).png"));
	if (img_player_N == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_E = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(East).png"));
	if (img_player_E == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player_W = SDL_CreateTextureFromSurface(Renderer, IMG_Load("lil_guy(West).png"));
	if (img_player_W == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("shot.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Load
	img_Black = SDL_CreateTextureFromSurface(Renderer, IMG_Load("PantallaInicio.png"));
	if (img_Black == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Press = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Start.png"));
	if (img_Press == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Hearts texture
	img_Heart = SDL_CreateTextureFromSurface(Renderer, IMG_Load("SpriteHeart.png"));
	if (img_Heart == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_EmptyHeart = SDL_CreateTextureFromSurface(Renderer, IMG_Load("SpriteHeartVacio.png"));
	if (img_EmptyHeart == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_WhiteHeart = SDL_CreateTextureFromSurface(Renderer, IMG_Load("WhiteHeart.png"));
	if (img_WhiteHeart == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreyHeart = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreyHeart.png"));
	if (img_GreyHeart == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Enemy texture
	img_GreenEnemy_S = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(south).png"));
	if (img_GreenEnemy_S == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_N = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(north).png"));
	if (img_GreenEnemy_N == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_SE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(south-east).png"));
	if (img_GreenEnemy_SE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_SW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(south-west).png"));
	if (img_GreenEnemy_SW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_NE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(north-east).png"));
	if (img_GreenEnemy_NE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_NW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(north-west).png"));
	if (img_GreenEnemy_NW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_E = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(east).png"));
	if (img_GreenEnemy_E == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemy_W = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemy(west).png"));
	if (img_GreenEnemy_W == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Blue Enemy
	img_BlueEnemy_S = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(south).png"));
	if (img_BlueEnemy_S == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_N = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(north).png"));
	if (img_BlueEnemy_N == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_SE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(south-east).png"));
	if (img_BlueEnemy_SE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_SW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(south-west).png"));
	if (img_BlueEnemy_SW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_NE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(north-east).png"));
	if (img_BlueEnemy_NE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_NW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(north-west).png"));
	if (img_BlueEnemy_NW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_E = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(east).png"));
	if (img_BlueEnemy_E == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_BlueEnemy_W = SDL_CreateTextureFromSurface(Renderer, IMG_Load("BlueEnemy(west).png"));
	if (img_BlueEnemy_W == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Red Enemy
	img_RedEnemy_S = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_S.png"));
	if (img_RedEnemy_S == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_N = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_N.png"));
	if (img_RedEnemy_N == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_SE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_SE.png"));
	if (img_RedEnemy_SE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_SW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_SW.png"));
	if (img_RedEnemy_SW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_NE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_NE.png"));
	if (img_RedEnemy_NE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_NW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_NW.png"));
	if (img_RedEnemy_NW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_E = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_E.png"));
	if (img_RedEnemy_E == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_RedEnemy_W = SDL_CreateTextureFromSurface(Renderer, IMG_Load("RedEnemy_W.png"));
	if (img_RedEnemy_W == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Enemy Group
	img_GreenEnemyGroup_S = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(S).png"));
	if (img_GreenEnemyGroup_S == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_N = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(N).png"));
	if (img_GreenEnemyGroup_N == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_SE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(SE).png"));
	if (img_GreenEnemyGroup_SE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_SW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(SW).png"));
	if (img_GreenEnemyGroup_SW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_NE = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(NE).png"));
	if (img_GreenEnemyGroup_NE == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_NW = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(NW).png"));
	if (img_GreenEnemyGroup_NW == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_E = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(E).png"));
	if (img_GreenEnemyGroup_E == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_GreenEnemyGroup_W = SDL_CreateTextureFromSurface(Renderer, IMG_Load("GreenEnemyGroup(W).png"));
	if (img_GreenEnemyGroup_W == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Game Over
	img_GameOver = SDL_CreateTextureFromSurface(Renderer, IMG_Load("game_over.png"));
	if (img_GameOver == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Score
	img_Zero = SDL_CreateTextureFromSurface(Renderer, IMG_Load("0.png"));
	if (img_Zero == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_One = SDL_CreateTextureFromSurface(Renderer, IMG_Load("1,1.png"));
	if (img_One == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Two = SDL_CreateTextureFromSurface(Renderer, IMG_Load("2.png"));
	if (img_Two == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Three = SDL_CreateTextureFromSurface(Renderer, IMG_Load("3.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Four = SDL_CreateTextureFromSurface(Renderer, IMG_Load("4.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Five = SDL_CreateTextureFromSurface(Renderer, IMG_Load("5.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Six = SDL_CreateTextureFromSurface(Renderer, IMG_Load("6.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Seven = SDL_CreateTextureFromSurface(Renderer, IMG_Load("7.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Eight = SDL_CreateTextureFromSurface(Renderer, IMG_Load("8.png"));
	if (img_Three == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Nine = SDL_CreateTextureFromSurface(Renderer, IMG_Load("9.png"));
	if (img_Nine == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	//Stages
	img_Stage1 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("STAGE1.png"));
	if (img_Stage1 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Stage2 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("STAGE2.png"));
	if (img_Stage2 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Stage3 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("STAGE3.png"));
	if (img_Stage3 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Stage4 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("STAGE4.png"));
	if (img_Stage4 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_Stage5 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("STAGE5.png"));
	if (img_Stage5 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player_S);
	SDL_DestroyTexture(img_player_W);
	SDL_DestroyTexture(img_player_E);
	SDL_DestroyTexture(img_player_NW);
	SDL_DestroyTexture(img_player_NE);
	SDL_DestroyTexture(img_player_N);
	SDL_DestroyTexture(img_player_SW);
	SDL_DestroyTexture(img_player_SE);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_GreenEnemy_S);
	SDL_DestroyTexture(img_GreenEnemy_E);
	SDL_DestroyTexture(img_GreenEnemy_W);
	SDL_DestroyTexture(img_GreenEnemy_N);
	SDL_DestroyTexture(img_GreenEnemy_NE);
	SDL_DestroyTexture(img_GreenEnemy_NW);
	SDL_DestroyTexture(img_GreenEnemy_SE);
	SDL_DestroyTexture(img_GreenEnemy_SW);
	SDL_DestroyTexture(img_GreenEnemyGroup_S);
	SDL_DestroyTexture(img_GreenEnemyGroup_E);
	SDL_DestroyTexture(img_GreenEnemyGroup_W);
	SDL_DestroyTexture(img_GreenEnemyGroup_N);
	SDL_DestroyTexture(img_GreenEnemyGroup_NE);
	SDL_DestroyTexture(img_GreenEnemyGroup_NW);
	SDL_DestroyTexture(img_GreenEnemyGroup_SE);
	SDL_DestroyTexture(img_GreenEnemyGroup_SW);
	SDL_DestroyTexture(img_BlueEnemy_S);
	SDL_DestroyTexture(img_BlueEnemy_E);
	SDL_DestroyTexture(img_BlueEnemy_W);
	SDL_DestroyTexture(img_BlueEnemy_N);
	SDL_DestroyTexture(img_BlueEnemy_NE);
	SDL_DestroyTexture(img_BlueEnemy_NW);
	SDL_DestroyTexture(img_BlueEnemy_SE);
	SDL_DestroyTexture(img_BlueEnemy_SW);
	SDL_DestroyTexture(img_RedEnemy_S);
	SDL_DestroyTexture(img_RedEnemy_E);
	SDL_DestroyTexture(img_RedEnemy_W);
	SDL_DestroyTexture(img_RedEnemy_N);
	SDL_DestroyTexture(img_RedEnemy_NE);
	SDL_DestroyTexture(img_RedEnemy_NW);
	SDL_DestroyTexture(img_RedEnemy_SE);
	SDL_DestroyTexture(img_RedEnemy_SW);
	SDL_DestroyTexture(img_Heart);
	SDL_DestroyTexture(img_EmptyHeart);
	SDL_DestroyTexture(img_WhiteHeart);
	SDL_DestroyTexture(img_GreyHeart);
	SDL_DestroyTexture(img_Black);
	SDL_DestroyTexture(img_Press);
	SDL_DestroyTexture(img_One);
	SDL_DestroyTexture(img_Zero);
	SDL_DestroyTexture(img_Two);
	SDL_DestroyTexture(img_Three);
	SDL_DestroyTexture(img_Nine);
	SDL_DestroyTexture(img_Eight);
	SDL_DestroyTexture(img_Four);
	SDL_DestroyTexture(img_Seven);
	SDL_DestroyTexture(img_Six);
	SDL_DestroyTexture(img_Five);
	SDL_DestroyTexture(img_Stage1);
	SDL_DestroyTexture(img_Stage2);
	SDL_DestroyTexture(img_Stage3);
	SDL_DestroyTexture(img_Stage4);
	SDL_DestroyTexture(img_Stage5);
	IMG_Quit();
	
	// Free Audios
	for (int i = 0; i < num_tracks; ++i)
		Mix_FreeMusic(tracks[i]);
	for (int i = 0; i < num_sfxs; ++i)
		Mix_FreeChunk(sfxs[i]);

	// Close SDL_Mixer
	Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();

	//Clean up all SDL initialized subsystems
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	buttons = SDL_GetMouseState(&mouseX, &mouseY);
	return true;
}
bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	float fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN) god_mode = !god_mode;
	if (keys[SDL_SCANCODE_F2] == KEY_DOWN)		toggle_enemies = !toggle_enemies;
	if (keys[SDL_SCANCODE_F3] == KEY_DOWN) DMG = 1000;
	if (keys[SDL_SCANCODE_F4] == KEY_DOWN) DMG = 12;
	if (keys[SDL_SCANCODE_W] == KEY_REPEAT && Player.GetY() > 0) fy = -1;
	if (keys[SDL_SCANCODE_S] == KEY_REPEAT && Player.GetY() < 685) fy = 1;
	if (keys[SDL_SCANCODE_A] == KEY_REPEAT && Player.GetX() > 0) fx = -1;
	if (keys[SDL_SCANCODE_D] == KEY_REPEAT && Player.GetX() < 920) fx = 1;
	if (buttons == SDL_BUTTON_LEFT && bullet_delay_c == 0) {

		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
	
		int offsetX, offsetY;

		if ((fabs(Player.GetX() - mouseX) <= 50) && ((Player.GetY() - mouseY) > 0)) {
			offsetX = 26;
			offsetY = 12;
		}
		else if ((fabs(Player.GetX() - mouseX) <= 50) && ((Player.GetY() - mouseY) < 0)) {
			offsetX = 26;
			offsetY = 50;
		}
		else if (((Player.GetX() - mouseX) > 0) && (fabs(Player.GetY() - mouseY) <= 50)) {
			offsetX = 0;
			offsetY = 38;
		}
		else if (((Player.GetX() - mouseX) < 0) && (fabs(Player.GetY() - mouseY) <= 50)) {
			offsetX = 54;
			offsetY = 38;
		}
		else if (((Player.GetX() - mouseX) > 0) && (((Player.GetY() - mouseY) > 0))) {
			offsetX = 8;
			offsetY = 24;
		}
		else if (((Player.GetX() - mouseX) < 0) && (((Player.GetY() - mouseY) > 0))) {
			offsetX = 46;
			offsetY = 24;
		}
		else if (((Player.GetX() - mouseX) > 0) && (((Player.GetY() - mouseY) < 0))) {
			offsetX = 8;
			offsetY = 48;
		}
		else if (((Player.GetX() - mouseX) < 0) && (((Player.GetY() - mouseY) < 0))) {
			offsetX = 46;
			offsetY = 48;
		}
		Shots[idx_shot].Init(x + offsetX, y + offsetY, 12, 12, 10, NULL, (mouseX - (x + offsetX)) / sqrt(pow(mouseY - (y + offsetY), 2) + pow(mouseX - (x + offsetX), 2)), (mouseY - (y + offsetY)) / sqrt(pow(mouseY - (y + offsetY), 2) + pow(mouseX - (x + offsetX), 2)), NULL);
		idx_shot++;
		idx_shot %= MAX_SHOTS;

		// Play a single Sound
		if (HPcounter > 0) {
			if (MusicC < 1000) {
				Mix_PlayChannel(1, sfxs[0], 0);
				Mix_Volume(-1, MIX_MAX_VOLUME / 40);
			}
			if (MusicC < 2000 && MusicC > 1000) {
				Mix_PlayChannel(1, sfxs[0], 0);
				Mix_Volume(-1, MIX_MAX_VOLUME / 40);
			}
			if (MusicC > 2000) MusicC = 0;
			MusicC++;
		}
	}
	//Bullet Delay
	if (bullet_delay_c < BULLET_DELAY) {
		bullet_delay_c++;
	}
	else {
		bullet_delay_c = 0;
	}
	//Enemy Init
	srand(time(NULL));
	if ((toggle_enemies == true && idx_Enemy < (MAX_ENEMIES - 1)) && (Enemy_delay == 0)) {
		int val1 = rand() % 2, val2 = rand() % 2, val3 = rand() % WINDOW_WIDTH, val4 = rand() % WINDOW_HEIGHT, x = 0, y = 0;
			if (val1 == 0 && val2 == 0) {
				x = -50;
				y = val4;
			}
			if (val1 == 1 && val2 == 0) {
				x = WINDOW_WIDTH;
				y = val4;
			}
			if (val1 == 0 && val2 == 1) {
				y = -50;
				x = val3;
			}
			if (val1 == 1 && val2 == 1) {
				y = WINDOW_HEIGHT;
				x = val3;
			}
			int enemyType = rand() % 21 + 1; // So that one type of enemy spawns less, we make an enemy type be multiple numbers
			//Stages
			if (STAGE == 1 && EnemyCounter <= 9) {
				enemyType = 1;
				if (enemyType < 11) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			if (STAGE == 2 && EnemyCounter >= 10 && EnemyCounter <= 29) {
				if (enemyType == 11 || enemyType == 12) enemyType = 1;
				if (enemyType < 11) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				else if (enemyType > 12) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 3, 60, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			if (STAGE == 3 && EnemyCounter >= 30 && EnemyCounter <= 49) {
				if (enemyType == 11 || enemyType == 12) enemyType = 1;
				if (enemyType < 11) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				else if (enemyType > 12) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 3, 60, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			if (STAGE == 4 && EnemyCounter >= 50 && EnemyCounter <= 64) {
				if (enemyType > 11) enemyType = 11;
				if (enemyType < 11) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				else if (enemyType == 11) {
					Enemy[idx_Enemy].Init(x, y, 72, 88, 1, 500, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			if (STAGE == 5 && EnemyCounter >= 65 && EnemyCounter <= 89) {
				if (enemyType == 12) enemyType = 11;
				if (enemyType < 11) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				if (enemyType == 11) {
					Enemy[idx_Enemy].Init(x, y, 72, 88, 1, 500, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				else if (enemyType > 12) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 3, 60, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			if (STAGE == 6 && EnemyCounter >= 90 && EnemyCounter <= 104) {
				if (enemyType >= 14) enemyType = 12;
				if (enemyType < 14) enemyType = 1;
				if (enemyType < 14) {
					Enemy[idx_Enemy].Init(x, y, 32, 64, 1, 110, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
				else if (enemyType == 12) {
					Enemy[idx_Enemy].Init(x, y, 36, 69, 2, 400, (Player.GetX() - x) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), (Player.GetY() - y) / sqrt(pow(Player.GetY() - y, 2) + pow(Player.GetX() - x, 2)), enemyType);
					EnemyCounter++;
				}
			}
			idx_Enemy++;
			idx_Enemy %= MAX_ENEMIES;
	}
	// Stage Max Enemies 1 => 2
	if (SCORE == 10 && X == false) {
		STAGE = 2;
		Enemy_delay = 0;
		ENEMY_DELAY = 300;
		X = true;
	}
	if (ENEMY_DELAY == 300 && Enemy_delay >= ENEMY_DELAY) {
		ENEMY_DELAY = 350;
		Enemy_delay = 0;
	}
	// Stage Max Enemies 2 => 3
	if (SCORE == 30 && X == true) {
		STAGE = 3;
		Enemy_delay = 0;
		ENEMY_DELAY = 301;
		X = false;
	}
	if (ENEMY_DELAY == 301 && Enemy_delay >= ENEMY_DELAY) {
		ENEMY_DELAY = 250;
		Enemy_delay = 0;
	}
	// Stage Max Enemies 3 => 4
	if (SCORE == 50 && X == false) {
		STAGE = 4;
		Enemy_delay = 0;
		ENEMY_DELAY = 302;
		X = true;
	}
	if (ENEMY_DELAY == 302 && Enemy_delay >= ENEMY_DELAY) {
		ENEMY_DELAY = 700;
		Enemy_delay = 0;
	}
	// Stage Max Enemies 4 => 5
	if (SCORE == 65 && X == true) {
		STAGE = 5;
		Enemy_delay = 0;
		ENEMY_DELAY = 303;
		X = false;
	}
	if (ENEMY_DELAY == 303 && Enemy_delay >= ENEMY_DELAY) {
		ENEMY_DELAY = 200;
		Enemy_delay = 0;
	}
	//Stage Max Enemies 5 => 6
	if (SCORE == 90 && X == false) {
		STAGE = 6;
		Enemy_delay = 0;
		ENEMY_DELAY = 304;
		X = true;
	}
	if (ENEMY_DELAY == 304 && Enemy_delay >= ENEMY_DELAY) {
		ENEMY_DELAY = 400;
		Enemy_delay = 0;
	}
	//Enemy Delay
	if (Enemy_delay < ENEMY_DELAY) {
		Enemy_delay++;
	}
	else {
		Enemy_delay = 0;
	}
	
	//Logic
	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(Shots[i].GetShotX(), Shots[i].GetShotY());
			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();
			if (Shots[i].GetX() < -WINDOW_WIDTH)	Shots[i].ShutDown();
			if (Shots[i].GetY() > WINDOW_HEIGHT)	Shots[i].ShutDown();
			if (Shots[i].GetY() < -WINDOW_HEIGHT)	Shots[i].ShutDown();
	
		}
	}
	//Enemy update
	for (int i = 0; i < idx_Enemy; i++) {

		if (Enemy[i].IsAlive())
		{
			Enemy[i].Move(((Player.GetX() + 16) - Enemy[i].GetX()) / sqrt(pow(Player.GetY() - Enemy[i].GetY(), 2) + pow((Player.GetX() + 16) - Enemy[i].GetX(), 2)), (Player.GetY() - Enemy[i].GetY()) / sqrt(pow(Player.GetY() - Enemy[i].GetY(), 2) + pow((Player.GetX() + 16) - Enemy[i].GetX(), 2)));
		}
	}
	//Enemy kill
	for (int i = 0; i < idx_Enemy; i++) {
		for (int j = 0; j < idx_shot; j++) {
			int enemy_x, enemy_y, enemy_w, enemy_h;
			Enemy[i].GetRect(&enemy_x, &enemy_y, &enemy_w, &enemy_h);
			int bullet_x, bullet_y, bullet_w, bullet_h;
			Shots[j].GetRect(&bullet_x, &bullet_y, &bullet_w, &bullet_h);
			if (((bullet_x >= enemy_x && bullet_x <= enemy_w + enemy_x) || (bullet_w + bullet_x >= enemy_x && bullet_w + bullet_x <= enemy_w + enemy_x)) && ((bullet_y >= enemy_y && bullet_y <= enemy_h + enemy_y ) || (bullet_h + bullet_y>= enemy_y && bullet_h + bullet_y <= enemy_h + enemy_y)))  {
				Enemy[i].EnemyHPloss(DMG);
				Shots[j].ShutDown();
				Shots[j].ResetEnemyPos();
			}
		}
		if (Enemy[i].GetEnemyHP() <= 0) {
			Enemy[i].ShutDown();
			Enemy[i].ResetEnemyPos();
			for (int k = 0; k < 200; k++) {
				if (ScoreAux[k] == -1 && ScoreAux[k] != i) {
					ScoreAux[k] = i;
					SCORE++;
					break;
				}
				if (ScoreAux[k] == i) {
					break;
				}
			}
		}
	}
	//Player kill
	if (god_mode == false) {
		for (int i = 0; i < idx_Enemy; i++) {
			int enemy_x, enemy_y, enemy_w, enemy_h;
			Enemy[i].GetRect(&enemy_x, &enemy_y, &enemy_w, &enemy_h);
			int player_x, player_y, player_w, player_h;
			Player.GetRect(&player_x, &player_y, &player_w, &player_h);
			if (((player_x >= enemy_x && player_x <= enemy_w + enemy_x) || (player_w + player_x >= enemy_x && player_w + player_x <= enemy_w + enemy_x)) && ((player_y >= enemy_y && player_y <= enemy_h + enemy_y) || (player_h + player_y >= enemy_y && player_h + player_y <= enemy_h + enemy_y))) {
				if (aux == true) {
					HPcounter--;
					aux = false;
				}
			}
		}
	}
	if (aux == false) {
		auxC++;
	}
	if (auxC == 200) {
		aux = true;
		auxC = 0;
	}
	return false;
}

void Game::Draw()
{
	SDL_Rect rc;
	SDL_Rect rc2;

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//God mode uses red wireframe rectangles for physical objects
	if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
		//Draw scene
		Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_background, NULL, &rc);
		rc.x += rc.w;
		SDL_RenderCopy(Renderer, img_background, NULL, &rc);

		//Draw player
		if (HPcounter > 0) {
			Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			if ((fabs(Player.GetX() - mouseX) <= 50) && ((Player.GetY() - mouseY) > 0)) {
				SDL_RenderCopy(Renderer, img_player_N, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if ((fabs(Player.GetX() - mouseX) <= 50) && ((Player.GetY() - mouseY) < 0)) {
				SDL_RenderCopy(Renderer, img_player_S, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) > 0) && (fabs(Player.GetY() - mouseY) <= 50)) {
				SDL_RenderCopy(Renderer, img_player_W, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) < 0) && (fabs(Player.GetY() - mouseY) <= 50)) {
				SDL_RenderCopy(Renderer, img_player_E, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) > 0) && (((Player.GetY() - mouseY) > 0))) {
				SDL_RenderCopy(Renderer, img_player_NW, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) < 0) && (((Player.GetY() - mouseY) > 0))) {
				SDL_RenderCopy(Renderer, img_player_NE, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) > 0) && (((Player.GetY() - mouseY) < 0))) {
				SDL_RenderCopy(Renderer, img_player_SW, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
			else if (((Player.GetX() - mouseX) < 0) && (((Player.GetY() - mouseY) < 0))) {
				SDL_RenderCopy(Renderer, img_player_SE, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
		}



		//Draw shots
		if (HPcounter > 0) {
			for (int i = 0; i < MAX_SHOTS; ++i)
			{
				if (Shots[i].IsAlive())
				{
					Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
					if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
				}
			}
		}

		//Draw enemies
		if (HPcounter > 0) {
			for (int i = 0; i < MAX_ENEMIES; ++i)
			{
				if (Enemy[i].IsAlive())
				{
					if (Enemy[i].getEnemyType() < 11) {
						if (fabs((Enemy[i].GetX() - Player.GetX())) <= 50 && (Enemy[i].GetY() - Player.GetY()) < 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_S, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((fabs(Enemy[i].GetX() - Player.GetX()) <= 50) && (Enemy[i].GetY() - Player.GetY()) > 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_N, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) > 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_W, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) < 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_E, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_NW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_NE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_SW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemy_SE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
					}
					if (Enemy[i].getEnemyType() == 11) {
						if (fabs((Enemy[i].GetX() - Player.GetX())) <= 50 && (Enemy[i].GetY() - Player.GetY()) < 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_S, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((fabs(Enemy[i].GetX() - Player.GetX()) <= 50) && (Enemy[i].GetY() - Player.GetY()) > 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_N, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) > 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_W, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) < 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_E, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_NW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_NE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_SW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_GreenEnemyGroup_SE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
					}
					if (Enemy[i].getEnemyType() == 12) {
						if (fabs((Enemy[i].GetX() - Player.GetX())) <= 50 && (Enemy[i].GetY() - Player.GetY()) < 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_S, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((fabs(Enemy[i].GetX() - Player.GetX()) <= 50) && (Enemy[i].GetY() - Player.GetY()) > 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_N, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) > 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_W, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) < 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_E, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_NW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_NE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_SW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_RedEnemy_SE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
					}
					else if (Enemy[i].getEnemyType() > 12) {
						if (fabs((Enemy[i].GetX() - Player.GetX())) <= 50 && (Enemy[i].GetY() - Player.GetY()) < 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_S, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((fabs(Enemy[i].GetX() - Player.GetX()) <= 50) && (Enemy[i].GetY() - Player.GetY()) > 0) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_N, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) > 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_W, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if ((Enemy[i].GetX() - Player.GetX()) < 0 && fabs((Enemy[i].GetY() - Player.GetY())) <= 50) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_E, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_NW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) > 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_NE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) > 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_SW, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
						else if (((Enemy[i].GetX() - Player.GetX()) < 0) && (((Enemy[i].GetY() - Player.GetY()) < 0))) {
							Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
							SDL_RenderCopy(Renderer, img_BlueEnemy_SE, NULL, &rc);
							if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
						}
					}



				}
			}
		}
		//Draw Hearts
		if (aux == false) {
			if (HPcounter == 2) {
				if (auxC <= 25 || (auxC > 50) && (auxC <= 75) || (auxC > 100) && (auxC <= 125) || (auxC > 150) && (auxC <= 175)) {
					for (int i = 0; i < 2; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_WhiteHeart, NULL, &rc);
					}
					Heart[2].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_GreyHeart, NULL, &rc);
				}
				if ((auxC > 25) && (auxC <= 50) || (auxC > 75) && (auxC <= 100) || (auxC > 125) && (auxC <= 150) || auxC > 175) {
					for (int i = 0; i < 2; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_Heart, NULL, &rc);
					}
					Heart[2].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_EmptyHeart, NULL, &rc);
				}
			}
			if (HPcounter == 1) {
				if (auxC <= 25 || (auxC > 50) && (auxC <= 75) || (auxC > 100) && (auxC <= 125) || (auxC > 150) && (auxC <= 175)) {
					Heart[0].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_WhiteHeart, NULL, &rc);
					for (int i = 1; i < 3; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_GreyHeart, NULL, &rc);
					}
				}
				if ((auxC > 25) && (auxC <= 50) || (auxC > 75) && (auxC <= 100) || (auxC > 125) && (auxC <= 150) || auxC > 175) {
					Heart[0].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_Heart, NULL, &rc);
					for (int i = 1; i < 3; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_EmptyHeart, NULL, &rc);
					}
				}
			}
			if (HPcounter == 0) {
				if (auxC <= 25 || (auxC > 50) && (auxC <= 75) || (auxC > 100) && (auxC <= 125) || (auxC > 150) && (auxC <= 175)) {
					for (int i = 0; i < 3; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_GreyHeart, NULL, &rc);
					}
				}
				if ((auxC > 25) && (auxC <= 50) || (auxC > 75) && (auxC <= 100) || (auxC > 125) && (auxC <= 150) || auxC > 175) {
					for (int i = 0; i < 3; i++) {
						Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
						SDL_RenderCopy(Renderer, img_GreyHeart, NULL, &rc);
					}
				}
			}
		}
		if (aux == true) {
			if (HPcounter == 3) {
				for (int i = 0; i < 3; i++) {
					Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_Heart, NULL, &rc);
				}
			}
			if (HPcounter == 2) {
				for (int i = 0; i < 2; i++) {
					Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_Heart, NULL, &rc);
				}
				Heart[2].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_EmptyHeart, NULL, &rc);
			}
			if (HPcounter == 1) {
				Heart[0].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Heart, NULL, &rc);
				for (int i = 1; i < 3; i++) {
					Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_EmptyHeart, NULL, &rc);
				}
			}
			if (HPcounter == 0) {
				for (int i = 0; i < 3; i++) {
					Heart[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_EmptyHeart, NULL, &rc);
				}
			}
		}

		//Draw game over
		if (HPcounter <= 0 || score >= 1000) {
			rc.x = 300;
			rc.y = 100;
			rc.h = 500;
			rc.w = 400;
			SDL_RenderCopy(Renderer, img_GameOver, NULL, &rc);
		}

		//Draw score
		Score1.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		Score2.GetRect(&rc2.x, &rc2.y, &rc2.w, &rc2.h);
		switch (SCORE)
		{
		case 0: case 10: case 20: case 30: case 40: case 50: case 60: case 70: case 80: case 90:
			SDL_RenderCopy(Renderer, img_Zero, NULL, &rc);
			if (SCORE == 10)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 20)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 30)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 40)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 50)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 60)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 70)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 80)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 90)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 1: case 11:case 21: case 31: case 41: case 51: case 61: case 71: case 81: case 91:
			SDL_RenderCopy(Renderer, img_One, NULL, &rc);
			if (SCORE == 11)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 21)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 31)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 41)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 51)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 61)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 71)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 81)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 91)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 2: case 12: case 22: case 32: case 42: case 52: case 62: case 72: case 82: case 92:
			SDL_RenderCopy(Renderer, img_Two, NULL, &rc);
			if (SCORE == 12)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 22)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 32)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 42)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 52)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 62)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 72)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 82)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 92)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 3: case 13 :case 23: case 33: case 43: case 53: case 63: case 73: case 83: case 93:
			SDL_RenderCopy(Renderer, img_Three, NULL, &rc);
			if (SCORE == 13)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 23)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 33)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 43)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 53)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 63)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 73)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 83)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 93)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 4: case 14: case 24: case 34: case 44: case 54: case 64: case 74: case 84: case 94:
			SDL_RenderCopy(Renderer, img_Four, NULL, &rc);
			if (SCORE == 14)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 24)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 34)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 44)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 54)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 64)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 74)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 84)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 94)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 5: case 15: case 25: case 35: case 45: case 55: case 65: case 75: case 85: case 95:
			SDL_RenderCopy(Renderer, img_Five, NULL, &rc);
			if (SCORE == 15)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 25)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 35)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 45)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 55)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 65)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 75)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 85)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 95)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 6: case 16: case 26: case 36: case 46: case 56: case 66: case 76: case 86: case 96:
			SDL_RenderCopy(Renderer, img_Six, NULL, &rc);
			if (SCORE == 16)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 26)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 36)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 46)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 56)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 66)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 76)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 86)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 96)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 7: case 17: case 27: case 37: case 47: case 57: case 67: case 77: case 87: case 97:
			SDL_RenderCopy(Renderer, img_Seven, NULL, &rc);
			if (SCORE == 17)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 27)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 37)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 47)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 57)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 67)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 77)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 87)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 97)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 8: case 18: case 28: case 38: case 48: case 58: case 68: case 78: case 88: case 98:
			SDL_RenderCopy(Renderer, img_Eight, NULL, &rc);
			if (SCORE == 18)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 28)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 38)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 48)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 58)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 68)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 78)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 88)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 98)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		case 9: case 19: case 29: case 39: case 49: case 59: case 69: case 79: case 89: case 99:
			SDL_RenderCopy(Renderer, img_Nine, NULL, &rc);
			if (SCORE == 19)
				SDL_RenderCopy(Renderer, img_One, NULL, &rc2);
			if (SCORE == 29)
				SDL_RenderCopy(Renderer, img_Two, NULL, &rc2);
			if (SCORE == 39)
				SDL_RenderCopy(Renderer, img_Three, NULL, &rc2);
			if (SCORE == 49)
				SDL_RenderCopy(Renderer, img_Four, NULL, &rc2);
			if (SCORE == 59)
				SDL_RenderCopy(Renderer, img_Five, NULL, &rc2);
			if (SCORE == 69)
				SDL_RenderCopy(Renderer, img_Six, NULL, &rc2);
			if (SCORE == 79)
				SDL_RenderCopy(Renderer, img_Seven, NULL, &rc2);
			if (SCORE == 89)
				SDL_RenderCopy(Renderer, img_Eight, NULL, &rc2);
			if (SCORE == 99)
				SDL_RenderCopy(Renderer, img_Nine, NULL, &rc2);
			break;
		}
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		//Draw Stage
		if (STAGE != 0) {
			if (STAGE == 1) {
				Stage.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Stage1, NULL, &rc);
			}
			if (STAGE == 2) {
				Stage.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Stage2, NULL, &rc);
			}
			if (STAGE == 3) {
				Stage.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Stage3, NULL, &rc);
			}
			if (STAGE == 4) {
				Stage.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Stage4, NULL, &rc);
			}
			if (STAGE == 5) {
				Stage.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Stage5, NULL, &rc);
			}
		}
		//Menu
		if (Menu == false) {
			StartBack.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_Black, NULL, &rc);
			rc.x += rc.w;
			SDL_RenderCopy(Renderer, img_Black, NULL, &rc);
			if (counter1 <= 75) {
				Text.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_Press, NULL, &rc);
			}
			if (counter1 >= 125) {
				counter1 = 0;
			}
			if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN) {
				Menu = true;
				toggle_enemies = true;
				STAGE = 1;
			}
			counter1++;
		}

		//Update screen
		SDL_RenderPresent(Renderer);

		SDL_Delay(10);	// 1000/10 = 100 fps max
	
}