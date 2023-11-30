#pragma once
#include "CommonFunc.h"
#include "LTexture.h"

using namespace std;

class Winner {
	LTexture WTexture;
	LTexture RankTexture;
	LTexture TimeTexture;
	LTexture StepTexture;
	int step;
	string time;
	SDL_Rect RankRect[10];	
public:
	Winner(int step, string time) : step(step), time(time) 
	{
		for (int i = 0; i < 10; ++i)
		{
			RankRect[i] = { 0, 72 * i, 184, 72 };
		}
	}

	bool loadMedia()
	{
		bool success = true;
		if (!WTexture.loadFromFile("Data//Winner//WTexture.png", false))
		{
			success = false;
			cout << "Can't Load the WTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!RankTexture.loadFromFile("Data//Winner//RankRect.png", false))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!StepTexture.loadFromRenderedText(to_string(step), {0x00, 0x00, 0x00, 0xFF}, 34))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!TimeTexture.loadFromRenderedText(time, { 0x00, 0x00, 0x00, 0xFF }, 34))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		return success;
	}

	~Winner()
	{
		time.clear();
		WTexture.free();
		RankTexture.free();
	}

	void HandleEvent()
	{
		
	}

	void render()
	{
		
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		WTexture.render(399, 79);
		RankTexture.render(553, 425, &RankRect[0]);
		TimeTexture.render((SCREEN_WIDTH - TimeTexture.getWidth()) / 2, 260);
		StepTexture.render((SCREEN_WIDTH - StepTexture.getWidth()) / 2, 358);
		cout << "rendered!!\n";
		SDL_RenderPresent(gRenderer);
	}

	void run()
	{
		if (!loadMedia())
		{
			cout << "Can't Load Media Winner " << endl;
			return;
		}
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

		// Đặt màu vẽ là đen (0, 0, 0, 200) - mờ
		SDL_SetRenderDrawColor(gRenderer, 0xDC, 0xBA, 0x79, 255 * 23 / 100);

		// Tạo một SDL_Rect có kích thước như bạn muốn
		SDL_Rect FillRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		// Vẽ hình chữ nhật mờ lên Renderer
		SDL_RenderFillRect(gRenderer, &FillRect);
		// Đặt lại chế độ blend về mặc định
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
		render();
		SDL_Delay(50000);
	}
};