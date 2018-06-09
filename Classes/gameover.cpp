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



	auto label1 = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 88);
	label1->setColor(Color3B(245, 30, 220));
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(label1, 1);

	char score[20] = { 0 };
	sprintf(score, "FINALSCORE:%d", CurScore);
	gamescore = Label::createWithTTF(score, "fonts/Marker Felt.ttf", 55);
	gamescore->setColor(Color3B(245, 100, 220));
	gamescore->setAnchorPoint(Vec2(0.5, 0.5));
	gamescore->setPosition(Vec2(origin.x + visibleSize.width / 2, 200));
	this->addChild(gamescore, 1);

	auto label = Label::createWithSystemFont("return", "fonts/arial.ttf", 30);

	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(gameover::enterHelloWorldScene, this));
	menuitem->setPosition(Vec2(0, -250));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);


	return true;
}

void gameover::enterHelloWorldScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());

}
