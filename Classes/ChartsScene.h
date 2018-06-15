#ifndef __MemoryCard__ChartsScene__
#define __MemoryCard__ChartsScene__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include <vector>

USING_NS_CC;

class ChartsScene :public Layer
{
protected:
	Sprite * _newScoreTitle;
	ui::Text*_newScore;

	Sprite*_chartScoreTitle;
	std::vector<Node*>_chartList;
	
	Node*createChart(int rank, int score);

public:
	ChartsScene();
	~ChartsScene();

	CREATE_FUNC(ChartsScene);
	virtual bool init();
	static Scene* createScene();
	void enterHelloWorldScene(Ref* pSender);

};

#endif