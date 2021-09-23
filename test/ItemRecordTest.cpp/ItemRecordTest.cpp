#include "../../ItemRecord.h"
#include "gtest/gtest.h"

TEST(ItemRecordTest, SinglePurchaseTest)
{
  // arrange
  ItemRecord record;

  // act
  const size_t quantity = 5;
  const double price = 2.0;
  record.buy(quantity, price);

  // assert
  ASSERT_EQ(quantity, record.numBought());
  ASSERT_EQ(price, record.avgPurchasePrice());
}

TEST(ItemRecordTest, MultiPurchaseTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 2.0);
  record.buy(3, 3.0);

  // assert
  ASSERT_EQ(7, record.numBought());
  // (5 * 2.0 + 3 * 3.0) / 8 = 2.375
  ASSERT_EQ(2.375, record.avgPurchasePrice());
}

TEST(ItemRecordTest, SingleSellTest)
{
  // arrange
  ItemRecord record;

  // act
  const size_t quantity = 5;
  const double price = 2.0;
  record.sell(quantity, price);

  // assert
  ASSERT_EQ(quantity, record.numSold());
  ASSERT_EQ(price, record.avgSalesPrice());
}

TEST(ItemRecordTest, MultiSellTest)
{
  // arrange
  ItemRecord record;

  // act
  record.sell(5, 2.0);
  record.sell(3, 3.0);

  // assert
  ASSERT_EQ(7, record.numBought());
  // (5 * 2.0 + 3 * 3.0) / 8 = 2.375
  ASSERT_EQ(2.375, record.avgSalesPrice());
}

TEST(ItemRecordTest, SingleBuySellPositiveProfitTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 2.0);
  record.sell(4, 3.0);

  // assert
  ASSERT_EQ(2.0, record.profit());
  ASSERT_EQ(1, record.numAvailable());
  // 1 remaining, bought at $2.00
  ASSERT_EQ(2.0, record.unsoldCost());
}

// NOTE: this class doesn't enforce business logic of preventing negative
// quantity
TEST(ItemRecordTest, SingleBuySellNegativeProfitTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(4, 3.0);
  record.sell(5, 2.0);

  // assert
  ASSERT_EQ(-2.0, record.profit());
  ASSERT_EQ(-1, record.numAvailable());
}

TEST(ItemRecordTest, MultipleBuySellTest)
{
  // arrange
  ItemRecord record;

  // act
  record.buy(5, 2.0);
  record.buy(3, 3.0);

  record.sell(4, 4.25);
  record.sell(2, 3.50);

  // assert
  // (5 * 2.0 + 3 * 3.0) / 8 = 2.375
  ASSERT_EQ(2.375, record.avgPurchasePrice());
  // (4 * 4.25 + 2 * 3.5) / 6 = 4.0
  ASSERT_EQ(4.0, record.avgSalesPrice());
}

/* Key Assumptions:
* - This class doesn't prohibit there being a negative quantity in stock
* -	It instead leaves that logic up to the consuming class.
* - Profits are based on average purchase and sales prices at the time it is
called
* - The averages are calculated on the fly to avoid memory storage.  This may
come with minor error
*/

/* tests:
* - single purchase test
*	+ numBought() correct
*	+ avgPurchasePrice() correct
* - multiple purchase test
*	+ numBought() correct
*	+ avgPurchasePrice() correct
* - single sell test
*	+ numSold() correct
*	+ avgSalesPrice() correct
* - multiple sell test
*	+ numSold() correct
*	+ avgSalesPrice() correct

* - single buy / sell test- positive profit
*	+ profit() correct
*	+ unsoldCost() correct
* - single buy / sell test- negative profit
*	+ profit() correct
*	+ unsoldCost() correct
* - multiple buy / sell test
*	+ profit() correct
*	+ unsoldCost() correct
*/
