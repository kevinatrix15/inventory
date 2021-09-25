#include "../../Inventory.h"
#include "gtest/gtest.h"

#include <exception>
#include <string>

using namespace std;

/******************************************************************************
 * HELPER FUNCTIONS ************************************************************
 ******************************************************************************/

namespace
{
bool substringFound(const string& str, const string& substr)
{
  return (str.find(substr) != string::npos);
}

// NOTE: We don't worry too much about testing the correct values here, as that
// is tested more thoroughly in the ItemRecordTest
void checkReportContents(const string& report,
                         const string& sku,
                         const size_t expectedSold)
{
  const string line1 =
      to_string(expectedSold) + " boxes of " + sku + " have been sold";
  EXPECT_TRUE(substringFound(report, line1));
}
} // namespace

/******************************************************************************
 * UNIT TESTS ******************************************************************
 ******************************************************************************/

TEST(InventoryTest, SingleSkuSingleReceiveSellTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku = "FRUITLOOPS";
  const size_t received = 5;
  const size_t sold = 3;
  inventory.receive(sku, received, 2.0);
  inventory.sell(sku, sold, 2.5);

  // assert
  ::testing::internal::CaptureStdout();
  inventory.report();
  const string capturedReport = ::testing::internal::GetCapturedStdout();

  checkReportContents(capturedReport, sku, sold);
}

TEST(InventoryTest, SingleSkuMultipleReceiveSellTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku = "FRUITLOOPS";
  inventory.receive(sku, 3, 2.0);
  inventory.receive(sku, 5, 2.9);

  inventory.sell(sku, 4, 2.5);
  inventory.sell(sku, 3, 4.5);

  // assert
  ::testing::internal::CaptureStdout();
  inventory.report();
  const string capturedReport = ::testing::internal::GetCapturedStdout();

  checkReportContents(capturedReport, sku, 7);
}

TEST(InventoryTest, MultipleSkuSingleReceiveSellTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku1 = "FRUITLOOPS";
  const size_t received1 = 5;
  const size_t sold1 = 3;
  inventory.receive(sku1, received1, 2.0);
  inventory.sell(sku1, sold1, 2.5);

  const string sku2 = "CHIPSAHOY";
  const size_t received2 = 7;
  const size_t sold2 = 4;
  inventory.receive(sku2, received2, 2.70);
  inventory.sell(sku2, sold2, 2.5);

  // assert
  ::testing::internal::CaptureStdout();
  inventory.report();
  const string capturedReport = ::testing::internal::GetCapturedStdout();

  checkReportContents(capturedReport, sku1, sold1);
  checkReportContents(capturedReport, sku2, sold2);
}

TEST(InventoryTest, InvalidSellUnknownSkuTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku = "LUCKYCHARMS";

  ::testing::internal::CaptureStderr();
  inventory.sell(sku, 3, 2.5);
  const string capturedStderr = ::testing::internal::GetCapturedStderr();

  // assert
  EXPECT_TRUE(
      substringFound(capturedStderr, "No inventory available of SKU " + sku));
}

TEST(InventoryTest, InvalidSellOutOfStockTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku = "LUCKYCHARMS";
  const size_t received = 3;

  // sell as much as received
  inventory.receive(sku, received, 1.5);
  inventory.sell(sku, received, 2.5);

  // then sell more
  ::testing::internal::CaptureStderr();
  inventory.sell(sku, 2, 2.5);
  const string capturedStderr = ::testing::internal::GetCapturedStderr();

  // assert
  EXPECT_TRUE(
      substringFound(capturedStderr, "No inventory available of SKU " + sku));
}

TEST(InventoryTest, InvalidSellMoreThanAvailableTest)
{
  // arrange
  Inventory inventory;

  // act
  const string sku = "LUCKYCHARMS";
  const size_t received = 3;

  // sell more than received
  ::testing::internal::CaptureStderr();
  inventory.receive(sku, received, 1.5);
  inventory.sell(sku, received + 2, 2.5);
  const string capturedStderr = ::testing::internal::GetCapturedStderr();

  // assert
  EXPECT_TRUE(substringFound(capturedStderr,
                             "Insufficient inventory available of SKU " + sku));
  EXPECT_TRUE(substringFound(capturedStderr,
                             "Selling " + to_string(received) + " instead."));
}
