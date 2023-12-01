﻿#pragma once
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
	int Current;
	vector<std::vector<LTexture>> StepTexture;
	vector<std::vector<LTexture>> NameTexture;
	vector<std::vector<LTexture>> TimeTexture;
	vector<std::vector<LTexture>> RankTexture;
	bool Quit;
public:
	LeaderBoard() {
		Current = 0;
		Quit = false;

		// Initialize vectors with proper size
		StepTexture.resize(ModeTotal, std::vector<LTexture>(10));
		NameTexture.resize(ModeTotal, std::vector<LTexture>(10));
		TimeTexture.resize(ModeTotal, std::vector<LTexture>(10));
		RankTexture.resize(ModeTotal, std::vector<LTexture>(10));

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

		const string filename = "LeaderBoard-Record//HighScore.txt";

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
			for (int j = 0; j < 10; ++j)
			{
				string tmp = to_string(j + 1);
				RankTexture[i][j].loadFromRenderedText(tmp, { 0xFF, 0xFF, 0xFF, 0xFF }, 48);
				StepTexture[i][j].loadFromRenderedText(to_string(LeaderBoardRecord[i][j].step), { 0xFF, 0xFF, 0xFF, 0xFF }, 48);
				NameTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].name, { 0xFF, 0xFF, 0xFF, 0xFF }, 48);
				TimeTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].time, { 0xFF, 0xFF, 0xFF, 0xFF }, 48);
			}
		}
	}

	void render(ModeLeaderBoard mode)
	{
		LBTexture.render(0, 0);
		for (int i = 0; i < 5; ++i)
		{
			RankTexture[mode][i].render(186, 272 + 78 * (i + Current));
			StepTexture[mode][i].render(989, 272 + 78 * (i + Current));
			NameTexture[mode][i].render(325, 272 + 78 * (i + Current));
			TimeTexture[mode][i].render(782, 272 + 78 * (i + Current));
		}
	}

	void run()
	{
		Test();
		SaveToFile(Mode3x3);
		if (!LoadFromFile(Mode3x3))
		{
			cout << "LeaderBoard can't load some file " << endl;
			return;
		}
		LoadAllData();
		DisplayScore();
		while (!Quit)
		{
			LBTexture.render(0, 0);
			render(Mode3x3);
			SDL_RenderPresent(gRenderer);
		}
	}
};