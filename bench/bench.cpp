#include "../mylog/header/mylog.h"

void bench(const std::string &loger_name, size_t thread_num, size_t msglen, size_t msg_count)
{
    using namespace mylog;
    Logger::ptr lp = getLogger(loger_name);
    if (lp.get() == nullptr)
        return;
    std::string msg(msglen, '1');
    size_t msg_count_per_thread = msg_count / thread_num;
    std::vector<double> cost_time(thread_num);
    std::vector<std::thread> threads;
    std::cout << "输入线程数量: " << thread_num << std::endl;
    std::cout << "输出日志数量: " << msg_count << std::endl;
    std::cout << "输出日志大小: " << msglen * msg_count / 1024 << "KB" << std::endl;
    for (int i = 0; i < thread_num; i++)
    {
        threads.emplace_back([&, i]()
                             {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t j = 0; j < msg_count_per_thread; j++) {
                lp->fatal("%s", msg.c_str());
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto cost = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            cost_time[i] = cost.count();
            auto avg = msg_count_per_thread / cost_time[i];
            std::cout << "线程" << i << "耗时: " << cost.count() << "s" << " 平均：" << (size_t)avg << "/s\n"; });
    }
    for (auto &thr : threads)
    {
        thr.join();
    }
    double max_cost = 0;
    for (auto cost : cost_time)
        max_cost = max_cost < cost ? cost : max_cost;
    std::cout << "总消耗时间: " << max_cost << std::endl;
    std::cout << "平均每秒输出: " << (size_t)(msg_count / max_cost) << "条日志" << std::endl;
    std::cout << "平均每秒输出: " << (size_t)(msglen * msg_count / max_cost / 1024 / 1024) << "MB" << std::endl;
}

void sync_bench_thread_log(size_t thread_count, size_t msg_count, size_t msglen)
{
    static int num = 1;
    std::string logger_name = "sync_bench_logger" + std::to_string(num++);
    mylog::FATAL("************************************************");
    mylog::FATAL("同步日志测试: %d threads, %d messages", thread_count, msg_count);
    mylog::GlobalLoggerBuilder::ptr lbp(new mylog::GlobalLoggerBuilder);
    lbp->setLoggerName(logger_name);
    lbp->setFormatter("%m%n");
    lbp->setSinker<mylog::FileSinker>("./logs/sync.log");
    lbp->setLoggerType(mylog::Logger::Type::SYNC);
    lbp->build();
    bench(logger_name, thread_count, msglen, msg_count);
    mylog::FATAL("************************************************");
}

void async_bench_thread_log(size_t thread_count, size_t msg_count, size_t msglen)
{
    static int num = 1;
    std::string logger_name = "async_bench_logger" + std::to_string(num++);
    mylog::FATAL("************************************************");
    mylog::FATAL("异步日志测试: %d threads, %d messages", thread_count, msg_count);
    mylog::GlobalLoggerBuilder::ptr lbp(new mylog::GlobalLoggerBuilder);
    lbp->setLoggerName(logger_name);
    lbp->setFormatter("%m");
    lbp->setSinker<mylog::FileSinker>("./logs/async.log");
    lbp->setLoggerType(mylog::Logger::Type::ASYNC);
    lbp->build();
    bench(logger_name, thread_count, msglen, msg_count);
    mylog::FATAL("************************************************");
}

void bench_test()
{
    // 同步写日志
    sync_bench_thread_log(1, 1000000, 100);
    sync_bench_thread_log(5, 1000000, 100);
    /*异步日志输出，为了避免因为等待落地影响时间所以日志数量降低为小于缓冲区大小进行测试*/
    async_bench_thread_log(1, 100000, 100);
    async_bench_thread_log(5, 100000, 100);
}
int main(int argc, char *argv[])
{
    bench_test();
    return 0;
}