#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <vector>
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
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Tetris", "fonts/arial.ttf", 60);
    if (label == nullptr)
    {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        // position the label on the center of the screen
		label->setColor(Color3B(255, 255, 0));
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));
		label->setAnchorPoint(Vec2(0.5, 0.5));
        // add the label as a child to this layer
        this->addChild(label, 1);
    }
	

    // add "HelloWorld" splash screen"
	auto sprite1 = Sprite::create("1.png");
	sprite1->setPosition(Vec2(100, visibleSize.height / 2 + origin.y));
	/*auto moveby = MoveBy::create(2, Vec2(200, 300));
	sprite1->runAction(moveby)*/;

	this->addChild(sprite1, 0);

	auto sprite2 = Sprite::create("3.png");
	sprite2->setPosition(Vec2(100, 200));
	this->addChild(sprite2, 0);

	auto background = Sprite::create("timg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);
	MenuItemFont::setFontSize(86);
	MenuItemFont* item1 = MenuItemFont::create("Start", CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));
	MenuItemFont* item2 = MenuItemFont::create("Help", CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	MenuItemFont* item3 = MenuItemFont::create("Set", CC_CALLBACK_1(HelloWorld::menuItem3Callback, this));              //设置三个菜单项
	Menu* mn = Menu::create(item1, item2, item3, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);

    return true;
}

void HelloWorld::menuItem1Callback(Ref* pSender)
{
	Director::getInstance()->replaceScene(gameplay::createScene());
}
void HelloWorld::menuItem2Callback(Ref* pSender)
{
	Director::getInstance()->replaceScene(help::createScene());
}
void HelloWorld::menuItem3Callback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SET::createScene());
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
