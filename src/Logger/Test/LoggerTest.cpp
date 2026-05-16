#include "Logger.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

class ScopedCoutRedirect {
public:
  ScopedCoutRedirect() : mOldBuffer(std::cout.rdbuf(mBuffer.rdbuf())) {}
  ~ScopedCoutRedirect() { std::cout.rdbuf(mOldBuffer); }

  std::string Get() const { return mBuffer.str(); }

private:
  std::streambuf* mOldBuffer;
  std::stringstream mBuffer;
};

using namespace Components::Logger;
struct Case {
  std::string testName;
  Category category;
  std::string categoryStr;
  Level level;
  std::string levelStr;
  std::string message;
};

void PrintTo(const Case& c, std::ostream* os) {
  *os << "Test Name: " << c.testName << "\n";
  *os << " (Category: " << c.categoryStr << ", Level: " << c.levelStr << ", Message: " << c.message << ")";
}

class LoggerTest : public ::testing::TestWithParam<Case> {
protected:
};

// Constructor Test
TEST_F(LoggerTest, Constructor) { EXPECT_NO_THROW(Components::Logger::Logger logger); }

TEST_P(LoggerTest, IOTests) {
  auto param = GetParam();

  Components::Logger::Logger logger;
  {
    ScopedCoutRedirect redirect;

    switch (param.level) {
    case Level::Info:
      logger.Info(param.category, LogMessage("%s", param.message.c_str()));
      break;
    case Level::Warn:
      logger.Warn(param.category, LogMessage("%s", param.message.c_str()));
      break;
    case Level::Error:
      logger.Error(param.category, LogMessage("%s", param.message.c_str()));
      break;
    case Level::Trace:
      logger.Trace(param.category, LogMessage("%s", param.message.c_str()));
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    EXPECT_THAT(redirect.Get(), testing::HasSubstr(param.levelStr)) << "Failed in test case: " << param.testName;
    EXPECT_THAT(redirect.Get(), testing::HasSubstr(param.message)) << "Failed in test case: " << param.testName;
    EXPECT_THAT(redirect.Get(), testing::HasSubstr(param.categoryStr)) << "Failed in test case: " << param.testName;
  }
}

INSTANTIATE_TEST_SUITE_P(
    LoggerTests, LoggerTest,
    ::testing::Values(Case{"Info - Init", Category::Init, "Init", Level::Info, "INF", "Initialization complete"},
                      Case{"Warn - Thread", Category::Thread, "Thread", Level::Warn, "WRN", "Thread is running low on resources"},
                      Case{"Error - Init", Category::Init, "Init", Level::Error, "ERR", "Failed to initialize component"},
                      Case{"Trace - Thread", Category::Thread, "Thread", Level::Trace, "TRC", "Thread execution trace"},
                      Case{"Info - Uknown", static_cast<Category>(0xFF), "Unknown", Level::Trace, "TRC", "Thread execution trace"}));