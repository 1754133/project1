#include "help.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
using namespace std;
USING_NS_CC;

Scene* help::createScene()
{
	auto scene = Scene::create();

	auto layer = help::create();

	scene->addChild(layer);

	return scene;
}

bool help::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(help::KeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(help::KeyReleased, this);
	/*listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event * event) {
		log("KeyPress:%d", keyCode);
	};
	listener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event * event) {
		log("KeyRelease:%d", keyCode);
	};*/
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//²Ù×÷ËµÃ÷
	auto sprite1 = Sprite::create("stop.jpg");
	sprite1->setPosition(Vec2(200,600));
	this->addChild(sprite1, 0);
	auto label1 = Label::createWithTTF("stop", "fonts/airstrikeacad.ttf", 32);
	label1->setPosition(Vec2(340,600));
	this->addChild(label1,1);

	auto sprite2 = Sprite::create("continue.jpg");
	sprite2->setPosition(Vec2(200, 550));
	this->addChild(sprite2, 0);
	auto label2 = Label::createWithTTF("continue", "fonts/airstrikeacad.ttf", 32);
	label2->setPosition(Vec2(380, 550));
	this->addChild(label2, 1);

	auto sprite3 = Sprite::create("change.jpg");
	sprite3->setPosition(Vec2(200, 500));
	this->addChild(sprite3, 0);
	auto label3 = Label::createWithTTF("change the type", "fonts/airstrikeacad.ttf", 32);
	label3->setPosition(Vec2(460, 500));
	this->addChild(label3, 1);

	auto sprite4 = Sprite::create("left.jpg");
	sprite4->setPosition(Vec2(200, 450));
	this->addChild(sprite4, 0);
	auto label4 = Label::createWithTTF("left", "fonts/airstrikeacad.ttf", 32);
	label4->setPosition(Vec2(300, 450));
	this->addChild(label4, 1);

	auto sprite5 = Sprite::create("right.jpg");
	sprite5->setPosition(Vec2(200, 400));
	this->addChild(sprite5, 0);
	auto label5 = Label::createWithTTF("right", "fonts/airstrikeacad.ttf", 32);
	label5->setPosition(Vec2(300, 400));
	this->addChild(label5, 1);

	auto sprite6 = Sprite::create("down.jpg");
	sprite6->setPosition(Vec2(200, 350));
	this->addChild(sprite6, 0);
	auto label6 = Label::createWithTTF("down", "fonts/airstrikeacad.ttf", 32);
	label6->setPosition(Vec2(300, 350));
	this->addChild(label6, 1);

	auto label = Label::create("return", "fonts/airstrikeacad.ttf", 30);
	
	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(help::enterHelloWorldScene, this));
	menuitem->setPosition(Vec2(0, -250));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

	auto background = Sprite::create("timg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);

	return true;
}

void help::KeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}

void help::KeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d released", keyCode);
}

void help::enterHelloWorldScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("return.mp3");
	Director::getInstance()->replaceScene(HelloWorld::createScene());

}