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


	auto label = Label::createWithSystemFont("return", "fonts/arial.ttf", 30);

	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(SET::enterHelloWorldScene, this));
	menuitem->setPosition(Vec2(0, -250));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

	MenuItemFont::setFontSize(42);
	MenuItemFont* item1 = MenuItemFont::create("Easy", CC_CALLBACK_1(SET::menuItem1Callback, this));
	MenuItemFont* item2 = MenuItemFont::create("Normal", CC_CALLBACK_1(SET::menuItem2Callback, this));
	MenuItemFont* item3 = MenuItemFont::create("Hard", CC_CALLBACK_1(SET::menuItem3Callback, this));              //设置三个菜单项
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
	speed = 1.0;
}
void SET::menuItem2Callback(Ref* pSender)
{
	speed = 0.5;
}
void SET::menuItem3Callback(Ref* pSender)
{
	speed = 0.3;
}


