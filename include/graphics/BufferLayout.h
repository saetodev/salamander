#pragma once

#include "Base.h"

namespace sal {

    enum DataType {
        Float,
        Float2,
        Float3,
        Float4,
    };

    struct Attribute {
        std::string name;
        DataType    type;
    };

    inline size_t GetDataTypeSize(DataType type) {
        switch (type) {
            case DataType::Float: return sizeof(float);
            case DataType::Float2: return sizeof(float) * 2;
            case DataType::Float3: return sizeof(float) * 3;
            case DataType::Float4: return sizeof(float) * 4;
        }

        assert(false);
        return 0;
    }

    inline size_t GetDataTypeComponentCount(DataType type) {
        switch (type) {
            case DataType::Float: return 1;
            case DataType::Float2: return 2;
            case DataType::Float3: return 3;
            case DataType::Float4: return 4;
        }

        assert(false);
        return 0;
    }

    class BufferLayout {
    public:
        void Push(const std::string& name, DataType type) {
            m_size += GetDataTypeSize(type);

            m_attributes.emplace_back(name, type);
        }

        size_t Size() const { return m_size; }

        std::vector<Attribute>::const_iterator begin() const { return m_attributes.cbegin(); }
        std::vector<Attribute>::const_iterator end() const { return m_attributes.cend(); }
    private:
        size_t m_size;
        std::vector<Attribute> m_attributes;
    };

}