#include <gtest/gtest.h>

#include "../../../src/Core/Serializers/IJsonSerializable.h"

namespace DreamEngine::CoreTests::Serializers
{
	class EntityTest : Core::IJsonSerializable<EntityTest>
	{
	public:
		std::string SerializeToJson() override
		{
			return "{}";
		}
	};

	TEST(SerializeToJson, ShouldReturnJson)
	{
		// arrange
		auto entityTest = EntityTest();

		// act & asserts
		EXPECT_EQ(entityTest.SerializeToJson(), "{}");
	}
}
