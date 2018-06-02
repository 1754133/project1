#include "help.h"
#include "HelloWorldScene.h"
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
	auto label = Label::createWithSystemFont("return", "fonts/arial.ttf", 30);
	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(help::enterHelloWorldScene, this));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

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
	Director::getInstance()->replaceScene(HelloWorld::createScene());

}