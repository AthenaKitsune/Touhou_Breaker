#include "HelloWorldScene.h"
#include "TitleScene.h"
#include "OpeningScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

SimpleAudioEngine *obgm;

Scene* OpeningScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = OpeningScreen::create();

	scene->addChild(layer);

	return scene;
}

bool OpeningScreen::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//bgm loading
	obgm = SimpleAudioEngine::getInstance();
	obgm->playBackgroundMusic("bgm_a shadow in the blue sky.wav", true);

	//background
	_title = Sprite::create("back_moriyashrine.png");
	_title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_title);

	//scroll texture
	_scroll = Sprite::create("title_base.png");
	_scroll->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_scroll);
	_scroll->setOpacity(0);

	//fireball
	_fireball = Sprite::create("title_ball.png");
	_fireball->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_title->addChild(_fireball);
	_fireball->setOpacity(0);

	//touhou musaigen
	_t = Sprite::create("title_b1.png");
	_t->setPosition(Vec2(visibleSize.width / 2 - 180, visibleSize.height / 2));
	this->addChild(_t);
	_t->setOpacity(0);
	_h = Sprite::create("title_b2.png");
	_h->setPosition(Vec2(visibleSize.width / 2 - 180, visibleSize.height / 2));
	this->addChild(_h);
	_h->setOpacity(0);
	_msg = Sprite::create("title_b3.png");
	_msg->setPosition(Vec2(visibleSize.width / 2 - 180, visibleSize.height / 2));
	this->addChild(_msg);
	_msg->setOpacity(0);

	//border of power
	_border = Sprite::create("title_bop.png");
	_border->setPosition(Vec2(visibleSize.width / 2 - 180, 25));
	this->addChild(_border);
	_border->setOpacity(0);

	//transparent black
	_black = Sprite::create("title_black.png");
	_black->setPosition(Vec2(visibleSize.width / 2 + 180, visibleSize.height / 2));
	this->addChild(_black);
	_black->setOpacity(0);

	//start game and exit game
	_start = Sprite::create("title_start.png");
	_start->setPosition(Vec2(visibleSize.width / 2 + 180, visibleSize.height / 2 + 56));
	this->addChild(_start);
	_start->setOpacity(0);
	_exit = Sprite::create("title_exit.png");
	_exit->setPosition(Vec2(visibleSize.width / 2 + 180, visibleSize.height / 2 - 56));
	this->addChild(_exit);
	_exit->setOpacity(0);

	//Cursor
	_cursor = Sprite::create("player_ball.png");
	_cursor->setPosition(Vec2(visibleSize.width / 2 + 98, visibleSize.height / 2 + 56));

	auto rotation = RotateBy::create(1, 45);
	auto repeat = RepeatForever::create(rotation);
	this->addChild(_cursor);
	_cursor->runAction(repeat);
	_cursor->setOpacity(0);

	//actions
	auto d1 = DelayTime::create(1.0f); //delay for one second
	auto d2 = DelayTime::create(0.5f); //delay for half second
	auto f_scroll = CallFunc::create([=]() {
		_scroll->runAction(FadeIn::create(1.0f));

	});
	auto m_scroll = CallFunc::create([=]() {
		_scroll->runAction(MoveBy::create(1, Vec2(-180, 0)));
	});
	auto f_fire = CallFunc::create([=]() {
		_fireball->runAction(FadeIn::create(0.5f));
	});
	auto f_tou = CallFunc::create([=]() {
		_t->runAction(FadeIn::create(0.5f));
	});
	auto f_hou = CallFunc::create([=]() {
		_h->runAction(FadeIn::create(0.5f));
	});
	auto f_msg = CallFunc::create([=]() {
		_msg->runAction(FadeIn::create(0.5f));
	});
	auto f_border = CallFunc::create([=]() {
		_border->runAction(FadeIn::create(1.0f));
	});
	auto f_black = CallFunc::create([=]() {
		_black->runAction(FadeIn::create(1.0f));
	});
	auto f_start = CallFunc::create([=]() {
		_start->runAction(FadeIn::create(1.0f));
	});
	auto f_exit = CallFunc::create([=]() {
		_exit->runAction(FadeIn::create(1.0f));
		ready = true;
	});
	auto f_cursor = CallFunc::create([=]() {
		_cursor->runAction(FadeIn::create(0.5f));
		//ready = true;
	});
	auto c_ready = CallFunc::create([=]() {
		ready = true;
	});
	auto seq = Sequence::create(f_scroll, d1, m_scroll, d1, f_tou, d2, f_hou, d2, f_msg, d2, f_fire, f_border, d1, f_black, d1, f_start, f_exit, d1, f_cursor, d2, c_ready, nullptr);
	_scroll->runAction(seq);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ENTER:
			if (current == 0 && ready == true)
			{
				//ebgm->playEffect("1UP.wav");
				auto newScene = TitleScreen::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(2, newScene, Color3B(0, 0, 0)));
			}
			else if (current == 1 && ready == true) { Director::getInstance()->end(); }
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (current == 1 && ready == true) { current = 0; _cursor->setPosition(Vec2(visibleSize.width / 2 + 98, visibleSize.height / 2 + 56)); }
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (current == 0 && ready == true) { current = 1; _cursor->setPosition(Vec2(visibleSize.width / 2 + 105, visibleSize.height / 2 - 56)); }
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _cursor);
	return true;
}