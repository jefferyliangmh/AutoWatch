// Logger.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AppLogger.h"
#include "boost/shared_ptr.hpp"

#include <boost/locale/generator.hpp>
#include <boost/log/common.hpp>
#include "boost/log/attributes/timer.hpp"
#include "boost/log/support/date_time.hpp"
#include "boost/log/sources/logger.hpp"
#include "boost/log/attributes.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/sinks.hpp"

#include <exception>
#include <boost/log/utility/exception_handler.hpp>
#include <boost/log/exceptions.hpp>

namespace Logging = boost::log;
namespace Attributes = boost::log::attributes;
namespace Sources = boost::log::sources;
namespace Sinks = boost::log::sinks;
namespace Expressions = boost::log::expressions;
namespace Keywords = boost::log::keywords;

using boost::shared_ptr;

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, SeverityLevel lvl)
{
	static const char* const str[] =
	{
		"Information",
		"Debug",
		"Warning",
		"Error",
		"Critical"
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}

BOOST_LOG_ATTRIBUTE_KEYWORD(_severity, "Severity", SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(_timestamp, "TimeStamp", boost::posix_time::ptime)


struct ErrorHandler
{
	void operator()(const Logging::runtime_error &ex) const
	{
		std::cerr << "boost::log::runtime_error: " << ex.what() << '\n';
	}

	void operator()(const std::exception &ex) const
	{
		std::cerr << "std::exception: " << ex.what() << '\n';
	}
};

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(glogger, Sources::wseverity_logger<SeverityLevel>)

class Logger::LoggerImpl
{
public:
	LoggerImpl(void)
	{
		InitLogger();
	}

	~LoggerImpl()
	{
	}

	void Log(const std::string& message, const SeverityLevel& level=Notification)
	{
		//Sources::wseverity_logger<SeverityLevel> lg;
		BOOST_LOG_SEV(glogger::get(), level) << message;
	}

private:
	typedef Sinks::text_file_backend BackEndType;
	typedef Sinks::synchronous_sink<BackEndType> FileSinkType;

	void InitLogger(void)
	{
		std::locale::global(std::locale("CHS"));

		shared_ptr<BackEndType> bankEnd = boost::make_shared<BackEndType>(
			Keywords::file_name = "%Y%m%d_%H%M%S_%5N.log",
			Keywords::rotation_size = 1 * 1024 * 1024,
			Keywords::time_based_rotation = Sinks::file::rotation_at_time_point(0, 0, 0),
			Keywords::auto_flush = true);

		shared_ptr<FileSinkType> sink(new FileSinkType(bankEnd));

		sink->locked_backend()->set_file_collector(Sinks::file::make_collector(
			Keywords::target = "logs",
			Keywords::min_free_space = 100 * 1024 * 1024));
		sink->locked_backend()->scan_for_files();

		sink->set_formatter(Expressions::stream
			<< "["
			<< Expressions::format_date_time(_timestamp, "%Y-%m-%d, %H:%M:%S.%f")
			<< "][" << _severity
			<< "][" << Expressions::message
			<< "]");

		std::locale loc = boost::locale::generator()("UTF-8");
		sink->imbue(loc);

		Logging::core::get()->add_sink(sink);

		Logging::core::get()->set_exception_handler(
			Logging::make_exception_handler<Logging::runtime_error, std::exception>(ErrorHandler{}));
	
		Logging::core::get()->add_global_attribute("TimeStamp", Attributes::local_clock());
	}
};
boost::shared_ptr<Logger::LoggerImpl> _logger(new Logger::LoggerImpl());


void Logger::Log(const std::string& message, const SeverityLevel& level)
{
	_logger->Log(message,level);
}

void Logger::Log(const std::string& message)
{
	_logger->Log(message);
}
