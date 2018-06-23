#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h" 
#include "gameplay.h"
#include "help.h"
#include "set.h"
#include "ChartsScene.h"
USING_NS_CC;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	void menuItem3Callback(cocos2d::Ref* pSender);
	void menuItem4Callback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
