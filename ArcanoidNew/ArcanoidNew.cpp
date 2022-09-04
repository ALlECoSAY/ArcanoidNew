#include "Framework.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include "IShape.h"
#include "CollisionManager.h"
#include <math.h>
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
		playgroundHeight = WINDOW_HEIGHT;
}

using namespace input;
using namespace options;

/* Test Framework realization */
class MyFramework : public Framework {

	unsigned int
		tickTime = 0,
		deltaTime = 0;

	int
		spriteMouseWidth = 20,
		spriteMouseHeight = 20,

		spriteTrampolineWidth = 200,
		spriteTrampolineHeight = 50;

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

		//rightwall 



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
		drawTestBackground();

		updateTrampoline();
		updateCursor();
		updateBall();
		//cout << "\n" << deltaTime;




		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		mousePos.x = x - spriteMouseWidth / 2;
		mousePos.y = y - spriteMouseWidth / 2;

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
		drawSprite(mouseCursoreSprite, mousePos.x, mousePos.y);
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
		}
	}

};




int main()
{


	int d = run(new MyFramework);


	return d;
};

