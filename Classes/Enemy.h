#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Enums.h"

class Enemy : public cocos2d::Sprite
{
public:
	// Target to chase.
	CC_SYNTHESIZE(cocos2d::Point, _target, Target);

	// Movement speed.
	CC_SYNTHESIZE(int, _speed, Speed);

	CREATE_FUNC(Enemy);

private:
	virtual bool init();

	void update(float dt);
};

#endif // __ENEMY_H__


