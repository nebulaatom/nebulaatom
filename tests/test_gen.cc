#include <iostream>
#include "gtest/gtest.h"
#include "library2.h"

class TestGen : public ::testing::Test
{
	protected:
		void SetUp() override;
		void TearDown() override;

		Library2 *TestObj2_;
};

//-----------------------------------------------------------------------------

void TestGen::SetUp()
{
	TestObj2_ = new Library2();
}

void TestGen::TearDown()
{
	delete TestObj2_;
}

//-----------------------------------------------------------------------------

TEST_F(TestGen, TestLib2)
{
	ASSERT_EQ("Hello!", TestObj2_->get_var1());
}


//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}