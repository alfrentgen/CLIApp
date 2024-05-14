#ifndef CLIAPP_THREADPOOL_H
#define CLIAPP_THREADPOOL_H

#include <functional>
#include <future>
#include <set>
#include <mutex>
#include <optional>
#include <thread>

class ITask
{
public:
    virtual ~ITask() = default;

    virtual void operator()() = 0;
};

template <typename FuncType, typename... ArgTypes>
class TemlateTask : public ITask
{
public:
    TemlateTask(TemlateTask&&) = delete;
    TemlateTask(const TemlateTask&) = delete;
    TemlateTask(FuncType func, ArgTypes... args)
      //: m_func{std::bind(func, args...)}
            : m_func{[func, args...](){func(args...);}}
    {
    }

    std::function<void()> m_func;
    void operator()() override
    {
        m_func();
    };
};

class ThreadPool
{
public:
    explicit ThreadPool(uint32_t thread_limit);
    ThreadPool() = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool(const ThreadPool&) = delete;
    ~ThreadPool();

    template <typename FuncType, typename... ArgTypes>
    void run(FuncType f, ArgTypes... args)
    {
        submitTask(new TemlateTask(f, std::forward<ArgTypes...>(args...)));
    }

private:
    class Worker
    {
    public:
        Worker(ThreadPool& tp);
        Worker(const Worker&) = delete;
        Worker(Worker&&) = delete;

        void setTask(std::unique_ptr<ITask> task);
        const ThreadPool& getOwner() const;

        ~Worker();

    private:
        std::unique_ptr<ITask> m_task;
        std::thread m_thread;
        std::atomic<bool> m_stop;
        std::condition_variable m_cv;
        std::mutex m_worker_mtx;
        ThreadPool& m_owner;
    };

    friend class Worker;

    using WorkerPtr = std::shared_ptr<ThreadPool::Worker>;
    void submitTask(ITask* task);
    void taskFinished(Worker& worker);

    std::atomic<bool> m_under_destruction;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::set<WorkerPtr> m_free_workers;
    std::set<WorkerPtr> m_busy_workers;
};

#endif // CLIAPP_THREADPOOL_H
