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

using namespace std;



#define WINDOW_WIDTH 1200
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
	Vec2<int>
		playgroundStartPosition = Vec2<int>(0, 0);
	int
		playgroundWidth = WINDOW_WIDTH,
		playgroundHeight = WINDOW_HEIGHT,
		
		spriteMouseWidth = 20,
		spriteMouseHeight = 20,

		spriteTrampolineWidth = 200,
		spriteTrampolineHeight = 50;
}

using namespace input;
using namespace options;

/* Test Framework realization */
class MyFramework : public Framework {

	unsigned int
		tickTime = 0,
		deltaTime = 0;

	float
		trampolineX = playgroundWidth / 2,
		trampolineY = playgroundHeight - 25,
		trampolineVelocity = 0.75,
		ballVelocity = trampolineVelocity * 1.2;

	Vec2<float>
		trampolinePos = Vec2<float>(trampolineX, trampolineY),
		ballPos = Vec2<float>(trampolineX, trampolineY - spriteTrampolineHeight),
		ballVelocityDirection = Vec2<float>(0, -1),
		trampolineVelocityDirection = Vec2<float>(0, 0);

	Sprite
		* mouseCursoreSprite,
		* trampolineSprite,
		* rightWallSprite,
		* ballSprite;

	RectangleShape
		* trampolin,
		* ball;
	
	list<RectangleShape*> tileList;



public:

	//only responsible for width/height/fullscreen
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;
		fullscreen = false;
	}

	virtual bool Init() {

		//cursor
		showCursor(true);
		mouseCursoreSprite = createSprite(".\\data\\green-circle.png");
		setSpriteSize(mouseCursoreSprite, spriteMouseWidth, spriteMouseHeight);

		//trampoline
		trampolineSprite = createSprite(".\\data\\56-Breakout-Tiles.png");
		trampolin = new Trampoline(trampolinePos, trampolineVelocity, trampolineVelocityDirection, spriteTrampolineWidth, spriteTrampolineHeight, trampolineSprite);
		
		//ball
		ballSprite = createSprite(".\\data\\58-Breakout-Tiles.png");
		ball = new Ball(ballPos , ballVelocity, ballVelocityDirection, 20, 20, ballSprite);

		//tiles
		for (int i = 0; i < 18; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 60, 100), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}
		for (int i = 0; i < 9; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 120, 120), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}
		for (int i = 0; i < 18; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 60, 140), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}
		for (int i = 0; i < 9; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 120, 160), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}
		for (int i = 0; i < 18; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 60, 180), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}
		for (int i = 0; i < 9; i++) {
			int random = 1 + (rand() % 21);
			string s = ".\\data\\" + to_string(random) + "-Breakout-Tiles.png";
			tileList.push_back(new Tile(Vec2<float>(100 + i * 120, 200), 0, Vec2<float>(0, 0), 60, 20, createSprite(s.c_str())));
		}


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
		drawTestBackground();

		updateTrampoline();
		updateCursor();
		updateBall();
		updateTiles();

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
			/*float
				dx = mouseX - ball->_x,
				dy = mouseY - ball->_y,
				c = sqrtf(powf(dx, 2) + powf(dy, 2));

			ballVelocityX = ballVelocity * dx / c;
			ballVelocityY = ballVelocity * dy / c;*/
		}
		if (!isReleased) cout << "\nx:" << mousePos.x << "  y:" << mousePos.y;
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
	void  updateTrampoline() {
		if (arrowKeyRightPressed && !arrowKeyLeftPressed) {
			trampolin->SetDirection(Vec2<float>(1, 0));
		}
		if (!arrowKeyRightPressed && arrowKeyLeftPressed)
		{
			trampolin->SetDirection(Vec2<float>(-1, 0));
		}
		trampolin->Update(deltaTime);
	}
	void  updateCursor() {
		drawSprite(mouseCursoreSprite, mousePos.x - spriteMouseWidth / 2, mousePos.y - spriteMouseHeight / 2);
	}

	void updateBall() {
		ball->Update(deltaTime);
		
		CollisionType t = CollisionManager::areColliding(ball, trampolin);
		if (t != NONE) {
			float adhereBorderCoordinate;
			switch (t) {
			case HORIZONTAL:
				if (ball->_position.x < trampolin->_position.x) adhereBorderCoordinate = trampolin->_position.x - trampolin->_width / 2 - ball->_width / 2;
				else adhereBorderCoordinate = trampolin->_position.x + trampolin->_width / 2 + ball->_width / 2;

				break;
			case VERTICAL:

				if (ball->_position.y < trampolin->_position.y) adhereBorderCoordinate = trampolin->_position.y - trampolin->_height / 2 - ball->_height / 2;
				else adhereBorderCoordinate = trampolin->_position.y + trampolin->_height / 2 + ball->_height / 2;

				break;
			case NONE:
				adhereBorderCoordinate = -1;
			default:
				break;
			}
			ball->OnCollide(t, TRAMPOLIN, adhereBorderCoordinate);
			trampolin->OnCollide(t, BALL, -1);
			ball->Update(deltaTime);
			return;
		}

		for (RectangleShape* tile : tileList)
		{
			CollisionType t = CollisionManager::areColliding(ball, tile);
			if (t != NONE) {
				float adhereBorderCoordinate;
				switch (t) {
				case HORIZONTAL:
					if (ball->_position.x < tile->_position.x) adhereBorderCoordinate = tile->_position.x - tile->_width / 2 - ball->_width / 2;
					else adhereBorderCoordinate = tile->_position.x + tile->_width / 2 + ball->_width / 2;

					break;
				case VERTICAL:

					if (ball->_position.y < tile->_position.y) adhereBorderCoordinate = tile->_position.y - tile->_height / 2 - ball->_height / 2;
					else adhereBorderCoordinate = tile->_position.y + tile->_height / 2 + ball->_height / 2;

					break;
				case NONE:
					adhereBorderCoordinate = -1;
				default:
					break;
				}
				ball->OnCollide(t, TILE, adhereBorderCoordinate);
				tile->OnCollide(t, BALL, -1);
				ball->Update(deltaTime);
				return;
			}
		}


	}
	void updateTiles() {

		
		std::list<RectangleShape*>::iterator i = tileList.begin();
		while (i != tileList.end())
		{
			bool isActive = (*i)->Update(deltaTime);
			if (!isActive)
			{
				tileList.erase(i++);  // alternatively, i = items.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
};




int main()
{
	srand(time(0));

	int d = run(new MyFramework);


	return d;
};

