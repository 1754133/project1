#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include "canshu.h"
using namespace std;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();

	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object


    // create menu, it's an autorelease object
 

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	Sprite * sprite = Sprite::create("Icon.png");
	sprite->setPosition(ccp(500, 575));
	this->addChild(sprite);
	

    // add "HelloWorld" splash screen"
	
	
	/*auto moveby = MoveBy::create(2, Vec2(200, 300));
	sprite1->runAction(moveby)*/;

	auto background = Sprite::create("timg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);

	
	MenuItemImage* item1 = MenuItemImage::create("playButton.png","playButton2.png", CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));
	MenuItemImage* item2 = MenuItemImage::create("helpButton.png","helpButton2.png", CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	MenuItemImage* item3 = MenuItemImage::create("settingsButt.png", "settingsButt2.png",CC_CALLBACK_1(HelloWorld::menuItem3Callback, this));             
	MenuItemImage* item4 = MenuItemImage::create("doneButton.png","doneButton2.png",CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));  
	MenuItemImage* item5 = MenuItemImage::create("SCORE.png", "SCORE2.png", CC_CALLBACK_1(HelloWorld::menuItem4Callback, this)); //设置5个菜单项
	Menu* mn = Menu::create(item1, item2, item3, item4,item5, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2-100));
	this->addChild(mn);

	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgmusic.mp3");
		isPause = true;
	}

    return true;
}

void HelloWorld::menuItem1Callback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	Director::getInstance()->replaceScene(gameplay::createScene());
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3", true);
	isPause = false;
}
void HelloWorld::menuItem2Callback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	Director::getInstance()->replaceScene(help::createScene());
}
void HelloWorld::menuItem3Callback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	Director::getInstance()->replaceScene(SET::createScene());
}
void HelloWorld::menuItem4Callback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	Director::getInstance()->replaceScene(ChartsScene::createScene());
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

 //To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
