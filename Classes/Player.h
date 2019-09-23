#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Bubble.h"
#include "Enums.h"

class Player : public cocos2d::Sprite
{
public:
	CREATE_FUNC(Player);

	Player();

private:
	int _maxBubbles;

	int _bubbleIndexToShoot;

	cocos2d::Vector<Bubble*> _bubblePool;

	bool _initialiced;

	bool _shouldUpdatePlayerRotation;

	void shoot();

	void update(float dt);

	void updatePlayerRotation(float dt);
	
	void updateShootCooldown(float dt);

	void updateAnimations(float dt);

	void checkIfTargetIsStillBeingAimed();

	bool canShoot();

	bool _shootAnimationEnded;

	bool _isTargetSet;

	float _shootCooldown;

	float _isShootAnimationRunning;

	float _shootAnimationTimer;

	const float SHOOT_ANIMATION_DURATION = 1.f;

	const float SHOOT_COOLDOWN_TIME = 0.5f;

	cocos2d::Point _tapPosition;

	cocos2d::Sprite* _aimObject;

	cocos2d::Sprite* _currentAimedEnemy;

	virtual bool init();

	virtual void setParent(Node* parent);

	void setShootAnimation(bool active);

	// Touch callbacks.
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	// Collision callback.
	bool onContactBegin(cocos2d::PhysicsContact& contact);
};

#endif // __PLAYER_H__
