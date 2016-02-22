#include "HelloWorldScene.h"
#include "TitleScene.h"
#include "InstructionScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

SimpleAudioEngine *ibgm;

Scene* InsScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = InsScreen::create();

	scene->addChild(layer);

	return scene;
}

bool InsScreen::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//bgm loading
	ibgm = SimpleAudioEngine::getInstance();
	ibgm->preloadBackgroundMusic("sfx_confirm.wav");
	ibgm->preloadBackgroundMusic("sfx_select.wav");
	ibgm->preloadBackgroundMusic("sfx_back.wav");
	ibgm->playBackgroundMusic("bgm_returning home from the sky ~ sky dream.wav", true);

	//initial instruction
	_current = Sprite::create("ins_p1.png");
	_current->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_current);

	//Cursor
	_cursor = Sprite::create("player_ball.png");
	_cursor->setPosition(Vec2(visibleSize.width / 2 + 98, visibleSize.height / 2 + 56));

	auto rotation = RotateBy::create(1, 45);
	auto repeat = RepeatForever::create(rotation);
	this->addChild(_cursor);
	_cursor->runAction(repeat);
	_cursor->setOpacity(0);

	ready = true;

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			if (ready == true)
			{
				auto delay = DelayTime::create(0.5f);
				auto c_back = CallFunc::create([=]() {
					ibgm->playEffect("sfx_back.wav");
				});
				auto p_back = CallFunc::create([=]() {
					auto newScene = TitleScreen::createScene();
					Director::getInstance()->replaceScene(TransitionFade::create(1, newScene, Color3B(0, 0, 0)));
				});
				auto seq = Sequence::create(c_back, delay, p_back, nullptr);
				_cursor->runAction(seq);
			}
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (current == 1 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 0;
				_current->setTexture("ins_p1.png");
				ready = true;
			}
			else if (current == 2 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 1;
				_current->setTexture("ins_p2.png");
				ready = true;
			}
			else if (current == 3 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 2;
				_current->setTexture("ins_p3.png");
				ready = true;
			}
			else if (current == 4 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 3;
				_current->setTexture("ins_p4.png");
				ready = true;
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (current == 0 && ready == true) { 
				ready = false; 
				ibgm->playEffect("sfx_select.wav");
				current = 1;
				_current->setTexture("ins_p2.png"); 
				ready = true;
			}
			else if (current == 1 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 2;
				_current->setTexture("ins_p3.png");
				ready = true;
			}
			else if (current == 2 && ready == true) { 
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 3;
				_current->setTexture("ins_p4.png");
				ready = true;
			}
			else if (current == 3 && ready == true) {
				ready = false;
				ibgm->playEffect("sfx_select.wav");
				current = 4;
				_current->setTexture("ins_p5.png");
				ready = true;
			}
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _cursor);
	return true;
}