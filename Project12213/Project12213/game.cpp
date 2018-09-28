#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "game.h"

using namespace sf;

bool musicVol = true;
bool soundVol = true;

bool chase = false;

// gameboard dimensions
int width = 30;
int height = 20;
int size = 32;
int w = size * width;
int h = size * height;

// point entities that start on board
int plays = 15;

// player 1 direction
int dir = 4;
// player 2 direction
int dir2 = 4;
// CPU direction
int dir3 = 4;

// player 1 score
int score1 = 0;
// player 2 score
int score2 = 0;
// CPU score
int score3 = 0;

// skin number save
int skinNum = 0;

// music on and off save
int musicNum = 0;

// sound effects on and off save
int soundNum = 0;

// countdown to game start
int countdown = 0;

// wins on easy, normal, and hard
int easyWins, normalWins, hardWins;

// menu music and in game music
Music menu, game;

// sound effect buffers
SoundBuffer sound, sound2, sound3, sound4, sound5;

// sound effects
Sound pointSound, pointSound2, computerSound, countdownSound, goSound;

// textures
Texture white, green, blue, yellow, magenta, grey, player1, player2, comp, lava, boogers, sun, lavaBlue, boogersBloody, sunDark, smiley, largeC;

// sprites
Sprite background, whiteSprite, player1Sprite, greenSprite, blueSprite, yellowSprite, magentaSprite, greySprite, player2Sprite, computerSprite, skin1, skin2, skin3, skin4, skin5, skin6, skin7, skin8;

// gets font
Font font;

// all the different text boxes
Text text, title, onePlayer, twoPlayers, instruct, stats, skins, musicOn, musicOff, soundOn, soundOff, instructTextS, instructTextSU, instructTextT, instructTextTU, easyW, normalW, hardW, clearStats, currSkin, skin1Text, skin2Text, skin3Text, skin4Text, skin5Text, skin6Text, skin7Text, skin8Text, selectDiff, changeDiff, pauseText, easy, normal, hard, winP1, winP2, winP, loseP, playAgain, backToMenu, three, two, one, go;

// assigns buffers to sound effects
void createSounds()
{
	// this is the ring sound from Sonic the Hedgehog, we do not own this
	sound.loadFromFile("Audio/PointSound.wav");
	pointSound.setBuffer(sound);

	sound2.loadFromFile("Audio/PointSound2.wav");
	pointSound2.setBuffer(sound2);

	sound3.loadFromFile("Audio/AISound.wav");
	computerSound.setBuffer(sound3);

	sound4.loadFromFile("Audio/Countdown.wav");
	countdownSound.setBuffer(sound4);

	sound5.loadFromFile("Audio/Go.wav");
	goSound.setBuffer(sound5);

	// sets sound effects on or off depending on save file
	if (soundNum == 1)
	{
		soundVol = false;
		pointSound.setVolume(0.0);
		pointSound2.setVolume(0.0);
		computerSound.setVolume(0.0);
		countdownSound.setVolume(0.0);
		goSound.setVolume(0.0);
	}
	else if (soundNum == 0)
	{
		soundVol = true;
		pointSound.setVolume(100.0);
		pointSound2.setVolume(100.0);
		computerSound.setVolume(100.0);
		countdownSound.setVolume(100.0);
		goSound.setVolume(100.0);
	}
}

// assigns music to menu and in game
void createMusic()
{
	// this is Darude - Sandstorm, we do not own this
	menu.openFromFile("Audio/Darude.ogg");

	// this is Assault on Mist Castle, we do not own this
	game.openFromFile("Audio/AssaultOnMistCastle.ogg");

	// sets music on or off depending on save file
	if (musicNum == 1)
	{
		musicVol = false;
		menu.setVolume(0.0);
		game.setVolume(0.0);
	}
	else if (musicNum == 0)
	{
		musicVol = true;
		menu.setVolume(100.0);
		game.setVolume(100.0);
	}
}

// assigns sprites
void createImages() 
{
	// loads textures from files
	largeC.loadFromFile("images/background.png");
	white.loadFromFile("images/white.png");
	green.loadFromFile("images/green.png");
	blue.loadFromFile("images/blue.png");
	yellow.loadFromFile("images/yellow.png");
	magenta.loadFromFile("images/magenta.png");
	grey.loadFromFile("images/grey.png");
	player1.loadFromFile("images/player1.png");
	player2.loadFromFile("images/player2.png");
	comp.loadFromFile("images/cpu.png");
	lava.loadFromFile("images/skin2.png");
	boogers.loadFromFile("images/skin3.png");
	sun.loadFromFile("images/skin4.png");
	lavaBlue.loadFromFile("images/skin5.png");
	boogersBloody.loadFromFile("images/skin6.png");
	sunDark.loadFromFile("images/skin7.png");
	smiley.loadFromFile("images/skin8.png");

	// assigns textures to sprites
	background.setTexture(largeC);
	whiteSprite.setTexture(white);
	greenSprite.setTexture(green);
	blueSprite.setTexture(blue);
	yellowSprite.setTexture(yellow);
	magentaSprite.setTexture(magenta);
	greySprite.setTexture(grey);
	player2Sprite.setTexture(player2);
	computerSprite.setTexture(comp);
	skin1.setTexture(player1);
	skin2.setTexture(lava);
	skin3.setTexture(boogers);
	skin4.setTexture(sun);
	skin5.setTexture(lavaBlue);
	skin6.setTexture(boogersBloody);
	skin7.setTexture(sunDark);
	skin8.setTexture(smiley);

	// sets player 1 skin to last save
	if (skinNum == 0)
		player1Sprite.setTexture(player1);
	if (skinNum == 1)
		player1Sprite.setTexture(lava);
	if (skinNum == 2)
		player1Sprite.setTexture(boogers);
	if (skinNum == 3)
		player1Sprite.setTexture(sun);
	if (skinNum == 4)
		player1Sprite.setTexture(lavaBlue);
	if (skinNum == 5)
		player1Sprite.setTexture(boogersBloody);
	if (skinNum == 6)
		player1Sprite.setTexture(sunDark);
	if (skinNum == 7)
		player1Sprite.setTexture(smiley);
}

// creates all text
void createTexts()
{
	// lodads in the Times New Roman font
	font.loadFromFile("Fonts/times.ttf");

	// score on bottom of board durning game
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);

	// menu title
	title.setFont(font);
	title.setCharacterSize(90);
	title.setFillColor(Color::White);
	title.setString("COLLECT THE POINTS");

	// single player menu option
	onePlayer.setFont(font);
	onePlayer.setCharacterSize(40);
	onePlayer.setString("Single Player");

	// two player menu option
	twoPlayers.setFont(font);
	twoPlayers.setCharacterSize(40);
	twoPlayers.setString("Two Players");

	// instructions menu option
	instruct.setFont(font);
	instruct.setCharacterSize(40);
	instruct.setString("Instructions");

	// statistics menu option
	stats.setFont(font);
	stats.setCharacterSize(40);
	stats.setString("Statistics");

	// skins menu option
	skins.setFont(font);
	skins.setCharacterSize(40);
	skins.setString("Skins");

	// music on menu option
	musicOn.setFont(font);
	musicOn.setCharacterSize(30);
	musicOn.setString("Music: On");

	// music off menu option
	musicOff.setFont(font);
	musicOff.setCharacterSize(30);
	musicOff.setString("Music: Off");

	// sound on menu option
	soundOn.setFont(font);
	soundOn.setCharacterSize(30);
	soundOn.setString("Sound Effects: On");

	// sound off menu option
	soundOff.setFont(font);
	soundOff.setCharacterSize(30);
	soundOff.setString("Sound Effects: Off");

	// single player instructions title
	instructTextS.setFont(font);
	instructTextS.setCharacterSize(60);
	instructTextS.setString("Single Player -");

	// single player instructions
	instructTextSU.setFont(font);
	instructTextSU.setCharacterSize(30);
	instructTextSU.setString("Move using WASD. Gain points by collecting colors before the computer:\nGrey - 10 Points\nGreen - 20 Points\nBlue - 30 Points\nMagenta - 40 Points\nYellow - 50 Points");

	// two players instructions title
	instructTextT.setFont(font);
	instructTextT.setCharacterSize(60);
	instructTextT.setString("Two Players -");

	// two players instructions
	instructTextTU.setFont(font);
	instructTextTU.setCharacterSize(30);
	instructTextTU.setString("Second player moves using arrow keys.\n\nExit game by pressing 'Backspace'\nPause game by pressing 'P'");

	// easy wins statistics
	easyW.setFont(font);
	easyW.setCharacterSize(90);
	easyW.setString("Easy Wins: " + convert(easyWins));
	easyW.setFillColor(Color::White);

	// normal wins statistics
	normalW.setFont(font);
	normalW.setCharacterSize(90);
	normalW.setString("Normal Wins: " + convert(normalWins));
	normalW.setFillColor(Color::White);

	// hard wins statistics
	hardW.setFont(font);
	hardW.setCharacterSize(90);
	hardW.setString("Hard Wins: " + convert(hardWins));
	hardW.setFillColor(Color::White);

	// clear statistics
	clearStats.setFont(font);
	clearStats.setCharacterSize(60);
	clearStats.setString("Clear Statistics");

	// shows current skin
	currSkin.setFont(font);
	currSkin.setCharacterSize(60);
	currSkin.setString("Current Skin: ");
	currSkin.setFillColor(Color::White);

	// skin1 requirements
	skin1Text.setFont(font);
	skin1Text.setCharacterSize(50);
	skin1Text.setString("Default skin");
	skin1Text.setFillColor(Color::White);

	// skin2 requirements
	skin2Text.setFont(font);
	skin2Text.setCharacterSize(50);
	skin2Text.setString("Win a game on easy");
	skin2Text.setFillColor(Color::White);

	// skin3 requirements
	skin3Text.setFont(font);
	skin3Text.setCharacterSize(50);
	skin3Text.setString("Win a game on normal");
	skin3Text.setFillColor(Color::White);

	// skin4 requirements
	skin4Text.setFont(font);
	skin4Text.setCharacterSize(50);
	skin4Text.setString("Win a game on hard");
	skin4Text.setFillColor(Color::White);

	// skin5 requirements
	skin5Text.setFont(font);
	skin5Text.setCharacterSize(50);
	skin5Text.setString("Win 15 games on easy");
	skin5Text.setFillColor(Color::White);

	// skin6 requirements
	skin6Text.setFont(font);
	skin6Text.setCharacterSize(50);
	skin6Text.setString("Win 10 games on normal");
	skin6Text.setFillColor(Color::White);

	// skin7 requirements
	skin7Text.setFont(font);
	skin7Text.setCharacterSize(50);
	skin7Text.setString("Win 5 games on hard");
	skin7Text.setFillColor(Color::White);

	// skin8 requirements
	skin8Text.setFont(font);
	skin8Text.setCharacterSize(50);
	skin8Text.setString("Collect all other skins");
	skin8Text.setFillColor(Color::White);

	// select difficulty title
	selectDiff.setFont(font);
	selectDiff.setCharacterSize(70);
	selectDiff.setString("Select Difficulty:");

	// screen display when game is paused
	pauseText.setFont(font);
	pauseText.setCharacterSize(200);
	pauseText.setString("PAUSED");
	pauseText.setFillColor(Color::Red);

	// easy from difficulty menu
	easy.setFont(font);
	easy.setCharacterSize(50);
	easy.setString("Easy");

	// normal from difficulty menu
	normal.setFont(font);
	normal.setCharacterSize(50);
	normal.setString("Normal");

	// hard from difficulty menu
	hard.setFont(font);
	hard.setCharacterSize(50);
	hard.setString("Hard");

	// three from in game countdown
	three.setFont(font);
	three.setCharacterSize(250);
	three.setFillColor(Color::Red);
	three.setString("3");

	// two from in game countdown
	two.setFont(font);
	two.setCharacterSize(250);
	two.setFillColor(Color::Red);
	two.setString("2");

	// one from in game countdown
	one.setFont(font);
	one.setCharacterSize(250);
	one.setFillColor(Color::Red);
	one.setString("1");

	// go from in game countdown
	go.setFont(font);
	go.setCharacterSize(300);
	go.setFillColor(Color::Red);
	go.setString("GO!");

	// single player win title
	winP.setFont(font);
	winP.setCharacterSize(70);
	winP.setFillColor(Color::White);
	winP.setString("You Win!");

	// single player loss title
	loseP.setFont(font);
	loseP.setCharacterSize(70);
	loseP.setFillColor(Color::White);
	loseP.setString("You Lose!");

	// two player, player one wins
	winP1.setFont(font);
	winP1.setCharacterSize(70);
	winP1.setFillColor(Color::White);
	winP1.setString("Player 1 Wins!");

	// two player, player two wins
	winP2.setFont(font);
	winP2.setCharacterSize(70);
	winP2.setFillColor(Color::White);
	winP2.setString("Player 2 Wins!");

	// play again (multiple menus)
	playAgain.setFont(font);
	playAgain.setCharacterSize(30);
	playAgain.setString("Play Again");

	// change difficulty after single player game
	changeDiff.setFont(font);
	changeDiff.setCharacterSize(30);
	changeDiff.setString("Change Difficulty");

	// back to main menu option (multiple menus)
	backToMenu.setFont(font);
	backToMenu.setCharacterSize(30);
	backToMenu.setString("Back to Menu");
}

// player 1 movement
void Tick()	
{
	// moves down
    if (dir==0) 
		P1.y += 1;

	// moves left
    if (dir==1) 
		P1.x -= 1;  

	// moves right
    if (dir==2) 
		P1.x += 1;  

	// moves up
    if (dir == 3) 
		P1.y -= 1;  

	// adding points to player one score and remove point entities from board
	if ((P1.x == Ten1.x) && (P1.y == Ten1.y)) 
    { 
		score1 += 10; 
    	Ten1.x = 1000000;
        Ten1.y = 1000000; 
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Ten2.x) && (P1.y == Ten2.y))
	{
		score1 += 10;
		Ten2.x = 1000000;
		Ten2.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Ten3.x) && (P1.y == Ten3.y))
	{
		score1 += 10;
		Ten3.x = 1000000;
		Ten3.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Twenty1.x) && (P1.y == Twenty1.y))
	{
		score1 += 20;
		Twenty1.x = 1000000;
		Twenty1.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Twenty2.x) && (P1.y == Twenty2.y))
	{
		score1 += 20;
		Twenty2.x = 1000000;
		Twenty2.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Twenty3.x) && (P1.y == Twenty3.y))
	{
		score1 += 20;
		Twenty3.x = 1000000;
		Twenty3.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Thirty1.x) && (P1.y == Thirty1.y))
	{
		score1 += 30;
		Thirty1.x = 1000000;
		Thirty1.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Thirty2.x) && (P1.y == Thirty2.y))
	{
		score1 += 30;
		Thirty2.x = 1000000;
		Thirty2.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Thirty3.x) && (P1.y == Thirty3.y))
	{
		score1 += 30;
		Thirty3.x = 1000000;
		Thirty3.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Forty1.x) && (P1.y == Forty1.y))
	{
		score1 += 40;
		Forty1.x = 1000000;
		Forty1.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Forty2.x) && (P1.y == Forty2.y))
	{
		score1 += 40;
		Forty2.x = 1000000;
		Forty2.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Forty3.x) && (P1.y == Forty3.y))
	{
		score1 += 40;
		Forty3.x = 1000000;
		Forty3.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Fifty1.x) && (P1.y == Fifty1.y))
	{
		score1 += 50;
		Fifty1.x = 1000000;
		Fifty1.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Fifty2.x) && (P1.y == Fifty2.y))
	{
		score1 += 50;
		Fifty2.x = 1000000;
		Fifty2.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}
	if ((P1.x == Fifty3.x) && (P1.y == Fifty3.y))
	{
		score1 += 50;
		Fifty3.x = 1000000;
		Fifty3.y = 1000000;
		plays--;
		chase = false;
		pointSound.play();
	}

	// keeps player 1 from moving out of the board
    if (P1.x > width - 2 || P1.x < 1 || P1.y > height - 2 || P1.y < 1)
		dir = 4;  
}

// player 2 movement
void Tick2()
{
	// moves down
	if (dir2 == 0)
		P2.y += 1;
	
	// moves left
	if (dir2 == 1)
		P2.x -= 1;

	// moves right
	if (dir2 == 2)
		P2.x += 1;

	// moves up
	if (dir2 == 3)
		P2.y -= 1;

	// adding points to player two score and remove point entities from board
	if ((P2.x == Ten1.x) && (P2.y == Ten1.y))
	{
		score2 += 10;
		Ten1.x = 1000000;
		Ten1.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Ten2.x) && (P2.y == Ten2.y))
	{
		score2 += 10;
		Ten2.x = 1000000;
		Ten2.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Ten3.x) && (P2.y == Ten3.y))
	{
		score2 += 10;
		Ten3.x = 1000000;
		Ten3.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Twenty1.x) && (P2.y == Twenty1.y))
	{
		score2 += 20;
		Twenty1.x = 1000000;
		Twenty1.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Twenty2.x) && (P2.y == Twenty2.y))
	{
		score2 += 20;
		Twenty2.x = 1000000;
		Twenty2.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Twenty3.x) && (P2.y == Twenty3.y))
	{
		score2 += 20;
		Twenty3.x = 1000000;
		Twenty3.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Thirty1.x) && (P2.y == Thirty1.y))
	{
		score2 += 30;
		Thirty1.x = 1000000;
		Thirty1.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Thirty2.x) && (P2.y == Thirty2.y))
	{
		score2 += 30;
		Thirty2.x = 1000000;
		Thirty2.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Thirty3.x) && (P2.y == Thirty3.y))
	{
		score2 += 30;
		Thirty3.x = 1000000;
		Thirty3.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Forty1.x) && (P2.y == Forty1.y))
	{
		score2 += 40;
		Forty1.x = 1000000;
		Forty1.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Forty2.x) && (P2.y == Forty2.y))
	{
		score2 += 40;
		Forty2.x = 1000000;
		Forty2.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Forty3.x) && (P2.y == Forty3.y))
	{
		score2 += 40;
		Forty3.x = 1000000;
		Forty3.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Fifty1.x) && (P2.y == Fifty1.y))
	{
		score2 += 50;
		Fifty1.x = 1000000;
		Fifty1.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Fifty2.x) && (P2.y == Fifty2.y))
	{
		score2 += 50;
		Fifty2.x = 1000000;
		Fifty2.y = 1000000;
		plays--;
		pointSound2.play();
	}
	if ((P2.x == Fifty3.x) && (P2.y == Fifty3.y))
	{
		score2 += 50;
		Fifty3.x = 1000000;
		Fifty3.y = 1000000;
		plays--;
		pointSound2.play();
	}

	// keeps player two from moving off the board
	if (P2.x > width - 2 || P2.x < 1 || P2.y > height - 2 || P2.y < 1)
		dir2 = 4;
}

// CPU movement
void Tick3()
{
	// keeps CPU from moving off board
	if (CPU.x > width - 2 && dir3 == 2)
		dir3 = 1;

	if (CPU.x < 1 && dir3 == 1)
		dir3 = 2;

	if (CPU.y > height - 2 && dir3 == 0)
		dir3 = 3;

	if (CPU.y < 1 && dir3 == 3)
		dir3 = 0;

	// moves down
	if (dir3 == 0)
		CPU.y += 1;

	// moves left
	if (dir3 == 1)
		CPU.x -= 1;

	// moves right
	if (dir3 == 2)
		CPU.x += 1;

	// moves up
	if (dir3 == 3)
		CPU.y -= 1;

	// adding points to CPU score and remove point entities from board
	if ((CPU.x == Ten1.x) && (CPU.y == Ten1.y))
	{
		score3 += 10;
		Ten1.x = 1000000;
		Ten1.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Ten2.x) && (CPU.y == Ten2.y))
	{
		score3 += 10;
		Ten2.x = 1000000;
		Ten2.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Ten3.x) && (CPU.y == Ten3.y))
	{
		score3 += 10;
		Ten3.x = 1000000;
		Ten3.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Twenty1.x) && (CPU.y == Twenty1.y))
	{
		score3 += 20;
		Twenty1.x = 1000000;
		Twenty1.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Twenty2.x) && (CPU.y == Twenty2.y))
	{
		score3 += 20;
		Twenty2.x = 1000000;
		Twenty2.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Twenty3.x) && (CPU.y == Twenty3.y))
	{
		score3 += 20;
		Twenty3.x = 1000000;
		Twenty3.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Thirty1.x) && (CPU.y == Thirty1.y))
	{
		score3 += 30;
		Thirty1.x = 1000000;
		Thirty1.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Thirty2.x) && (CPU.y == Thirty2.y))
	{
		score3 += 30;
		Thirty2.x = 1000000;
		Thirty2.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Thirty3.x) && (CPU.y == Thirty3.y))
	{
		score3 += 30;
		Thirty3.x = 1000000;
		Thirty3.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Forty1.x) && (CPU.y == Forty1.y))
	{
		score3 += 40;
		Forty1.x = 1000000;
		Forty1.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Forty2.x) && (CPU.y == Forty2.y))
	{
		score3 += 40;
		Forty2.x = 1000000;
		Forty2.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Forty3.x) && (CPU.y == Forty3.y))
	{
		score3 += 40;
		Forty3.x = 1000000;
		Forty3.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Fifty1.x) && (CPU.y == Fifty1.y))
	{
		score3 += 50;
		Fifty1.x = 1000000;
		Fifty1.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Fifty2.x) && (CPU.y == Fifty2.y))
	{
		score3 += 50;
		Fifty2.x = 1000000;
		Fifty2.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
	if ((CPU.x == Fifty3.x) && (CPU.y == Fifty3.y))
	{
		score3 += 50;
		Fifty3.x = 1000000;
		Fifty3.y = 1000000;
		plays--;
		chase = false;
		computerSound.play();
	}
}

// converts int to string
static inline std::string convert(int x)
{
	std::stringstream words;
	words << x;
	return words.str();
}

// sets all entities at random positions
void randomPositions()
{
	// board arrays
	int positionX[30];
	int positionY[20];

	// resets level
	plays = 15;
	score1 = 0;
	score2 = 0;
	score3 = 0;
	countdown = 0;

	// assigns values to x array
	for (int i = 0; i < 30; i++)
	{
		positionX[i] = i;
	}

	// assigns values to y array
	for (int i = 0; i < 20; i++)
	{
		positionY[i] = i;
	}

	// randomizes x array
	for (int i = 0; i < 30; i++)
	{
		int r = rand() % 30;
		int temp = positionX[i];
		positionX[i] = positionX[r];
		positionX[r] = temp;
	}

	// randomizes y array
	for (int i = 0; i < 20; i++)
	{
		int r = rand() % 20;
		int temp = positionY[i];
		positionY[i] = positionY[r];
		positionY[r] = temp;
	}

	// assigns board positions to entities
	Ten1.x = positionX[0];
	Ten1.y = positionY[0];

	Ten2.x = positionX[1];
	Ten2.y = positionY[1];

	Ten3.x = positionX[2];
	Ten3.y = positionY[2];

	Twenty1.x = positionX[3];
	Twenty1.y = positionY[3];

	Twenty2.x = positionX[4];
	Twenty2.y = positionY[4];

	Twenty3.x = positionX[5];
	Twenty3.y = positionY[5];

	P1.x = positionX[6];
	P1.y = positionY[6];

	Thirty1.x = positionX[7];
	Thirty1.y = positionY[7];

	Thirty2.x = positionX[8];
	Thirty2.y = positionY[8];

	Thirty3.x = positionX[9];
	Thirty3.y = positionY[9];

	Forty1.x = positionX[10];
	Forty1.y = positionY[10];

	Forty2.x = positionX[11];
	Forty2.y = positionY[11];

	Forty3.x = positionX[12];
	Forty3.y = positionY[12];

	Fifty1.x = positionX[13];
	Fifty1.y = positionY[13];

	Fifty2.x = positionX[14];
	Fifty2.y = positionY[14];

	Fifty3.x = positionX[15];
	Fifty3.y = positionY[15];

	P2.x = positionX[16];
	P2.y = positionY[16];

	CPU.x = positionX[17];
	CPU.y = positionY[17];
}

// gets the nearest point entity to the CPU
int* getNearestPoint()
{
	int x;
	int y;
	int distance = 100;
	int figure = 0;

	// array for distances from points
	int distances[15];

	// array for coordinates of closest point
	int coords[2];
	coords[0] = 0;
	coords[1] = 0;

	// find distances between points
	distances[0] = (abs(CPU.x - Ten1.x) + abs(CPU.y - Ten1.y));
	distances[1] = (abs(CPU.x - Ten2.x) + abs(CPU.y - Ten2.y));
	distances[2] = (abs(CPU.x - Ten3.x) + abs(CPU.y - Ten3.y));
	distances[3] = (abs(CPU.x - Twenty1.x) + abs(CPU.y - Twenty1.y));
	distances[4] = (abs(CPU.x - Twenty2.x) + abs(CPU.y - Twenty2.y));
	distances[5] = (abs(CPU.x - Twenty3.x) + abs(CPU.y - Twenty3.y));
	distances[6] = (abs(CPU.x - Thirty1.x) + abs(CPU.y - Thirty1.y));
	distances[7] = (abs(CPU.x - Thirty2.x) + abs(CPU.y - Thirty2.y));
	distances[8] = (abs(CPU.x - Thirty3.x) + abs(CPU.y - Thirty3.y));
	distances[9] = (abs(CPU.x - Forty1.x) + abs(CPU.y - Forty1.y));
	distances[10] = (abs(CPU.x - Forty2.x) + abs(CPU.y - Forty2.y));
	distances[11] = (abs(CPU.x - Forty3.x) + abs(CPU.y - Forty3.y));
	distances[12] = (abs(CPU.x - Fifty1.x) + abs(CPU.y - Fifty1.y));
	distances[13] = (abs(CPU.x - Fifty2.x) + abs(CPU.y - Fifty2.y));
	distances[14] = (abs(CPU.x - Fifty3.x) + abs(CPU.y - Fifty3.y));

	// finds the shortest distance
	for (int i = 0; i < 15; i++)
	{
		if (distances[i] < distance)
		{
			distance = distances[i];
			figure = i;
		}
	}

	// assigns coordinats of nearest point to coordinate array
	if (figure == 0)
	{
		coords[0] = Ten1.x;
		coords[1] = Ten1.y;
	}

	if (figure == 1)
	{
		coords[0] = Ten2.x;
		coords[1] = Ten2.y;
	}

	if (figure == 2)
	{
		coords[0] = Ten3.x;
		coords[1] = Ten3.y;
	}

	if (figure == 3)
	{
		coords[0] = Twenty1.x;
		coords[1] = Twenty1.y;
	}

	if (figure == 4)
	{
		coords[0] = Twenty2.x;
		coords[1] = Twenty2.y;
	}

	if (figure == 5)
	{
		coords[0] = Twenty3.x;
		coords[1] = Twenty3.y;
	}

	if (figure == 6)
	{
		coords[0] = Thirty1.x;
		coords[1] = Thirty1.y;
	}

	if (figure == 7)
	{
		coords[0] = Thirty2.x;
		coords[1] = Thirty2.y;
	}

	if (figure == 8)
	{
		coords[0] = Thirty3.x;
		coords[1] = Thirty3.y;
	}

	if (figure == 9)
	{
		coords[0] = Forty1.x;
		coords[1] = Forty1.y;
	}

	if (figure == 10)
	{
		coords[0] = Forty2.x;
		coords[1] = Forty2.y;
	}

	if (figure == 11)
	{
		coords[0] = Forty3.x;
		coords[1] = Forty3.y;
	}

	if (figure == 12)
	{
		coords[0] = Fifty1.x;
		coords[1] = Fifty1.y;
	}

	if (figure == 13)
	{
		coords[0] = Fifty2.x;
		coords[1] = Fifty2.y;
	}

	if (figure == 14)
	{
		coords[0] = Fifty3.x;
		coords[1] = Fifty3.y;
	}

	return coords;
}

// gets the farthest point entity from the CPU
int* getFarthestPoint()
{
	int x;
	int y;
	int distance = 0;
	int figure = 0;

	// array for distances between points
	int distances[15];

	// array for coordinates of farthest point
	int coords[2];
	coords[0] = 0;
	coords[1] = 0;

	// find distances between points
	distances[0] = (abs(CPU.x - Ten1.x) + abs(CPU.y - Ten1.y));
	distances[1] = (abs(CPU.x - Ten2.x) + abs(CPU.y - Ten2.y));
	distances[2] = (abs(CPU.x - Ten3.x) + abs(CPU.y - Ten3.y));
	distances[3] = (abs(CPU.x - Twenty1.x) + abs(CPU.y - Twenty1.y));
	distances[4] = (abs(CPU.x - Twenty2.x) + abs(CPU.y - Twenty2.y));
	distances[5] = (abs(CPU.x - Twenty3.x) + abs(CPU.y - Twenty3.y));
	distances[6] = (abs(CPU.x - Thirty1.x) + abs(CPU.y - Thirty1.y));
	distances[7] = (abs(CPU.x - Thirty2.x) + abs(CPU.y - Thirty2.y));
	distances[8] = (abs(CPU.x - Thirty3.x) + abs(CPU.y - Thirty3.y));
	distances[9] = (abs(CPU.x - Forty1.x) + abs(CPU.y - Forty1.y));
	distances[10] = (abs(CPU.x - Forty2.x) + abs(CPU.y - Forty2.y));
	distances[11] = (abs(CPU.x - Forty3.x) + abs(CPU.y - Forty3.y));
	distances[12] = (abs(CPU.x - Fifty1.x) + abs(CPU.y - Fifty1.y));
	distances[13] = (abs(CPU.x - Fifty2.x) + abs(CPU.y - Fifty2.y));
	distances[14] = (abs(CPU.x - Fifty3.x) + abs(CPU.y - Fifty3.y));

	// finds farthest point
	for (int i = 0; i < 15; i++)
	{
		if (distances[i] < 100 && distances[i] > distance)
		{
			distance = distances[i];
			figure = i;
		}
	}

	// assigns coordinates of farthest point to coordinate array
	if (figure == 0)
	{
		coords[0] = Ten1.x;
		coords[1] = Ten1.y;
	}

	if (figure == 1)
	{
		coords[0] = Ten2.x;
		coords[1] = Ten2.y;
	}

	if (figure == 2)
	{
		coords[0] = Ten3.x;
		coords[1] = Ten3.y;
	}

	if (figure == 3)
	{
		coords[0] = Twenty1.x;
		coords[1] = Twenty1.y;
	}

	if (figure == 4)
	{
		coords[0] = Twenty2.x;
		coords[1] = Twenty2.y;
	}

	if (figure == 5)
	{
		coords[0] = Twenty3.x;
		coords[1] = Twenty3.y;
	}

	if (figure == 6)
	{
		coords[0] = Thirty1.x;
		coords[1] = Thirty1.y;
	}

	if (figure == 7)
	{
		coords[0] = Thirty2.x;
		coords[1] = Thirty2.y;
	}

	if (figure == 8)
	{
		coords[0] = Thirty3.x;
		coords[1] = Thirty3.y;
	}

	if (figure == 9)
	{
		coords[0] = Forty1.x;
		coords[1] = Forty1.y;
	}

	if (figure == 10)
	{
		coords[0] = Forty2.x;
		coords[1] = Forty2.y;
	}

	if (figure == 11)
	{
		coords[0] = Forty3.x;
		coords[1] = Forty3.y;
	}

	if (figure == 12)
	{
		coords[0] = Fifty1.x;
		coords[1] = Fifty1.y;
	}

	if (figure == 13)
	{
		coords[0] = Fifty2.x;
		coords[1] = Fifty2.y;
	}

	if (figure == 14)
	{
		coords[0] = Fifty3.x;
		coords[1] = Fifty3.y;
	}

	return coords;
}

int main()
{
	// rng generator
	srand(time(0));

	bool programEnd = false;
	bool difficulty = false;
	bool easy1 = false;
	bool normal1 = false;
	bool hard1 = false;
	bool pause = false;

	bool gameOver = true;
	bool pvpOver = true;
	bool instructOver = true;
	bool statsOver = true;
	bool skinsOver = true;
	bool endGame = true;
	bool menuMusic = true;
	bool gameMusic = true;
	bool goBeep = true;
	bool addWin = true;
	bool writeData = true;

	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	int cSound = 0;

	// takes in the save file and assigns he values
	std::ifstream dataIn;
	dataIn.open("SaveData.txt");
	dataIn >> easyWins >> normalWins >> hardWins >> skinNum >> musicNum >> soundNum;
	dataIn.close();

	// declares out stream
	std::ofstream dataOut;

	// creates window
	RenderWindow window(VideoMode(w, h + 4 * height), "Collect the Points!", Style::Titlebar | Style::Close);

	createSounds();
	createMusic();
	createImages();
	createTexts();

	// program clock and two music clocks
	Clock clock, menuClock, gameClock;
	float timer = 0.0;
	float delay = 0.1;
	float menuTimer = 0.0;
	float gameTimer = 0.0;

	while (!programEnd)
	{
		float menuTime;
		float gameTime;

		// sets the menu options back to white becaus they turn red if you hover over them with your mouse
		onePlayer.setFillColor(Color::White);
		twoPlayers.setFillColor(Color::White);
		instruct.setFillColor(Color::White);
		stats.setFillColor(Color::White);
		skins.setFillColor(Color::White);
		musicOn.setFillColor(Color::White);
		musicOff.setFillColor(Color::White);
		soundOn.setFillColor(Color::White);
		soundOff.setFillColor(Color::White);

		// plays menu music from where it left off when menu is open or from start when music ends
		while (menuMusic || menu.getStatus() == SoundSource::Status::Stopped || menu.getStatus() == SoundSource::Status::Paused)
		{
			menu.play();
			menuMusic = false;
		}

		// gets how long menu music has been playing in seconds
		if (menu.getStatus() == SoundSource::Status::Playing)
		{
			menuTime = menuClock.getElapsedTime().asSeconds();
			menuClock.restart();
			menuTimer += menuTime;
		}

		// signals to restart menu music if it ends
		if (((int)menuTimer != 0) && ((int)menuTimer % 229) == 0)
		{
			menu.stop();
		}

		// pauses in game music when menu is open
		while (!gameMusic)
		{
			game.pause();
			gameMusic = true;
		}

		// checks for user mouse input
		Event d;
		while (window.pollEvent(d))
		{
			switch (d.type)
			{
			// closes window if red 'x' is clicked
			case Event::Closed:
				window.close();
				return 0;
				break;

			case Event::MouseButtonReleased:
				if (d.mouseButton.button == Mouse::Left)
				{
					// starts single player if clicked
					if (onePlayer.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						chase = false;
						gameOver = false;
						difficulty = false;
					}

					// starts two players if clicked
					if (twoPlayers.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						pvpOver = false;
					}

					// shows instructions if clicked
					if (instruct.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						instructOver = false;
					}

					// shows statistics if clicked
					if (stats.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						statsOver = false;
					}

					// shows skins if clicked
					if (skins.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						skinsOver = false;
					}

					// turns music on and off if clicked
					if (musicVol && musicOn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						musicNum = 1;
						musicVol = false;
						menu.setVolume(0.0);
						game.setVolume(0.0);
						dataOut.open("SaveData.txt");
						dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
						dataOut.close();
					}
					else if (!musicVol && musicOff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						musicNum = 0;
						musicVol = true;
						menu.setVolume(100.0);
						game.setVolume(100.0);
						dataOut.open("SaveData.txt");
						dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
						dataOut.close();
					}

					// turns sound effects on and off if clicked
					if (soundVol && soundOn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						soundNum = 1;
						soundVol = false;
						pointSound.setVolume(0.0);
						pointSound2.setVolume(0.0);
						computerSound.setVolume(0.0);
						countdownSound.setVolume(0.0);
						goSound.setVolume(0.0);
						dataOut.open("SaveData.txt");
						dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum << std::endl << musicNum << std::endl << soundNum;
						dataOut.close();
					}
					else if (!soundVol && soundOff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
					{
						soundNum = 0;
						soundVol = true;
						pointSound.setVolume(100.0);
						pointSound2.setVolume(100.0);
						computerSound.setVolume(100.0);
						countdownSound.setVolume(100.0);
						goSound.setVolume(100.0);
						dataOut.open("SaveData.txt");
						dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum << std::endl << musicNum << std::endl << soundNum;
						dataOut.close();
					}
				}
			}
		}

		// sets random positions for every game
		randomPositions();

		// these turn the option text red if the mouse hovers over them 
		if (onePlayer.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			onePlayer.setFillColor(Color::Red);
		}
		if (twoPlayers.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			twoPlayers.setFillColor(Color::Red);
		}
		if (instruct.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			instruct.setFillColor(Color::Red);
		}
		if (stats.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			stats.setFillColor(Color::Red);
		}
		if (skins.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			skins.setFillColor(Color::Red);
		}
		if (musicOn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			musicOn.setFillColor(Color::Red);
		}
		if (musicOff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			musicOff.setFillColor(Color::Red);
		}
		if (soundOn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			soundOn.setFillColor(Color::Red);
		}
		if (soundOff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			soundOff.setFillColor(Color::Red);
		}

		// window display
		window.clear();

		// sets background
		background.setPosition(0, 0);
		window.draw(background);

		// sets and draws game title
		title.setPosition(10, 60);
		window.draw(title);

		// sets and draws slingle player option
		onePlayer.setPosition(10, 170);
		window.draw(onePlayer);

		// sets and draws two player option
		twoPlayers.setPosition(10, 230);
		window.draw(twoPlayers);

		// sets and draws instructions option
		instruct.setPosition(10, 290);
		window.draw(instruct);

		// sets and draws statistics option
		stats.setPosition(10, 350);
		window.draw(stats);

		// sets and draws skins option
		skins.setPosition(10, 410);
		window.draw(skins);

		// sets and draws music on/off option
		if (musicVol)
		{
			musicOn.setPosition(650, 600);
			window.draw(musicOn);
		}
		else
		{
			musicOff.setPosition(650, 600);
			window.draw(musicOff);
		}

		// sets and draws sound effect on/off option
		if (soundVol)
		{
			soundOn.setPosition(650, 650);
			window.draw(soundOn);
		}
		else
		{
			soundOff.setPosition(650, 650);
			window.draw(soundOff);
		}

		window.display();

		// single player game
		while (!gameOver)
		{
			// difficulty menu
			while (!difficulty)
			{
				easy1 = false;
				normal1 = false;
				hard1 = false;

				// sets the menu options back to white becaus they turn red if you hover over them with your mouse
				easy.setFillColor(Color::White);
				normal.setFillColor(Color::White);
				hard.setFillColor(Color::White);

				// checks for click events
				Event d;
				while (window.pollEvent(d))
				{
					switch (d.type)
					{
					// close window
					case Event::Closed:
						window.close();
						return 0;
						break;
					// click on difficulty
					case Event::MouseButtonReleased:
						if (d.mouseButton.button == Mouse::Left)
						{
							if (easy.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								easy1 = true;
								difficulty = true;
							}

							if (normal.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								normal1 = true;
								difficulty = true;
							}

							if (hard.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								hard1 = true;
								difficulty = true;
							}
						}
					}
				}

				// change text color while hovering over it with mouse
				if (easy.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					easy.setFillColor(Color::Red);
				}
				if (normal.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					normal.setFillColor(Color::Red);
				}
				if (hard.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					hard.setFillColor(Color::Red);
				}

				// draws window
				window.clear();

				// sets background
				background.setPosition(0, 0);
				window.draw(background);

				// sets and draws select difficulty title
				selectDiff.setPosition(10, 60);
				window.draw(selectDiff);

				// sets and draws easy
				easy.setPosition(10, 150);
				window.draw(easy);

				// sets and draws normal
				normal.setPosition(10, 220);
				window.draw(normal);

				// sets and draws hard
				hard.setPosition(10, 290);
				window.draw(hard);

				window.display();
			}

			// pauses menu music when game starts
			menu.pause();
			menuMusic = true;

			// starts game music
			while (gameMusic || game.getStatus() == SoundSource::Status::Stopped || game.getStatus() == SoundSource::Status::Paused)
			{
				game.play();
				gameMusic = false;
			}

			// gets time in game music has been playing
			if (game.getStatus() == SoundSource::Status::Playing)
			{
				gameTime = gameClock.getElapsedTime().asSeconds();
				gameClock.restart();
				gameTimer += gameTime;
			}

			// tell game music to start over if ends
			if (((int)gameTimer != 0) && ((int)gameTimer % 159) == 0)
			{
				game.stop();
			}

			dir = 4;
			dir3 = 4;
			addWin = true;
			writeData = true;

			// when round ends
			if (plays == 0)
			{
				chase = false;
				gameOver = true;
				endGame = false;
			}

			// game clock
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			// searches for while unpaused events
			Event e;
			while (!pause && window.pollEvent(e))
			{
				switch (e.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// pause game
				case Event::KeyReleased:
					if (!pause && (e.key.code == Keyboard::P))
					{
						pause = true;
					}
				}
			}

			// searches for events while paused
			Event r;
			while (pause && window.pollEvent(r))
			{
				switch (r.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// unpause game
				case Event::KeyReleased:
					if (pause && (r.key.code == Keyboard::P))
					{
						pause = false;
					}
				}
			}

			// once game starts
			if (countdown > 30 && !pause)
			{
				// player one movements when keys are pressed
				if (P1.x > 0 && Keyboard::isKeyPressed(Keyboard::A))
					dir = 1;
				if (P1.x < width - 1 && Keyboard::isKeyPressed(Keyboard::D))
					dir = 2;
				if (P1.y > 0 && Keyboard::isKeyPressed(Keyboard::W))
					dir = 3;
				if (P1.y < height - 1 && Keyboard::isKeyPressed(Keyboard::S))
					dir = 0;

				// easy mode CPU movements
				if (easy1)
				{
					// goes for farthest point if more than 8 point entities on board and player score is less than 120
					if (plays > 8 && plays < 15 && score1 < 120)
					{
						if (!chase)
						{
							x1 = getFarthestPoint()[0];
							y1 = getFarthestPoint()[1];
							chase = true;
						}

						x = CPU.x - x1;
						y = CPU.y - y1;

						if (x < 0)
							dir3 = 2;
						else if (x > 0)
							dir3 = 1;
						else if (y < 0)
							dir3 = 0;
						else if (y > 0)
							dir3 = 3;
					}
					// ges for nearest point
					else
					{
						x = CPU.x - getNearestPoint()[0];
						y = CPU.y - getNearestPoint()[1];

						if (x < 0)
							dir3 = 2;
						else if (x > 0)
							dir3 = 1;
						else if (y < 0)
							dir3 = 0;
						else if (y > 0)
							dir3 = 3;
					}
				}

				// normal mode CPU movements
				else if (normal1)
				{
					// goes for farthest point if more than 10 point entities on board and player score is less than 100
					if (plays > 10 && plays < 15 && score1 < 100)
					{
						if (!chase)
						{
							x1 = getFarthestPoint()[0];
							y1 = getFarthestPoint()[1];
							chase = true;
						}

						x = CPU.x - x1;
						y = CPU.y - y1;

						if (x < 0)
							dir3 = 2;
						else if (x > 0)
							dir3 = 1;
						else if (y < 0)
							dir3 = 0;
						else if (y > 0)
							dir3 = 3;
					}
					// goes for nearest point
					else
					{
						x = CPU.x - getNearestPoint()[0];
						y = CPU.y - getNearestPoint()[1];

						if (x < 0)
							dir3 = 2;
						else if (x > 0)
							dir3 = 1;
						else if (y < 0)
							dir3 = 0;
						else if (y > 0)
							dir3 = 3;
					}
				}
				
				// hard mode CPU movements (always goes to nearest point)
				else if (hard1)
				{
					x = CPU.x - getNearestPoint()[0];
					y = CPU.y - getNearestPoint()[1];

					if (x < 0)
						dir3 = 2;
					else if (x > 0)
						dir3 = 1;
					else if (y < 0)
						dir3 = 0;
					else if (y > 0)
						dir3 = 3;
				}
			}

			// exit to main menu if backspace is pressed
			if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				gameOver = true;

			// calls the actual movement functions
			if (!pause && timer > delay)
			{
				timer = 0;
				Tick();
				Tick3();
				countdown++;
			}

			// draw
			window.clear();

			// sets background
			background.setPosition(0, 0);
			window.draw(background);

			// draws out board
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					whiteSprite.setPosition(i * size, j * size);
					window.draw(whiteSprite);
				}
			}

			// sets positions and draws out the players and point entities
			player1Sprite.setPosition(P1.x * size, P1.y * size);
			window.draw(player1Sprite);

			greySprite.setPosition(Ten1.x * size, Ten1.y * size);
			window.draw(greySprite);

			greySprite.setPosition(Ten2.x * size, Ten2.y * size);
			window.draw(greySprite);

			greySprite.setPosition(Ten3.x * size, Ten3.y * size);
			window.draw(greySprite);

			greenSprite.setPosition(Twenty1.x * size, Twenty1.y * size);
			window.draw(greenSprite);

			greenSprite.setPosition(Twenty2.x * size, Twenty2.y * size);
			window.draw(greenSprite);

			greenSprite.setPosition(Twenty3.x * size, Twenty3.y * size);
			window.draw(greenSprite);

			blueSprite.setPosition(Thirty1.x * size, Thirty1.y * size);
			window.draw(blueSprite);

			blueSprite.setPosition(Thirty2.x * size, Thirty2.y * size);
			window.draw(blueSprite);

			blueSprite.setPosition(Thirty3.x * size, Thirty3.y * size);
			window.draw(blueSprite);

			magentaSprite.setPosition(Forty1.x * size, Forty1.y * size);
			window.draw(magentaSprite);

			magentaSprite.setPosition(Forty2.x * size, Forty2.y * size);
			window.draw(magentaSprite);

			magentaSprite.setPosition(Forty3.x * size, Forty3.y * size);
			window.draw(magentaSprite);

			yellowSprite.setPosition(Fifty1.x * size, Fifty1.y * size);
			window.draw(yellowSprite);

			yellowSprite.setPosition(Fifty2.x * size, Fifty2.y * size);
			window.draw(yellowSprite);

			yellowSprite.setPosition(Fifty3.x * size, Fifty3.y * size);
			window.draw(yellowSprite);

			computerSprite.setPosition(CPU.x * size, CPU.y * size);
			window.draw(computerSprite);

			// countdown-prints 3
			if (!pause && countdown < 10)
			{
				three.setPosition(400, 120);
				window.draw(three);
				if (cSound == 0)
				{
					countdownSound.play();
					cSound++;
				}
			}

			// countdown-prints 2
			if (!pause && countdown > 10 && countdown < 20)
			{
				two.setPosition(400, 120);
				window.draw(two);
				if (cSound == 1)
				{
					countdownSound.play();
					cSound++;
				}
			}

			// countdown-prints 1
			if (!pause && countdown > 20 && countdown < 30)
			{
				one.setPosition(400, 120);
				window.draw(one);
				if (cSound == 2)
				{
					countdownSound.play();
					cSound = 0;
					goBeep = true;
				}
			}

			// countdown-prints Go
			if (!pause && countdown > 30 && countdown < 40)
			{
				go.setPosition(200, 120);
				window.draw(go);
				if (goBeep)
				{
					goSound.play();
					goBeep = false;
				}
			}

			// pause text
			if (pause)
			{
				pauseText.setPosition(100, 100);
				window.draw(pauseText);
			}

			// prints current score underneath board
			text.setString("Player Score: " + convert(score1) + "          CPU Score: " + convert(score3));
			text.setPosition(10, h);
			window.draw(text);

			window.display();

			// end game screen
			while (!endGame)
			{
				// sets options back to white because they turn red when you hover over them
				playAgain.setFillColor(Color::White);
				changeDiff.setFillColor(Color::White);
				backToMenu.setFillColor(Color::White);

				// searches for mouse click events
				Event p;
				while (window.pollEvent(p))
				{
					switch (p.type)
					{
					// close window
					case Event::Closed:
						window.close();
						return 0;
						break;
					// checks for play again, change difficulty, or back to menu
					case Event::MouseButtonReleased:
						if (p.mouseButton.button == Mouse::Left)
						{
							if (playAgain.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								chase = false;
								gameOver = false;
								endGame = true;
								addWin = true;
								writeData = true;

								plays = 15;
								randomPositions();
							}

							if (changeDiff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								difficulty = false;
								gameOver = false;
								endGame = true;
								addWin = true;
								writeData = true;

								plays = 15;
								randomPositions();
							}

							if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								difficulty = false;
								gameOver = true;
								endGame = true;
							}
						}
					}
				}

				window.clear();

				// sets background
				background.setPosition(0, 0);
				window.draw(background);

				// if player 1 wins
				if (score1 > score3)
				{
					// draws win text
					winP.setPosition(10, 60);
					window.draw(winP);

					// adds win to whichever difficulty was played on
					if (addWin && easy1)
					{
						easyWins++;
						addWin = false;
					}

					if (addWin && normal1)
					{
						normalWins++;
						addWin = false;
					}

					if (addWin && hard1)
					{
						hardWins++;
						addWin = false;
					}

					// adds new win to strings
					easyW.setString("Easy Wins: " + convert(easyWins));
					normalW.setString("Normal Wins: " + convert(normalWins));
					hardW.setString("Hard Wins: " + convert(hardWins));

					// saves data to savefile
					if (writeData)
					{
						dataOut.open("SaveData.txt");
						dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
						writeData = false;
						dataOut.close();
					}
				}

				// lose screen if lose
				else
				{
					loseP.setPosition(10, 60);
					window.draw(loseP);
				}

				// turns menu options red when hover over them
				if (playAgain.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					playAgain.setFillColor(Color::Red);
				}

				if (changeDiff.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					changeDiff.setFillColor(Color::Red);
				}

				if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					backToMenu.setFillColor(Color::Red);
				}

				// draws menu options
				playAgain.setPosition(10, 150);
				window.draw(playAgain);

				changeDiff.setPosition(10, 210);
				window.draw(changeDiff);

				backToMenu.setPosition(10, 270);
				window.draw(backToMenu);

				// draws end score
				text.setString("Player Score: " + convert(score1) + "          CPU Score: " + convert(score3));
				text.setPosition(10, h);
				window.draw(text);

				window.display();
			}
		}

		// two player game
		while (!pvpOver)
		{
			// pause menu music
			menu.pause();
			menuMusic = true;

			// play game music
			while (gameMusic || game.getStatus() == SoundSource::Status::Stopped || game.getStatus() == SoundSource::Status::Paused)
			{
				game.play();
				gameMusic = false;
			}

			// game music clock
			if (game.getStatus() == SoundSource::Status::Playing)
			{
				gameTime = gameClock.getElapsedTime().asSeconds();
				gameClock.restart();
				gameTimer += gameTime;
			}

			// stops and restarts music when it ends
			if (((int)gameTimer != 0) && ((int)gameTimer % 159) == 0)
			{
				game.stop();
			}

			dir = 4;
			dir2 = 4;

			// ends game when all points are collected
			if (plays == 0)
			{
				pvpOver = true;
				endGame = false;
			}

			// game clock
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			// searches for events when unpaused
			Event e;
			while (!pause && window.pollEvent(e))
			{
				switch (e.type)
				{
				// cose window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// pause game
				case Event::KeyReleased:
					if (!pause && (e.key.code == Keyboard::P))
					{
						pause = true;
					}
				}
			}

			// searches for events when paused
			Event r;
			while (pause && window.pollEvent(r))
			{
				switch (r.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// unpause game
				case Event::KeyReleased:
					if (pause && (r.key.code == Keyboard::P))
					{
						pause = false;
					}
				}
			}

			// when game starts
			if (!pause && countdown > 30)
			{
				// gets player one key presses
				if (P1.x > 0 && Keyboard::isKeyPressed(Keyboard::A))
					dir = 1;
				if (P1.x < width - 1 && Keyboard::isKeyPressed(Keyboard::D))
					dir = 2;
				if (P1.y > 0 && Keyboard::isKeyPressed(Keyboard::W))
					dir = 3;
				if (P1.y < height - 1 && Keyboard::isKeyPressed(Keyboard::S))
					dir = 0;

				// gets player two key presses
				if (P2.x > 0 && Keyboard::isKeyPressed(Keyboard::Left))
					dir2 = 1;
				if (P2.x < width - 1 && Keyboard::isKeyPressed(Keyboard::Right))
					dir2 = 2;
				if (P2.y > 0 && Keyboard::isKeyPressed(Keyboard::Up))
					dir2 = 3;
				if (P2.y < height - 1 && Keyboard::isKeyPressed(Keyboard::Down))
					dir2 = 0;
			}

			// exit to menu by presing backspace
			if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				pvpOver = true;

			// actual call for player movements
			if (!pause && timer > delay)
			{
				timer = 0;
				Tick();
				Tick2();
				countdown++;
			}

			// draw
			window.clear();

			// sets background
			background.setPosition(0, 0);
			window.draw(background);

			// draws game board
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					whiteSprite.setPosition(i * size, j * size);
					window.draw(whiteSprite);
				}
			}

			// set positions and draws player and point entities
			player1Sprite.setPosition(P1.x * size, P1.y * size);
			window.draw(player1Sprite);

			greySprite.setPosition(Ten1.x * size, Ten1.y * size);
			window.draw(greySprite);

			greySprite.setPosition(Ten2.x * size, Ten2.y * size);
			window.draw(greySprite);

			greySprite.setPosition(Ten3.x * size, Ten3.y * size);
			window.draw(greySprite);

			greenSprite.setPosition(Twenty1.x * size, Twenty1.y * size);
			window.draw(greenSprite);

			greenSprite.setPosition(Twenty2.x * size, Twenty2.y * size);
			window.draw(greenSprite);

			greenSprite.setPosition(Twenty3.x * size, Twenty3.y * size);
			window.draw(greenSprite);

			blueSprite.setPosition(Thirty1.x * size, Thirty1.y * size);
			window.draw(blueSprite);

			blueSprite.setPosition(Thirty2.x * size, Thirty2.y * size);
			window.draw(blueSprite);

			blueSprite.setPosition(Thirty3.x * size, Thirty3.y * size);
			window.draw(blueSprite);

			magentaSprite.setPosition(Forty1.x * size, Forty1.y * size);
			window.draw(magentaSprite);

			magentaSprite.setPosition(Forty2.x * size, Forty2.y * size);
			window.draw(magentaSprite);

			magentaSprite.setPosition(Forty3.x * size, Forty3.y * size);
			window.draw(magentaSprite);

			yellowSprite.setPosition(Fifty1.x * size, Fifty1.y * size);
			window.draw(yellowSprite);

			yellowSprite.setPosition(Fifty2.x * size, Fifty2.y * size);
			window.draw(yellowSprite);

			yellowSprite.setPosition(Fifty3.x * size, Fifty3.y * size);
			window.draw(yellowSprite);

			player2Sprite.setPosition(P2.x * size, P2.y * size);
			window.draw(player2Sprite);

			// countdown-prints 3
			if (!pause && countdown < 10)
			{
				three.setPosition(400, 120);
				window.draw(three);
				if (cSound == 0)
				{
					countdownSound.play();
					cSound++;
				}
			}

			// countdown-prints 2
			if (!pause && countdown > 10 && countdown < 20)
			{
				two.setPosition(400, 120);
				window.draw(two);
				if (cSound == 1)
				{
					countdownSound.play();
					cSound++;
				}
			}

			// countdown-prints 1
			if (!pause && countdown > 20 && countdown < 30)
			{
				one.setPosition(400, 120);
				window.draw(one);
				if (cSound == 2)
				{
					countdownSound.play();
					cSound = 0;
					goBeep = true;
				}
			}

			// countdown-prints Go
			if (!pause && countdown > 30 && countdown < 40)
			{
				go.setPosition(200, 120);
				window.draw(go);
				if (goBeep)
				{
					goSound.play();
					goBeep = false;
				}
			}

			// pause screen
			if (pause)
			{
				pauseText.setPosition(100, 100);
				window.draw(pauseText);
			}

			// prints players scores underneath the board
			text.setString("Player 1 Score: " + convert(score1) + "          Player 2 Score: " + convert(score2));
			text.setPosition(10, h);
			window.draw(text);

			window.display();

			// when game ends
			while (!endGame)
			{
				// returns menu options to white because they turn red when you hover over them
				playAgain.setFillColor(Color::White);
				backToMenu.setFillColor(Color::White);

				// searches for click event
				Event p;
				while (window.pollEvent(p))
				{
					switch (p.type)
					{
					// close window
					case Event::Closed:
						window.close();
						return 0;
						break;
					// click on play agin or return to menu
					case Event::MouseButtonReleased:
						if (p.mouseButton.button == Mouse::Left)
						{
							if (playAgain.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								pvpOver = false;
								endGame = true;

								plays = 15;
								randomPositions();
							}

							if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
							{
								pvpOver = true;
								endGame = true;
							}
						}
					}
				}

				window.clear();

				// sets background
				background.setPosition(0, 0);
				window.draw(background);

				// draws player 1 win screen
				if (score1 > score2)
				{
					winP1.setPosition(10, 60);
					window.draw(winP1);
				}

				// draws player 2 win screen
				if (score1 < score2)
				{
					winP2.setPosition(10, 60);
					window.draw(winP2);
				}

				// turns menu options red when you hover over them
				if (playAgain.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					playAgain.setFillColor(Color::Red);
				}

				if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					backToMenu.setFillColor(Color::Red);
				}

				// draws options
				playAgain.setPosition(10, 150);
				window.draw(playAgain);

				backToMenu.setPosition(10, 210);
				window.draw(backToMenu);

				// draws final score
				text.setString("Player 1 Score: " + convert(score1) + "          Player 2 Score: " + convert(score2));
				text.setPosition(10, h);
				window.draw(text);

				window.display();
			}
		}

		// instructions
		while (!instructOver)
		{
			// turns option back to white becuase it turns red when you hover over it
			backToMenu.setFillColor(Color::White);

			// looks for click event
			Event e;
			while (window.pollEvent(e))
			{
				switch (e.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// click back to menu
				case Event::MouseButtonReleased:
					if (e.mouseButton.button == Mouse::Left)
					{
						if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							instructOver = true;
						}
					}
				}
			}

			window.clear();

			// sets background
			background.setPosition(0, 0);
			window.draw(background);
			
			if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				backToMenu.setFillColor(Color::Red);
			}

			instructTextS.setPosition(10, 60);
			window.draw(instructTextS);

			instructTextSU.setPosition(10, 140);
			window.draw(instructTextSU);

			instructTextT.setPosition(10, 360);
			window.draw(instructTextT);

			instructTextTU.setPosition(10, 440);
			window.draw(instructTextTU);

			backToMenu.setPosition(10, 600);
			window.draw(backToMenu);

			window.display();
		}

		// statistics
		while (!statsOver)
		{
			// turns options back to white because they turn red when you have over them
			clearStats.setFillColor(Color::White);
			backToMenu.setFillColor(Color::White);

			// checks for click events
			Event e;
			while (window.pollEvent(e))
			{
				switch (e.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// cneck for return to menu or clear statistics
				case Event::MouseButtonReleased:
					if (e.mouseButton.button == Mouse::Left)
					{
						if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							statsOver = true;
						}

						// wipes player data
						if (clearStats.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							easyWins = 0;
							normalWins = 0;
							hardWins = 0;

							easyW.setString("Easy Wins: " + convert(easyWins));
							normalW.setString("Normal Wins: " + convert(normalWins));
							hardW.setString("Hard Wins: " + convert(hardWins));

							player1Sprite.setTexture(player1);

							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}
					}
				}
			}

			window.clear();

			// sets background
			background.setPosition(0, 0);
			window.draw(background);

			// turns options red when you hover over them
			if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				backToMenu.setFillColor(Color::Red);
			}

			if (clearStats.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				clearStats.setFillColor(Color::Red);
			}

			// draws out how many wins per mode and clear stats and back to menu options
			easyW.setPosition(10, 60);
			window.draw(easyW);

			normalW.setPosition(10, 170);
			window.draw(normalW);

			hardW.setPosition(10, 280);
			window.draw(hardW);

			clearStats.setPosition(10, 390);
			window.draw(clearStats);

			backToMenu.setPosition(10, 600);
			window.draw(backToMenu);

			window.display();
		}

		// skins
		while (!skinsOver)
		{
			// turns options back to white because they turn red when you hover over them
			clearStats.setFillColor(Color::White);
			backToMenu.setFillColor(Color::White);

			// checks for click events
			Event e;
			while (window.pollEvent(e))
			{
				switch (e.type)
				{
				// close window
				case Event::Closed:
					window.close();
					return 0;
					break;
				// click to change skins or back to menu
				case Event::MouseButtonReleased:
					if (e.mouseButton.button == Mouse::Left)
					{
						// back to menu
						if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinsOver = true;
						}

						// default skin
						if (skin1.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 0;
							player1Sprite.setTexture(player1);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 2
						if (easyWins > 0 && skin2.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 1;
							player1Sprite.setTexture(lava);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 3
						if (normalWins > 0 && skin3.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 2;
							player1Sprite.setTexture(boogers);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 4
						if (hardWins > 0 && skin4.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 3;
							player1Sprite.setTexture(sun);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 5
						if (easyWins > 14 && skin5.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 4;
							player1Sprite.setTexture(lavaBlue);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 6
						if (normalWins > 9 && skin6.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 5;
							player1Sprite.setTexture(boogersBloody);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 7
						if (hardWins > 4 && skin7.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 6;
							player1Sprite.setTexture(sunDark);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}

						// skin 8
						if (easyWins > 14 && normalWins > 9 && hardWins > 4 && skin8.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							skinNum = 7;
							player1Sprite.setTexture(smiley);
							dataOut.open("SaveData.txt");
							dataOut << easyWins << std::endl << normalWins << std::endl << hardWins << std::endl << skinNum << std::endl << musicNum << std::endl << soundNum;
							dataOut.close();
						}
					}
				}
			}

			window.clear();

			// sets background
			background.setPosition(0, 0);
			window.draw(background);

			// turns option red or show skin description when you hover over them
			if (backToMenu.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				backToMenu.setFillColor(Color::Red);
			}

			if (skin1.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin1Text.setPosition(10, 300);
				window.draw(skin1Text);
			}

			if (skin2.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin2Text.setPosition(10, 300);
				window.draw(skin2Text);
			}

			if(skin3.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin3Text.setPosition(10, 300);
				window.draw(skin3Text);
			}

			if (skin4.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin4Text.setPosition(10, 300);
				window.draw(skin4Text);
			}

			if (skin5.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin5Text.setPosition(10, 300);
				window.draw(skin5Text);
			}

			if (skin6.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin6Text.setPosition(10, 300);
				window.draw(skin6Text);
			}

			if (skin7.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin7Text.setPosition(10, 300);
				window.draw(skin7Text);
			}

			if (skin8.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				skin8Text.setPosition(10, 300);
				window.draw(skin8Text);
			}

			// draws current skin and all the other skins
			currSkin.setPosition(10, 60);
			window.draw(currSkin);

			player1Sprite.setPosition(350, 90);
			window.draw(player1Sprite);

			skin1.setPosition(45, 200);
			window.draw(skin1);

			skin2.setPosition(165, 200);
			window.draw(skin2);

			skin3.setPosition(285, 200);
			window.draw(skin3);

			skin4.setPosition(405, 200);
			window.draw(skin4);

			skin5.setPosition(525, 200);
			window.draw(skin5);

			skin6.setPosition(645, 200);
			window.draw(skin6);

			skin7.setPosition(765, 200);
			window.draw(skin7);

			skin8.setPosition(885, 200);
			window.draw(skin8);

			backToMenu.setPosition(10, 600);
			window.draw(backToMenu);

			window.display();
		}
	}
	return 0;
}