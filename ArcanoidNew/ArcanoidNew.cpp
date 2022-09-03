#include "Framework.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include "IShape.h"
#include "CollisionManager.h"
#include <math.h>

using namespace std;


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

//std::string GetCurrentDirectory()
//{
//	char buffer[MAX_PATH];
//	GetModuleFileNameA(NULL, buffer, MAX_PATH);
//	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
//
//	return std::string(buffer).substr(0, pos);
//}




/* Test Framework realization */
class MyFramework : public Framework {

	unsigned int
		tickTime = 0,
		deltaTime = 0;



	bool
		arrowKeyRightPressed = false,
		arrowKeyLeftPressed = false,
		leftMouseButtonPressed = false;
		

	int
		playgroundWidth = WINDOW_WIDTH,
		playgroundHeight = WINDOW_HEIGHT,

		spriteMouseWidth = 20,
		spriteMouseHeight = 20,

		spriteTrampolineWidth = 200,
		spriteTrampolineHeight = 50;




	float
		mouseX = 0,
		mouseY = 0,
		trampolineX = playgroundWidth / 2,
		trampolineY = playgroundHeight - 50,
		trampolineVelocity = 0.75,
		ballVelocity = trampolineVelocity * 1.2,

		ballVelocityX = -0.5,
		ballVelocityY = -0.5,
		trampolineVelocityX = 1,
		trampolineVelocityY = 0;


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
		CollisionManager::xPlayground = 0;
		CollisionManager::yPlayground = 0;
		CollisionManager::playgroundWidth = playgroundWidth;
		CollisionManager::playgroundHeight = playgroundHeight;

		showCursor(true);
		//cursor
		mouseCursoreSprite = createSprite(".\\data\\green-circle.png");
		setSpriteSize(mouseCursoreSprite, spriteMouseWidth, spriteMouseHeight);

		//trampoline
		trampolineSprite = createSprite(".\\data\\56-Breakout-Tiles.png");
		trampolin = new RectangleShape(trampolineX, trampolineY, &trampolineVelocity, spriteTrampolineWidth, spriteTrampolineHeight, trampolineSprite);
		
		//ball
		ballSprite = createSprite(".\\data\\58-Breakout-Tiles.png");
		ball = new RectangleShape(trampolineX, trampolineY - spriteTrampolineHeight, &ballVelocity, 20, 20, ballSprite);

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
		deltaTime = 20;
		drawTestBackground();

		updateTrampoline();
		updateCursor();
		updateBall();
		//cout << "\n" << deltaTime;




		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		mouseX = x - spriteMouseWidth / 2;
		mouseY = y - spriteMouseHeight / 2;
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
		if (!isReleased) cout << "\nx:" << mouseX << "  y:" << mouseY;
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
			break;
		case FRKey::LEFT:
			arrowKeyLeftPressed = false;
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
			trampolineVelocityX = 1;
			trampolin->Move(&trampolineVelocityX, &trampolineVelocityY, deltaTime);
		}
		if (!arrowKeyRightPressed && arrowKeyLeftPressed)
		{
			trampolineVelocityX = -1;
			trampolin->Move(&trampolineVelocityX, &trampolineVelocityY, deltaTime);
		}
		trampolin->Update();
	}
	void  updateCursor() {
		drawSprite(mouseCursoreSprite, mouseX, mouseY);
	}
	void updateBall() {
		ball->Move(&ballVelocityX, &ballVelocityY, deltaTime);
		
		switch (CollisionManager::areColliding(ball, trampolin)) {
			case HORIZONTAL:
				ballVelocityX = -ballVelocityX;
				ball->Move(&ballVelocityX, &ballVelocityY, deltaTime*5);
				break;
			case VERTICAL:
				ballVelocityY = -ballVelocityY;
				if (leftMouseButtonPressed) {
					float
						dx = mouseX - ball->_x,
						dy = mouseY - ball->_y,
						c = sqrtf(powf(dx, 2) + powf(dy, 2));

					ballVelocityX = ballVelocity * dx / c;
					ballVelocityY = ballVelocity * dy / c;
				}
				ball->Move(&ballVelocityX, &ballVelocityY, deltaTime);
				break;
			case NONE:
			default:
				break;
		}


		ball->Update();
	}

};




int main()
{


	int d = run(new MyFramework);


	return d;
};

