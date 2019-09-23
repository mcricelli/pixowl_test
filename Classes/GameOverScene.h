#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

private:

	CREATE_FUNC(GameOverScene);

	void goToMainMenu(float dt);
};

#endif // __GAMEOVER_SCENE_H__
