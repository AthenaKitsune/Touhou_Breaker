#include "HelloWorldScene.h"
#include "TitleScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

SimpleAudioEngine *bgm;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //draws the red mask thing
	scene->getPhysicsWorld()->setGravity(Vec2(0, -1.0f));

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//bgm
	bgm = SimpleAudioEngine::getInstance();
	bgm->preloadBackgroundMusic("sfx_pichuun.wav");
	bgm->preloadBackgroundMusic("sfx_destroy.wav");
	bgm->preloadBackgroundMusic("sfx_bounce.wav");
	bgm->preloadBackgroundMusic("sfx_confirm.wav");
	bgm->preloadBackgroundMusic("sfx_select.wav");
	bgm->preloadBackgroundMusic("sfx_back.wav");
	bgm->preloadBackgroundMusic("sfx_graze.wav");
	bgm->playBackgroundMusic("bgm_heian alien.wav", true);

	//background sprite
	_back = Sprite::create("back_moriyashrine.png");
	_back->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_back);

	//wall creation and physics
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	edgeBody->setCollisionBitmask(4);
	edgeBody->setContactTestBitmask(true);
	this->addChild(edgeNode);

	//ball creation and physics
	_ball = Sprite::create("player_ball.png");
	_ball->setPosition(Point(visibleSize.width / 2, 50+40));
	spriteBody = PhysicsBody::createCircle(_ball->getContentSize().width / 2, PhysicsMaterial(0.0f, 3.0f, 0.0f));
	//spriteBody->setVelocity(Vect(150, 200));
	spriteBody->setGravityEnable(false);
	spriteBody->setRotationEnable(true);
	spriteBody->setVelocityLimit(400);
	spriteBody->setLinearDamping(0);
	_ball->setPhysicsBody(spriteBody);
	spriteBody->setCollisionBitmask(3);
	spriteBody->setContactTestBitmask(true);
	this->addChild(_ball);

	//creation of the player
	_player = Sprite::create("player_sprite.png");
	_player->setAnchorPoint(Vec2(0.5, 0.5));
	_player->setPosition(visibleSize.width / 2, 50);
	this->addChild(_player);

	//player hitbox
	_hitbox = Sprite::create("player_ball.png");
	auto hitbox = PhysicsBody::createCircle(_ball->getContentSize().width / 4, PhysicsMaterial(10.0f, 0.0f, 0.0f));
	_hitbox->setPosition(48, 16);
	_hitbox->setPhysicsBody(hitbox);
	hitbox->setCollisionBitmask(6);
	hitbox->setContactTestBitmask(2);
	hitbox->setDynamic(false);
	_player->addChild(_hitbox);
	_hitbox->setOpacity(0);

	//player physics
	auto playerBody = PhysicsBody::createBox(_player->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	playerBody->getShape(0)->setRestitution(2.0f);
	playerBody->getShape(0)->setFriction(0.0f);
	playerBody->getShape(0)->setDensity(10.0f);
	playerBody->setDynamic(false);
	_player->setPhysicsBody(playerBody);
	playerBody->setCollisionBitmask(2);
	playerBody->setContactTestBitmask(true);

	//moving the player (only left and right)
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_SHIFT:
			if (start == true && gameover == false) keys[keyCode] = std::chrono::high_resolution_clock::now();
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (start == true && gameover == false) keys[keyCode] = std::chrono::high_resolution_clock::now();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (start == true && gameover == false) keys[keyCode] = std::chrono::high_resolution_clock::now();
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (gameover == true && gomove == true && current == 1) {
				_cursor->setPosition(Vec2(252, 480 - 357)); //389
				_curref->setPosition(Vec2(640 - 247, 480 - 357));
				current = 0;
				bgm->playEffect("sfx_select.wav");
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (gameover == true && gomove == true && current == 0) {
				_cursor->setPosition(Vec2(220, 480 - 416));
				_curref->setPosition(Vec2(640 - 215, 480 - 416)); //448 
				current = 1;
				bgm->playEffect("sfx_select.wav");
			}
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			if (start == false && gameover == false) { 
				spriteBody->applyImpulse(Vec2((32 * ((rand() % 11) + (-10))), 200)); 
				start = true; 
				spriteBody->setGravityEnable(true);
			}
			else if (gameover == true && gomove == true) {
				if (current == 0) {
					auto delay = DelayTime::create(0.5f);
					auto p_sound = CallFunc::create([=]() {
						bgm->playEffect("sfx_confirm.wav");
					});
					auto c_scene = CallFunc::create([=]() {
						auto newScene = HelloWorld::createScene();
						Director::getInstance()->replaceScene(newScene);
					});
					auto seq = Sequence::create(p_sound, delay, c_scene, nullptr);
					_cursor->runAction(seq);
					//bgm->playEffect("sfx_confirm.wav");
					//auto newScene = HelloWorld::createScene();
					//Director::getInstance()->replaceScene(newScene);
				}
				else if (current == 1) {
					auto delay = DelayTime::create(0.5f);
					auto p_sound = CallFunc::create([=]() {
						bgm->playEffect("sfx_back.wav");
					});
					auto c_scene = CallFunc::create([=]() {
						auto newScene = TitleScreen::createScene();
						Director::getInstance()->replaceScene(newScene);
					});
					auto seq = Sequence::create(p_sound, delay, c_scene, nullptr);
					_cursor->runAction(seq);
	
				}
			}
			break;
		}
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys.erase(keyCode);
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _player);
	this->scheduleUpdate();

	//brick spawn
	int y = 410;
	int col = 0;
	for (int i = 0; i < 18; i++) {
		_brick = Sprite::create("level_brick.png");
		auto blockBody = PhysicsBody::createBox(_brick->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		_brick->setPosition(48 + 32 * col, y); //First row of bricks (18 bricks)
		blockBody->setCollisionBitmask(1);
		blockBody->setContactTestBitmask(true);
		_brick->setPhysicsBody(blockBody);
		_brick->setTag(3);
		this->addChild(_brick);
		col++;
	}
	y = 410 - 32;
	col = 0;
	for (int i = 0; i < 18; i++) {
		_brick = Sprite::create("level_brick.png");
		auto blockBody = PhysicsBody::createBox(_brick->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		_brick->setPosition(48 + 32 * col, y); //Second row of bricks (18 bricks)
		blockBody->setCollisionBitmask(1);
		blockBody->setContactTestBitmask(true);
		_brick->setPhysicsBody(blockBody);
		_brick->setTag(3);
		this->addChild(_brick);
		col++;
	}
	y = 410 - 64;
	col = 0;
	for (int i = 0; i < 12; i++) {
		_brick = Sprite::create("level_brick.png");
		auto blockBody = PhysicsBody::createBox(_brick->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		_brick->setPosition(144 + 32 * col, y); //Third row of bricks (12 bricks)
		blockBody->setCollisionBitmask(1);
		blockBody->setContactTestBitmask(true);
		_brick->setPhysicsBody(blockBody);
		_brick->setTag(3);
		this->addChild(_brick);
		col++;
	}
	y = 410 - 96; 
	col = 0;
	for (int i = 0; i < 8; i++) {
		_brick = Sprite::create("level_brick.png");
		auto blockBody = PhysicsBody::createBox(_brick->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		_brick->setPosition(208 + 32 * col, y); //Fourth row of bricks (8 bricks)
		blockBody->setCollisionBitmask(1);
		blockBody->setContactTestBitmask(true);
		_brick->setPhysicsBody(blockBody);
		_brick->setTag(3);
		this->addChild(_brick);
		col++;
	}
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//child stuff for danmaku
	_overhead = Sprite::create("back_moriyashrine.png");
	_overhead->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_overhead);
	_overhead->setOpacity(0);

	//gameover screen
	_goscreen = Sprite::create("scr_gameover.png");
	_goscreen->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_goscreen);
	_goscreen->setOpacity(0);

	//cursor creation
	_cursor = Sprite::create("player_ball.png");
	_cursor->setPosition(Vec2(252, 480 - 357)); //252
	auto rotation = RotateBy::create(1, 45);
	auto repeat = RepeatForever::create(rotation);
	this->addChild(_cursor);
	_cursor->runAction(repeat);
	_cursor->setOpacity(0);

	_curref = Sprite::create("player_ball.png");
	_curref->setPosition(Vec2(640 - 247, 480 - 357));
	auto rotation2 = RotateBy::create(1, 45);
	auto repeat2 = RepeatForever::create(rotation2);
	this->addChild(_curref);
	_curref->runAction(repeat2);
	_curref->setOpacity(0);

	return true;
}

bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	
	//check ball vs danmaku
	if (3 == a->getCollisionBitmask() && 5 == b->getCollisionBitmask()) {
		return false;
	}
	else if (3 == b->getCollisionBitmask() && 5 == a->getCollisionBitmask()) {
		return false;
	}

	//check hitbox vs player
	if (2 == a->getCollisionBitmask() && 6 == b->getCollisionBitmask()) {
		return false;
	}
	else if (2 == b->getCollisionBitmask() && 6 == a->getCollisionBitmask()) {
		return false;
	}

	//check if the bodies have collided, erase brick if ball hits the brick
	if (1 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask())
	{
		auto brick = (Sprite*)contact.getShapeA()->getBody()->getNode();
		this->removeChild(brick, true);
		bgm->playEffect("sfx_destroy.wav");
	}
	else if (3 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())
	{
		auto brick = (Sprite*)contact.getShapeB()->getBody()->getNode();
		this->removeChild(brick, true);
		bgm->playEffect("sfx_destroy.wav");
	}
	//check if the ball has collided with the wall, if the y of it is smaller than the player, game over! 
	else if (4 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() && _player->getPosition().y > _ball->getPosition().y)
	{
		for (int i = 0; i < vdanmaku.size(); i++) {
			vdanmaku.at(i)->setVelocity(Vec2(0, 0));
			vdanmaku.at(i)->setGravityEnable(false);
		}
		bgm->playEffect("sfx_pichuun.wav");
		spriteBody->setGravityEnable(false);
		spriteBody->setVelocity(Vec2(0, 0));
		start = false;
		gameover = true;

		auto delay = DelayTime::create(1.0f);
		auto f_cursor = CallFunc::create([=]() {
			_cursor->runAction(FadeIn::create(1.0f));
		});
		auto c_gomove = CallFunc::create([=]() {
			gomove = true;
		});
		auto seq = Sequence::create(FadeIn::create(1.0f), f_cursor, delay, c_gomove, nullptr);
		_goscreen->runAction(seq);
	}
	else if (3 == a->getCollisionBitmask() && 4 == b->getCollisionBitmask() && _player->getPosition().y > _ball->getPosition().y)
	{
		for (int i = 0; i < vdanmaku.size(); i++) {
			vdanmaku.at(i)->setVelocity(Vec2(0, 0));
			vdanmaku.at(i)->setGravityEnable(false);
		}
		dtime = false;
		bgm->playEffect("sfx_pichuun.wav");
		spriteBody->setGravityEnable(false);
		spriteBody->setVelocity(Vec2(0, 0));
		start = false;
		gameover = true;

		auto delay = DelayTime::create(1.0f);
		auto f_cursor = CallFunc::create([=]() {
			_cursor->runAction(FadeIn::create(1.0f));
			_curref->runAction(FadeIn::create(1.0f));
		});
		auto c_gomove = CallFunc::create([=]() {
			gomove = true;
		});
		auto seq = Sequence::create(FadeIn::create(1.0f), f_cursor, delay, c_gomove, nullptr);
		_goscreen->runAction(seq);
	}
	else if (3 == a->getCollisionBitmask() || 3 == b->getCollisionBitmask() && (5 == (a->getCollisionBitmask() | b->getCollisionBitmask()))) {
		bgm->playEffect("sfx_bounce.wav");
	}

	if ((a->getContactTestBitmask() == 2 && b->getContactTestBitmask() == 3) || (b->getContactTestBitmask() == 2 && a->getContactTestBitmask() == 3)) {
		//(5 == a->getCollisionBitmask() && 6 == b->getCollisionBitmask()) || (6 == a->getCollisionBitmask() && 5 == a->getCollisionBitmask())
		for (int i = 0; i < vdanmaku.size(); i++) {
			vdanmaku.at(i)->setVelocity(Vec2(0, 0));
			vdanmaku.at(i)->setGravityEnable(false);
		}
		bgm->playEffect("sfx_pichuun.wav");
		spriteBody->setGravityEnable(false);
		spriteBody->setVelocity(Vec2(0, 0));
		start = false;
		gameover = true;

		auto delay = DelayTime::create(1.0f);
		auto f_cursor = CallFunc::create([=]() {
			_cursor->runAction(FadeIn::create(1.0f));
			_curref->runAction(FadeIn::create(1.0f));
		});
		auto c_gomove = CallFunc::create([=]() {
			gomove = true;
		});
		auto seq = Sequence::create(FadeIn::create(1.0f), f_cursor, delay, c_gomove, nullptr);
		_goscreen->runAction(seq);

		return true;
	}
	else if ((3 == a->getContactTestBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 3 == b->getContactTestBitmask())) {
		bgm->playEffect("sfx_graze.wav");
		return false;
	}
	else if (3 == a->getContactTestBitmask() && 4 == b->getCollisionBitmask() && _player->getPosition().y-30 > contact.getShapeA()->getBody()->getNode()->getPosition().y) {
		auto danmaku = (Sprite*)contact.getShapeA()->getBody()->getNode();
		danmaku->removeFromParent();
		//bgm->playEffect("sfx_destroy.wav");
		return true;
	}
	else if (3 == b->getContactTestBitmask() && 4 == a->getCollisionBitmask() && _player->getPosition().y-30 > contact.getShapeB()->getBody()->getNode()->getPosition().y) {
		auto danmaku = (Sprite*)contact.getShapeB()->getBody()->getNode();
		danmaku->removeFromParent();
		//bgm->playEffect("sfx_destroy.wav");
		return true;
	}
	else return true;
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode code) {
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

double HelloWorld::keyPressedDuration(EventKeyboard::KeyCode code) {
	if (!isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL))
		return 0;
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - keys[code]).count();
}

void HelloWorld::update(float delta) {
	//player movement update
	//Node::update(delta);
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
		if (_player->getPosition().x > 16 && isKeyPressed(EventKeyboard::KeyCode::KEY_SHIFT) && gameover == false && start == true) _player->setPosition(_player->getPosition().x - 8, _player->getPosition().y);
		else if (_player->getPosition().x > 16 && gameover == false && start == true) _player->setPosition(_player->getPosition().x - 4, _player->getPosition().y);
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
		if (_player->getPosition().x < 624 && isKeyPressed(EventKeyboard::KeyCode::KEY_SHIFT) && gameover == false && start == true) _player->setPosition(_player->getPosition().x + 8, _player->getPosition().y);
		else if (_player->getPosition().x < 624 && gameover == false && start == true) _player->setPosition(_player->getPosition().x + 4, _player->getPosition().y);
	}

	//delay + danmaku (1 minute)
	if (delay > 0 && dtime == false && gameover == false) delay--;
	else dtime = true;
	if (dtime == true && gameover == false) {
		delay++;
	}
	if (delay % 60 == 0 && dtime == true && gameover == false) {
		_danmaku = Sprite::create("level_danmaku.png");
		auto danmakuBody = PhysicsBody::createBox(_danmaku->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		danmakuBody->getShape(0)->setDensity(0.0f);
		danmakuBody->getShape(0)->setFriction(0.0f);
		danmakuBody->getShape(0)->setRestitution(0.0f);
		danmakuBody->setDynamic(true);
		_danmaku->setPosition(32 * (rand() % 20) + 16, 480);
		danmakuBody->setCollisionBitmask(false);
		danmakuBody->setContactTestBitmask(3);
		_danmaku->setPhysicsBody(danmakuBody);
		_danmaku->setTag(5);
		_overhead->addChild(_danmaku);
		danmakuBody->applyImpulse(Vec2(0, -100.0f));
		vdanmaku.pushBack(danmakuBody);
	}
}
// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> HelloWorld::keys;
