#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include "Enums.h"

class Bubble : public cocos2d::Sprite
{
public:
	// Direction that the bubble will follow once it was shooted.
	CC_SYNTHESIZE(cocos2d::Point, _direction, Direction);

	static Bubble* createBubble();

private:
	virtual bool initWithFile(const std::string& filename);

	void update(float dt);
};

#endif // __BUBBLE_H__
