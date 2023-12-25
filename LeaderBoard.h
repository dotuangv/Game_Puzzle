#pragma once
#include "CommonFunc.h"

struct Record {
	string name;
	string time;
	int step;
	Record() {}
	Record(string name_i, string time_i, int step_i)
	{
		name = name_i;
		time = time_i;
		step = step_i;
	}
};

enum ModeLeaderBoard {
	Mode3x3 = 0,
	Mode4x4,
	Mode5x5,
	Mode6x6,
	ModeTotal = 4
};

class LeaderBoard {
private:
	vector<vector<Record>> LeaderBoardRecord;
	LTexture LBTexture;
	SDL_Renderer* LBRenderer;
	//Mốc rank hiện tại
	int Current;
	//Chế độ hiện tại
	int CurrentMode;
	int MAX = 10;

	const int ButtonWidth = 134;
	const int ButtonHeight = 71;
	const int y = 81;

	vector<std::vector<LTexture>> StepTexture;
	vector<std::vector<LTexture>> NameTexture;
	vector<std::vector<LTexture>> TimeTexture;
	vector<std::vector<LTexture>> RankTexture;
	SDL_Rect *SpriteRect;
	vector<LTexture> ModeDisplay;
	vector<LButton> ModeDisplayButton;

	bool Quit;
	SDL_Event e;
public:
	LeaderBoard() : SpriteRect(new SDL_Rect[3]) {
		Current = 0;
		Quit = false;
		SpriteRect[0] = { 0, 0, 154, 71 };
		SpriteRect[1] = { 0, 99, 154, 71 };
		SpriteRect[2] = { 0, 198, 154, 71 };
		// Initialize vectors with proper size
		StepTexture.resize(ModeTotal, std::vector<LTexture>(10));
		NameTexture.resize(ModeTotal, std::vector<LTexture>(10));
		TimeTexture.resize(ModeTotal, std::vector<LTexture>(10));
		RankTexture.resize(ModeTotal, std::vector<LTexture>(10));
		ModeDisplay.resize(ModeTotal);
		ModeDisplayButton.resize(ModeTotal);
		LeaderBoardRecord.resize(ModeTotal);

		ModeDisplayButton[0].SetAllValue(142, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[1].SetAllValue(323, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[2].SetAllValue(817, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[3].SetAllValue(997, 81, ButtonWidth, ButtonHeight);

		// ... rest of your constructor code ...
	}
	~LeaderBoard() {
		LeaderBoardRecord.clear();
		LBTexture.free();
	}

	void Test()
	{
		for (int i = 0; i < 4; ++i)
		{
			vector<Record> tmp;
			for (int j = 0; j < 10; ++j)
			{
				tmp.emplace_back("Linh", "00 00", i + j);
			}
			LeaderBoardRecord.push_back(tmp);
		}
	}

	void RemoveAllScore()
	{
		LeaderBoardRecord.clear();
	}

	void DisplayScore()
	{
		for (int i = 0; i < LeaderBoardRecord.size(); ++i)
		{
			for (int j = 0; j < LeaderBoardRecord[i].size(); ++j)
			{
				cout << LeaderBoardRecord[i][j].name << " " << LeaderBoardRecord[i][j].time << " " << LeaderBoardRecord[i][j].step << endl;
			}
		}
	}

	static bool Compare(Record R1, Record R2)
	{
		int check = strcmp(R1.time.c_str(), R2.time.c_str());
		if (check < 0)
		{
			return true;
		}
		else if (check == 0)
		{
			return (R1.step < R2.step) ? true : false;
		}
		return false;
	}

	void Add(struct Record R, ModeLeaderBoard mode) {
		LeaderBoardRecord[static_cast<int>(mode)].push_back(R);
		sort(LeaderBoardRecord[static_cast<int>(mode)].begin(), LeaderBoardRecord[static_cast<int>(mode)].end(), Compare);
		if (LeaderBoardRecord[static_cast<int>(mode)].size() > 10) LeaderBoardRecord[static_cast<int>(mode)].pop_back();
	}

	bool SaveToFile(ModeLeaderBoard mode) {
		const string filename = "LeaderBoard-Record//HighScore.txt";
		std::ofstream file(filename);

		if (file.is_open()) {
			for (const auto& record : LeaderBoardRecord[static_cast<int>(mode)]) {
				file << record.name << "," << record.time << "," << record.step << std::endl;
			}
			file.close();
			std::cout << "Data has been saved to file " << filename << std::endl;
			return true;
		}
		else {
			std::cerr << "Unable to open file to save data." << std::endl;
			return false;
		}
	}

	bool LoadFromFile(ModeLeaderBoard mode) {
		bool success = true;
		if (!LBTexture.loadFromFile("IMG//LeaderBoard//Leader Board - Main.png", false))
		{
			cout << "Can't load the LBTexture!! SDL ERROR: " << SDL_GetError() << " IMG ERROR: " << IMG_GetError() << endl;
			success = false;
		}

		for (int i = 0; i < ModeTotal; ++i)
		{
			ModeDisplay[i].loadFromFile("IMG//LeaderBoard//Mode" + to_string(i) + ".png", false);
		}

		const string filename = "LeaderBoard-Record//HighScore" + to_string(mode + 3) + ".txt";

		std::ifstream file(filename);
		LeaderBoardRecord[static_cast<int>(mode)].clear(); // Clear old data from the vector before reading from the file

		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream ss(line);
				std::string name, time;
				int step;

				std::getline(ss, name, ',');
				std::getline(ss, time, ',');
				ss >> step;

				LeaderBoardRecord[static_cast<int>(mode)].emplace_back(name, time, step);
			}
			file.close();
			std::cout << "Data has been successfully read into LeaderBoardScore vector " << filename << std::endl;
			for (int i = 0; i < LeaderBoardRecord[static_cast<int>(mode)].size(); ++i) {
				cout << LeaderBoardRecord[static_cast<int>(mode)][i].name << " "
					<< LeaderBoardRecord[static_cast<int>(mode)][i].time << " "
					<< LeaderBoardRecord[static_cast<int>(mode)][i].step << endl;
			}
		}
		else {
			std::cerr << "Unable to open file to read data." << std::endl;
			success = false;
		}
		return success;
	}

	void LoadAllData()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < LeaderBoardRecord[i].size(); ++j)
			{
				string tmp = to_string(j + 1);
				RankTexture[i][j].loadFromRenderedText(tmp, { 0xFF, 0xFF, 0xFF, 0xFF }, 36);
				StepTexture[i][j].loadFromRenderedText(to_string(LeaderBoardRecord[i][j].step), { 0xFF, 0xFF, 0xFF, 0xFF }, 36);
				NameTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].name, { 0xFF, 0xFF, 0xFF, 0xFF }, 36);
				TimeTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].time, { 0xFF, 0xFF, 0xFF, 0xFF }, 36);
			}
		}
	}

	void render(ModeLeaderBoard mode)
	{
		LBTexture.render(0, 0);
		for (int i = 0; i < ModeTotal; ++i)
		{
			ModeDisplayButton[i].render(ModeDisplay[i], SpriteRect);
		}

		for (int i = 0; i < 5; ++i)
		{
			RankTexture[mode][Current + i].render(186, 272 + 78 * i);
			StepTexture[mode][Current + i].render(989, 272 + 78 * i);
			NameTexture[mode][Current + i].render(325, 272 + 78 * i);
			TimeTexture[mode][Current + i].render(782, 272 + 78 * i);
		}
	}
	void HandleEvent()
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				Quit = true;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				Quit = true;
				outGame = true;
			}
			if (e.wheel.y < 0)
			{
				if (Current + 1 < MAX - 4)
					++Current;
			}
			else if (e.wheel.y > 0)
			{
				if (Current - 1 >= 0)
					--Current;
			}
			for (int i = 0; i < 4; ++i)
			{
				ModeDisplayButton[i].HandleEvent(&e);
			}
		}
	}

	void run()
	{
		for (int i = Mode3x3; i < ModeTotal; ++i)
		{
			if (!LoadFromFile(ModeLeaderBoard(i)))
			{
				cout << "LeaderBoard can't load some file " << endl;
				return;
			}
		}
		LoadAllData();
		DisplayScore();
		while (!Quit)
		{
			LBTexture.render(0, 0);
			HandleEvent();
			render(ModeLeaderBoard(CurrentMode));
			SDL_RenderPresent(gRenderer);
		}
	}
};