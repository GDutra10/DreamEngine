#include <gtest/gtest.h>

//#include "../../../src/Core/Text/Json/JsonProperty.h"
//#include "../../../src/Core/Text/Json/JsonObject.h"
//#include "../../../src/Core/Text/Json/JsonValue.h"

namespace DreamEngine::CoreTests::Text
{
	//using namespace Core::Text::Json;

	//TEST(JsonArrayEmptyToString, ShouldReturnEmptyJson)
	//{
	//	// arrange
	//	auto jsonArray = JsonArray<std::string>();

	//	// act & asserts
	//	EXPECT_EQ(jsonArray.ToString(), "[]");
	//}

	//TEST(JsonArrayWithDataToString, ShouldReturnValidJson)
	//{
	//	// arrange
	//	auto jsonArray = JsonArray<JsonObject>();
	//	auto jsonObject = JsonObject();
	//	jsonObject.AddField("field1", std::string("something"));
	//	jsonObject.AddField("field2", 123);
	//	jsonArray.Add(jsonObject);

	//	auto jsonObject2 = JsonObject();
	//	jsonObject2.AddField("field1", std::string("banana"));
	//	jsonObject2.AddField("field2", 40.2f);
	//	jsonArray.Add(jsonObject2);

	//	// act & asserts
	//	EXPECT_EQ(jsonArray.ToString(), "[{\"field1\":\"something\",\"field2\":123},{\"field1\":\"banana\",\"field2\":40.2}]");
	//}

	//TEST(JsonPropertyToString, ShouldReturnValidJson)
	//{
	//	// arrange
	//	auto jsonFieldNumber = JsonProperty("field", 123);
	//	auto jsonFieldString = JsonProperty("field", std::string("123"));

	//	// act & asserts
	//	EXPECT_EQ(jsonFieldNumber.ToString(), "\"field\":123");
	//	EXPECT_EQ(jsonFieldString.ToString(), "\"field\":\"123\"");
	//}

	//TEST(JsonObjectToString, ShouldReturnValidJson)
	//{
	//	// arrange
	//	auto jsonObject = JsonObject();
	//	jsonObject.AddProperty("field1", std::string("something"));
	//	jsonObject.AddProperty("field2", 123);

	//	// act & asserts
	//	EXPECT_EQ(jsonObject.ToString(), "{\"field1\":\"something\",\"field2\":123}");
	//}

	//TEST(JsonObjectWithComplexData, ShouldReturnValidJson)
	//{
	//	// arrange
	//	auto jsonObject = JsonObject();
	//	jsonObject.AddField("field1", std::string("something"));
	//	jsonObject.AddField("field2", 123);
	//	jsonObject.AddField("list", std::vector<std::string>{});

	//	// act && asserts
	//	EXPECT_EQ(jsonObject.ToString(), "{\"field1\":\"something\",\"field2\":123,\"list\":[]}");
	//}
}
