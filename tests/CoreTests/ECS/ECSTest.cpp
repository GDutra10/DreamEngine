#include <gtest/gtest.h>
#include "../../../src/Core/ECS/EntityManager.h"
using namespace DreamEngine::Core::ECS;

namespace DreamEngine::CoreTests::ECS
{
	TEST(EntityManagerTest, ShouldAddEntity)
	{
		// arrange
		auto manager = EntityManager();

		// act
		auto playerEntity = manager.AddEntity("Player");

		// asserts
		EXPECT_EQ(playerEntity->GetId(), 0);
		EXPECT_EQ(playerEntity->GetName(), "New Entity");
		EXPECT_EQ(playerEntity->GetTag(), "Player");
		EXPECT_EQ(playerEntity->IsActive(), true);
	}

	TEST(EntityManagerTest, ShouldRemoveEntity)
	{
		// arrange
		auto manager = EntityManager();
		auto playerEntity = manager.AddEntity("Player");

		// act
		playerEntity->Destroy();

		// asserts
		EXPECT_EQ(playerEntity->IsActive(), false);
	}
}