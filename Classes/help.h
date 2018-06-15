#ifndef __HELP_H__
#define __HELP_H__

#include "cocos2d.h"
USING_NS_CC;

class help : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void enterHelloWorldScene(Ref* pSender);
	// a selector callback
	// implement the "static create()" method manually
	CREATE_FUNC(help);
	void KeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void KeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

};

#endif 
