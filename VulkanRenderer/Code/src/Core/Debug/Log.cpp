#include "Debug/Log.h"

namespace Core
{
	namespace Debug
	{
		int Log::LogPrint(const LogType _Type, const char* _Format, va_list _Args)
		{
			std::string token = "";
			std::ostringstream finalPrint;

			for (size_t i = 0; _Format[i] != '\0'; ++i)
			{
				if (_Format[i] != '%')
				{
					finalPrint << _Format[i];
					continue;
				}

				while (_Format[i + 1] != '\0' && _Format[i + 1] != ' ' && _Format[i + 1] >= 'a' && _Format[i + 1] <= 'u')
				{
					token += _Format[i + 1];
					++i;
				}

				if (token.size() > 1)
				{
					switch (token[0])
					{
					case 'l':
						// long cases
						switch (token[1])
						{
							// long int case
						case 'i': case 'd': case 'u': case 'h':
							finalPrint << va_arg(_Args, long);
							break;
							// double case
						case 'f':
							finalPrint << va_arg(_Args, double);
							break;
						}
						break;
					case 'L':
						// long long cases
						switch (token[1])
						{
							// long long int case
						case 'i': case 'd':
							finalPrint << va_arg(_Args, long long);
							break;
							// long double case
						case 'f':
							finalPrint << va_arg(_Args, long double);
							break;
						}
						break;
						// short cases
					case 'h':
						switch (token[1])
						{
							// short
						case 'i': case 'd':
							finalPrint << va_arg(_Args, short);
							break;
						}
						break;
					}
				}
				else if (token.size() == 1)
				{
					switch (token[0])
					{
						// int case
					case 'i': case 'd':
						finalPrint << va_arg(_Args, int);
						break;
						// usigned int
					case 'u':
						finalPrint << va_arg(_Args, unsigned int);
						break;
						// char case
					case 'c':
						finalPrint << va_arg(_Args, char);
						break;
						// float case
					case 'f':
						finalPrint << va_arg(_Args, float);
						break;
						// string case
					case 's':
						finalPrint << va_arg(_Args, char*);
						break;
						// pointer case
					case 'p':
						finalPrint << va_arg(_Args, void*);
					}
				}

				token = "";
			}

			int result = 0;

			std::string strForm = finalPrint.str();
			std::string finalForm;

			switch (_Type)
			{
			default: case LogType::LOG:
				finalForm = "[Log]: " + strForm + '\n';
				result = printf(finalForm.c_str());
				break;
			case LogType::WARN:
				finalForm = "[Warning]: " + strForm + '\n';
				result = printf("%s%s%s", DL_YELLOW, finalForm.c_str(), DEND);
				break;
			case LogType::ERR:
				finalForm += "[Error]: " + strForm + '\n';
				result = printf("%s%s%s", DL_RED, finalForm.c_str(), DEND);
				break;
			}

			if (m_LogFile.is_open())
				m_LogFile << Log::GetCurrentLocalTime() << " " << finalForm;

			return result;
		}

		const bool Log::OpenFile(const std::filesystem::path& _Path)
		{
			// Checks if Logs folder exist and create it if not
			if (!std::filesystem::exists("Logs"))
			{
				DEBUG_WARN("Creation of the Logs folder because it doesn't exists");
				std::filesystem::create_directories("Logs");
			}

			// Checks if log file is already openned
			if (Log::m_LogFile.is_open())
			{
				DEBUG_WARN("Log file is already opened");
				return true;
			}

			// Creates log file
			m_LogFile.open(_Path);

			// Checks if log file didn't openned
			if (!Log::m_LogFile.is_open())
			{
				DEBUG_ERROR("Unable to open the Log file")

					// Cheks if the reason is the log folder
					if (!std::filesystem::exists("Logs"))
						DEBUG_ERROR("Logs folder is missing - Uable to create it - Try creating it manually");

				return false;
			}

			DEBUG_LOG("Log file has been created and opened successfully");

			return true;
		}

		const bool Log::CloseFile()
		{
			DEBUG_LOG("Closing Log file");

			// Cheks if log file is already closed
			if (!Log::m_LogFile.is_open())
			{
				DEBUG_WARN("Log file already closed or not opened")
					return true;
			}

			// Close the file
			m_LogFile.close();

			// Checks if it has been closed correctly
			if (Log::m_LogFile.is_open())
			{
				DEBUG_ERROR("Unable to close Log file");
				return false;
			}

			DEBUG_LOG("Log file successfully closed");

			return true;
		}

		int Log::DebugPrint(const char* _Format, ...)
		{
			//TODO: rework
			va_list args;
			va_start(args, _Format);

			int result = Log::LogPrint(LogType::LOG, _Format, args);

			va_end(args);

			return result;
		}

		int Log::DebugWarn(const char* _Format, ...)
		{
			//TODO: rework
			va_list args;
			va_start(args, _Format);

			int result = Log::LogPrint(LogType::WARN, _Format, args);

			va_end(args);

			return result;
		}

		int Log::DebugError(const char* _Format, ...)
		{
			//TODO: rework
			va_list args;
			va_start(args, _Format);

			int result = Log::LogPrint(LogType::ERR, _Format, args);

			va_end(args);

			return result;
		}

		std::string Log::GetCurrentLocalTime()
		{
			std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
			time_t currentTime = std::chrono::system_clock::to_time_t(time);

			std::tm localTime;
			localtime_s(&localTime, &currentTime);

			std::ostringstream oss;
			oss << std::put_time(&localTime, "%H:%M:%S");

			std::string format = oss.str();

			return format;
		}

	}
}