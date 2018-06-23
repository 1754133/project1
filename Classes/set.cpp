#include "set.h"
#include "HelloWorldScene.h"
#include "canshu.h"
using namespace std;
USING_NS_CC;


Scene* SET::createScene()
{
	auto scene = Scene::create();

	auto layer = SET::create();

	scene->addChild(layer);

	return scene;
}


bool SET::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto label = Label::create("OK", "fonts/airstrikeacad.ttf", 60);

	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(SET::enterHelloWorldScene, this));
	menuitem->setPosition(Vec2(0, -250));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

	auto background = Sprite::create("timg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);
	sprite1 = Sprite::create("pointer.png");
	sprite1->setPosition(Vec2(635, 440));
	this->addChild(sprite1);
	MenuItemImage* item1 = MenuItemImage::create("easy.png","easy.png", CC_CALLBACK_1(SET::menuItem1Callback, this));
	MenuItemImage* item2 = MenuItemImage::create("normal.png","normal.png", CC_CALLBACK_1(SET::menuItem2Callback, this));
	MenuItemImage* item3 = MenuItemImage::create("hard.png","hard.png", CC_CALLBACK_1(SET::menuItem3Callback, this));              //设置三个菜单项
	Menu* mn = Menu::create(item1, item2, item3, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);


	return true;
}

void SET::enterHelloWorldScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());

}


void SET::menuItem1Callback(Ref* pSender)
{
		speed = 0.5;
		basicScore = 50;
		auto moveTo = MoveTo::create(0, Vec2(635, 380));
		sprite1->runAction(moveTo);
}
void SET::menuItem2Callback(Ref* pSender)
{
		speed = 0.3;
		basicScore = 100;
		auto moveTo = MoveTo::create(0, Vec2(635, 340));
		sprite1->runAction(moveTo);
}
void SET::menuItem3Callback(Ref* pSender)
{
		speed = 0.1;
		basicScore = 200;
		auto moveTo = MoveTo::create(0, Vec2(635, 300));
		sprite1->runAction(moveTo);
}


