#include <boost/log/core.hpp>
#include "log.h"

namespace dev {
std::string const FileLogger = "FileLogger";
boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string> FileLoggerHandler(
    boost::log::keywords::channel = FileLogger);

std::string const StatFileLogger = "StatFileLogger";
boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string> StatFileLoggerHandler(
    boost::log::keywords::channel = StatFileLogger);

LogLevel c_fileLogLevel = LogLevel::TRACE;
LogLevel c_statLogLevel = LogLevel::INFO;

void setFileLogLevel(LogLevel const& _level) { c_fileLogLevel = _level; }

void setStatLogLevel(LogLevel const& _level) { c_statLogLevel = _level; }

std::string GetFileName(const std::string& file_path) {
    auto pos = file_path.find_last_of('/');
    if (pos != std::string::npos) {
        return file_path.substr(pos + 1, file_path.size() - pos);
    }
    return file_path;
}
}  // namespace dev