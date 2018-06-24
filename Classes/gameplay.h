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
	void menuItemCallback(cocos2d::Ref* pSender);
	void menuMusicCallback(cocos2d::Ref* pSender);
	void menuStartCallback(cocos2d::Ref* pSender);
	virtual bool init();
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);                   //处理键盘按住
	// a selector callback
	void enterHelloWorldScene(Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(gameplay);

	void newSquareType();               //产生一个新的方块
	void clearLine(int nLineStart, int nLineEnd);       //清除一行
	void clearSquare();                 //清除方块预测栏中的图形
	void updateDown(float dt);          //方块下落
	void copyLine(int nlineNum);        //拷贝一行
	void changeSquareType();              //方块变形
	void nextSquareType();               //下一个会出现的形状
	void updateLeft();                  //方块左移
	void updateRight();                 //方块右移
	void updateScore();                  //更新分数
	void checkgameover();                    //检查游戏是否结束
	void gameover();                     //游戏结束函数
	void down();                        //图形直接到最下层
	Sprite* Square[LINE][COLUME];	//方块单元格
	Sprite* square[4][4];                  //显示下一个出现方块的单元格
	int Curline;						//方块所处当前行
	int Curcolume;					//方块所处当前列
	int CurSquareType;				//当前方块类型	
	int nextsquare;                 //下一个出现的方块
	Label* gameScore;
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);                //键盘监听函数
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void update(float delta);
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif 