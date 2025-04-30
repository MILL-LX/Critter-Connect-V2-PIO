#include <atomic>

class Application
{
public:
    void run();

private:
    std::atomic<bool> _is_running{false};
};