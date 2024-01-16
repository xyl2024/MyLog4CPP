#include "../mylog/header/mylog.h"


void test1()
{
    using namespace mylog;

    /// 1.构造日志器建造者，并指定日志器名称、日志器类型、日志器最低等级、日志器输出格式、日志输出方向
    LoggerBuilder::ptr bd(new GlobalLoggerBuilder());
    bd->setLoggerName("同步日志器");
    bd->setLoggerType(Logger::Type::SYNC);
    bd->setMinLevel(Level::Value::DEBUG);
    bd->setFormatter();
    bd->setSinker<ConsoleSinker>();
    bd->setSinker<FileSinker>("./outputTest/test_Logger.log");

    /// 2.建造出Logger对象
    Logger::ptr logger = bd->build();
    
    /// 3.使用日志器
    logger->debug("%s", "欢迎使用mylog!");
    logger->info("这是一条日志系统测试信息");
    logger->warn("输出时, 你可以像printf一样指定日志消息格式: %d, %f, %p", 2333, 666.6, logger.get());
    logger->error("日志消息格式与日志输出格式是不同的概念, 前者是你的日志有效消息的格式, 后者是实际输出的日志文本的格式");
    logger->fatal("实际输出的日志格式需要通过日志器建造者指定");
    logger->debug("祝使用愉快~");
}


void test2()
{
    mylog::DEBUG("%s", "你也可以直接使用自带的默认日志器");
    mylog::INFO("%s", "它默认是进行控制台输出");
    mylog::WARN("%s", "而且是同步日志器");
    mylog::ERROR("%s", "通过使用大写的宏函数, 例如DEBUG");
    mylog::FATAL("%s", "并指定格式即可完成输出");
}

int main()
{
    test1();
    test2();
    return 0;
}
