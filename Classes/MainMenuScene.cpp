#include "MainMenuScene.h"

#include "Enums.h"
#include "GameScene.h"

USING_NS_CC;

// =====================================================
// =====================================================

// PUBLIC

// =====================================================

// -----------------------------------------------------
Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// -----------------------------------------------------
bool MainMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Add background.
	auto background = Sprite::create("background.png");
    if (background != nullptr)
    {
		background->setAnchorPoint(origin);
		background->setPosition(origin);
		background->setContentSize(visibleSize);
        this->addChild(background, 2);
    }

    // Create title.
    auto title = Label::createWithTTF("The Fish Game", "fonts/Marker Felt.ttf", 36);
    if (title != nullptr)
    {
        // Position the label on the top center of the screen.
        title->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - title->getContentSize().height/2));
        this->addChild(title, MAIN_MENU_DEPTH::BACKGROUND);
    }

	// Create play image button.
    auto playItem = MenuItemImage::create("menu/buttonPlay.png", "menu/buttonPlayPressed.png", CC_CALLBACK_1(MainMenu::menuPlayCallback, this));
    if (playItem != nullptr)
    {
		playItem->setScaleX(12.f);
		playItem->setScaleY(6.f);
		playItem->setPosition( origin + visibleSize/2.f);
    }

	// Create play text.
	auto playText = MenuItemFont::create("Play");
	if (playText != nullptr)
	{
		playText->setPosition(playItem->getPosition());
		playText->setFontSize(24);
		playText->setFontName("Marker Felt"); // TODO: Investigate why this doesn't work.
	}

    // Create menu.
    auto menu = Menu::create(playItem, NULL);
	if (menu != nullptr)
	{
		menu->setPosition(origin);
		menu->setContentSize(Size(visibleSize.width, visibleSize.height - title->getContentSize().height));

		menu->addChild(playText);

		this->addChild(menu, MAIN_MENU_DEPTH::ITEM);
	}

    return true;
}

// =====================================================
// =====================================================

// PRIVATE

// =====================================================

// -----------------------------------------------------
void MainMenu::menuPlayCallback(Ref* pSender)
{
	auto gameScene = GameLayer::createScene();
    Director::getInstance()->replaceScene(gameScene);
}