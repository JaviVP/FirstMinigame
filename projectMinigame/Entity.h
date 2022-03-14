#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s, int hp, float posx2, float posy2);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	int  GetX();
	int  GetY();
	float  GetShotX();
	float  GetShotY();
	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	void Move(float dx, float dy);
	void Stop();
    int GetEnemyHP();
	void EnemyHPloss(int dmg);
	int PlayerHPloss();
	void ResetEnemyPos();

private:
	int PlayerHP[3] = { 1,1,1 };
	float x, y;
	int width, height;
	float speed;
	int hitPoints;
	bool is_alive;
	float x_shot, y_shot; //Position for bullets to travel to (should be given as (y2-y1) / (x2-x1))
};

