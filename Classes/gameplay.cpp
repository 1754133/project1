#include "gameplay.h"
#include "HelloWorldScene.h"
using namespace std;
USING_NS_CC;

gameplay::gameplay()
{
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			Square[i][j] = NULL;
		}
	}

	Label = NULL;
	CurScore = 0;
	newSquareType();
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

	Size VisibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			Square[i][j] = Sprite::create("square.png");
			Square[i][j]->setPosition(Director::sharedDirector()->convertToGL(ccp(j * 30 + j * 2 + 20, i * 30 + i * 2 + 20)));
			Square[i][j]->setTag(0);
			Square[i][j]->setColor(ccc3(255, 255, 255));	
			this->addChild(Square[i][j]);
		}
	}
	this->schedule(schedule_selector(gameplay::updateDown), 1.0);
	/*auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(gameplay::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(gameplay::keyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);*/


	auto label = Label::createWithSystemFont("return", "fonts/arial.ttf", 30);
	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(gameplay::enterHelloWorldScene, this));
	auto menu = Menu::create(menuitem, NULL);
	this->addChild(menu);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(gameplay::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(gameplay::keyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

void gameplay::keyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
	/*log("Key with keycode %d pressed", keyCode);*/
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		updateDown(0.0);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::sharedDirector()->pause();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		Director::sharedDirector()->resume();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		nextSquareType();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		updateLeft();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		updateRight();
	}
}
void gameplay::keyReleased(EventKeyboard::KeyCode keyCode,Event *event)
{
	log("Key with keycode %d released", keyCode);
}


void gameplay::enterHelloWorldScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void gameplay::newSquareType()
{
	CurSquareType = rand() % 19 + 1;
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

void gameplay::nextSquareType()
{
	switch (CurSquareType)
	{
	case 1:
		//判断是否可以变换
		if (Curline + 1 > LINE - 1)
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
			if (i != 1 && Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			if (i != 1 && Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(52, 228, 249));
				Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			}
		}

		Curline += 2;
		Curcolume++;
		CurSquareType = 2;
		break;
	case 2:
		//判断是否可以变换
		if (Curcolume - 1 < 0 || Curcolume + 2 > COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && Curline - 2 > -1 && Square[Curline - 2][Curcolume - 1 + i]->getTag() == 1)
			{
				return;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != 2 && Curline - 4 + i > -1)
			{
				Square[Curline - 4 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 4 + i][Curcolume]->setTag(0);
			}
			if (i != 1 && Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume - 1 + i]->setColor(ccc3(52, 228, 249));
				Square[Curline - 2][Curcolume - 1 + i]->setTag(1);
			}
		}

		Curline--;
		Curcolume--;
		CurSquareType = 1;
		break;
	case 3:
		//判断是否可以变换
		if (Curline > LINE - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 1 + i]->getTag() == 1)
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
				Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 2]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline][Curcolume + 1]->setTag(1);
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(ccc3(245, 30, 217));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(1);
			}
		}

		Curline++;
		Curcolume++;
		CurSquareType = 4;
		break;
	case 4:
		//判断是否可以变换
		if (Curcolume - 1 < 0)
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
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(245, 30, 217));
				Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			}
		}

		Curcolume--;
		CurSquareType = 5;
		break;
	case 5:
		//判断是否可以变换
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 3 > -1 && Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3][Curcolume + 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(245, 30, 217));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}

		CurSquareType = 6;
		break;
	case 6:
		//判断是否可以变换
		if (Curcolume + 2 > COLUME - 1)
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
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(ccc3(245, 30, 217));
				Square[Curline - 3 + i][Curcolume]->setTag(1);
			}
		}

		Curline--;
		CurSquareType = 3;
		break;
	case 7:
		//判断是否可以变换
		if (Curline > LINE - 1)
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
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + 1 + i]->setColor(ccc3(245, 30, 217));
			Square[Curline][Curcolume + 1 + i]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}

		Curline++;
		Curcolume++;
		CurSquareType = 8;
		break;
	case 8:
		//判断是否可以变换
		if (Curcolume - 1 < 0)
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
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume - 1]->setColor(ccc3(245, 30, 217));
				Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 9;
		break;
	case 9:
		//判断是否可以变换
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 > -1 && Square[Curline - 3][Curcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(ccc3(245, 30, 217));
				Square[Curline - 3][Curcolume + i]->setTag(1);
			}
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		CurSquareType = 10;
		break;
	case 10:
		//判断是否可以变换
		if (Curcolume + 2 > COLUME - 1)
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
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume + 2]->setColor(ccc3(245, 30, 217));
				Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			}
		}

		Curline--;
		CurSquareType = 7;
		break;
	case 11:
		//判断是否可以变换
		if (Curline > LINE - 1)
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

		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 + i > -1)
			{
				Square[Curline - 1 + i][Curcolume + 2]->setColor(ccc3(26, 242, 26));
				Square[Curline - 1 + i][Curcolume + 2]->setTag(1);
			}
		}

		Curline++;
		Curcolume++;
		CurSquareType = 12;
		break;
	case 12:
		//判断是否可以变换
		if (Curcolume - 1 < 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1 && Square[Curline - 1][Curcolume - 1 + i]->getTag() == 1)
			{
				return;
			}
		}

		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume - 1 + i]->setColor(ccc3(26, 242, 26));
				Square[Curline - 1][Curcolume - 1 + i]->setTag(1);
			}
		}

		Curcolume--;
		CurSquareType = 11;
		break;
	case 13:
		//判断是否可以变换
		if (Curline > LINE - 1)
		{
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(26, 242, 26));
		Square[Curline][Curcolume + 1]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(26, 242, 26));
			Square[Curline - 2][Curcolume + 2]->setTag(1);
		}

		Curline++;
		Curcolume++;
		CurSquareType = 14;
		break;
	case 14:
		//判断是否可以变换
		if (Curcolume - 1 < 0)
		{
			return;
		}
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (Curline - 1 > -1 && Square[Curline - 1][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (Curline - 3 + i > -1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(ccc3(26, 242, 26));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(26, 242, 26));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 13;
		break;
	case 15:
		//判断是否可以变换
		if (Curline > LINE - 1)
		{
			return;
		}
		if (Square[Curline][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		Curcolume++;
		CurSquareType = 16;
		break;
	case 16:
		//判断是否可以变换
		if (Curcolume - 1 < 0)
		{
			return;
		}
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume - 1]->setColor(ccc3(233, 178, 11));
			Square[Curline - 2][Curcolume - 1]->setTag(1);
		}

		Curcolume--;
		CurSquareType = 17;
		break;
	case 17:
		//判断是否可以变换
		if (Curline - 3 > -1 && Square[Curline - 3][Curcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(233, 178, 11));
			Square[Curline - 3][Curcolume + 1]->setTag(1);
		}

		CurSquareType = 18;
		break;
	case 18:
		//判断是否可以变换
		if (Curcolume + 2 > COLUME - 1)
		{
			return;
		}
		if (Curline - 2 > -1 && Square[Curline - 2][Curcolume + 2]->getTag() == 1)
		{
			return;
		}

		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(233, 178, 11));
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
	//检查游戏是否结束

	for (int i = nLineStart; i <= nLineEnd; i++)
	{
		int j;
		for (j = 0; j < COLUME; j++)
		{
			if (i > -1 && Square[i][j]->getTag() == 0)
			{
				break;
			}
		}

		if (COLUME == j)
		{
			//清除一行
			for (int k = i - 1; k >= 0; k--)
			{
				copyLine(k);
			}
			for (int x = 0; x < COLUME; x++)
			{
				Square[0][x]->setColor(ccc3(255, 255, 255));
				Square[0][x]->setTag(0);
			}
			CurScore++;
		}
	}

}

void gameplay::copyLine(int nlineNum)
{
	for (int i = 0; i < COLUME; i++)
	{
		Square[nlineNum + 1][i]->setColor(Square[nlineNum][i]->getColor());
		Square[nlineNum + 1][i]->setTag(Square[nlineNum][i]->getTag());
	}
}

void gameplay::updateDown(float dt)
{
	switch (CurSquareType)
	{
	case 1:
		//判断是否允许继续下降
		if (Curline >= LINE)
		{
			clearLine(LINE - 1, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (Square[Curline][Curcolume + i]->getTag() == 1)
			{
				clearLine(Curline - 1, Curline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 4; i++)
		{
			//消除原色
			if (Curline < LINE && Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			//显示新色
			if (Curline < LINE)
			{
				Square[Curline][Curcolume + i]->setColor(ccc3(52, 228, 249));
				Square[Curline][Curcolume + i]->setTag(1);
			}
		}

		Curline++;
		break;
	case 2:
		//判断是否允许继续下降
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

		//下降一格
		//消除原色
		if (Curline < LINE && Curline - 4 > -1)
		{
			Square[Curline - 4][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 4][Curcolume]->setTag(0);
		}
		//显示新色
		if (Curline < LINE)
		{
			Square[Curline][Curcolume]->setColor(ccc3(52, 228, 249));
			Square[Curline][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 3:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			//消除原色
			if (i != 0 && Curline < LINE && Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
			//显示新色
			if (Curline < LINE)
			{
				Square[Curline][Curcolume + i]->setColor(ccc3(245, 30, 217));
				Square[Curline][Curcolume + i]->setTag(1);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}

		Curline++;
		break;
	case 4:
		//判断是否允许继续下降
		if (Curline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (Square[Curline][Curcolume]->getTag() == 1 ||
			(Curline - 2 > -1 && Square[Curline - 2][Curcolume + 1]->getTag() == 1))
		{
			clearLine(Curline - 3, Curline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		//显示新色
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume + 1]->setTag(1);
		}
		Square[Curline][Curcolume]->setColor(ccc3(245, 30, 217));
		Square[Curline][Curcolume]->setTag(1);

		Curline++;
		break;
	case 5:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			//显示新色
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(245, 30, 217));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume + 2]->setColor(ccc3(245, 30, 217));
		Square[Curline][Curcolume + 2]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}

		Curline++;
		break;
	case 6:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(ccc3(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 7:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline][Curcolume + i]->setColor(ccc3(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}

		Curline++;
		break;
	case 8:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(ccc3(245, 30, 217));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 9:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(0);
			}
			//显示新色
			if (Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + 1 + i]->setColor(ccc3(245, 30, 217));
				Square[Curline - 1][Curcolume + 1 + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume]->setColor(ccc3(245, 30, 217));
		Square[Curline][Curcolume]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}

		Curline++;
		break;
	case 10:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (Curline - 3 > -1)
			{
				Square[Curline - 3][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3][Curcolume + i]->setTag(0);
			}
		}
		//显示新色
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2][Curcolume]->setTag(1);
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 11:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + 1 + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + 1 + i]->setTag(0);
			}
			Square[Curline][Curcolume + i]->setColor(ccc3(26, 242, 26));
			Square[Curline][Curcolume + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume]->setTag(0);
			Square[Curline - 1][Curcolume + 2]->setColor(ccc3(26, 242, 26));
			Square[Curline - 1][Curcolume + 2]->setTag(1);
		}

		Curline++;
		break;
	case 12:
		//判断是否允许继续下降
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

		//下降一格
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(26, 242, 26));
			Square[Curline - 1][Curcolume]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(26, 242, 26));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 13:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			Square[Curline][Curcolume + 1 + i]->setColor(ccc3(26, 242, 26));
			Square[Curline][Curcolume + 1 + i]->setTag(1);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 1][Curcolume + 2]->setTag(0);
			Square[Curline - 1][Curcolume]->setColor(ccc3(26, 242, 26));
			Square[Curline - 1][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 14:
		//判断是否允许继续下降
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

		//下降一格
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(26, 242, 26));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume]->setColor(ccc3(26, 242, 26));
		Square[Curline][Curcolume]->setTag(1);

		Curline++;
		break;
	case 15:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline][Curcolume + i]->setColor(ccc3(233, 178, 11));
			Square[Curline][Curcolume + i]->setTag(1);
			if (Curline - 1 > -1 && i != 1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 1][Curcolume + i]->setTag(0);
			}
		}
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}

		Curline++;
		break;
	case 16:
		//判断是否允许继续下降
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

		//下降一格
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume]->setTag(0);
		}
		Square[Curline][Curcolume]->setColor(ccc3(233, 178, 11));
		Square[Curline][Curcolume]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume + 1]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume + 1]->setColor(ccc3(233, 178, 11));
			Square[Curline - 1][Curcolume + 1]->setTag(1);
		}

		Curline++;
		break;
	case 17:
		//判断是否允许继续下降
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

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2][Curcolume + i]->setTag(0);
			}
			if (i != 1 && Curline - 1 > -1)
			{
				Square[Curline - 1][Curcolume + i]->setColor(ccc3(233, 178, 11));
				Square[Curline - 1][Curcolume + i]->setTag(1);
			}
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);

		Curline++;
		break;
	case 18:
		//判断是否允许继续下降
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

		//下降一格
		if (Curline - 3 > -1)
		{
			Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3][Curcolume + 1]->setTag(0);
		}
		Square[Curline][Curcolume + 1]->setColor(ccc3(233, 178, 11));
		Square[Curline][Curcolume + 1]->setTag(1);
		if (Curline - 2 > -1)
		{
			Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2][Curcolume]->setTag(0);
		}
		if (Curline - 1 > -1)
		{
			Square[Curline - 1][Curcolume]->setColor(ccc3(233, 178, 11));
			Square[Curline - 1][Curcolume]->setTag(1);
		}

		Curline++;
		break;
	case 19:
		//判断是否允许继续下降
		if (Curline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (Square[Curline][Curcolume]->getTag() == 1)
			{
				clearLine(Curline - 2, Curline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline][Curcolume + i]->setColor(ccc3(244, 69, 46));
			Square[Curline][Curcolume + i]->setTag(1);
			if (Curline - 2 > -1)
			{
				Square[Curline - 2][Curcolume + i]->setColor(ccc3(255, 255, 255));
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
		//判断是否允许继续左移
		if (Curline - 1 < 0 || Curcolume <= 0 || Square[Curline - 1][Curcolume - 1]->getTag() == 1)
		{
			return;
		}

		//左移一格
		Square[Curline - 1][Curcolume - 1]->setColor(ccc3(52, 228, 249));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 3]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 3]->setTag(0);

		Curcolume--;
		break;
	case 2:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 4; i++)
		{
			Square[Curline - 4 + i][Curcolume - 1]->setColor(ccc3(52, 228, 249));
			Square[Curline - 4 + i][Curcolume - 1]->setTag(1);
			Square[Curline - 4 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 4 + i][Curcolume]->setTag(0);
		}

		Curcolume--;
		break;
	case 3:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 4:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);

			if (i != 2 && Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 5:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			if (Curline - 2 + i > -1)
			{
				Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(255, 255, 255));
				Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume - 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume + 1]->setTag(1);

		Curcolume--;
		break;
	case 6:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume]->setTag(1);
			Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 1][Curcolume - 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 7:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 2][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume - 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume - 1]->setTag(1);

		Curcolume--;
		break;
	case 8:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
			if (i != 2)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 9:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 10:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume]->setTag(1);
			Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 3][Curcolume - 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 3][Curcolume - 1]->setTag(1);
		Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 11:
		//判断是否允许继续左移
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

		//左移一格
		Square[Curline - 1][Curcolume - 1]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		Square[Curline - 2][Curcolume]->setColor(ccc3(26, 242, 26));
		Square[Curline - 2][Curcolume]->setTag(1);
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 12:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(ccc3(26, 242, 26));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 13:
		//判断是否允许继续左移
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

		//左移一格
		Square[Curline - 2][Curcolume - 1]->setColor(ccc3(26, 242, 26));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 14:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(ccc3(26, 242, 26));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
		}
		Square[Curline - 3][Curcolume]->setColor(ccc3(26, 242, 26));
		Square[Curline - 3][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume--;
		break;
	case 15:
		//判断是否允许继续左移
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

		//左移一格
		Square[Curline - 1][Curcolume - 1]->setColor(ccc3(233, 178, 11));
		Square[Curline - 1][Curcolume - 1]->setTag(1);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);
		Square[Curline - 2][Curcolume]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 16:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume - 1]->setColor(ccc3(233, 178, 11));
			Square[Curline - 3 + i][Curcolume - 1]->setTag(1);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume--;
		break;
	case 17:
		//判断是否允许继续左移
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

		//左移一格
		Square[Curline - 1][Curcolume]->setColor(ccc3(233, 178, 11));
		Square[Curline - 1][Curcolume]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);
		Square[Curline - 2][Curcolume - 1]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume - 1]->setTag(1);
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);

		Curcolume--;
		break;
	case 18:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume]->setColor(ccc3(233, 178, 11));
				Square[Curline - 3 + i][Curcolume]->setTag(1);
			}
		}
		Square[Curline - 2][Curcolume - 1]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume - 1]->setTag(1);

		Curcolume--;
		break;
	case 19:
		//判断是否允许继续左移
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

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume - 1]->setColor(ccc3(244, 69, 46));
			Square[Curline - 2 + i][Curcolume - 1]->setTag(1);
			Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
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
		//判断是否允许继续右移
		if (Curline - 1 < 0 || Curcolume + 3 >= COLUME - 1 || Square[Curline - 1][Curcolume + 3 + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		Square[Curline - 1][Curcolume + 3 + 1]->setColor(ccc3(52, 228, 249));
		Square[Curline - 1][Curcolume + 3 + 1]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 2:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 4; i++)
		{
			Square[Curline - 4 + i][Curcolume + 1]->setColor(ccc3(52, 228, 249));
			Square[Curline - 4 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 4 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 4 + i][Curcolume]->setTag(0);
		}

		Curcolume++;
		break;
	case 3:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 2][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume + 3]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume + 3]->setTag(1);

		Curcolume++;
		break;
	case 4:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);
		Square[Curline - 3][Curcolume + 2]->setColor(ccc3(245, 30, 217));
		Square[Curline - 3][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 5:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 3]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 3]->setTag(1);
		}
		Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 2]->setTag(0);

		Curcolume++;
		break;
	case 6:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 2)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 7:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 3]->setColor(ccc3(245, 30, 217));
			Square[Curline - 2 + i][Curcolume + 3]->setTag(1);
		}
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 2]->setTag(0);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 8:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 1]->setTag(1);
			Square[Curline - 3 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 9:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 2][Curcolume + 3]->setColor(ccc3(245, 30, 217));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(245, 30, 217));
		Square[Curline - 1][Curcolume + 1]->setTag(1);

		Curcolume++;
		break;
	case 10:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(ccc3(245, 30, 217));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 0)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 3][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 11:
		//判断是否允许继续右移
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

		//右移一格
		Square[Curline - 2][Curcolume + 3]->setColor(ccc3(26, 242, 26));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume + 2]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 12:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(26, 242, 26));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 3][Curcolume + 1]->setColor(ccc3(26, 242, 26));
		Square[Curline - 3][Curcolume + 1]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 13:
		//判断是否允许继续右移
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

		//右移一格
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(26, 242, 26));
		Square[Curline - 2][Curcolume + 2]->setTag(1);
		Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 3]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 14:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(ccc3(26, 242, 26));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(26, 242, 26));
		Square[Curline - 1][Curcolume + 1]->setTag(1);
		Square[Curline - 3][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 3][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 15:
		//判断是否允许继续右移
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

		//右移一格
		Square[Curline - 1][Curcolume + 3]->setColor(ccc3(233, 178, 11));
		Square[Curline - 1][Curcolume + 3]->setTag(1);
		Square[Curline - 1][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume]->setTag(0);
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume + 2]->setTag(1);
		Square[Curline - 2][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 16:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 3 + i][Curcolume]->setTag(0);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(233, 178, 11));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(1);
			}
		}
		Square[Curline - 2][Curcolume + 2]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume + 2]->setTag(1);

		Curcolume++;
		break;
	case 17:
		//判断是否允许继续右移
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

		//右移一格
		Square[Curline - 2][Curcolume + 3]->setColor(ccc3(233, 178, 11));
		Square[Curline - 2][Curcolume + 3]->setTag(1);
		Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);
		Square[Curline - 1][Curcolume + 2]->setColor(ccc3(233, 178, 11));
		Square[Curline - 1][Curcolume + 2]->setTag(1);
		Square[Curline - 1][Curcolume + 1]->setColor(ccc3(255, 255, 255));
		Square[Curline - 1][Curcolume + 1]->setTag(0);

		Curcolume++;
		break;
	case 18:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			Square[Curline - 3 + i][Curcolume + 2]->setColor(ccc3(233, 178, 11));
			Square[Curline - 3 + i][Curcolume + 2]->setTag(1);
			if (i != 1)
			{
				Square[Curline - 3 + i][Curcolume + 1]->setColor(ccc3(255, 255, 255));
				Square[Curline - 3 + i][Curcolume + 1]->setTag(0);
			}
		}
		Square[Curline - 2][Curcolume]->setColor(ccc3(255, 255, 255));
		Square[Curline - 2][Curcolume]->setTag(0);

		Curcolume++;
		break;
	case 19:
		//判断是否允许继续右移
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

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			Square[Curline - 2 + i][Curcolume + 2]->setColor(ccc3(244, 69, 46));
			Square[Curline - 2 + i][Curcolume + 2]->setTag(1);
			Square[Curline - 2 + i][Curcolume]->setColor(ccc3(255, 255, 255));
			Square[Curline - 2 + i][Curcolume]->setTag(0);
		}

		Curcolume++;
		break;
	}
}

