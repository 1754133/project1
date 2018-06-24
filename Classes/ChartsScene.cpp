#include "ChartsScene.h"
#include "HelloWorldScene.h"
#include "UserData.h"
USING_NS_CC;
ChartsScene::ChartsScene() :
	_newScoreTitle(nullptr),
	_newScore(nullptr),
	_chartScoreTitle(nullptr)
{

}

ChartsScene::~ChartsScene()
{

}

bool ChartsScene::init()
{
	if (Layer::init() == false) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("timg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x+140, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);

	auto label = Label::create("return", "fonts/airstrikeacad.ttf", 30);                    //返回                  

	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(ChartsScene::enterHelloWorldScene, this));
	menuitem->setPosition(Vec2(0, -250));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

	_newScoreTitle = Sprite::create("newscore.png");
	_newScoreTitle->setAnchorPoint(Vec2(0, 0.5f));
	_newScoreTitle->setPosition(Vec2(10, visibleSize.height / 2 + 300));
	addChild(_newScoreTitle);

	_newScore = ui::Text::create("0", "fonts/airstrikeacad.ttf", 20);
	_newScore->setPosition(Vec2(visibleSize.width / 2-360, visibleSize.height / 2 + 200));
	auto newScore = UserDefault::getInstance()->getIntegerForKey(NEW_SCORE, 0);
	_newScore->setString(StringUtils::format("%d", newScore));
	this->addChild(_newScore);

	_chartScoreTitle = Sprite::create("chartsscore.png");
	_chartScoreTitle->setAnchorPoint(Vec2(0, 0.5));
	_chartScoreTitle->setPosition(Vec2(10, visibleSize.height / 2 + 120));
	addChild(_chartScoreTitle);

	for (int i = 0; i < 5; i++) {                        //只显示前五名
		int score = UserDefault::getInstance()->getIntegerForKey( StringUtils::format("%s%d",RANK_SCORE,i).c_str(),0);

		auto row = createChart(i, score);
		row->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50 - 60 * i));   //设置相邻排名的间距

		addChild(row);

	}
	return true;
}

Node*ChartsScene::createChart(int rank, int score)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto row = Node::create();
	auto r = ui::Text::create(StringUtils::format("%d", rank + 1), "fonts/airstrikeacad.ttf", 15);
	auto s = ui::Text::create(StringUtils::format("%d", score), "fonts/airstrikeacad.ttf", 15);

	r->setAnchorPoint(Vec2(0, 0.5));
	s->setAnchorPoint(Vec2(1, 0.5));
	r->setPosition(Vec2(30, 0));
	s->setPosition(Vec2(visibleSize.width - 800, 0));
	row->addChild(r);
	row->addChild(s);

	row->setContentSize(Size(visibleSize.width, 100));
	row->setAnchorPoint(Vec2(0.5, 0.5));

	return row;
}

Scene*ChartsScene::createScene()
{
	auto  Layer = ChartsScene::create();
	auto scene = Scene::create();

	scene->addChild(Layer);
	return scene;
}
void ChartsScene::enterHelloWorldScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
	SimpleAudioEngine::getInstance()->playEffect("return.mp3");
}