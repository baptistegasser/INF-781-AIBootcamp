#pragma once
#include <fstream>

class Logger
{
public:
	Logger();
	virtual ~Logger();

	void Init(const std::string& _logPath, const std::string& _logFileName);
	void Log(const std::string& _data, bool _bAutoNewLine = true);
	template <typename ...Args>
	void Logf(const std::string& _format, Args ..._args);

protected:
	std::ofstream mDebugFile;

private:
	template <typename ...Args>
	std::string format(const std::string& _format, Args ..._args);
};


template<typename ...Args>
void Logger::Logf(const std::string& _format, Args ..._args)
{
	auto msg = Logger::format(_format, _args...);
	this->Log(msg, false);
}

// Solution based on the excelente answer by iFreilicht : https://stackoverflow.com/a/26221725
template<typename ...Args>
std::string Logger::format(const std::string& _format, Args ..._args)
{
	// Attempt to write in a null, 0 sized buffer to get the wanted string size (+1 for null byte)
	int wantedSize = 1 + std::snprintf(nullptr, 0, _format.c_str(), _args...);

	if (wantedSize <= 0) {
		std::string errorMsg = "Failed to format message (invalid string size).";
		this->Log(errorMsg);
		throw std::runtime_error(errorMsg);
	}

	// Create a buffer with the necessary size and write to it
	auto buffer = std::make_unique<char[]>(wantedSize);
	int writtenSize = sprintf_s(buffer.get(), wantedSize, _format.c_str(), _args...);

	if (writtenSize <= 0) {
		std::string errorMsg = "Failed to format message (no char written).";
		this->Log(errorMsg);
		throw std::runtime_error(errorMsg);
	}

	// Return the formatted string
	return std::string(buffer.get(), buffer.get() + writtenSize);
}
