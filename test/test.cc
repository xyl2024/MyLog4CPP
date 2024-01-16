#include "../mylog/header/mylog.h"

using std::cout;
using std::endl;

void test_util()
{
    // cout << mylog::TimeUtil::getTime() << endl;
    // std::cout << "The current system time is: " << mylog::TimeUtil::get_system_time().count() << std::endl;

    // std::string file = "/root/a.out";
    // if (mylog::FileUtil::exists(file))
    //     cout << "存在\n";
    // else
    //     cout << "不存在\n";

    // std::string path = "/root/my-logger/src/logger.cc";
    // std::string path = "logger.cc";
    // std::string path = "/logger.cc";
    // std::string path = "./logger.cc";
    // cout << mylog::FileUtil::getPath(path) << endl;

    // std::string path = "./testDir/log/xxxx.log";
    std::string path = "./a/b/c/xxxx.log";
    mylog::FileUtil::createDir(path);
}

void test_Level()
{
    cout << mylog::Level::toString(mylog::Level::Value::INFO) << endl;
    cout << mylog::Level::toString(mylog::Level::Value::DEBUG) << endl;
    cout << mylog::Level::toString(mylog::Level::Value::ERROR) << endl;
    cout << mylog::Level::toString(mylog::Level::Value::FATAL) << endl;
}

// void printLogInfo(const mylog::LogInfo &info)
// {
//     printf("日志等级：%s\n", mylog::Level::toString(info.level).c_str());
//     printf("时间戳：%d\n", info.timestamp);
//     printf("文件名：%s\n", info.file.c_str());
//     printf("行号：%d\n", info.line);
//     printf("线程ID：");
//     cout << info.tid << endl;
//     printf("日志器名称：%s\n", info.loggerName.c_str());
//     printf("日志主题消息：%s\n", info.message.c_str());
// }

// /*
//     测试格式化模块，
//     由于格式化模块使用了ostream
//     需要：测试一下多线程同时调用格式化器有没有问题。
// */
// void test_formatter()
// {
//     mylog::LogInfo info(mylog::Level::Value::DEBUG, "root", "main.cc", 123, "Formatter测试....");
//     printLogInfo(info);
//     // mylog::Formatter fmt;
//     mylog::Formatter fmt("abcd测大是大非啊实打实大三[%t][%c asdasdasdas][%n ] [%i ][%f%S:%S%l]%T%m%N%%");
//     std::string str = fmt.run(info);
//     std::cout << str;
// }

/**
 * @brief 测试日志落地模块
 *
 */
void test_sinker()
{
    // using namespace mylog;

    // // LogInfo info(Level::Value::DEBUG, "root", "main.cc", 123, "套接字创建失败......");

    // Formatter fmt;
    // // std::string message = fmt.run(info);

    // std::cout << message;

    // Sinker::ptr console_sinker = SinkerCreator::run<ConsoleSinker>();
    // console_sinker->run(message.c_str(), message.size());

    // Sinker::ptr file_sinker = SinkerCreator::run<FileSinker>("./testDir/log/xxxx.log");
    // file_sinker->run(message.c_str(), message.size());
}

/**
 * @brief 测试日志器类
 */
void test_Logger()
{
    // using namespace mylog;
    // // // 1.获得日志器对象
    // // std::string name = "SyncLogger";
    // // Level::Value minLevel = Level::Value::WARNNING;
    // // Formatter::ptr formatter(new Formatter);
    // // Sinker::ptr console_sinker = SinkerCreator::run<ConsoleSinker>();
    // // Sinker::ptr file_sinker = SinkerCreator::run<FileSinker>("./testDir/log/test_Logger.log");
    // // std::vector<Sinker::ptr> sinkers = {console_sinker, file_sinker};
    // // Logger::ptr logger(new SyncLogger(name, minLevel, formatter, sinkers));

    // // // 2.进行不同等级的日志输出
    // // logger->debug(__FILE__, __LINE__, "%s", "套接字bind失败......");
    // // logger->info(__FILE__, __LINE__, "%s", "套接字bind失败......");
    // // logger->warnning(__FILE__, __LINE__, "%s", "套接字bind失败......");
    // // logger->error(__FILE__, __LINE__, "%s", "套接字bind失败......");
    // // logger->fatal(__FILE__, __LINE__, "%s", "套接字bind失败......");
    // // logger->fatal(__FILE__, __LINE__, "%s, errno: %d, strerror: %s", "套接字bind失败", errno, strerror(errno));

    // // 可以看到，日志器对象的创造非常难受，因此使用建造者模式来获得日志器对象。
    // // 1.获得日志器建造者类对象，并建造日志器各成员
    // std::unique_ptr<LoggerBuilder> bd(new LocalLoggerBuilder());
    // bd->setLoggerName("同步日志器");
    // // bd->setLoggerName("");
    // bd->setLoggerType(Logger::Type::ASYNC);
    // bd->setMinLevel(Level::Value::DEBUG);
    // bd->setFormatter();
    // bd->setSinker<ConsoleSinker>();
    // bd->setSinker<FileSinker>("./testDir/log/test_Logger_v2.log");

    // // 2.建造出Logger对象
    // Logger::ptr logger = bd->build();

    // std::cout << "logger对象建造成功\n";

    // // 3. 进行不同等级的日志输出
    // int cnt = 500000;
    // while (cnt--)
    // {
    //     std::cout << "生产者正在阻塞......\n";
    //     logger->debug(__FILE__, __LINE__, "%s", "套接字bind失败......");
    //     logger->info(__FILE__, __LINE__, "%s", "套接字bind失败......");
    //     logger->warnning(__FILE__, __LINE__, "%s", "套接字bind失败......");
    //     logger->error(__FILE__, __LINE__, "%s", "套接字bind失败......");
    //     logger->fatal(__FILE__, __LINE__, "%s", "套接字bind失败......");
    //     logger->fatal(__FILE__, __LINE__, "%s, errno: %d, strerror: %s", "套接字bind失败", errno, strerror(errno));
    // }
}

void test_loggerManager()
{
    using namespace mylog;
    Logger::ptr logger = LoggerManager::getInstance().get("同步日志器");
    // logger->debug(__FILE__, __LINE__, "%s", "测试日志器管理者~~~~");
    // logger->info(__FILE__, __LINE__, "%s", "测试日志器管理者~~~~");
    // logger->warnning(__FILE__, __LINE__, "%s", "测试日志器管理者~~~~");
    // logger->error(__FILE__, __LINE__, "%s", "测试日志器管理者~~~~");
    // logger->fatal(__FILE__, __LINE__, "%s", "测试日志器管理者~~~~");

    logger->debug("%s", "测试测试测试~~~~~");
    logger->info("%s", "测试测试测试~~~~~");
    logger->warn("%s", "测试测试测试~~~~~");
    logger->error("%s", "测试测试测试~~~~~");
    logger->fatal("%s", "测试测试测试~~~~~");

    DEBUG("%s","默认Console日志器");

}

int main()
{
    using namespace mylog;
    LoggerBuilder::ptr bd(new GlobalLoggerBuilder());
    bd->setLoggerName("同步日志器");
    // bd->setLoggerName("");
    bd->setLoggerType(Logger::Type::SYNC);
    bd->setMinLevel(Level::Value::DEBUG);
    bd->setFormatter();
    bd->setSinker<ConsoleSinker>();
    bd->setSinker<FileSinker>("./outputTest/test_Logger.log");

    // 2.建造出Logger对象
    Logger::ptr logger = bd->build();
    std::cout << "logger对象建造成功\n";
    test_loggerManager();
    return 0;
}
