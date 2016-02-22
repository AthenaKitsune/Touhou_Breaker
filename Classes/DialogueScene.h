#include "cocos2d.h"
using namespace cocos2d;

class DialogueScreen : public cocos2d::Layer
{
private:
	Sprite* _title;
	Sprite* _cursor;
	Sprite* _dialogue;
	Sprite* _sanae;
	Sprite* _location;
	Sprite* _cline;
	Sprite* _cword;
	int current = 0;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(DialogueScreen);
};