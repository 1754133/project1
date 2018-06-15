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

	void newSquareType();               //����һ���µķ���
	void clearLine(int nLineStart, int nLineEnd);       //���һ��
	void updateDown(float dt);          //��������
	void copyLine(int nlineNum);        //����һ��
	void nextSquareType();              //�������
	void updateLeft();                  //��������
	void updateRight();                 //��������
	void updateScore();                  //���·���
	void checkgameover();                    //�����Ϸ�Ƿ����
	void gameover();
	Sprite* Square[LINE][COLUME];	//���鵥Ԫ��
	int Curline;						//����������ǰ��
	int Curcolume;					//����������ǰ��
	int CurSquareType;				//��ǰ��������					//��ǰ��Ϸ����
	Label* gameScore;
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);                //���̼�������
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	void menuItemCallback(cocos2d::Ref* pSender);
};

#endif 