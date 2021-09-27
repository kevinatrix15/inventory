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
/**
 * @brief Search a string for a given substring.
 *
 * @param str The string to search.
 * @param substr The substring to search for.
 *
 * @return True if found, else false.
 */
bool substringFound(const string& str, const string& substr)
{
  return (str.find(substr) != string::npos);
}

/**
 * @brief Check that the report contents match the expected contents.
 *
 * NOTE:  We don't worry about testing the correct values here, as that is
 * the responsibility of the ItemRecord class and tested there.
 *
 * @param report The report string.
 * @param sku The SKU.
 * @param expectedSold The expected amount reported to be sold.
 */
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

/**
 * @brief Attempt to process a single receive and sell transaction for a single
 * SKU in the inventory, and inspect the captured report string. Expect the
 * report to contain relevant information.
 * REQ: 1, 2, 3.
 */
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

/**
 * @brief Attempt to process multiple receive and sell transaction for a single
 * SKU in the inventory, and inspect the captured report string. Expect the
 * report to contain relevant information.
 * REQ: 1, 2, 3.
 */
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

/**
 * @brief Attempt to process multiple receive and sell transaction for multiple
 * SKUs in the inventory, and inspect the captured report string. Expect the
 * report to contain relevant information for each SKU.
 * REQ: 1, 2, 3.
 */
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

/**
 * @brief Attempt to process the sell of a SKU that has no prior receipts.
 * Expect a descriptive error message printed to stderr.
 * REQ: 2.
 */
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

/**
 * @brief Attempt to process the sell of a SKU that is out of stock. Expect a
 * descriptive error message printed to stderr.
 * REQ: 2
 */
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

/**
 * @brief Attempt to process the sell of a SKU in which the requested quantity
 * exceeds the available inventory. Expect a descriptive error message printed
 * to stderr stating that the quantity sold has been reduced.
 * REQ: 2
 */
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
