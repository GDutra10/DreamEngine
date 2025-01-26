#ifndef CORE_SERIALIZERS_I_JSON_SERIALIZABLE_H_
#define CORE_SERIALIZERS_I_JSON_SERIALIZABLE_H_

#include <string>

namespace DreamEngine::Core
{
template <typename T>
class IJsonSerializable
{
public:
    virtual ~IJsonSerializable() = default;
    virtual std::string SerializeToJson() = 0;
};
}
#endif