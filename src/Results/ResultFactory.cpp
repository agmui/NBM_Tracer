#include "ResultFactory.h"

namespace ResultFactory{
    vector<uint8_t> serialize(Result& result) {
        //TODO: optimize
        vector<uint8_t> v = result.serialize();
        // v.insert(v.begin(), task.type, v.size());
        return v;
    }

    unique_ptr<Result> deserialize(vector<uint8_t> &serializedResult) {
        printf("Deserializing %d\n", serializedResult.size());

        switch (SerializationHelper::readResultType(serializedResult.data())) {
            case BitCoinResult_:
                return make_unique<BitCoinResult>(serializedResult);
            default:
                //TODO: throw an error
                return nullptr;
        }
    }
}