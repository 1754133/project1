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
	virtual bool init();
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	// a selector callback
	void enterHelloWorldScene(Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(gameplay);

	void newSquareType();               //����һ���µķ���
	void clearLine(int nLineStart, int nLineEnd);       //���һ��
	void clearSquare();
	void updateDown(float dt);          //��������
	void copyLine(int nlineNum);        //����һ��
	void changeSquareType();              //�������
	void nextSquareType();               //��һ������ֵ���״
	void updateLeft();                  //��������
	void updateRight();                 //��������
	void updateScore();                  //���·���
	void checkgameover();                    //�����Ϸ�Ƿ����
	void gameover();
	void down();                        //����ֱ�ӵ����²�
	Sprite* Square[LINE][COLUME];	//���鵥Ԫ��
	Sprite* square[4][4];                  //��ʾ��һ�����ַ���ĵ�Ԫ��
	int Curline;						//����������ǰ��
	int Curcolume;					//����������ǰ��
	int CurSquareType;				//��ǰ��������	
	int nextsquare;                 //��һ�����ֵķ���
	Label* gameScore;
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);                //���̼�������
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void update(float delta);
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif 