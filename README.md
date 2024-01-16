# MyLog4CPP

这是一个基于 C++11 的 header-only 的日志系统。

# 特性

- 灵活的日志输出格式控制
- 支持五种日志等级：DEBUG, INFO, WARN, ERROR, FATAL
- 控制台输出日志
- 指定文件输出日志
- 用户自定义日志输出方式
- 同步日志
- 异步日志

# 快速使用

将 `mylog/header/` 拷贝到你的项目中，使用时包含头文件 `mylog.h` 即可。

```cpp
#include "../mylog/header/mylog.h"

void test1()
{
    using namespace mylog;

    /// 1.构造日志器建造者，并指定日志器名称、日志器类型、日志器最低等级、日志器输出格式、日志输出方向
    LoggerBuilder::ptr bd(new GlobalLoggerBuilder());
    bd->setLoggerName("同步日志器");
    bd->setLoggerType(Logger::Type::SYNC);
    bd->setMinLevel(Level::Value::DEBUG);
    bd->setFormatter(); //没有指定时，则使用默认格式[%t][%c][%n][%i][%f%S:%S%l]%T%m%N
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
```

控制台输出为：

```bash
alonely@DESKTOP-3M5BE4L:~/MyLog4CPP/examples$ ./test 
[2024-1-17 7:35:30][DEBUG][同步日志器][140110362470208][test.cc : 21]   欢迎使用mylog!
[2024-1-17 7:35:30][INFO][同步日志器][140110362470208][test.cc : 22]    这是一条日志系统测试信息
[2024-1-17 7:35:30][WARN][同步日志器][140110362470208][test.cc : 23]    输出时, 你可以像printf一样指定日志消息格式: 2333, 666.600000, 0x564827f70dc0
[2024-1-17 7:35:30][ERROR][同步日志器][140110362470208][test.cc : 24]   日志消息格式与日志输出格式是不同的概念, 前者是你的日志有效消息的格式, 后者是实际输出的日志文本的格式
[2024-1-17 7:35:30][FATAL][同步日志器][140110362470208][test.cc : 25]   实际输出的日志格式需要通过日志器建造者指定
[2024-1-17 7:35:30][DEBUG][同步日志器][140110362470208][test.cc : 26]   祝使用愉快~
[2024-1-17 7:35:30][DEBUG][defaultConsoleSyncLogger][140110362470208][test.cc : 32]     你也可以直接使用自带的默认日志器
[2024-1-17 7:35:30][INFO][defaultConsoleSyncLogger][140110362470208][test.cc : 33]      它默认是进行控制台输出
[2024-1-17 7:35:30][WARN][defaultConsoleSyncLogger][140110362470208][test.cc : 34]      而且是同步日志器
[2024-1-17 7:35:30][ERROR][defaultConsoleSyncLogger][140110362470208][test.cc : 35]     通过使用大写的宏函数, 例如DEBUG
[2024-1-17 7:35:30][FATAL][defaultConsoleSyncLogger][140110362470208][test.cc : 36]     并指定格式即可完成输出
alonely@DESKTOP-3M5BE4L:~/MyLog4CPP/examples$ 
```

同时，生成了指定的文件：

```bash
alonely@DESKTOP-3M5BE4L:~/MyLog4CPP/examples$ cat ./outputTest/test_Logger.log 
[2024-1-17 7:35:30][DEBUG][同步日志器][140110362470208][test.cc : 21]   欢迎使用mylog!
[2024-1-17 7:35:30][INFO][同步日志器][140110362470208][test.cc : 22]    这是一条日志系统测试信息
[2024-1-17 7:35:30][WARN][同步日志器][140110362470208][test.cc : 23]    输出时, 你可以像printf一样指定日志消息格式: 2333, 666.600000, 0x564827f70dc0
[2024-1-17 7:35:30][ERROR][同步日志器][140110362470208][test.cc : 24]   日志消息格式与日志输出格式是不同的概念, 前者是你的日志有效消息的格式, 后者是实际输出的日志文本的格式
[2024-1-17 7:35:30][FATAL][同步日志器][140110362470208][test.cc : 25]   实际输出的日志格式需要通过日志器建造者指定
[2024-1-17 7:35:30][DEBUG][同步日志器][140110362470208][test.cc : 26]   祝使用愉快~
```

# 同步日志器 & 异步日志器

同步日志器：业务线程使用日志器输出时直接输出。业务和日志输出串行执行。

异步日志器：日志器提供额外的一个线程去执行日志的输出，与业务线程并行执行，业务线程只需将日志消息放入异步日志器提供的缓冲区中即可，无需关心日志的实际输出。可以减少业务线程为日志输出消耗的时间。

# 日志输出格式控制

你可以使用以下占位符进行日志输出格式的控制：

- %t : 时间

- %c : 日志等级

- %n : 日志器名称

- %i : 线程id

- %f : 文件名

- %l : 行号

- %m : 日志有效消息

- %S : 空格

- %T : 制表符

- %N : 换行

- %% : %