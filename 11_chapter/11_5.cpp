#include <mutex>
#include <vector>
#include <pthread.h>
#include <condition_variable>
#include <iostream>

class Barrier
{
public:
    Barrier(int limit) : mlimit(limit)
    {
    }


    Barrier(const Barrier&) = delete;
    Barrier& operator=(const Barrier&) = delete;

    int wait()
    {
        std::unique_lock l(m);
        if (++mcount == mlimit)
        {
            l.unlock();
            cv.notify_all();
            return 0;
        }

        cv.wait(l);
        ++mcount;
        return 0;
    }

private:
    int mlimit;
    int mcount;
    std::mutex m;
    std::condition_variable cv;
};

static Barrier barrier(4);
static constexpr int barrier_count = 4;

void f()
{
    std::cout << "thread blocks\n";
    barrier.wait();
    std::cout << "thread starts\n";
}

int main(int argc, char const *argv[])
{
    std::vector<std::thread> thread_group;
    for (size_t i = 0; i < barrier_count - 1; i++)
    {
        thread_group.emplace_back(std::thread(f));
    }

    barrier.wait();
    for (auto &&i : thread_group)
    {
        i.join();
    }

    return 0;
}
