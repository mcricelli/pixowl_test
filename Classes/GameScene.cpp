#include "GameScene.h"

#include "Enums.h"
#include "GameOverScene.h"
#include "Player.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PUBLIC

// =====================================================

// -----------------------------------------------------
Scene* GameLayer::createScene()
{
	auto scene = Scene::createWithPhysics();

	// We don't need gravity.
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

	auto layer = GameLayer::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);

	return scene;
}

// -----------------------------------------------------
bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Add background.
	auto background = Sprite::create("game/background.png");
	if (background != nullptr)
	{
		// Stretch the background to fit the screen size.
		background->setAnchorPoint(origin);
		background->setPosition(origin);
		background->setContentSize(visibleSize);

		this->addChild(background, GAMEPLAY_DEPTH::BACKGROUND);
	}

	// Add player.
	auto player = Player::create();
	this->addChild(player, GAMEPLAY_DEPTH::PLAYER);

	_maxEnemies = 50;
	_enemyIndexToSpawn = 0;

	// Create enemies pool.
	for (int i = 0; i < _maxEnemies; i++)
	{
		auto enemy = Enemy::create();
		enemy->setTarget(player->getPosition());
		enemy->setVisible(false);
		_enemyPool.pushBack(enemy);

		addChild(enemy);
	}

	// Set a sequence to spawn enemies every 1 second.
	DelayTime* delayAction = DelayTime::create(1.0f);
	CallFunc* spawnEnemyAction = CallFunc::create(CC_CALLBACK_0(GameLayer::spawnEnemy, this));
	auto spawnEnemySequence = Sequence::create(delayAction, spawnEnemyAction, NULL);
	runAction(RepeatForever::create(spawnEnemySequence));

	// Create collition listener.
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

// =====================================================
// =====================================================

// PRIVATE

// =====================================================

// -----------------------------------------------------
void GameLayer::spawnEnemy()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();

	_enemyIndexToSpawn = _enemyIndexToSpawn % _maxEnemies;
	
	auto enemy = _enemyPool.at(_enemyIndexToSpawn);

	auto spawnFromXAxis = (bool)RandomHelper::random_int(0, 1);

	// Indicates if the enemy spawns from the left or right border (in case it spawns from the y axis) or from the bottom or top border (in case it spawns from the x axis).
	auto normalizedBorderToSpawn = RandomHelper::random_int(0, 1);

	// If the enemy spawns from the x axis the random x position can be any point in between 0 and visible size.
	// and the y position can only be 0 or size height.
	if (spawnFromXAxis)
	{
		enemy->setPositionX(RandomHelper::random_int(0, (int)visibleSize.width));
		enemy->setPositionY( normalizedBorderToSpawn * (int)visibleSize.height);
	}
	else
	{
		enemy->setPositionX(normalizedBorderToSpawn * (int)visibleSize.width);
		enemy->setPositionY(RandomHelper::random_int(0, (int)visibleSize.height));
	}
	
	// Reset the enemy visibility state.
	if (!enemy->isVisible())
	{
		enemy->setVisible(true);
	}

	int speed = 12;
	int deltaSpeed = 5;
	enemy->setSpeed(RandomHelper::random_int(speed - deltaSpeed, speed + deltaSpeed));

	_enemyIndexToSpawn++;
}

// -----------------------------------------------------
bool GameLayer::onContactBegin(PhysicsContact& contact)
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((ColliderType::PLAYER == a->getCollisionBitmask() && ColliderType::ENEMY == b->getCollisionBitmask())
		|| (ColliderType::ENEMY == a->getCollisionBitmask() && ColliderType::PLAYER == b->getCollisionBitmask()))
	{
		auto gameOverScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(gameOverScene);
	}

	if ((ColliderType::BUBBLE == a->getCollisionBitmask() && ColliderType::ENEMY == b->getCollisionBitmask())
		|| (ColliderType::ENEMY == a->getCollisionBitmask() && ColliderType::BUBBLE == b->getCollisionBitmask()))
	{
		Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

		bool bothActive = spriteA->isVisible() && spriteB->isVisible();

		if (bothActive)
		{
			spriteA->setVisible(false);
			spriteB->setVisible(false);

			// Relocate bubbles to the center and enemies on the top right corner.
			if (ColliderType::BUBBLE == a->getCollisionBitmask())
			{
				spriteA->setPosition(origin + visibleSize/2);
				spriteB->setPosition(origin + visibleSize);
			}
			else
			{
				spriteA->setPosition(origin + visibleSize);
				spriteB->setPosition(origin + visibleSize/2);
			}
		}
	}

	return false;
}