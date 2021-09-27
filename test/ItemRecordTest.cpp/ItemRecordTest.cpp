#include "../../ItemRecord.h"
#include "gtest/gtest.h"

#include <exception>
#include <string>

using namespace std;

/**
 * @brief Attempt to record a single purchase of an item. Expect both the
 * quantity returned and the number available to match the purchased quantity.
 * REQ: 1 & 3.
 */
TEST(ItemRecordTest, SinglePurchaseTest)
{
  // arrange
  ItemRecord record;

  // act
  const size_t quantity = 5;
  record.buy(quantity, 2.0);

  // assert
  EXPECT_EQ(quantity, record.numBought());
  EXPECT_EQ(quantity, record.numAvailable());
}

/**
 * @brief Attempt to record multiple purchases of an item. Expect both the
 * quantity returned and the number available to match the sum of the purchases.
 * REQ: 1 & 3.
 */
TEST(ItemRecordTest, MultiPurchaseTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 2.0);
  record.buy(3, 3.0);

  // assert
  EXPECT_EQ(8, record.numBought());
}

/**
 * @brief Attempt to record a single sell of an item. Expect the quantity
 * returned to match, and the number available to be the negative of the
 * quantity sold.
 * REQ: 2 & 3.
 */
TEST(ItemRecordTest, SingleSellTest)
{
  // arrange
  ItemRecord record;

  // act
  const size_t quantity = 5;
  const double price = 2.0;
  record.sell(quantity, price);

  // assert
  EXPECT_EQ(quantity, record.numSold());
}

/**
 * @brief Attempt to record multiple sales of an item. Expect the quantity
 * returned to match the sum of the sales, and the number available to be the
 * negative of that sum.
 * REQ: 2 & 3.
 */
TEST(ItemRecordTest, MultiSellTest)
{
  // arrange
  ItemRecord record;

  // act
  record.sell(5, 2.0);
  record.sell(3, 3.0);

  // assert
  EXPECT_EQ(8, record.numSold());
}

/**
 * @brief Attempt to sell more items than are availble. Expect an exception to
 * be thrown with a descriptive message.
 * REQ: 2
 */
TEST(ItemRecordTest, SingleSellMoreThanAvailableTest)
{
  // arrange
  ItemRecord record;

  // act & assert
  try {
    record.buy(4, 3.0);
    record.sell(5, 2.0);
  } catch (exception& e) {
    EXPECT_TRUE(string(e.what()).find("more items than available") !=
                string::npos);
  }
}

/**
 * @brief Attempt to record a single pair of purchase and sell of an item in
 * which the sales price is higher than the purchase price,
 * obtaining the resulting profit and unsold cost. Expect the returned values to
 * match the pre-calculated value.
 * REQ: 4 & 5.
 */
TEST(ItemRecordTest, SingleBuySellPositiveProfitTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 2.0);
  record.sell(4, 3.0);

  // assert
  EXPECT_EQ(4.0, record.profit());
  EXPECT_EQ(1, record.numAvailable());
  // 1 remaining, bought at $2.00
  EXPECT_EQ(2.0, record.unsoldCost());
}

/**
 * @brief Attempt to record a single pair of purchase and sell of an item in
 * which the sales price is lower than the purchase price,
 * obtaining the resulting profit and unsold cost. Expect the returned value to
 * match the pre-calculated values.
 * REQ: 4 & 5.
 */
TEST(ItemRecordTest, SingleBuySellNegativeProfitTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 3.0);
  record.sell(4, 2.0);

  // assert
  EXPECT_EQ(-4.0, record.profit());
  EXPECT_EQ(1, record.numAvailable());
  // 1 remaining, bought at $2.00
  EXPECT_EQ(3.0, record.unsoldCost());
}

/**
 * @brief Attempt to record multiple purchases and sales of an item and obtain
 * the profit and cost of unsold items. Expect the returned values to match
 * their respective pre-calculated values.
 * REQ: 3, 4, 5.
 */
TEST(ItemRecordTest, SampleProblemTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(8, 1.0);
  record.buy(2, 3.5);

  record.sell(2, 2.5);
  record.sell(4, 3.0);

  // assert
  EXPECT_DOUBLE_EQ(4, record.numAvailable());
  EXPECT_DOUBLE_EQ(8.0, record.profit());
  EXPECT_DOUBLE_EQ(6.0, record.unsoldCost());
}
