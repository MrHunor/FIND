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

bool RelaunchAsAdmin()
{
    wchar_t exePath[MAX_PATH];
    // Get the full path of the current executable
    if (GetModuleFileNameW(NULL, exePath, MAX_PATH) == 0)
        return false;

    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";              // Run as administrator
    sei.lpFile = exePath;               // Path to this executable
    sei.lpParameters = NULL;            // No parameters, change if needed
    sei.nShow = SW_NORMAL;
    sei.fMask = SEE_MASK_NOASYNC;       // Wait until the process is launched

    if (!ShellExecuteExW(&sei)) {
        DWORD err = GetLastError();
        if (err == ERROR_CANCELLED) {
            // User refused the elevation
            MessageBoxW(NULL, L"Administrator privileges are required to continue.", L"Elevation required", MB_OK | MB_ICONWARNING);
        }
        return false;
    }
    return true;
}


bool writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename, std::ios::trunc); // std::ios::trunc ensures overwrite
    if (!outFile) {
        return 0;
    }
    outFile << content;
    outFile.close();
    return 1;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
std::wstring getExecutablePath() {
    wchar_t buffer[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, buffer, MAX_PATH);
    return std::wstring(buffer, length);
}
std::wstring getExecutableDir() {
    std::wstring fullPath = getExecutablePath();
    size_t pos = fullPath.find_last_of(L"\\/");
    return (pos != std::wstring::npos) ? fullPath.substr(0, pos) : L"";
}


std::string Wstring2String(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

void TerminalError(string message,HWND consoleWindow)
{
    SetForegroundWindow(consoleWindow);
	cout << "A TERMINAL error occured, error details:\n" << message << "\n" << "Exit?"<<endl;
	system("pause");
    exit(-1);



}



int random(int lower_bound, int upper_bound)
{
    srand(time(0));


    return rand() % (upper_bound - lower_bound + 1)
        + lower_bound;


}

bool yes_or_no_random()
{
    if (random(0, 1) == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

string read_file_to_string(string file_path)
{
    string filestring;

    ifstream filehandler(file_path);

    if (!filehandler.is_open())
    {
        cout << "Error opening file" << endl;
        return NULL;
    }
    filehandler >> filestring;
    filehandler.close();
    if (filehandler.is_open())
    {
        cout << "Error closing file" << endl;
        return 0;
    }


    return filestring;

}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::size_t start = 0;
    std::size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1; // Move past the delimiter
        end = str.find(delimiter, start);
    }

    // Add the last substring
    result.push_back(str.substr(start));

    return result;
}

void overwrite_line_in_file(const std::string& filePath, int lineNumber, const std::string& newContent) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open the file for reading: " + filePath);
    }

    std::vector<std::string> lines;
    std::string line;
    int currentLine = 0;

    // Read all lines from the file
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    // Check if the line number is valid
    if (lineNumber <= 0 || lineNumber > static_cast<int>(lines.size())) {
        throw std::out_of_range("Line number is out of range.");
    }

    // Update the specific line
    lines[lineNumber - 1] = newContent;  // lineNumber is 1-based, vector is 0-based

    // Write the updated content back to the file
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Could not open the file for writing: " + filePath);
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << '\n';
    }
}

void Log(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios_base::app);
    logFile << message << std::endl;
}




bool copy_file(const std::string& source, const std::string& destinationDir) {
    // Open the source file for reading in binary mode
    std::ifstream sourceFile(source, std::ios::binary);
    if (!sourceFile) {
        std::cerr << "Error opening source file: " << source << std::endl;
        return false;
    }

    // Extract the file name from the source path
    size_t lastSlash = source.find_last_of("\\/");
    std::string fileName = (lastSlash != std::string::npos) ? source.substr(lastSlash + 1) : source;

    // Create the destination file path
    std::string destination = destinationDir + fileName;

    // Open the destination file for writing in binary mode
    std::ofstream destinationFile(destination, std::ios::binary);
    if (!destinationFile) {
        std::cerr << "Error opening destination file: " << destination << std::endl;
        return false;
    }

    // Copy the contents from source to destination
    destinationFile << sourceFile.rdbuf();

    // Check if everything went well
    if (!destinationFile) {
        std::cerr << "Error writing to destination file." << std::endl;
        return false;
    }

    return true;
}

std::string convertSecondsToTime(int totalSeconds) {
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

  ostringstream timeStream;
    timeStream << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return timeStream.str();
}


int countLinesInFile(const std::string& filePath) {
    std::ifstream file(filePath); // Open the file
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filePath);
    }

    int lineCount = 0;
    std::string line;

    // Read each line until the end of the file
    while (std::getline(file, line)) {
        ++lineCount; // Increment the line count for each line read
    }

    return lineCount;
}



std::string getLineFromFile(const std::string& filePath, int lineNumber) {
    std::ifstream file(filePath); // Open the file
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filePath);
    }

    std::string line;
    int currentLine = 0;

    // Read lines until the desired line number is reached
    while (std::getline(file, line)) {
        if (++currentLine == lineNumber) {
            return line; // Return the line when lineNumber is matched
        }
    }

    throw std::out_of_range("Line number exceeds the total lines in the file.");
}

//uses #iostream, #vector, #string
bool brute_force_password(string current_guess/*Leave this empty ("")*/, const string& correct_password) {//chatgpt :-)
    vector<string> alphabet = {
          "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",//26
          "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    };
    // Base case: if the current guess matches the password
    if (current_guess == correct_password) {
        cout << "Password cracked: " << current_guess << endl;
        return true;
    }

    // Stop recursion if the guess length exceeds the correct password length
    if (current_guess.length() >= correct_password.length()) {
        return false;
    }

    // Loop through the alphabet and build the next guess
    for (const string& letter : alphabet) {
        string next_guess = current_guess + letter; // Append the next letter
        cout << "Trying: " << next_guess << endl;

        // Recursive call to test the next combination
        if (brute_force_password(next_guess, correct_password)) {
            return true; // Stop further guesses once the password is found
        }
    }

    return false;
}
