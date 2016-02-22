#include "cocos2d.h"
using namespace cocos2d;

class InsScreen : public cocos2d::Layer
{
private:
	Sprite* _title; //Background
	Sprite* _current;
	Sprite* _cursor;

	bool ready = false;
	int current = 0;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(InsScreen);
};