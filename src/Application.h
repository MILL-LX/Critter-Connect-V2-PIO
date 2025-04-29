class Application
{
public:
    Application() : _is_running(false) {}
    void start();

private:
    bool _is_running;
};