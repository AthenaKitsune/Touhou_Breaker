#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <map>

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::Sprite* _brick;
	cocos2d::Sprite* _player;
	cocos2d::Sprite* _hitbox;
	cocos2d::Sprite* _ball;
	cocos2d::Sprite* _danmaku;
	cocos2d::Sprite* _back;
	cocos2d::Sprite* _overhead;
	cocos2d::Sprite* _cursor;
	cocos2d::Sprite* _curref;
	cocos2d::Sprite* _goscreen;
	cocos2d::PhysicsBody* spriteBody;
	cocos2d::PhysicsWorld *sceneWorld;
	Vector<PhysicsBody*> vdanmaku;
	int delay = 3600;
	int current = 0;
	bool dtime = false;
	bool start = false;
	bool gameover = false;
	bool gomove = false;
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) { sceneWorld = world; };
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float delta) override;

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

};

#endif // __HELLOWORLD_SCENE_H__