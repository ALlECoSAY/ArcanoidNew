#include "Framework.h"
#include <iostream>
#include <Windows.h>
#include <list>
#include <string> 
#include <ctime>
#include <cstdlib>

#include "IShape.h"
#include "CollisionManager.h"
#include "Shared.h"
#include "IEffect.h"

using namespace std;



#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

namespace input {
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

		playgroundWidth = WINDOW_WIDTH-300,
		playgroundHeight = WINDOW_HEIGHT,
		
		spriteMouseWidth = 20,
		spriteMouseHeight = 20,

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
		* trampolineSprite, * trampolineSprite2, * trampolineSprite3, * trampolineSprite4,
		* rightWallSprite,
		* ballSprite,
		* backgroundSprite,
		* infoBackgroundSprite,
		* heartSprite,
		* savingWallSprite;

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
		//background
		backgroundSprite = createSprite(".\\data\\blackBackground.png");
		setSpriteSize(backgroundSprite, WINDOW_WIDTH, WINDOW_HEIGHT);

		//hearts
		heartSprite = createSprite(".\\data\\60-Breakout-Tiles.png");
		setSpriteSize(heartSprite, 50, 50);

		//infobackground
		infoBackgroundSprite = createSprite(".\\data\\29-Breakout-Tiles.png");
		setSpriteSize(infoBackgroundSprite, WINDOW_WIDTH-playgroundWidth, WINDOW_HEIGHT*2);

		//cursor
		showCursor(false);
		mouseCursoreSprite = createSprite(".\\data\\green-circle.png");
		setSpriteSize(mouseCursoreSprite, spriteMouseWidth, spriteMouseHeight);

		//trampoline
		trampolineSprite = createSprite(".\\data\\56-Breakout-Tiles.png");
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

	}

	//rendering ticks
	virtual bool Tick() {
		

		deltaTime = tickTime;
		tickTime = getTickCount();//time ticks
		deltaTime = tickTime - deltaTime;
		if (deltaTime > 100) { 
			deltaTime = 20; 
			arrowKeyLeftPressed = false;
			arrowKeyRightPressed = false;
			trampolin->_velocityDirecrion.x = 0;
		}

		drawBackground();
		updateTrampoline();

		//loose condition
		areaVelocityEffect->Apply();
		areaVelocityPurgeEffect->Purge();

		if (updateBall() || ballHealthDeafault==0) {
			cout << "\nYOU LOST";
			return true;
		}
		if (updateTiles()) {
			cout << "\nYOU WON";
			return true;
		}
	
		updateTemporalEffects();
		updateHP();

		updateCursor();

		//cout << "\n" << deltaTime;




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


		//trampolin check
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
		//savingWall
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


		//tile check
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
				points += 20;
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
			drawSprite(heartSprite, WINDOW_WIDTH - 275 + i * 100, 150);
	}

	void buyEffect() {
		if (points >= 20) points-=20;
			else return;

		int percent = 0 + rand() % (100 + 1);
		if (percent <= 35) { applyTemporalEffect(new SizeChangeEffect(1.4, 20000, trampolin)); cout << "\n Ability: bigger"; return; }
		if (percent <= 70) { applyTemporalEffect(new SizeChangeEffect(0.6, 20000, trampolin)); cout << "\n Ability: smaller"; return; }
		if (savingWallActive) { applyTemporalEffect(new SizeChangeEffect(1.4, 20000, trampolin)); cout << "\n Ability: bigger"; return; }
		if (percent <= 95) { savingWallActive = true; savingWall->_position.y = playgroundHeight + savingWall->_height/4; cout << "\n Ability: saving wall"; return; }//saving wall 
		//damage
		cout << "\n Ability: -1 hp";
		ballHealthDeafault--;
	}


};




int main()
{
	srand(time(0));

	int d = run(new MyFramework);




	cin >> d;
	return d;
};

