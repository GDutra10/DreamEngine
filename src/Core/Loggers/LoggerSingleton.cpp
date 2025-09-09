#include "LoggerSingleton.h"

using namespace DreamEngine::Core::Loggers;

LoggerSingleton& LoggerSingleton::Instance()
{
    static LoggerSingleton logger;

    return logger;
}

void LoggerSingleton::LogTrace(const std::string& message)
{
    LogMessage(LogLevel::Trace, message);
}

void LoggerSingleton::LogDebug(const std::string& message)
{
    LogMessage(LogLevel::Debug, message);
}

void LoggerSingleton::LogInfo(const std::string& message)
{
    LogMessage(LogLevel::Info, message);
}

void LoggerSingleton::LogWarning(const std::string& message)
{
    LogMessage(LogLevel::Warning, message);
}

void LoggerSingleton::LogError(const std::string& message)
{
    LogMessage(LogLevel::Error, message);
}

void LoggerSingleton::Attach(Logger* logger)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    const auto iterator = std::find(m_loggers.begin(), m_loggers.end(), logger);

    if (iterator == m_loggers.end() || m_loggers.empty())
    {
        m_loggers.push_back(logger);
    }
}

void LoggerSingleton::Detach(Logger* logger)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto iterator = std::remove(m_loggers.begin(), m_loggers.end(), logger);
    m_loggers.erase(iterator, m_loggers.end());
}

void LoggerSingleton::HandleMessages()
{
    while (!m_stopFlag.load())
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return !m_logs.empty() || m_stopFlag.load(); });
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        while (!m_logs.empty())
        {
            const auto& log = m_logs.front();

            for (const auto logger : m_loggers)
            {
                logger->Process(log);
            }

            m_logs.pop();
        }
    }
}

LoggerSingleton::LoggerSingleton()
{
    m_loggers = std::vector<Logger*>();
    m_logs = std::queue<Log>();

    // initialize the thread
    m_loggerThread = std::thread(&LoggerSingleton::HandleMessages, this);
    //m_loggerThread.detach(); // detach to run independently
}

LoggerSingleton::~LoggerSingleton()
{
    m_stopFlag.store(true);

    // just wait to finish the thread
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m_cv.notify_all();  // Wake up the thread to finish
    if (m_loggerThread.joinable())
    {
        m_loggerThread.join();  // Properly wait for the thread to finish
    }
}

void LoggerSingleton::LogMessage(const LogLevel logLevel, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    Log log;
    log.dateTime = std::chrono::system_clock::now();
    log.logLevel = logLevel;
    log.message = message;

    m_logs.push(log);
    m_cv.notify_one();  // Notify the logging thread
}
