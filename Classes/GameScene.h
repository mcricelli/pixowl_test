#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "Enemy.h"

class GameLayer : public cocos2d::Layer
{
public:
	int _maxEnemies;

	int _enemyIndexToSpawn;

	cocos2d::Vector<Enemy*> _enemyPool;

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameLayer);

private:
	cocos2d::PhysicsWorld* sceneWorld;

	bool onContactBegin(cocos2d::PhysicsContact& contact);

	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };

	void spawnEnemy();
};

#endif // __GAME_LAYER_H__

