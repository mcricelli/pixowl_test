#include "Enemy.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PRIVATE

// =====================================================

// -----------------------------------------------------
bool Enemy::init()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (!Sprite::init())
	{
		return false;
	}

	this->setTexture("game/enemy.png");
	this->setContentSize(Size(30, 30));
	this->setScaleY(-this->getScaleY());

	// Default position is at the top right of the screen.
	Point offset = Point(visibleSize.width, visibleSize.height);
	this->setPosition(origin + visibleSize / 2 + offset);
	_speed = 60;

	// Default target is the center of the screen.
	_target = origin + visibleSize / 2;

	// Add collider.
	auto body = PhysicsBody::createCircle(this->getContentSize().width / 2);
	body->setCollisionBitmask(ColliderType::ENEMY);
	body->setContactTestBitmask(true);
	body->setGravityEnable(false);
	this->setPhysicsBody(body);

	this->schedule(schedule_selector(Enemy::update));

	return true;
}

// -----------------------------------------------------
void Enemy::update(float dt)
{
	if (!this->isVisible())
	{
		return;
	}

	const Point finalDirection = _target - this->_position;

	// Update rotation to face the target.
	auto angle = -atan2(-finalDirection.y, -finalDirection.x);
	this->setRotation(CC_RADIANS_TO_DEGREES(angle) - 90);

	// Update position based on speed.
	const Vec2 newPos = this->getPosition() + finalDirection.getNormalized() * _speed * dt;
	this->setPosition(newPos);
}

