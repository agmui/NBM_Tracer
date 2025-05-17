#include "TaskFactory.h"

namespace TaskFactory{
    vector<uint8_t> serialize(Task& task) {
        //TODO: optimize
        vector<uint8_t> v = task.serialize();
        // v.insert(v.begin(), task.type, v.size());
        return v;
    }

    unique_ptr<Task> deserialize(vector<uint8_t> serializedTask) {
        //TODO: refactor for array solution
        switch (SerializationHelper::readTaskType(serializedTask.data())) {
            case MineBitCoin_:
                return make_unique<MineBitCoin>(serializedTask);
            case RenderPixel_:
                return make_unique<RenderPixel>(serializedTask);
            default:
                //TODO: throw an error
                printf("[ERROR] could not find task\n");
                return nullptr;
        }
    }
}