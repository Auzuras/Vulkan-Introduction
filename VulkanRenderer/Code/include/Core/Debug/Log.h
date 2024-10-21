#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <Windows.h>

#define LOG_FOLDER_PATH "Logs/"
#define LOG_PRINT "Log: "
#define LOG_WARN "Warning: "
#define LOG_ERROR "Error: "

#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// TODO: Change the vs console output
#define DEBUG_LOG(_Format, ...) \
{ \
	Debug::Log::DebugPrint(_Format, __VA_ARGS__);\
	std::ostringstream _output; \
	_output << __FILE_NAME__ << "(" << __LINE__ << ") [Log]: " << _Format << '\n';\
	OutputDebugStringA(_output.str().c_str());\
}

// TODO: Change the vs console output
#define DEBUG_WARN(_Format, ...) \
{ \
	Debug::Log::DebugWarn(_Format, __VA_ARGS__);\
	std::ostringstream _output; \
	_output << __FILE_NAME__ << "(" << __LINE__ << ") [Warning]: " << _Format << '\n';\
	OutputDebugStringA(_output.str().c_str());\
}

// TODO: Change the vs console output
#define DEBUG_ERROR(_Format, ...) \
{ \
	Debug::Log::DebugError(_Format, __VA_ARGS__);\
	std::ostringstream _output; \
	_output << __FILE_NAME__ << "(" << __LINE__ << ") [Error]: " << _Format << '\n';\
	OutputDebugStringA(_output.str().c_str());\
}

constexpr auto DL_RED = "\33[91m";
constexpr auto DL_YELLOW = "\33[93m";
constexpr auto DEND = "\033[0m";


namespace Core
{
	namespace Debug
	{
		enum class LogType
		{
			LOG,
			WARN,
			ERR
		};

		class Log
		{
		private:
			static inline std::ofstream m_LogFile;

			// TODO: REFACTOR AND ENCAPSULATE THIS SHIT MEN
			static int LogPrint(const LogType _Type, const char* _Format, va_list _Args);

		public:
			/// <summary>
			/// Opens a single log file that will register all console messages on it
			/// </summary>
			/// <param name="_Path"> : Path of the log file </param>
			/// <returns></returns>
			static const bool OpenFile(const std::filesystem::path& _Path);

			/// <summary>
			/// Closes the log file if openned
			/// </summary>
			/// <returns></returns>
			static const bool CloseFile();

			static int DebugPrint(const char* _Format, ...);
			static int DebugWarn(const char* _Format, ...);
			static int DebugError(const char* _Format, ...);

			static std::string GetCurrentLocalTime();

		};
	}
}