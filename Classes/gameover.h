#pragma once
#ifndef GAMEOVER_H_
#define GAMEOVER_H_

#include "cocos2d.h"
USING_NS_CC;

class gameover : public cocos2d::Layer
{
public:
	gameover();
	~gameover();

	static cocos2d::Scene* createScene();

	virtual bool init();
	void enterHelloWorldScene(Ref* pSender);
	// a selector callback
	// implement the "static create()" method manually
	CREATE_FUNC(gameover);
	Label* gamescore;
};

#endif // !GAMEOVER_H
