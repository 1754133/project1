#pragma once
#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "cocos2d.h"
USING_NS_CC;
#define LINE 21
#define COLUME 12

class gameplay : public cocos2d::Layer
{
public:
	gameplay();
	~gameplay();
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void enterHelloWorldScene(Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(gameplay);

	void newSquareType();               //产生一个新的方块
	void clearLine(int nLineStart, int nLineEnd);       //清除一行
	void updateDown(float dt);          //方块下落
	void copyLine(int nlineNum);        //拷贝一行
	void nextSquareType();              //方块变形
	void updateLeft();                  //方块左移
	void updateRight();                 //方块右移
	void updateScore();                  //更新分数
	void checkgameover();                    //检查游戏是否结束
	void gameover();
	Sprite* Square[LINE][COLUME];	//方块单元格
	int Curline;						//方块所处当前行
	int Curcolume;					//方块所处当前列
	int CurSquareType;				//当前方块类型					//当前游戏分数
	Label* gameScore;
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);                //键盘监听函数
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	void menuItemCallback(cocos2d::Ref* pSender);
};

#endif 