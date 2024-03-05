#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CircularQueue.h"

// Mock class for arithmetic types
template<typename T>
class MockArithmeticType {
public:
    MOCK_CONST_METHOD0_T(front, T&());
    MOCK_METHOD1_T(push, void(const T&));
    MOCK_CONST_METHOD0_T(size, size_t());
    MOCK_CONST_METHOD0_T(full, bool());
    MOCK_CONST_METHOD0_T(empty, bool());
    MOCK_CONST_METHOD0_T(average, double());
};

// Test fixture for CircularQueue
template<typename T>
class CircularQueueTest : public testing::Test {
protected:
    CircularQueue<T> queue{5};
};

// Test case for pushing and popping elements
TYPED_TEST_SUITE_P(CircularQueueTest);

TYPED_TEST_P(CircularQueueTest, PushPopTest) {
    TypeParam val1 = 1;
    TypeParam val2 = 2;

    this->queue.push(val1);
    this->queue.push(val2);

    ASSERT_EQ(this->queue.front(), val1);
    ASSERT_EQ(this->queue.size(), 2U);

    this->queue.pop();
    ASSERT_EQ(this->queue.front(), val2);
    ASSERT_EQ(this->queue.size(), 1U);
}

// Test case for checking if the queue is full
TYPED_TEST_P(CircularQueueTest, FullTest) {
    for (size_t i = 0; i < 5; ++i) {
        this->queue.push(i);
    }

    ASSERT_TRUE(this->queue.full());
}

// Test case for checking if the queue is empty
TYPED_TEST_P(CircularQueueTest, EmptyTest) {
    ASSERT_TRUE(this->queue.empty());
    this->queue.push(1);
    ASSERT_FALSE(this->queue.empty());
}

// Register the test cases with different data types
REGISTER_TYPED_TEST_SUITE_P(CircularQueueTest, PushPopTest, FullTest, EmptyTest);

// Define the data types to be tested
typedef testing::Types<int, float, std::string> MyTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(My, CircularQueueTest, MyTypes);

// Main function for running the tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
