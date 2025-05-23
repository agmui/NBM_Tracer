#include "SerializationHelper.h"

namespace SerializationHelper
{

    void appendInt(std::vector<uint8_t> &data, int num)
    {
        for (int i = 0; i < 4; ++i)
            data.push_back((num >> (i * 8)) & 0xFF);
    }

    int readInt(uint8_t *ptr) {
        return ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24);
    }

}
