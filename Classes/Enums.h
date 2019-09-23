#ifndef __ENUMS_H__
#define __ENUMS_H__

	namespace ColliderType
	{
		enum ColliderType
		{
			PLAYER = 0,
			ENEMY,
			BUBBLE,
			PLAYER_AIM
		};
	}

	namespace GAMEPLAY_DEPTH
	{
		enum DEPTH
		{
			BACKGROUND = 0,
			PLAYER
		};
	}

	namespace MAIN_MENU_DEPTH
	{
		enum DEPTH
		{
			BACKGROUND = 0,
			ITEM
		};
	}
#endif // !__ENUMS_H__

