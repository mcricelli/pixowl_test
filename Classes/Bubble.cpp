#include "Bubble.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PUBLIC

// =====================================================

// -----------------------------------------------------
Bubble* Bubble::createBubble()
{
	Bubble* newBubble = new (std::nothrow) Bubble();

	if (newBubble && newBubble->initWithFile("game/bubble.png"))
	{
		const Vec2 origin = Director::getInstance()->getVisibleOrigin();

		newBubble->setPosition(origin);
		newBubble->setContentSize(Size(15, 15));
		newBubble->setVisible(false);
		newBubble->autorelease();

		auto body = PhysicsBody::createCircle(newBubble->getContentSize().width / 2);
		body->setCollisionBitmask(ColliderType::BUBBLE);
		body->setGravityEnable(false);
		body->setContactTestBitmask(true);
		newBubble->setPhysicsBody(body);

		return newBubble;
	}
	CC_SAFE_DELETE(newBubble);
	return nullptr;
}

// =====================================================
// =====================================================

// PRIVATE

// =====================================================

// -----------------------------------------------------
bool Bubble::initWithFile(const std::string& filename)
{
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	setVisible(false);
	schedule(schedule_selector(Bubble::update));

	return true;
}

// -----------------------------------------------------
void Bubble::update(float dt)
{
	if (!this->isVisible())
	{
		return;
	}

	// If the bubble goes beyond the screen limits we hide it.
	if (getPositionY() > Director::getInstance()->getVisibleSize().height
		|| getPositionY() < 0
		|| getPositionX() > Director::getInstance()->getVisibleSize().width
		|| getPositionX() < 0
	)
	{
		setVisible(false);
	}

	const int speed = 80;

	// Update position based on speed.
	const Vec2 newPos = this->getPosition() + this->_direction.getNormalized() * speed * dt;
	this->setPosition(newPos);
}