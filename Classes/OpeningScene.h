#include "cocos2d.h"
using namespace cocos2d;

class OpeningScreen : public cocos2d::Layer
{
private:
	Sprite* _title; //Background
	Sprite* _t; //Tou
	Sprite* _h; //hou
	Sprite* _msg; //Musaigen
	Sprite* _cursor;
	Sprite* _scroll; //Scroll Texture
	Sprite* _border; //Border of Power
	Sprite* _start; //Start Game
	Sprite* _exit; //Exit Game
	Sprite* _black; //Transparent Black
	Sprite* _fireball;

	bool ready = false;
	int current = 0;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(OpeningScreen);
};