#pragma once
#ifndef SET_H_
#define SET_H_

#include "cocos2d.h"
USING_NS_CC;

class SET : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void enterHelloWorldScene(Ref* pSender);
	CREATE_FUNC(SET);
	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	void menuItem3Callback(cocos2d::Ref* pSender);
	Sprite* sprite1;
};
#endif