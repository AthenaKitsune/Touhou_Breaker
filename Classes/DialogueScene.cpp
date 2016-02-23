#include "HelloWorldScene.h"
#include "DialogueScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

SimpleAudioEngine *dbgm;

Scene* DialogueScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = DialogueScreen::create();

	scene->addChild(layer);

	return scene;
}

bool DialogueScreen::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//bgm loading
	dbgm = SimpleAudioEngine::getInstance();
	dbgm->preloadBackgroundMusic("sfx_pichuun.wav");
	dbgm->preloadBackgroundMusic("sfx_destroy.wav");
	dbgm->preloadBackgroundMusic("sfx_bounce.wav");
	dbgm->preloadBackgroundMusic("sfx_confirm.wav");
	dbgm->preloadBackgroundMusic("sfx_select.wav");
	dbgm->preloadBackgroundMusic("sfx_back.wav");
	dbgm->preloadBackgroundMusic("sfx_graze.wav");
	dbgm->playBackgroundMusic("bgm_returning home from the sky ~ sky dream.wav", true);

	//background
	_title = Sprite::create("back_moriyashrine.png");
	_title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_title);

	//dialogue box
	_dialogue = Sprite::create("dia/dia_p1.png");
	_dialogue->setPosition(Vec2(visibleSize.width / 2, 90));
	_title->addChild(_dialogue);

	_cursor = Sprite::create("player_ball.png");
	_cursor->setAnchorPoint(Vec2(0.5, 0.5));
	_cursor->setPosition(Vec2(414, 186));
	this->addChild(_cursor);
	_cursor->setOpacity(0);

	//auto rotation = RotateBy::create(1, 45);
	//auto repeat = RepeatForever::create(rotation);
	//this->addChild(_cursor);
	//_cursor->runAction(repeat);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ENTER:
			if (current == 0) {
				current++;
				_dialogue->setTexture("dia/dia_p2.png");
			}
			else if (current == 1) {
				current++;
				_dialogue->setTexture("dia/dia_s1.png");
			}
			else if (current == 2) {
				current++;
				_dialogue->setTexture("dia/dia_k1.png");
			}
			else if (current == 3) {
				current++;
				_dialogue->setTexture("dia/dia_k2.png");
			}
			else if (current == 4) {
				current++;
				_dialogue->setTexture("dia/dia_s2.png");
			}
			else if (current == 5) {
				current++;
				_dialogue->setTexture("dia/dia_k3.png");
			}
			else if (current == 6) {
				current++;
				_dialogue->setTexture("dia/dia_s3.png");
			}
			else if (current == 7) {
				current++;
				_dialogue->setTexture("dia/dia_s4.png");
			}
			else if (current == 8) {
				current++;
				_dialogue->setTexture("dia/dia_s5.png");
			}
			else if (current == 9) {
				current++;
				_dialogue->setTexture("dia/dia_k4.png");
			}
			else if (current == 10) {
				current++;
				_dialogue->setTexture("dia/dia_s6.png");
			}
			else if (current == 11) {
				current++;
				_dialogue->setTexture("dia/dia_k5.png");
			}
			else if (current == 12) {
				current++;
				_dialogue->setTexture("dia/dia_k6.png");
			}
			else if (current == 13) {
				current++;
				_dialogue->setTexture("dia/dia_s7.png");
			}
			else if (current == 14) {
				current++;
				_dialogue->setTexture("dia/dia_k7.png");
			}
			else if (current == 15) {
				current++;
				_dialogue->setTexture("dia/dia_s8.png");
			}
			else if (current == 16) {
				current++;
				_dialogue->setTexture("dia/dia_k8.png");
			}
			else if (current == 17) {
				current++;
				_dialogue->setTexture("dia/dia_s9.png");
			}
			else if (current == 18) {
				current++;
				_dialogue->setTexture("dia/dia_k9.png");
			}
			else if (current == 19) {
				current++;
				_dialogue->setTexture("dia/dia_s10.png");
			}
			else if (current == 20) {
				current++;
				_dialogue->setTexture("dia/dia_k10.png");
			}
			else if (current == 21) {
				current++;
				_dialogue->setTexture("dia/dia_k11.png");
			}
			else if (current == 22) {
				current++;
				_dialogue->setTexture("dia/dia_s11.png");
			}
			else if (current == 23)
			{
				auto newScene = HelloWorld::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(1, newScene, Color3B(0, 0, 0)));
			}
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _dialogue);

	return true;
}