#include "gameplay.h"
#include "HelloWorldScene.h"
#include "set.h"
#include "canshu.h"
#include "gameover.h"
#include "SimpleAudioEngine.h"
#include "Userdata.h"            

using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

gameplay::gameplay()                           //��ʼ������
{
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			Square[i][j] = NULL;
		}
	}
	//��������Ԥ����
	for (int i = 0; i < 4; i++)
	{
		for (int j = COLUME + 5; j < COLUME + 9; j++)
		{
			square[i][j - COLUME - 5] = Sprite::create("gamesquare.png");
			square[i][j - COLUME - 5]->setPosition(Director::getInstance()->convertToGL(Vec2(j * 30 + j * 2 + 20, i * 30 + i * 2 + 20)));      //��opengl����ת��Ϊul����
			square[i][j - COLUME - 5]->setColor(Color3B(255, 255, 255));
			this->addChild(square[i][j - COLUME - 5]);
		}
	}
	gameScore = NULL;
	CurScore = 0;
	nextsquare = rand() % 19 + 1;              
	newSquareType();                   //������ɵ�һ��ͼ��
}

gameplay::~gameplay()
{

}



Scene* gameplay::createScene()
{
	Scene* scene = Scene::create();

	gameplay *layer = gameplay::create();

	scene->addChild(layer);

	return scene;
}

bool gameplay::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//���÷�����ʾ��λ��
	char score[20] = { 0 };              //�������������
	CurScore = 0;
	sprintf(score, "SCORE:%d", CurScore);
	gameScore = Label::createWithTTF(score, "fonts/airstrikeacad.ttf", 32);
	gameScore->setColor(Color3B(245, 30, 220));
	gameScore->setAnchorPoint(Vec2(0, 0));
	gameScore->setPosition(Vec2(500, 400));
	this->addChild(gameScore, 1);
	//��������˹������Ϸ���򣨰�ɫ���飩
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			Square[i][j] = Sprite::create("gamesquare.png");
			Square[i][j]->setPosition(Director::getInstance()->convertToGL(Vec2(j * 30 + j * 2 + 20, i * 30 + i * 2 + 34)));      //��opengl����ת��Ϊul����
			Square[i][j]->setTag(0);
			Square[i][j]->setColor(Color3B(255, 255, 255));	
			this->addChild(Square[i][j]);
		}
	}
	this->schedule(schedule_selector(gameplay::updateDown), speed);


	auto background = Sprite::create("gameplaybg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x + 200, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);

	auto frame = Sprite::create("frame.png");             //Ϊ��Ϸ������ӱ߿�
	frame->setPosition(Vec2(196, 354));
	this->addChild(frame);

	auto frame2 = Sprite::create("frame2.png");             //ΪԤ�ⷽ�������ӱ߿�
	frame2->setPosition(Vec2(612, 630));
	this->addChild(frame2);

	MenuItemImage* item = MenuItemImage::create("return.png", "return2.png", CC_CALLBACK_1(gameplay::menuItemCallback, this));
	Menu* mn = Menu::create(item, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(940, 650));
	this->addChild(mn);

	auto turnOn = MenuItemImage::create("button_voi_on.png", "button_voi_on.png");
	auto turnOff = MenuItemImage::create("button_voi_off.png", "button_voi_off.png");
	auto toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(gameplay::menuMusicCallback, this), turnOn, turnOff, NULL);
	toggleItem->setPosition(Vec2(940, 150));
	auto menu = Menu::create(toggleItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	auto pause = MenuItemImage::create("pause.png", "pause.png");
	auto play = MenuItemImage::create("play.png", "play.png");
	auto toggleItem1 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(gameplay::menuStartCallback, this), pause, play, NULL);
	toggleItem1->setPosition(Vec2(940, 550));
	auto menu1 = Menu::create(toggleItem1, NULL);
	menu1->setPosition(Point::ZERO);
	this->addChild(menu1);

	auto next = Sprite::create("next.png");
	next->setPosition(Vec2(470, 600));
	this->addChild(next, -1);

	auto label1 = Label::create("Next:", "fonts/Marker Felt.ttf", 40);
	label1->setPosition(Vec2(470, 600));
	label1->setColor(Color3B(148, 0, 211));
	this->addChild(label1);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(gameplay::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(gameplay::keyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->schedule(schedule_selector(gameplay::update), 0.1);                //ѭ������update

	return true;
}


bool gameplay::isKeyPressed(EventKeyboard::KeyCode keyCode)              //����������ڰ���״̬���򷵻�true,���򷵻�false
{
	if (keys[keyCode])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void gameplay::update(float delta)                                //������̰�ס�¼�
{
	Node::update(delta);
	auto downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(downArrow)) {
		updateDown(0.0);
	}
}

void gameplay::keyPressed(EventKeyboard::KeyCode keyCode, Event *event)           //���̴�����
{
	keys[keyCode] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)         //ÿ������һ��
	{
		updateDown(0.0);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)           //��ͣ
	{
		Director::getInstance()->pause();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)               //�ָ�
	{
		Director::getInstance()->resume();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)             //����
	{
		changeSquareType();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)            //������һ��
	{
		updateLeft();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)           //������һ��
	{
		updateRight();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)                  //ֱ�����䵽�����������λ��
	{
		down();
	}
}
void gameplay::keyReleased(EventKeyboard::KeyCode keyCode,Event *event)
{
	keys[keyCode] = false;
}


void gameplay::enterHelloWorldScene(Ref* pSender)
{ 
	Director::getInstance()->resume();                                     //���������ͣ״̬��ָ�
	Director::getInstance()->replaceScene(HelloWorld::createScene());      //�ص����˵�
}


void gameplay::updateScore()                        //���·�������
{
	char score[20] = { 0 };
	sprintf(score, "SCORE:%d", CurScore);
	gameScore->setString(score);
}

void gameplay::gameover()
{
	Director::getInstance()->replaceScene(gameover::createScene());
	UserDefault::getInstance()->setIntegerForKey(NEW_SCORE, CurScore);              //���а����
	std::vector<int> scoreList;
	scoreList.push_back(CurScore);
	for (int i = 0; i < 5; i++) {
		int score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, i).c_str(), 0);
		scoreList.push_back(score);
	}

	std::sort(scoreList.begin(), scoreList.end(), [](int &a, int &b) {
		return a > b;
	});      //����ǰ������ݲ���������

	int rank = 0;
	for (auto i = scoreList.begin(); i != scoreList.end(); i++) {
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, rank).c_str(), *i);
		rank++;
	}
}

void gameplay::down()
{
	int min,sum;
	switch (CurSquareType)
	{
	case 1:
		if (Curline < 1)             //�ж��ܷ���ж���
			break;
		min = 21;                         //��ʼ����Сֵ
		for (int j = Curcolume; j < Curcolume + 4; j++)            //�Ƚ�һ�����ĸ������������·��ķǰ�ɫ����ȽϽ�
		{
			sum = 0;                                               //��ʼ������
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		for (int j = Curcolume; j < Curcolume + 4; j++)                                  //����ԭ����ͼ��
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		for (int j = Curcolume; j < Curcolume + 4; j++)                                    //��ʾ�µ�ͼ�Σ�ʵ��ͼ������
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(52, 228, 249));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 2:
		if (Curline < 4)
			break;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		for (int i = Curline - 4; i < Curline; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		for (int i = Curline - 4 + sum; i < Curline + sum; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(52, 228, 249));
			Square[i][Curcolume]->setTag(1);
		}
		Curline = Curline + sum;
		break;
	case 3:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		Square[Curline - 2 + min][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2 + min][Curcolume]->setTag(1);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(245, 30, 217));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 4:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline - 2; i < LINE - 2; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline + 2;
				break;
			}
			if (i == LINE - 3)
				sum = i - Curline + 3;
		}
		if (sum <= min)
			min = sum;
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		Square[Curline - 3 + min][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 3 + min][Curcolume + 1]->setTag(1);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[i][Curcolume]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 5:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			sum = 0;
			for (int i = Curline - 1; i < LINE - 1; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline + 1;
					break;
				}
				if (i == LINE - 2)
					sum = i - Curline + 2;
			}
			if (sum <= min)
				min = sum;
		}
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume + 2]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][j]->setTag(0);
		}
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2 + min][j]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + min][j]->setTag(1);
		}
		Square[Curline -1 + min][Curcolume + 2]->setColor(Color3B(245, 30, 217));
		Square[Curline -1 + min][Curcolume + 2]->setTag(1);
		Curline = Curline + min;
		break;
	case 6:
		if (Curline < 3)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[i][Curcolume + 1]->setTag(1);
		}
		Square[Curline - 1 + min][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1 + min][Curcolume]->setTag(1);
		Curline = Curline + min;
		break;
	case 7:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		Square[Curline - 2 + min][Curcolume + 2]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2 + min][Curcolume + 2]->setTag(1);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(245, 30, 217));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 8:
		if (Curline < 3)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[i][Curcolume]->setTag(1);
		}
		Square[Curline - 1 + min][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1 + min][Curcolume + 1]->setTag(1);
		Curline = Curline + min;
		break;
	case 9:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			sum = 0;
			for (int i = Curline - 1; i < LINE - 1; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline + 1;
					break;
				}
				if (i == LINE - 2)
					sum = i - Curline + 2;
			}
			if (sum <= min)
				min = sum;
		}
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][j]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2 + min][j]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + min][j]->setTag(1);
		}
		Square[Curline - 1 + min][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1 + min][Curcolume]->setTag(1);
		Curline = Curline + min;
		break;
	case 10:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline - 2; i < LINE - 2; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline + 2;
				break;
			}
			if (i == LINE - 3)
				sum = i - Curline + 3;
		}
		if (sum <= min)
			min = sum;
		Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 3 + min][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline - 3 + min][Curcolume]->setTag(1);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[i][Curcolume + 1]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 11:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume + 2]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			Square[Curline - 2][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][j]->setTag(0);
		}
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			Square[Curline - 2 + min][j]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 12:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		for (int i = Curline - 3; i < Curline - 1; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		for (int i = Curline - 2; i < Curline; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume + 1]->setTag(0);
		}
		for (int i = Curline - 3 + min; i < Curline - 1 + min; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(26, 242, 26));
			Square[i][Curcolume]->setTag(1);
		}
		for (int i = Curline - 2 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(26, 242, 26));
			Square[i][Curcolume + 1]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 13:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			Square[Curline - 2][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][j]->setTag(0);
		}
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			Square[Curline - 2 + min][j]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2 + min][j]->setTag(1);
		}
		for (int j = Curcolume + 1; j < Curcolume + 3; j++)
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 14:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		for (int i = Curline - 3; i < Curline - 1; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume + 1]->setTag(0);
		}
		for (int i = Curline - 2; i < Curline; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		for (int i = Curline - 3 + min; i < Curline - 1 + min; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(26, 242, 26));
			Square[i][Curcolume + 1]->setTag(1);
		}
		for (int i = Curline - 2 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(26, 242, 26));
			Square[i][Curcolume]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 15:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][j]->setTag(0);
		}
		Square[Curline - 2 + min][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2 + min][Curcolume + 1]->setTag(1);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 1 + min][j]->setColor(Color3B(233, 178, 11));
			Square[Curline - 1 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 16:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume]->setColor(Color3B(233, 178, 11));
			Square[i][Curcolume]->setTag(1);
		}
		Square[Curline - 2 + min][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2 + min][Curcolume + 1]->setTag(1);
		Curline = Curline + min;
		break;
	case 17:
		if (Curline < 2)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume + 2]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2][j]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][j]->setTag(0);
		}
		Square[Curline - 1 + min][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 1 + min][Curcolume + 1]->setTag(1);
		for (int j = Curcolume; j < Curcolume + 3; j++)
		{
			Square[Curline - 2 + min][j]->setColor(Color3B(233, 178, 11));
			Square[Curline - 2 + min][j]->setTag(1);
		}
		Curline = Curline + min;
		break;
	case 18:
		if (Curline < 3)
			break;
		min = 21;
		sum = 0;
		for (int i = Curline; i < LINE; i++)
		{
			if (Square[i][Curcolume + 1]->getTag() == 1)
			{
				sum = i - Curline;
				break;
			}
			if (i == LINE - 1)
				sum = i - Curline + 1;
		}
		if (sum <= min)
			min = sum;
		sum = 0;
		for (int i = Curline - 1; i < LINE - 1; i++)
		{
			if (Square[i][Curcolume]->getTag() == 1)
			{
				sum = i - Curline + 1;
				break;
			}
			if (i == LINE - 2)
				sum = i - Curline + 2;
		}
		if (sum <= min)
			min = sum;
		for (int i = Curline - 3; i < Curline; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		for (int i = Curline - 3 + min; i < Curline + min; i++)
		{
			Square[i][Curcolume + 1]->setColor(Color3B(233, 178, 11));
			Square[i][Curcolume + 1]->setTag(1);
		}
		Square[Curline - 2 + min][Curcolume]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2 + min][Curcolume]->setTag(1);
		Curline = Curline + min;
		break;
	case 19:
		if (Curline < 2)
			break;
		min = 21;
		for (int j = Curcolume; j < Curcolume + 2; j++)
		{
			sum = 0;
			for (int i = Curline; i < LINE; i++)
			{
				if (Square[i][j]->getTag() == 1)
				{
					sum = i - Curline;
					break;
				}
				if (i == LINE - 1)
					sum = i - Curline + 1;
			}
			if (sum <= min)
				min = sum;
		}
		for (int i = Curline - 2; i < Curline; i++)
		{
			for (int j = Curcolume; j < Curcolume + 2; j++)
			{
				Square[i][j]->setColor(Color3B(255, 255, 255));
				Square[i][j]->setTag(0);
			}
		}
		for (int i = Curline - 2 + min; i < Curline + min; i++)
		{
			for (int j = Curcolume; j < Curcolume + 2; j++)
			{
				Square[i][j]->setColor(Color3B(244, 69, 46));
				Square[i][j]->setTag(1);
			}
		}
		Curline = Curline + min;
		break;
	}
}

void gameplay::checkgameover()
{
	switch (CurSquareType)
	{
	case 1:
		if (Curline  < 1)
		gameover();
		break;
	case 2:
		if (Curline  < 4)
		gameover();
		break;
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
	case 19:
		if (Curline  < 2)
		gameover();
		break;
	case 4:
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
		if (Curline  < 3)
	    gameover();
		break;
	}
}

void gameplay::nextSquareType()                                    //����Ԥ�����ʾ
{
	nextsquare = rand() % 19 + 1;
	switch (nextsquare)
	{
	case 1:
		for (int j = 0; j < 4; j++)
		{
			square[1][j]->setColor(Color3B(52, 228, 249));
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++)
		{
			square[i][1]->setColor(Color3B(52, 228, 249));
		}
		break;
	case 3:
		square[1][0]->setColor(Color3B(245, 30, 217));
		for (int j = 0; j < 3; j++)
		{
			square[2][j]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 4:
		square[0][2]->setColor(Color3B(245, 30, 217));
		for (int i = 0; i < 3; i++)
		{
			square[i][1]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 5:
		square[2][2]->setColor(Color3B(245, 30, 217));
		for (int j = 0; j < 3; j++)
		{
			square[1][j]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 6:
		square[2][1]->setColor(Color3B(245, 30, 217));
		for (int i = 0; i < 3; i++)
		{
			square[i][2]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 7:
		square[1][2]->setColor(Color3B(245, 30, 217));
		for (int j = 0; j < 3; j++)
		{
			square[2][j]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 8:
		square[2][2]->setColor(Color3B(245, 30, 217));
		for (int i = 0; i < 3; i++)
		{
			square[i][1]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 9:
		square[2][0]->setColor(Color3B(245, 30, 217));
		for (int j = 0; j < 3; j++)
		{
			square[1][j]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 10:
		square[0][1]->setColor(Color3B(245, 30, 217));
		for (int i = 0; i < 3; i++)
		{
			square[i][2]->setColor(Color3B(245, 30, 217));
		}
		break;
	case 11:
		for (int j = 0; j < 2; j++)
		{
			square[2][j]->setColor(Color3B(26, 242, 26));
		}
		for (int j = 1; j < 3; j++)
		{
			square[1][j]->setColor(Color3B(26, 242, 26));
		}
		break;
	case 12:
		for (int i = 0; i < 2; i++)
		{
			square[i][1]->setColor(Color3B(26, 242, 26));
		}
		for (int i = 1; i < 3; i++)
		{
			square[i][2]->setColor(Color3B(26, 242, 26));
		}
		break;
	case 13:
		for (int j = 0; j < 2; j++)
		{
			square[1][j]->setColor(Color3B(26, 242, 26));
		}
		for (int j = 1; j < 3; j++)
		{
			square[2][j]->setColor(Color3B(26, 242, 26));
		}
		break;
	case 14:
		for (int i = 0; i < 2; i++)
		{
			square[i][2]->setColor(Color3B(26, 242, 26));
		}
		for (int i = 1; i < 3; i++)
		{
			square[i][1]->setColor(Color3B(26, 242, 26));
		}
		break;
	case 15:
		square[1][1]->setColor(Color3B(233, 178, 11));
		for (int j = 0; j < 3; j++)
		{
			square[2][j]->setColor(Color3B(233, 178, 11));
		}
		break;
	case 16:
		square[1][2]->setColor(Color3B(233, 178, 11));
		for (int i = 0; i < 3; i++)
		{
			square[i][1]->setColor(Color3B(233, 178, 11));
		}
		break;
	case 17:
		square[2][1]->setColor(Color3B(233, 178, 11));
		for (int j = 0; j < 3; j++)
		{
			square[1][j]->setColor(Color3B(233, 178, 11));
		}
		break;
	case 18:
		square[1][1]->setColor(Color3B(233, 178, 11));
		for (int i = 0; i < 3; i++)
		{
			square[i][2]->setColor(Color3B(233, 178, 11));
		}
		break;
	case 19:
		for (int i = 1; i < 3; i++)
		{
			for (int j = 1; j < 3; j++)
			{
				square[i][j]->setColor(Color3B(244, 69, 46));
			}
		}
		break;
	}
}

void gameplay::clearSquare()                     //�������Ԥ�����е�ͼ��
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			square[i][j]->setColor(Color3B(255, 255, 255));
		}
	}
}


void gameplay::newSquareType()
{
	
	clearSquare();                       //�������Ԥ�����е�ͼ��
	CurSquareType = nextsquare;          //����һ������Ԥ�⸳ֵ����ǰ����
	nextSquareType();                   //Ԥ����һ�����ֵ�ͼ��
	switch (CurSquareType)
	{
	case 1:
	case 3:
	case 4:
	case 9:
	case 10:
	case 11:
	case 12:
	case 15:
	case 16:
		Curline = 0;
		Curcolume = 3;
		break;
	case 2:
	case 5:
	case 6:
	case 7:
	case 8:
	case 13:
	case 14:
	case 17:
	case 18:
	case 19:
		Curline = 0;
		Curcolume = 4;
		break;
	}
}

void gameplay::changeSquareType()
{
	switch (CurSquareType)
	{
	case 1:
		//�ж��Ƿ���Ա任
		if (Curline + 1 > LINE - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && Curline - 2 + i > -1 && Square[Curline - 2 + i][Curcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			if (i != 1 && Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(52, 228, 249));
				Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			}
		}

		Curline += 2;
		Curcolume++;
		CurSquareType = 2;
		break;
	case 2:
		//�ж��Ƿ���Ա任
		if (Curcolume < 1 || Curcolume + 2 > COLUME - 1 || Curline < 2)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && Curline > 2 && Square[Curline - 3][Curcolume - 1 + i]->getTag() == 1)
			{
				return;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && Curline - 4 + i > -1)
			{
				Square[Curline - 4 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 4 + i][Curcolume]->setTag(0);
			}
			if (i != 1 && Curline > 2)
			{
				Square[Curline - 3][Curcolume - 1 + i]->setColor(Color3B(52, 228, 249));
				Square[Curline - 3][Curcolume - 1 + i]->setTag(1);
			}
		}

		Curline = Curline - 2;
		Curcolume--;
		CurSquareType = 1;
		break;
	case 3:
		//�ж��Ƿ���Ա任
		if (Curline > LINE - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 1 && Square[Curline - 2][Curcolume + 1 + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume + 2]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline][Curcolume + 1]->setTag(1);
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(Color3B(245, 30, 217));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(1);
			}
		}

		Curline++;
		Curcolume++;
		CurSquareType = 4;
		break;
	case 4:
		//�ж��Ƿ���Ա任
		if (Curcolume < 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1 && Square[Curline - 2 + i][Curcolume + 1]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline > 2)
			{
				Square[Curline - 3][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(245, 30, 217));
				Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			}
		}

		Curcolume--;
		CurSquareType = 5;
		break;
	case 5:
		//�ж��Ƿ���Ա任
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 0 && Square[Curline - 1][Curcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 2 && Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		if (Curline > 2)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3][Curcolume + 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(245, 30, 217));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}

		CurSquareType = 6;
		break;
	case 6:
		//�ж��Ƿ���Ա任
		if (Curcolume + 2 > COLUME - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1 && Square[Curline - 3 + i][Curcolume]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline > 2)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(Color3B(245, 30, 217));
				Square[Curline - 3 + i][Curcolume]->setTag(1);
			}
		}

		Curline--;
		CurSquareType = 3;
		break;
	case 7:
		//�ж��Ƿ���Ա任
		if (Curline > LINE - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + 1 + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + 1 + i]->setColor(Color3B(245, 30, 217));
			Square[Curline][Curcolume + 1 + i]->setTag(1);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}

		Curline++;
		Curcolume++;
		CurSquareType = 8;
		break;
	case 8:
		//�ж��Ƿ���Ա任
		if (Curcolume < 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1 && Square[Curline - 2 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline > 2)
		{
			Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume - 1]->setColor(Color3B(245, 30, 217));
				Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
			}
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 9;
		break;
	case 9:
		//�ж��Ƿ���Ա任
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 2 && Square[Curline - 3][Curcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 0 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 2)
			{
				Square[Curline - 3][Curcolume + i]->setColor(Color3B(245, 30, 217));
				Square[Curline - 3][Curcolume + i]->setTag(1);
			}
		}
		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		CurSquareType = 10;
		break;
	case 10:
		//�ж��Ƿ���Ա任
		if (Curcolume + 2 > COLUME - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1 && Square[Curline - 3 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline  > 2)
			{
				Square[Curline - 3][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume + 2]->setColor(Color3B(245, 30, 217));
				Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			}
		}
		Curline--;
		CurSquareType = 7;
		break;
	case 11:
		//�ж��Ƿ���Ա任
		if (Curline > LINE - 1 || Curline == 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 + i > -1 && Square[Curline - 1 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 + i > -1)
			{
				Square[Curline - 1 + i][Curcolume + 2]->setColor(Color3B(26, 242, 26));
				Square[Curline - 1 + i][Curcolume + 2]->setTag(1);
			}
		}

		Curline++;
		Curcolume++;
		CurSquareType = 12;
		break;
	case 12:
		//�ж��Ƿ���Ա任
		if (Curcolume < 1 || Curline < 3)
		{
			return;
		}
		if (Curline > 2 && Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		for (int i =0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 2][Curcolume - 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 3][Curcolume + 1]->setTag(1);
		Curline--;
		Curcolume--;
		CurSquareType = 11;
		break;
	case 13:
		//�ж��Ƿ���Ա任
		if (Curline > LINE - 1 || Curline == 0)
		{
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline > 1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(26, 242, 26));
		Square[Curline][Curcolume + 1]->setTag(1);
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2][Curcolume + 2]->setTag(1);
		}

		Curline++;
		Curcolume++;
		CurSquareType = 14;
		break;
	case 14:
		//�ж��Ƿ���Ա任
		if (Curcolume  < 1 || Curline == 0)
		{
			return;
		}
		if (Curline  > 1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline > 0 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}
		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 13;
		break;
	case 15:
		//�ж��Ƿ���Ա任
		if (Curline > LINE - 1 || Curline == 0)
		{
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);
		Curline++;
		Curcolume++;
		CurSquareType = 16;
		break;
	case 16:
		//�ж��Ƿ���Ա任
		if (Curcolume < 1 || Curline == 0)
		{
			return;
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		if (Curline > 2)
		{
			Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(Color3B(233, 178, 11));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 17;
		break;
	case 17:
		//�ж��Ƿ���Ա任
		if (Curline - 3 > -1 && Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline == 0)
		{
			return;
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		if (Curline > 2)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(233, 178, 11));
			Square[Curline - 3][Curcolume + 1]->setTag(1);
		}

		CurSquareType = 18;
		break;
	case 18:
		//�ж��Ƿ���Ա任
		if (Curcolume + 2 > COLUME - 1 || Curline == 0)
		{
			return;
		}
		if (Curline > 1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		if (Curline > 0)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(233, 178, 11));
			Square[Curline - 2][Curcolume + 2]->setTag(1);
		}

		Curline--;
		CurSquareType = 15;
		break;
	case 19:
		break;
	}
}


void gameplay::clearLine(int nLineStart, int nLineEnd)
{
	checkgameover();//�����Ϸ�Ƿ����

	for (int i = nLineStart; i <= nLineEnd; i++)
	{
		int j;
		for (j = 0; j < COLUME; j++)       //�ж��Ƿ��������
		{
			if (i > -1 && Square[i][j]->getTag() == 0)     //����а�ɫ������������
			{
				break;
			}
		}

		if (COLUME == j)
		{
			//���һ�У��������������θ���
			for (int line = i - 1; line >= 0; line--)
			{
				copyLine(line);
			}
			for (int colume = 0; colume < COLUME; colume++)     //������һ�л���ȫ��ɫ
			{
				Square[0][colume]->setColor(Color3B(255, 255, 255));
				Square[0][colume]->setTag(0);
			}
			SimpleAudioEngine::getInstance()->playEffect("getscore.mp3");
			CurScore=CurScore+basicScore;
		}
	}
	updateScore();                       //���·���
}

void gameplay::copyLine(int nlineNum)
{
	//������һ�е���ɫ�ͱ��
	for (int i = 0; i < COLUME; i++)
	{
		Square[nlineNum + 1][i]->setColor(Square[nlineNum][i]->getColor());
		Square[nlineNum + 1][i]->setTag(Square[nlineNum][i]->getTag());
	}
}

void gameplay::updateDown(float dt)
{
	switch (CurSquareType)     //ÿ�������½�ʱ�������ж����ܷ�����
	{
	case 1:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 1, LINE - 1);              //����һ���ж����Ƿ������
			newSquareType();
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1&&Curline > 0)              //�����һ���б��Ϊ1���ǰ�ɫ�ķ��飬���������� 
			{
				clearLine(Curline - 1, Curline - 1);             //����һ���ж����Ƿ������
				newSquareType();
				return;
			}
		}
		//�½�һ��
		for (int i = 0; i < 4; i++)
		{
			//����ԭɫ
			if (Curline < LINE && Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			//��ʾ��ɫ
			if (Curline < LINE)
			{
				Square[Curline][Curcolume + i]->setColor(Color3B(52, 228, 249));
				Square[Curline][Curcolume + i]->setTag(1);
			}
		}
		Curline++;
		break;
	case 2:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 4, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 4, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		//����ԭɫ
		if (Curline < LINE && Curline > 3)
		{
			Square[Curline - 4][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 4][Curcolume]->setTag(0);
		}
		//��ʾ��ɫ
		if (Curline < LINE)
		{
			Square[Curline][Curcolume]->setColor(Color3B(52, 228, 249));
			Square[Curline][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 3:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 3; i++)
		{
			//����ԭɫ
			if (i != 0 && Curline < LINE && Curline > 0)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			//��ʾ��ɫ
			if (Curline < LINE)
			{
				Square[Curline][Curcolume + i]->setColor(Color3B(245, 30, 217));
				Square[Curline][Curcolume + i]->setTag(1);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}

		Curline++;
		break;
	case 4:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1 ||
			(Curline > 1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1))
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}
		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			//����ԭɫ
			if (Curline > 2)
			{
				Square[Curline - 3][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		//��ʾ��ɫ
		if (Curline > 1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}
		Square[Curline][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline][Curcolume]->setTag(1);

		Curline++;
		break;
	case 5:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume + 2]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			//����ԭɫ
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			//��ʾ��ɫ
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(245, 30, 217));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume + 2]->setColor(Color3B(245, 30, 217));
		Square[Curline][Curcolume + 2]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}

		Curline++;
		break;
	case 6:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 3, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(Color3B(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 7:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline][Curcolume + i]->setColor(Color3B(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}

		Curline++;
		break;
	case 8:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 3, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(Color3B(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 9:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 1 + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			//����ԭɫ
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(0);
			}
			//��ʾ��ɫ
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + 1 + i]->setColor(Color3B(245, 30, 217));
				Square[Curline - 1][Curcolume + 1 + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume]->setColor(Color3B(245, 30, 217));
		Square[Curline][Curcolume]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}

		Curline++;
		break;
	case 10:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1 ||
			(Curline - 2 > -1 && Square[Curline - 2][Curcolume]->getTag() == 1))
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			//����ԭɫ
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		//��ʾ��ɫ
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2][Curcolume]->setTag(1);
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 11:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 2]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			if (Curline > 1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(0);
			}
			Square[Curline][Curcolume + i]->setColor(Color3B(26, 242, 26));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
			Square[Curline - 1][Curcolume + 2]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1][Curcolume + 2]->setTag(1);
		}

		Curline++;
		break;
	case 12:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1][Curcolume]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(26, 242, 26));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 13:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + 1 + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			Square[Curline][Curcolume + 1 + i]->setColor(Color3B(26, 242, 26));
			Square[Curline][Curcolume + 1 + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 1][Curcolume + 2]->setTag(0);
			Square[Curline - 1][Curcolume]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 14:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(26, 242, 26));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume]->setColor(Color3B(26, 242, 26));
		Square[Curline][Curcolume]->setTag(1);

		Curline++;
		break;
	case 15:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//�½�һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline][Curcolume + i]->setColor(Color3B(233, 178, 11));
			Square[Curline][Curcolume + i]->setTag(1);
			if (Curline - 1 > -1 && i != 1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 16:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume]->setColor(Color3B(233, 178, 11));
		Square[Curline][Curcolume]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(Color3B(233, 178, 11));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		Curline++;
		break;
	case 17:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 2]->getTag() == 1)
		{
			clearLine(Curline - 2, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			if (i != 1 && Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(Color3B(233, 178, 11));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 18:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//�½�һ��
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(Color3B(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(Color3B(233, 178, 11));
			Square[Curline - 1][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 19:
		//�ж��Ƿ���������½�
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)         //�����²��������������飬��ʱ����Ƿ��������
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}
		//�½�һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(Color3B(244, 69, 46));
			Square[Curline][Curcolume + i]->setTag(1);
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
		}

		Curline++;
		break;
	}
}
void gameplay::updateLeft()
{
	switch (CurSquareType)
	{
	case 1:
		//�ж��Ƿ������������
		if (Curline < 1 || Curcolume <= 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume - 1]->setColor(Color3B(52, 228, 249));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 3]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 3]->setTag(0);

		Curcolume--;
		break;
	case 2:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (Curline - 4 + i < 0 || Square[Curline - 4 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 4; i++)
		{
			Square[Curline - 4 + i][Curcolume - 1]->setColor(Color3B(52, 228, 249));
			Square[Curline - 4 + i][Curcolume - 1]->setTag(1);
			Square[Curline - 4 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 4 + i][Curcolume]->setTag(0);
		}

		Curcolume--;
		break;
	case 3:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 4:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);

			if (i != 2 && Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 5:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume - 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume + 1]->setTag(1);

		Curcolume--;
		break;
	case 6:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume]->setTag(1);
			Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 1][Curcolume - 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 7:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume - 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume - 1]->setTag(1);

		Curcolume--;
		break;
	case 8:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
			if (i != 2)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 9:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 10:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume]->setTag(1);
			Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 3][Curcolume - 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 3][Curcolume - 1]->setTag(1);
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 11:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume - 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		Square[Curline - 2][Curcolume]->setColor(Color3B(26, 242, 26));
		Square[Curline - 2][Curcolume]->setTag(1);
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 12:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(Color3B(26, 242, 26));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 13:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 2][Curcolume - 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 14:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 3][Curcolume]->setColor(Color3B(26, 242, 26));
		Square[Curline - 3][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 15:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume - 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);
		Square[Curline - 2][Curcolume]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 16:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(Color3B(233, 178, 11));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 17:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume]->setColor(Color3B(233, 178, 11));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		Square[Curline - 2][Curcolume - 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 18:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(Color3B(233, 178, 11));
				Square[Curline - 3 + i][Curcolume]->setTag(1);
			}
		}
		Square[Curline - 2][Curcolume - 1]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume - 1]->setTag(1);

		Curcolume--;
		break;
	case 19:
		//�ж��Ƿ������������
		if (Curcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(Color3B(244, 69, 46));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
			Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}

		Curcolume--;
		break;
	}
}

void gameplay::updateRight()
{
	switch (CurSquareType)
	{
	case 1:
		//�ж��Ƿ������������
		if (Curline - 1 < 0 || Curcolume + 3 >= COLUME - 1 || Square[Curline - 1][Curcolume + 3 + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume + 3 + 1]->setColor(Color3B(52, 228, 249));
		Square[Curline - 1][Curcolume + 3 + 1]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 2:
		//�ж��Ƿ������������
		if (Curcolume >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (Curline - 4 + i < 0 || Square[Curline - 4 + i][Curcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 4; i++)
		{
			Square[Curline - 4 + i][Curcolume + 1]->setColor(Color3B(52, 228, 249));
			Square[Curline - 4 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 4 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 4 + i][Curcolume]->setTag(0);
		}
		Curcolume++;
		break;
	case 3:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume + 3]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume + 3]->setTag(1);

		Curcolume++;
		break;
	case 4:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume + 2]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);
		Square[Curline - 3][Curcolume + 2]->setColor(Color3B(245, 30, 217));
		Square[Curline - 3][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 5:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume + 3]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 3]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 3]->setTag(1);
		}
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume++;
		break;
	case 6:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 2)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 7:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume + 3]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 3]->setColor(Color3B(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 3]->setTag(1);
		}
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 8:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 2]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 3 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 9:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 3]->setColor(Color3B(245, 30, 217));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(245, 30, 217));
		Square[Curline - 1][Curcolume + 1]->setTag(1);

		Curcolume++;
		break;
	case 10:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(Color3B(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 0)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 3][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 11:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 2][Curcolume + 3]->setColor(Color3B(26, 242, 26));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume + 2]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 12:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(26, 242, 26));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 3][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 13:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(26, 242, 26));
		Square[Curline - 2][Curcolume + 2]->setTag(1);
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 3]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 14:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 < 0 || Square[Curline - 3][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(Color3B(26, 242, 26));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(26, 242, 26));
		Square[Curline - 1][Curcolume + 1]->setTag(1);
		Square[Curline - 3][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 15:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 1][Curcolume + 3]->setColor(Color3B(233, 178, 11));
		Square[Curline - 1][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume + 2]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 16:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}
		if (Curline - 3 < 0 || Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(233, 178, 11));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(1);
			}
		}
		Square[Curline - 2][Curcolume + 2]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 17:
		//�ж��Ƿ������������
		if (Curcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (Curline - 2 < 0 || Square[Curline - 2][Curcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 < 0 || Square[Curline - 1][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		//����һ��
		Square[Curline - 2][Curcolume + 3]->setColor(Color3B(233, 178, 11));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(Color3B(233, 178, 11));
		Square[Curline - 1][Curcolume + 2]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(Color3B(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 18:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 3 + i < 0 || Square[Curline - 3 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(Color3B(233, 178, 11));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(Color3B(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume]->setColor(Color3B(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 19:
		//�ж��Ƿ������������
		if (Curcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i < 0 || Square[Curline - 2 + i][Curcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//����һ��
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(Color3B(244, 69, 46));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(1);
			Square[Curline - 2 + i][Curcolume]->setColor(Color3B(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}

		Curcolume++;
		break;
	}
}

void gameplay::menuItemCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	Director::getInstance()->resume();
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}
void gameplay::menuMusicCallback(cocos2d::Ref* pSender)
{
	if (isPause == false)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		isPause = true;
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		isPause = false;
	}
}

void gameplay::menuStartCallback(cocos2d::Ref* pSender)
{
	if (ispause == false)
	{
		Director::getInstance()->pause();
		ispause = true;
	}
	else
	{
		Director::getInstance()->resume();
		ispause = false;
	}
}
