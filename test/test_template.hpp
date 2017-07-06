#include <my_test_file.hpp>

class MyClassTest : public ::testing::Test
{
public:
  // Method that will execute before any tests are run
  virtual void SetUp()
  {
    someThingWeNeed = std::make_shared<MyOtherClass>();
  }

  // Set useful test data as member variables on the class
  std::shared_ptr<MyOtherClass> someThingWeNeedInMultipleTests;
};

// Below are test fixtures that will be executed once after SetUp() has
// been executed

TEST_F(MyClassTest, someMethod)
{
  // Notice we can use methods defined on test class
  MyClass subject(someThingWeNeedInMultipleTests);
  EXPECT_EQ(subject.someMethod(), 1);
}
