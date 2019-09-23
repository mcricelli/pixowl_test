#include "GameOverScene.h"

#include "Enums.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PUBLIC

// =====================================================

// -----------------------------------------------------
Scene* GameOverScene::createScene()
{
	return GameOverScene::create();
}

// -----------------------------------------------------
bool GameOverScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Add background.
	auto background = Sprite::create("menu/background.png");
	if (background != nullptr)
	{
		background->setAnchorPoint(origin);
		background->setPosition(origin);
		background->setContentSize(visibleSize);
		this->addChild(background, MAIN_MENU_DEPTH::BACKGROUND);
	}

	// Create title.
	auto title = Label::createWithTTF("Game Over :(", "fonts/Marker Felt.ttf", 36);
	if (title != nullptr)
	{
		// Position the label on the center of the screen.
		title->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		this->addChild(title, MAIN_MENU_DEPTH::ITEM);
	}

	scheduleOnce(schedule_selector(GameOverScene::goToMainMenu), 3.f);

	return true;
}

// =====================================================
// =====================================================

// PRIVATE

// -----------------------------------------------------
void::GameOverScene::goToMainMenu(float dt)
{
	auto mainMenu = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenu);
}

// =====================================================
