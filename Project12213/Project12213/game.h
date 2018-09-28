#pragma once

// charcter entities
struct Player
{
	int x;
	int y;
}  P1, P2, CPU;

// point entities
struct Points
{
	int x;
	int y;
}	Ten1, Ten2, Ten3,
Twenty1, Twenty2, Twenty3,
Thirty1, Thirty2, Thirty3,
Forty1, Forty2, Forty3,
Fifty1, Fifty2, Fifty3;

void createSounds();
void createMusic();
void createImages();
void createTexts();
void Tick();
void Tick2();
void Tick2();
static inline std::string convert(int x);
void randomPositions();
int* getNearestPoint();
int* getFarthestPoint();