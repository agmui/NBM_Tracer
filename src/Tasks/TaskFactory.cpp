//
// Created by agmui on 5/15/25.
//

#include "TaskFactory.h"
namespace TaskFactory{
    vector<uint8_t> serialize(Task& task) {
        //TODO: optimize
        vector<uint8_t> v = task.serialize();
        v.insert(v.begin(), task.type, v.size());
        return v;
    }

    unique_ptr<Task> deserialize(vector<uint8_t> serializedTask) {
        switch (((Task *) &serializedTask[0])->type) {
            case MineBitCoin_:
                return make_unique<MineBitCoin>(serializedTask);
            default:
                //TODO: throw an error
                return nullptr;
        }
    }
}

