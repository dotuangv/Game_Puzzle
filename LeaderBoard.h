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

class LeaderBoard {
private:
	vector<Record> LeaderBoardRecord;
	LTexture LBTexture;
	SDL_Renderer* LBRenderer;
public:
	LeaderBoard() {
		/*LBRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (LBRenderer == NULL) {
			cout << "Can't Create LBRenderer: " << SDL_GetError() << endl;
		}*/
	}
	~LeaderBoard() {
		LeaderBoardRecord.clear();
		LBTexture.free();
	}

	void Test()
	{
		for (int i = 0; i < 10; ++i)
		{
			LeaderBoardRecord.emplace_back("Linh", "00 00", i);
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
			cout << LeaderBoardRecord[i].name << " " << LeaderBoardRecord[i].time << " " << LeaderBoardRecord[i].step << endl;
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

	void Add(struct Record R)
	{
		LeaderBoardRecord.push_back(R);
		sort(LeaderBoardRecord.begin(), LeaderBoardRecord.end(), Compare);
		if (LeaderBoardRecord.size() > 10) LeaderBoardRecord.pop_back();
	}
	
	bool SaveToFile() {
		const string filename = "LeaderBoard-Record//HighScore.txt";
		std::ofstream file(filename);

		if (file.is_open()) {
			for (const auto& record : LeaderBoardRecord) {
				file << record.name << "," << record.time << "," << record.step << std::endl;
			}
			file.close();
			std::cout << "Dữ liệu đã được lưu vào file " << filename << std::endl;
			return true;
		}
		else {
			std::cerr << "Không thể mở file để lưu dữ liệu." << std::endl;
			return false;
		}
	}

	bool LoadFromFile() {
		bool success = true;
		if (!LBTexture.loadFromFile("IMG//LeaderBoard//Leader Board - Main.png", false))
		{
			cout << "Can't load the LBTexture!! SDL ERROR: " << SDL_GetError() << " IMG ERROR: " << IMG_GetError() << endl;
			success = false;
		}

		const string filename = "LeaderBoard-Record//HighScore.txt";

		std::ifstream file(filename);
		LeaderBoardRecord.clear(); // Xóa dữ liệu cũ trong vector trước khi đọc từ file

		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream ss(line);
				std::string name, time;
				int step;

				std::getline(ss, name, ',');
				std::getline(ss, time, ',');
				ss >> step;

				LeaderBoardRecord.emplace_back(name, time, step);
			}
			file.close();
			std::cout << "Du lieu da duoc doc thanh cong vao Vector LeaderBoardScore " << filename  << std::endl;
			for (int i = 0; i < LeaderBoardRecord.size(); ++i)
			{
				cout << LeaderBoardRecord[i].name <<" " 
					<< LeaderBoardRecord[i].time << " " 
					<< LeaderBoardRecord[i].step << endl;
			}
		}
		else {
			std::cerr << "Khong The Mo File Doc du lieu" << std::endl;
			success = false;
		}
		return success;
	}

	void render()
	{
		LBTexture.render(0, 0);

	}

	void run()
	{
		/*SaveToFile();*/
		if (!LoadFromFile())
		{	
			cout << "LeaderBoard can't load some file " << endl;
			return;
		}
		DisplayScore();
		render();
		SDL_RenderPresent(gRenderer);
		SDL_Delay(50000);
	}
};