#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
extern bool brute_force_password(string current_guess, const string& correct_password);
extern int random(int lower_bound, int upper_bound);
extern std::string getLineFromFile(const std::string& filePath, int lineNumber);
extern int countLinesInFile(const std::string& filePath);
extern bool copy_file(const std::string& source, const std::string& destinationDir);
extern string read_file_to_string(string file_path);
extern void overwrite_line_in_file(const std::string& filePath, int lineNumber, const std::string& newContent);
extern std::vector<std::string> splitString(const std::string& str, char delimiter);
extern std::string convertSecondsToTime(int totalSeconds);
extern bool yes_or_no_random();
extern void Log(const std::string& message);
extern void TerminalError(string message, HWND consoleWindow);
extern bool CheckYggdrasilInstallStatus();
extern bool DownloadYggdrasil(const std::wstring& destinationDir);
extern bool InstallYggdrasilMSI(const std::wstring& msiPath);
extern std::wstring getExecutablePath();
extern std::string Wstring2String(const std::wstring& wstr);
extern std::wstring getExecutableDir();
extern bool addToUserPath(const std::wstring& folderPath);
extern bool RelaunchAsAdmin();
extern void StartYggdrasil();