#include <cassert>
#include "ThreadPool.h"

ThreadPool::ThreadPool(uint32_t thread_limit)
  : m_under_destruction{false}
{
    for (size_t i = 0; i < thread_limit; ++i) {
        m_free_workers.emplace(new Worker(*this));
    }
}

ThreadPool::~ThreadPool()
{
    auto lock = std::lock_guard{m_mtx};
    m_under_destruction = true;
}

void ThreadPool::taskFinished(Worker& worker)
{
    auto lock = std::lock_guard{m_mtx};

    assert(&worker.getOwner() == this);
    if (m_under_destruction) {
        return;
    }

    auto worker_it = std::find_if(m_busy_workers.cbegin(), m_busy_workers.cend(), [&worker](const WorkerPtr& a) { return a.get() == &worker; });
    if (worker_it != m_busy_workers.end()) {
        auto worker_ptr = *worker_it;
        m_busy_workers.erase(worker_it);
        m_free_workers.insert(*worker_it);
    }
    m_cv.notify_one();
}

ThreadPool::Worker::Worker(ThreadPool& tp)
  : m_stop{false}
  , m_owner(tp)
{
    m_thread = std::thread(&ThreadPool::Worker::work, this);
}

void ThreadPool::Worker::work()
{
    for (auto lock = std::unique_lock{m_worker_mtx}; m_task || !m_stop; m_cv.wait(lock, [&] { return m_task || m_stop; })) {
        if (m_task) {
            (*m_task)();
            m_task.release();
            m_owner.taskFinished(*this);
        }
    }
}

void ThreadPool::Worker::setTask(std::unique_ptr<ITask> task)
{
    auto lock = std::lock_guard{m_worker_mtx};
    if (m_stop) {
        return;
    }
    m_task.swap(task);
    m_cv.notify_one();
}

const ThreadPool& ThreadPool::Worker::getOwner() const
{
    return m_owner;
}

ThreadPool::Worker::~Worker()
{
    {
        auto lock = std::lock_guard{m_worker_mtx};

        m_stop = true;
        m_cv.notify_one();
    }
    m_thread.join();
};