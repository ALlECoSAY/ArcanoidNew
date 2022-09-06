#include "Framework.h"
#include <iostream>
#include <list>
#include <string> 
#include <ctime>
#include <cstdlib>

#include "IShape.h"
#include "CollisionManager.h"
#include "Shared.h"
#include "IEffect.h"

using namespace std;




namespace input {
	int WINDOW_WIDTH,
		WINDOW_HEIGHT;


	bool
		arrowKeyRightPressed = false,
		arrowKeyLeftPressed = false,
		leftMouseButtonPressed = false;
	Vec2<int>
		mousePos = Vec2<int>(0, 0);
}
namespace options {
	bool 
		ballResting = true;
	Vec2<int>
		playgroundStartPosition = Vec2<int>(0, 0);
	int
		ballHealthDeafault = 3,

		points = 0,
		reward = 20,

		playgroundWidth,
		playgroundHeight,
		
		spriteMouseWidth = 15,
		spriteMouseHeight = 15,

		spriteTrampolineWidth = 120,
		spriteTrampolineHeight = 30,

		spriteTileWidth = 50,
		spriteTileHeight = 20;

	float
		trampolineVelocity = 0.5,
		ballVelocity = trampolineVelocity * 1.6;
}

using namespace input;
using namespace options;


class MyFramework : public Framework {
	bool savingWallActive = false;

	unsigned int
		tickTime = 0,
		deltaTime = 0;

	float
		trampolineX = playgroundWidth / 2,
		trampolineY = playgroundHeight - spriteTrampolineHeight/2;
		
	Vec2<float>
		trampolinePos = Vec2<float>(trampolineX, trampolineY),
		ballPos = Vec2<float>(trampolineX, trampolineY - spriteTrampolineHeight),
		ballVelocityDirection = Vec2<float>(0, 0),
		trampolineVelocityDirection = Vec2<float>(0, 0);

	Sprite
		* mouseCursoreSprite,
		* digit0, * digit1, * digit2, * digit3, * digit4, * digit5, * digit6, * digit7, * digit8, * digit9, * digitX,
		* trampolineSprite, * trampolineSprite2, * trampolineSprite3, * trampolineSprite4,
		* rightWallSprite,
		* ballSprite,
		* backgroundSprite,
		* infoBackgroundSprite,
		* heartSprite,
		* savingWallSprite,
		* smallerSizeSprite,
		* biggerSizeSprite,
		* fastSprite,
		* slowSprite;

	RectangleShape	
		*trampolin,
		*savingWall;
	CircleShape *ball;
	
	list<RectangleShape*> tileList;

	IEffect 
		*areaVelocityEffect,
		*areaVelocityPurgeEffect;

	list<ITemporalEffect*> temporalEffects;
	

public:

	//only responsible for width/height/fullscreen
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;
		fullscreen = false;
	}

	virtual bool Init() {
		//digits
		digit0 = createSprite(".\\data\\0-Number-PNG.png"); setSpriteSize(digit0, 28, 40);
		digit1 = createSprite(".\\data\\1-Number-PNG.png"); setSpriteSize(digit1, 40, 40);
		digit2 = createSprite(".\\data\\2-Number-PNG.png"); setSpriteSize(digit2, 40, 40);
		digit3 = createSprite(".\\data\\3-Number-PNG.png"); setSpriteSize(digit3, 40, 40);
		digit4 = createSprite(".\\data\\4-Number-PNG.png"); setSpriteSize(digit4, 40, 40);
		digit5 = createSprite(".\\data\\5-Number-PNG.png"); setSpriteSize(digit5, 40, 40);
		digit6 = createSprite(".\\data\\6-Number-PNG.png"); setSpriteSize(digit6, 40, 40);
		digit7 = createSprite(".\\data\\7-Number-PNG.png"); setSpriteSize(digit7, 40, 40);
		digit8 = createSprite(".\\data\\8-Number-PNG.png"); setSpriteSize(digit8, 40, 40);
		digit9 = createSprite(".\\data\\9-Number-PNG.png"); setSpriteSize(digit9, 40, 40);
		digitX = createSprite(".\\data\\gold-letter-X-T.png"); setSpriteSize(digitX, 40, 40);
		
		//effects info sprites
		smallerSizeSprite = createSprite(".\\data\\46-Breakout-Tiles.png"); setSpriteSize(smallerSizeSprite, 100, 40);
		biggerSizeSprite = createSprite(".\\data\\47-Breakout-Tiles.png"); setSpriteSize(biggerSizeSprite, 100, 40);
		fastSprite = createSprite(".\\data\\42-Breakout-Tiles.png"); setSpriteSize(fastSprite, 100, 40);
		slowSprite = createSprite(".\\data\\41-Breakout-Tiles.png"); setSpriteSize(slowSprite, 100, 40);


		//background
		backgroundSprite = createSprite(".\\data\\blackBackground.png");
		setSpriteSize(backgroundSprite, WINDOW_WIDTH, WINDOW_HEIGHT);

		//hearts health view
		heartSprite = createSprite(".\\data\\60-Breakout-Tiles.png");
		setSpriteSize(heartSprite, 50, 50);

		//infobackground (right section)
		infoBackgroundSprite = createSprite(".\\data\\29-Breakout-Tiles.png");
		setSpriteSize(infoBackgroundSprite, WINDOW_WIDTH-playgroundWidth, WINDOW_HEIGHT*2);

		//cursor
		showCursor(false);
		mouseCursoreSprite = createSprite(".\\data\\green-circle.png");
		setSpriteSize(mouseCursoreSprite, spriteMouseWidth, spriteMouseHeight);

		//trampoline (platform)
		trampolineSprite = createSprite(".\\data\\50-Breakout-Tiles.png");
		trampolineSprite2 = createSprite(".\\data\\51-Breakout-Tiles.png");
		trampolineSprite3 = createSprite(".\\data\\52-Breakout-Tiles.png");
		trampolin = new Trampoline(trampolinePos, trampolineVelocity, trampolineVelocityDirection, spriteTrampolineWidth, spriteTrampolineHeight, trampolineSprite);
		
		//ball
		ballSprite = createSprite(".\\data\\58-Breakout-Tiles.png");
		ball = new BallCircle(ballPos , ballVelocity, ballVelocityDirection, 6, ballSprite);

		//tiles
		int ds = playgroundWidth / spriteTileWidth;
		for (int j = 0; j < 5; j+=2)
		{
			for (int i = 0; i < ds; i++) {
				int random = 1 + (rand() % 20);
				string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
				tileList.push_back(new Tile(Vec2<float>(spriteTileWidth / 2 + i * spriteTileWidth, 100 + j * spriteTileHeight), 0, Vec2<float>(0, 0), spriteTileWidth, spriteTileHeight, createSprite(s.c_str())));
			}
			for (int i = 0; i < ds/2; i++) {
				int random = 1 + (rand() % 20);
				string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
				tileList.push_back(new Tile(Vec2<float>(spriteTileWidth / 2 + 2*i * spriteTileWidth, 100 + (j+1) * spriteTileHeight), 0, Vec2<float>(0, 0), spriteTileWidth, spriteTileHeight, createSprite(s.c_str())));
			}
		}

		//savingWall
		savingWallSprite = createSprite(".\\data\\56-Breakout-Tiles.png");
		savingWall = new Tile(Vec2<float>(playgroundWidth/2,-1000),0,Vec2<float>(0,0),WINDOW_WIDTH,100, savingWallSprite);
		
		
		//permament effects
		list<Vec2<float>> dots; //map 1.5, 0.5 ball speed modifiers
		dots.push_back(Vec2<float>(100 + (rand() % (playgroundWidth - 100)), 100 + (rand() % (playgroundHeight - 100))));
		dots.push_back(Vec2<float>(100 + (rand() % (playgroundWidth - 100)), 100 + (rand() % (playgroundHeight - 100))));

		list<Vec2<float>> dotsPurge; //overlapping map of 1 ball speed
		dotsPurge.push_back(Vec2<float>(100 + (rand() % (playgroundWidth - 100)), 100 + (rand() % (playgroundHeight - 100))));
		dotsPurge.push_back(Vec2<float>(100 + (rand() % (playgroundWidth - 100)), 100 + (rand() % (playgroundHeight - 100))));

		areaVelocityEffect = new AreaVelocityEffect(dots, ball);
		areaVelocityPurgeEffect = new AreaVelocityEffect(dotsPurge, ball);
		

		return true;
	}

	//onWindowClose
	virtual void Close() {
		ballResting = true;
	}

	//rendering ticks
	virtual bool Tick() {
		//deltatime calculation
		deltaTime = tickTime;
		tickTime = getTickCount();
		deltaTime = tickTime - deltaTime;
		if (deltaTime > 100) { 
			deltaTime = 20; 
			arrowKeyLeftPressed = false;
			arrowKeyRightPressed = false;
			trampolin->_velocityDirecrion.x = 0;
		}

		drawBackground();
		updateTrampoline();

		//applies change ball speed effect on some areas
		areaVelocityEffect->Apply();
		areaVelocityPurgeEffect->Purge();

		//loose condition
		if (updateBall() || ballHealthDeafault==0) {
			cout << "\nYOU LOST";
			return true;
		}
		//win condition
		if (updateTiles()) {
			cout << "\nYOU WON";
			return true;
		}
	
		updateTemporalEffects();
		updateHP();
		updatePoints();
		updateEffectsView();
		updateCursor();

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		mousePos.x = x;
		mousePos.y = y;

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

		if (button == FRMouseButton::LEFT) leftMouseButtonPressed = !isReleased;
		if (button == FRMouseButton::RIGHT) {
			if (!isReleased) buyEffect();
		}
		
	}

	virtual void onKeyPressed(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			arrowKeyRightPressed = true;
			break;
		case FRKey::LEFT:
			arrowKeyLeftPressed = true;
			break;
		case FRKey::DOWN:
			break;
		case FRKey::UP:
			break;
		case FRKey::COUNT:
			break;
		default:
			break;
		}
	}

	virtual void onKeyReleased(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			arrowKeyRightPressed = false;
			if(!arrowKeyLeftPressed) trampolin->SetDirection(Vec2<float>(0, 0));
			break;
		case FRKey::LEFT:
			arrowKeyLeftPressed = false;
			if (!arrowKeyRightPressed) trampolin->SetDirection(Vec2<float>(0, 0));
			break;
		case FRKey::DOWN:
			break;
		case FRKey::UP:
			break;
		case FRKey::COUNT:
			break;
		default:
			break;
		}
	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}

private:
	void updateTrampoline() {
		switch (deltaTime%3)
		{
			case 0:
				trampolin->_sprite = trampolineSprite;
				break;
			case 1:
				trampolin->_sprite = trampolineSprite2;
				break;
			case 2:
				trampolin->_sprite = trampolineSprite3;
				break;
			default:
				break;
		}
		trampolin->OnEffectChange();

		if (arrowKeyRightPressed && !arrowKeyLeftPressed) {
			trampolin->SetDirection(Vec2<float>(1, 0));
		}
		if (!arrowKeyRightPressed && arrowKeyLeftPressed)
		{
			trampolin->SetDirection(Vec2<float>(-1, 0));
		}
		trampolin->Update(deltaTime);
	}
	void updateCursor() {
		drawSprite(mouseCursoreSprite, mousePos.x - spriteMouseWidth / 2, mousePos.y - spriteMouseHeight / 2);
	}

	bool updateBall() {
		Vec2<float> initialPos = ball->_position;
		Vec2<float> delta = Vec2<float>(1000000, 1000000);
		
		//if ball is recently damaged or the start of level
		if (ballResting) {
			if (leftMouseButtonPressed) {
				ball->OnCollide(VERTICAL, TRAMPOLIN, playgroundHeight - trampolin->_height - ball->_radius);
				ballResting = false;
			}
			ball->_position.x = trampolin->_position.x;
			ball->_position.y = trampolin->_position.y - trampolin->_height/2 - ball->_radius;
			ball->Update(deltaTime);
			return false;
		}

		//loosecondition
		if (!ball->Update(deltaTime)) {
			return true;
		}

		//trampolin collision check
		CollisionType t = CollisionManager::areColliding(ball, trampolin);

		if (t != NONE) {
			float adhereBorderCoordinate;
			switch (t) {
			case HORIZONTAL:
				if (ball->_position.x < trampolin->_position.x) adhereBorderCoordinate = trampolin->_position.x - trampolin->_width / 2 - ball->_radius;
				else adhereBorderCoordinate = trampolin->_position.x + trampolin->_width / 2 + ball->_radius;

				break;
			case VERTICAL:

				if (ball->_position.y < trampolin->_position.y) adhereBorderCoordinate = trampolin->_position.y - trampolin->_height / 2 - ball->_radius;
				else adhereBorderCoordinate = trampolin->_position.y + trampolin->_height / 2 + ball->_radius;

				break;
			case NONE:
				adhereBorderCoordinate = -1;
			default:
				break;
			}
			ball->OnCollide(t, TRAMPOLIN, adhereBorderCoordinate);
			trampolin->OnCollide(t, BALL, -1);
			return false;
		}
		
		//savingWall collision check
		CollisionType td = CollisionManager::areColliding(ball, savingWall);
		if (td != NONE) {
			float adhereBorderCoordinate;
			switch (td) {
			case HORIZONTAL:
				if (ball->_position.x < savingWall->_position.x) adhereBorderCoordinate = savingWall->_position.x - savingWall->_width / 2 - ball->_radius;
				else adhereBorderCoordinate = savingWall->_position.x + savingWall->_width / 2 + ball->_radius;

				break;
			case VERTICAL:

				if (ball->_position.y < savingWall->_position.y) adhereBorderCoordinate = savingWall->_position.y - savingWall->_height / 2 - ball->_radius;
				else adhereBorderCoordinate = savingWall->_position.y + savingWall->_height / 2 + ball->_radius;

				break;
			case NONE:
				adhereBorderCoordinate = -1;
			default:
				break;
			}
			ball->OnCollide(td, TILE, adhereBorderCoordinate);
			savingWallActive = false;
			savingWall->_position.y = -10000;
		}


		//tiles collision check
		list<RectangleShape*> collidedTiles;
		RectangleShape* closestTile;
		for (RectangleShape* tile : tileList)
		{
			CollisionType t = CollisionManager::areColliding(ball, tile);
			if (t != NONE) {
				collidedTiles.push_back(tile);
			}
		}
		if (collidedTiles.size() > 0) {
			closestTile = collidedTiles.front();
			for (RectangleShape* tile : collidedTiles)
			{
				Vec2<float> deltaTemp = tile->_position - initialPos;
				if (delta.length() > deltaTemp.length()) {
					delta = deltaTemp;
					closestTile = tile;
				}
			}
			CollisionType t = CollisionManager::areColliding(ball, closestTile);
			if (t != NONE) {
				float adhereBorderCoordinate;
				switch (t) {
				case HORIZONTAL:
					if (ball->_position.x < closestTile->_position.x) adhereBorderCoordinate = closestTile->_position.x - closestTile->_width / 2 - ball->_radius;
					else adhereBorderCoordinate = closestTile->_position.x + closestTile->_width / 2 + ball->_radius;

					break;
				case VERTICAL:

					if (ball->_position.y < closestTile->_position.y) adhereBorderCoordinate = closestTile->_position.y - closestTile->_height / 2 - ball->_radius;
					else adhereBorderCoordinate = closestTile->_position.y + closestTile->_height / 2 + ball->_radius;

					break;
				case NONE:
					adhereBorderCoordinate = -1;
				default:
					break;
				}
				ball->OnCollide(t, TILE, adhereBorderCoordinate);
				closestTile->OnCollide(t, BALL, -1);
				return false;
			}
		}
		return false;
	}

	bool updateTiles() {
		//wincondition
		if (tileList.size() == 0) {
			return true;
		}
		
		std::list<RectangleShape*>::iterator i = tileList.begin();
		while (i != tileList.end())
		{
			bool isActive = (*i)->Update(deltaTime);
			if (!isActive)
			{
				points += reward;
				cout <<"\nPoints: " << points;
				destroySprite((*i)->_sprite);
				tileList.erase(i++); 
			}
			else
			{
				++i;
			}
		}
		return false;
	}

	void drawBackground() {
		drawSprite(backgroundSprite, 0, 0);
		if (savingWallActive) savingWall->Update(deltaTime);
		drawSprite(infoBackgroundSprite, playgroundWidth, -300);
	}

	void applyTemporalEffect(ITemporalEffect* effect){
		effect->Apply();
		temporalEffects.push_back(effect);
	}
	void updateTemporalEffects() {
		list<ITemporalEffect*>::iterator i = temporalEffects.begin();
		while (i != temporalEffects.end())
		{
			bool isActive = (*i)->Update(deltaTime);
			if (!isActive)
			{
				(*i)->Purge();
				temporalEffects.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}
	
	void updateHP() {
		for (int i = 0; i < ballHealthDeafault; i++)
			drawSprite(heartSprite, WINDOW_WIDTH - 275 + i * 100, 10);
	}

	void buyEffect() {
		if (points >= 20) points-=20;
			else return;

		int percent = 0 + rand() % (100 + 1);
		if (percent <= 40) { applyTemporalEffect(new SizeChangeEffect(1.4, 20000, trampolin)); cout << "\nAbility: bigger"; return; }
		if (percent <= 70) { applyTemporalEffect(new SizeChangeEffect(0.6, 20000, trampolin)); cout << "\nAbility: smaller"; return; }
		if (savingWallActive) { applyTemporalEffect(new SizeChangeEffect(1.4, 20000, trampolin)); cout << "\nAbility: bigger"; return; }
		if (percent <= 90) { savingWallActive = true; savingWall->_position.y = playgroundHeight + savingWall->_height/4; cout << "\nAbility: saving wall"; return; }//saving wall 
		//damage
		cout << "\nAbility: -1 hp";
		ballHealthDeafault--;
	}

	void updatePoints() {
		int d = points;
		int i = 1;
		while (d > 0) {
			switch (d % 10)
			{
				case 0:
					drawSprite(digit0, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 1:
					drawSprite(digit1, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 2:
					drawSprite(digit2, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 3:
					drawSprite(digit3, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 4:
					drawSprite(digit4, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 5:
					drawSprite(digit5, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 6:
					drawSprite(digit6, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 7:
					drawSprite(digit7, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 8:
					drawSprite(digit8, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				case 9:
					drawSprite(digit9, WINDOW_WIDTH - i * 40 - 50, 100);
					break;
				default:
					break;
			}
			i++;
			d /= 10;
		}
	}

	void updateEffectsView() {
		if (ball->_velocityModifier < 1) { drawSprite(slowSprite, WINDOW_WIDTH-200,150); }
		else if (ball->_velocityModifier > 1) { drawSprite(fastSprite, WINDOW_WIDTH - 200, 150); }
		if(trampolin->_sizeModifier<1) { drawSprite(smallerSizeSprite, WINDOW_WIDTH - 200, 200); } 
		else if (trampolin->_sizeModifier > 1) { drawSprite(biggerSizeSprite, WINDOW_WIDTH - 200, 200); }
		
			
			
			
	}
};




int main(int argc, char** argv)
{
	if (argc == 3) {
		string width = argv[1]; WINDOW_WIDTH = stoi(width);
		string height = argv[2]; WINDOW_HEIGHT = stoi(height);
	}
	else {
		cout << "Invalid or no arguments";
		cout << "\nPlease, input playground width and height,\nNote that the game area will be 300 pixels narrower:\n\n";
		cin >> WINDOW_WIDTH >> WINDOW_HEIGHT;
	}


	
	if (WINDOW_WIDTH < 500) { cout << "\nWrong width: minimal width is 500"; WINDOW_WIDTH = 500; }
	if (WINDOW_HEIGHT < 400) { cout << "\nWrong height: minimal height is 400"; WINDOW_HEIGHT = 400; }
	playgroundWidth = WINDOW_WIDTH - 300;
	playgroundHeight = WINDOW_HEIGHT;

	for (;;) {
		cout << "\nNEW GAME STARTED!\n";
		ballHealthDeafault = 3;
		trampolineVelocity = 0.5;
		ballVelocity = trampolineVelocity * 1.6;
		points = 0;
		reward = 20;

		srand(time(0));
		int d;
		d = run(new MyFramework);
		while (ballHealthDeafault > 0) {
			trampolineVelocity *= 1.15;
			ballVelocity = trampolineVelocity * 1.5;
			reward += 5;
			cout << "\nNext level! Speed increased!\n";
			d = run(new MyFramework);
		}
	}

};

