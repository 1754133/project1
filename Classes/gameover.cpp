#include "gameover.h"
#include "gameplay.h"
#include "HelloWorldScene.h"
#include "canshu.h"
using namespace std;
USING_NS_CC;

gameover::gameover()
{
	gamescore = NULL;
}

gameover::~gameover()
{

}

Scene* gameover::createScene()
{
	auto scene = Scene::create();

	auto layer = gameover::create();

	scene->addChild(layer);

	return scene;
}

bool gameover::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("background1.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 ));
	this->addChild(background, -1);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgmusic.mp3");
	SimpleAudioEngine::getInstance()->playEffect("gameover.mp3");

	auto sprite = Sprite::create("gameover.png");
	sprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 155));
	this->addChild(sprite, 1);

	char score[20] = { 0 };
	sprintf(score, "%d", CurScore);
	gamescore = Label::createWithTTF(score, "fonts/Marker Felt.ttf", 55);
	gamescore->setColor(Color3B(245, 245, 220));
	gamescore->setAnchorPoint(Vec2(0, 0));
	gamescore->setPosition(Vec2(origin.x + visibleSize.width / 2, 275));
	this->addChild(gamescore, 1);

	auto sprite1 = Sprite::create("score3.png");
	sprite1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 130, 310));
	this->addChild(sprite1, 1);

	MenuItemImage* item1 = MenuItemImage::create("newgame1.png", "newgame2.png", CC_CALLBACK_1(gameover::entergameplayScene, this));
	MenuItemImage* item2 = MenuItemImage::create("mainmenu3.png", "mainmenu4.png", CC_CALLBACK_1(gameover::enterHelloWorldScene, this));
	Menu* mn = Menu::create(item1, item2, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
	this->addChild(mn);

	return true;
}

void gameover::enterHelloWorldScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("return.mp3");
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}
void gameover::entergameplayScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("return.mp3");
	Director::getInstance()->replaceScene(gameplay::createScene());
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3", true);
	isPause = false;
}
