// FIND.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Header.h"
#include <ShlObj_core.h>
#pragma comment(lib, "urlmon.lib")    // For URLDownloadToFileW
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <locale>
#include <codecvt>
#define WIN32_LEAN_AND_MEAN // Prevent windows.h from including winsock.h
#include <winsock2.h>       // Must come first
#include <windows.h>        // After winsock2.h
#include <ws2tcpip.h>       // For getaddrinfo and inet_ntop
#include <iostream>
#include <iphlpapi.h>       // For GetAdaptersAddresses
#include <shellapi.h>
#include <urlmon.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct countryData { //Each red rectangle is 11x11 pixels
	string name;
	int x;
	int y;


};

int main()
{
	//XXXXXXXXXXXXXXXXXXXXXXXXXX works locally but not on the internet, likely firewall. install errorcatching for sending and reciving messages 
	HWND consoleWindow = GetConsoleWindow();
	string placeholderString;
	int placeholderInt;
	bool Mode; //0 for countrys 1 for cities
	SDL_Color black = { 0, 0, 0, 0 };
	SDL_Event event;
	bool placeholderBool = false;
	int score=0;
	int highScore;
	if (!IsUserAnAdmin())
	{
		if (!RelaunchAsAdmin())TerminalError("RelaunchAsAdmin Error Exiting.....\n", GetConsoleWindow());
		cout << "This Program requires Admin Permissions, relaunching.....\n";
		return 0;
	}


	countryData countries[38] = { //These Coordinates are within the Picture, keep in mind to change them accoring to where the picture is renderd
		{"Ireland",122,388},
		{"United Kingdom",242,409},
		{"Portugal",88,695},
		{"Spain",183,679},
		{"France",294,512},
		{"Belgium",340,442},
		{"Netherlands",349,411},
		{"Germany",421,410},
		{"Switzerland",398,521},
		{"Italy",454,553},
		{"Norway",443,235},
		{"Sweden",501,297},
		{"Finland",689,220},
		{"Czechia",501,458},
		{"Austria",532,503},
		{"Slovenia",521,540},
		{"Croatia",528,573},
		{"Bosnia and Herzigovina",581,591},
		{"Serbia",632,590},
		{"Montenegro",602,618},
		{"Kosovo",634,622},
		{"North Macedonia",634,622},
		{"Albania",624,666},
		{"Greece",652,686},
		{"Bulgaria",713,624},
		{"Romania",725,555},
		{"Hungary",601,520},
		{"Slovakia",588,484},
		{"Poland",565,396},
		{"Lithuania",673,347},
		{"Latvia",680,301},
		{"Estonia",696,265},
		{"Ukraine",794,463},
		{"Belarus",726,381},
		{"Russia",873,300},
		{"Russia",794,463},
		{"Turkey",801,698},
		{"Moldova",769,521}
	};

	cout << "Welcome!\n";
	if (fileExists("Highscore.txt"))
	{
		cout << "Current Highscore:" << read_file_to_string("Highscore.txt") << endl;
		highScore = stoi(read_file_to_string("Highscore.txt"));
	}

	else
	{
		cout << "No Highscore found, creating one new\n";
		writeToFile("Highscore.txt", "0");
		highScore = 0;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != true) TerminalError("SDL_INIT Error.->" + string(SDL_GetError()), consoleWindow);
	if (TTF_Init() != true) TerminalError("TTF_INIT Error.->" + string(SDL_GetError()), consoleWindow);

	SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1000, 1000, NULL);
	if (!window) TerminalError("SDL_CreateWindow Error.->" + string(SDL_GetError()), consoleWindow);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) TerminalError("SDL_CreateRenderer Error.->" + string(SDL_GetError()), consoleWindow);

	SDL_Texture* chooseTexture = IMG_LoadTexture(renderer, "Choose_Mode.png");
	if (!chooseTexture) TerminalError("IMG_LoadTexture Error. ->" + string(SDL_GetError()), consoleWindow);
	SDL_RenderTexture(renderer, chooseTexture, 0, 0);
	SDL_RenderPresent(renderer);

	while (!placeholderBool) {
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (event.button.x > 91 && event.button.x < 419 && event.button.y>230 && event.button.y < 500)
				{
					Mode = 0;
					placeholderBool = true;


				}


			}


		}
	}
	if (Mode == 0)//country mode
	{

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		SDL_Texture* europeMap = IMG_LoadTexture(renderer, "europe_map.png");
		if (!europeMap)TerminalError("IMG_LoadTexture Error. ->" + string(SDL_GetError()), consoleWindow);
		SDL_FRect Map_rect{ 100.0f,100.0f, 900.0f,900.0f };
		while (true)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, europeMap, 0, &Map_rect);
			placeholderInt = random(0, 37);
			TTF_Font* font = TTF_OpenFont("arial.ttf", 50); if (!font) { TerminalError("TTF_OpenFont Error.->" + string(SDL_GetError()), consoleWindow); }
			SDL_Surface* countryTextSurface = TTF_RenderText_Solid(font, countries[placeholderInt].name.c_str(), countries[placeholderInt].name.length(), black); if (!countryTextSurface)TerminalError("TTF_RenderText_Solid Error.->" + string(SDL_GetError()), consoleWindow);
			SDL_Texture* countryTextTexture = SDL_CreateTextureFromSurface(renderer, countryTextSurface); if (!countryTextTexture)TerminalError("SDL_CreateTextureFromSurface Error.->" + string(SDL_GetError()), consoleWindow);
			SDL_FRect countryTextRect{ 5,5,countryTextSurface->w,countryTextSurface->h };
			SDL_RenderTexture(renderer, countryTextTexture, 0, &countryTextRect);
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			for (int i = 0; i < 38; ++i) {
				SDL_FRect rect = {
					static_cast<float>(countries[i].x + 100),
					static_cast<float>(countries[i].y + 100),
					11.0f,
					11.0f
				};
				SDL_RenderFillRect(renderer, &rect);
			}
			SDL_RenderPresent(renderer);
			placeholderBool = false;
			while (!placeholderBool)
			{
				while (SDL_PollEvent(&event))
				{


					if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
					{
						if (event.button.x > countries[placeholderInt].x + 100 && event.button.x < countries[placeholderInt].x + 100 + 11 && event.button.y > countries[placeholderInt].y + 100 && event.button.y < countries[placeholderInt].y + 100 + 11)
						{
							cout << "Correct! You clicked at:" << event.button.x << "," << event.button.y << "\n";
							placeholderBool = true;
							score++;
							if (score > highScore)
							{
								highScore = score;
								writeToFile("Highscore.txt", to_string(highScore));
								cout << "New Highscore: " << highScore << "\n";
							}
						}
						else
						{
							cout << "Wrong! You clicked at:" << event.button.x << "," << event.button.y << "\n";

						}

					}



				}



			}

		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();



	return 0;
}