#include "set.h"
#include "HelloWorldScene.h"
#include "canshu.h"
using namespace std;
USING_NS_CC;
int dif = 0;

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

	
	MenuItemImage* item1 = MenuItemImage::create("easy.png","easy.png", CC_CALLBACK_1(SET::menuItem1Callback, this));
	MenuItemImage* item2 = MenuItemImage::create("normal.png","normal.png", CC_CALLBACK_1(SET::menuItem2Callback, this));
	MenuItemImage* item3 = MenuItemImage::create("hard.png","hard.png", CC_CALLBACK_1(SET::menuItem3Callback, this));              //设置三个菜单项
	Menu* mn = Menu::create(item1, item2, item3, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);

	dif = 0;

	return true;
}

void SET::enterHelloWorldScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("return.mp3");
	Director::getInstance()->replaceScene(HelloWorld::createScene());

}


void SET::menuItem1Callback(Ref* pSender)
{
	if (dif == 0) {
		SimpleAudioEngine::getInstance()->playEffect("choose.mp3");
		speed = 0.4;
		Sprite * sprite = Sprite::create("pointer.png");
		sprite->setPosition(ccp(635, 390));
		this->addChild(sprite);
		dif = 1;
	}

}
void SET::menuItem2Callback(Ref* pSender)
{
	if (dif == 0) {
		SimpleAudioEngine::getInstance()->playEffect("choose.mp3");
		speed = 0.2;
		Sprite * sprite = Sprite::create("pointer.png");
		sprite->setPosition(ccp(635, 350));
		this->addChild(sprite);
		dif = 1;
	}
}
void SET::menuItem3Callback(Ref* pSender)
{
	if (dif == 0) {
		SimpleAudioEngine::getInstance()->playEffect("choose.mp3");
		speed = 0.09;
		Sprite * sprite = Sprite::create("pointer.png");
		sprite->setPosition(ccp(635, 300));
		this->addChild(sprite);
		dif = 1;
	}
}


