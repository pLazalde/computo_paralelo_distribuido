#include <memory>
#include <string>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <print>

struct Fork {
    bool isTaken = false;
    std::string_view philosopher;
    std::mutex forkMutex;
    std::condition_variable taken;

    void takeFork(std::string_view philosopherName, std::string_view hand) {
        {
            std::unique_lock lock(forkMutex);
            taken.wait(lock, [&] {
                std::println("{} is waiting for {} hand fork", philosopherName, hand);
                return !isTaken;
            });
            std::println("{} is taking {} hand fork", philosopherName, hand);

            isTaken = true;
            philosopher = philosopherName;
        }
    }

    void releaseFork(std::string_view hand) {
        std::println("{} is releasing {} hand fork", philosopher, hand);
        isTaken = false;
        philosopher = "";
        taken.notify_all();
    }
};

class Philosopher {
private:
    std::string name;
    std::shared_ptr<Fork> rightFork;
    std::shared_ptr<Fork> leftFork;

public:
    std::shared_ptr<Fork> RightFork() const {
        return rightFork;
    }
    std::shared_ptr<Fork>& RightFork() {
        return rightFork;
    }
    std::shared_ptr<Fork>& LeftFork() {
        return leftFork;
    }

    void eat() {
        while (true) {
            leftFork->takeFork(name, "left");
            if (rightFork->isTaken) {
                leftFork->releaseFork("left");
                continue;
            }
            rightFork->takeFork(name, "right");
            std::println("{} started eating", name);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::println("{} done eating", name);
            rightFork->releaseFork("right");
            leftFork->releaseFork("left");
        }
    }

    Philosopher(std::string name) : name(name) {}
};

int main() {
    auto fork1 = std::make_shared<Fork>();
    auto fork2 = std::make_shared<Fork>();
    auto fork3 = std::make_shared<Fork>();

    Philosopher socrates("Socrates");
    Philosopher diogenes("Diogenes");
    Philosopher plato("Plato");

    socrates.LeftFork() = fork1;
    socrates.RightFork() = fork2;

    diogenes.LeftFork() = fork2;
    diogenes.RightFork() = fork3;

    plato.LeftFork() = fork3;
    plato.RightFork() = fork1;

    std::thread t1([&]() { socrates.eat(); });
    std::thread t2([&]() { diogenes.eat(); });
    std::thread t3([&]() { plato.eat(); });

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
