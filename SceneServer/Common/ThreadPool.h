#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
#include <deque>

//线程池单例
class ThreadPool {
public:
    static ThreadPool& instance() {
        static ThreadPool pool(std::thread::hardware_concurrency());
        return pool;
    }

    // 提交任务到线程池  生产者-消费者
    template<class F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {
        auto task = std::make_shared<std::packaged_task<decltype(f())()>>(std::forward<F>(f));  //任务函数包装  传进来对象完美转发
        std::future<decltype(f())> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace_back([task]() { (*task)(); });
        }
        cv.notify_one();
        return res;
    }

    // 等待所有任务执行完
    void wait_all() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [this]() { return tasks.empty(); });
    }

private:
    ThreadPool(size_t threads) {        //根据硬件线程数量
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;         //每个线程执行的包装任务
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop_front();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all();
        for (std::thread& worker : workers) worker.join();
    }

    std::vector<std::thread> workers;
    std::deque<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool stop = false;
};