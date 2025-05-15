#include "Task.h"

class MineBitCoin : public Task {
public:
    MineBitCoin(int id, int num1, int num2) : id(id), num1(num1), num2(num2) {}
    ~MineBitCoin() {}

    size_t size() override;
    void doTask(Result &result) override;

private:
    int id;
    int num1;
    int num2;
};