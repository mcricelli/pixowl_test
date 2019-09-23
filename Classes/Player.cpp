#include "Player.h"
#include "Bubble.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PUBLIC

// =====================================================

// -----------------------------------------------------
Player::Player() :
	_maxBubbles(80),
	_bubbleIndexToShoot(0),
	_shouldUpdatePlayerRotation(false),
	_initialiced(false),
	_isTargetSet(false),
	_currentAimedEnemy(nullptr),
	_shootAnimationEnded(true)
{
}

// =====================================================
// =====================================================

// PRIVATE

// =====================================================

// -----------------------------------------------------
bool Player::init()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (!Sprite::init())
	{
		return false;
	}

	this->setTexture("game/fish.png");
	this->setContentSize(Size(30,30));
	this->setScaleY(-this->getScaleY());
	this->setPosition(origin + visibleSize / 2);

	// At the beginning the fish looks up.
	this->setRotation(0);
	this->_tapPosition = this->getPosition() + Vec2::UNIT_Y;

	for (int i = 0; i < _maxBubbles; i++) {
		_bubblePool.pushBack(Bubble::createBubble());
	}

	// Starts with shooting enabled.
	_shootAnimationEnded = true;
	_shootCooldown = 0;
	_isShootAnimationRunning = false;
	_shootAnimationTimer = 0;

	auto body = PhysicsBody::createCircle(this->getContentSize().width / 2);
	body->setCollisionBitmask(ColliderType::PLAYER);
	body->setContactTestBitmask(true);
	body->setGravityEnable(false);
	this->setPhysicsBody(body);

	// Create aim object.
	_aimObject = Sprite::create();
	_aimObject->setPosition(getPosition());
	auto aimBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
	aimBody->setCollisionBitmask(ColliderType::PLAYER_AIM);
	aimBody->setContactTestBitmask(true);
	aimBody->setGravityEnable(false);
	_aimObject->setPhysicsBody(aimBody);

	// Create touch listeners.
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Player::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Player::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Create collition listener.
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Player::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->schedule(schedule_selector(Player::update));
	
	return true;
}

// -----------------------------------------------------
bool Player::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (touch)
	{
		_shouldUpdatePlayerRotation = true;
		_tapPosition = touch->getLocation();
		_aimObject->setPosition(touch->getLocation());
		shoot();
	}
	return true;
}

// -----------------------------------------------------
void Player::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (touch)
	{
		_shouldUpdatePlayerRotation = true;
		_tapPosition = touch->getLocation();
		_aimObject->setPosition(touch->getLocation());
		shoot();
	}
}

// -----------------------------------------------------
void Player::onTouchEnded(Touch* touch, Event* unused_event)
{
	if (touch)
	{
		//_shouldUpdatePlayerRotation = false;// Uncomment this if you want the fish to stop rotating when you lift the finger.
		_tapPosition = touch->getLocation();
		_aimObject->setPosition(getPosition());
		_isTargetSet = false;
	}
}

// -----------------------------------------------------
void Player::update(float dt)
{
	if (_shouldUpdatePlayerRotation)
	{
		updatePlayerRotation(dt);
	}

	// This is to be able to shoot only if i'm touching an enemy.
	checkIfTargetIsStillBeingAimed();

	updateShootCooldown(dt);

	updateAnimations(dt);
}

// -----------------------------------------------------
void Player::checkIfTargetIsStillBeingAimed()
{
	if (_currentAimedEnemy != nullptr)
	{
		const bool notCollidying =
			(_currentAimedEnemy->getBoundingBox().getMaxX() < _aimObject->getBoundingBox().getMinX())
			|| (_aimObject->getBoundingBox().getMaxX() < _currentAimedEnemy->getBoundingBox().getMinX())
			|| ((_currentAimedEnemy->getBoundingBox().getMinY() > _aimObject->getBoundingBox().getMaxY()))
			|| ((_aimObject->getBoundingBox().getMinY() > _currentAimedEnemy->getBoundingBox().getMaxY()));

		if (notCollidying)
		{
			_isTargetSet = false;
			_currentAimedEnemy = nullptr;
		}
	}
}

// -----------------------------------------------------
void Player::updatePlayerRotation(float dt)
{
	const float maxDeltaAngleDegrees = 20.f;
	const float speed = 30.f;

	const Point finalDirection = _tapPosition - this->_position;
	const auto finalAngleDegrees = CC_RADIANS_TO_DEGREES(-atan2(-finalDirection.y, -finalDirection.x)) - 90;

	auto deltaAngleDegrees = finalAngleDegrees - this->getRotation();

	// This is to maintain the direction in the transition from angle 360 to 0.
	if (abs(deltaAngleDegrees) >= 180)
	{
		deltaAngleDegrees = deltaAngleDegrees  > 0 ? -360 + deltaAngleDegrees : 360 + deltaAngleDegrees;
	}

	// We make sure delta angle is not greater than the max delta allowed.
	if (abs(deltaAngleDegrees) > maxDeltaAngleDegrees)
	{
		deltaAngleDegrees = deltaAngleDegrees > 0 ? maxDeltaAngleDegrees : -maxDeltaAngleDegrees;
	}

	this->setRotation(this->getRotation() + deltaAngleDegrees * dt * speed);
}

// -----------------------------------------------------
void Player::updateShootCooldown(float dt)
{
	if (!_shootAnimationEnded)
	{
		_shootCooldown -= dt;
		_shootAnimationEnded = _shootCooldown <= 0;
	}
}

// -----------------------------------------------------
void Player::updateAnimations(float dt)
{
	// Update the shooting animation.
	if (_isShootAnimationRunning)
	{
		_shootAnimationTimer -= dt;
		if (_shootAnimationTimer <= 0)
		{
			setShootAnimation(false);
		}
	}
}

// -----------------------------------------------------
void Player::setShootAnimation(bool active)
{
	if (active)
	{
		if (!_isShootAnimationRunning)
		{
			this->setTexture("game/fish_bubble.png");
			this->setContentSize(Size(30, 30));
		}
	}
	else
	{
		if (_isShootAnimationRunning)
		{
			this->setTexture("game/fish.png");
			this->setContentSize(Size(30, 30));
		}
	}

	_shootAnimationTimer = SHOOT_ANIMATION_DURATION;

	_isShootAnimationRunning = active;
}

// -----------------------------------------------------
bool Player::canShoot()
{
	return _shootAnimationEnded && _isTargetSet;
}

// -----------------------------------------------------
void Player::shoot()
{
	if (!canShoot())
	{
		return;
	}

	// Run the shooting animation.
	setShootAnimation(true);

	const Point finalDirection = Vec2::UNIT_X.rotateByAngle(Vec2::ZERO, -CC_DEGREES_TO_RADIANS(this->getRotation() - 90));
	
	// Get the new bubble to shoot.
	_bubbleIndexToShoot = _bubbleIndexToShoot % _maxBubbles;
	auto bubble = _bubblePool.at(_bubbleIndexToShoot);
	bubble->setAnchorPoint(Point(0.5, 0.5));

	// Shoot the bubble if it's disabled.
	if (!bubble->isVisible())
	{
		bubble->setPosition(getPositionX() + finalDirection.getNormalized().x * getBoundingBox().size.height/2, getPositionY() + finalDirection.getNormalized().y * getBoundingBox().size.height/2);
		bubble->setVisible(true);
		bubble->setDirection(finalDirection);
	}
	_bubbleIndexToShoot++;

	// Reset shooting cooldown once we shoot.
	_shootAnimationEnded = false;
	_shootCooldown = SHOOT_COOLDOWN_TIME;
}

// -----------------------------------------------------
void Player::setParent(Node* parent)
{
	Sprite::setParent(parent);

	if (!_initialiced)
	{
		// The bubbles are in the same level that the player to be independent to the player movements.
		for (int i = 0; i < _maxBubbles; i++)
		{
			getParent()->addChild(_bubblePool.at(i));
		}

		getParent()->addChild(_aimObject);

		_initialiced = true;
	}
}

// -----------------------------------------------------
bool Player::onContactBegin(PhysicsContact& contact)
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((ColliderType::PLAYER_AIM == a->getCollisionBitmask() && ColliderType::ENEMY == b->getCollisionBitmask())
		|| (ColliderType::ENEMY == a->getCollisionBitmask() && ColliderType::PLAYER_AIM == b->getCollisionBitmask()))
	{
		Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

		bool bothActive = spriteA->isVisible() && spriteB->isVisible();

		if (bothActive)
		{
			_isTargetSet = true;
			_currentAimedEnemy = ColliderType::ENEMY == a->getCollisionBitmask() ? spriteA : spriteB;
		}
	}

	return false;
}
