#include "Inventory.h"
#include "ItemRecord.h"

using namespace std;

/* TODO:
 * - Add unit tests
 * - Format output to display two decimals
 * - Decide if profit calculation is correct if called in different order
 * - clang format
 * - add doc
 * - better name for 'Item'
 */

int main(int argc, char** argv)
{
  Inventory inventory;
  inventory.receive("CORNFLAKES", 8, 1.0);
  inventory.receive("CORNFLAKES", 2, 3.5);
  inventory.receive("APPLEJACKS", 3, 2.5);

  inventory.sell("CORNFLAKES", 2, 2.5);
  inventory.sell("CORNFLAKES", 4, 3.0);
  inventory.sell("APPLEJACKS", 4, 1.5);

  inventory.sell("CINNAMONTOASTCRUNCH", 4, 3.0);

  inventory.report();
  return 0;
}

/* README:
 * - Improvements if had more time:
 *	+ refer to FluentCpp blog on AoS vs SoA
 *
 * Assumptions:
 * * If requested to sell inventory that doesn't exist, isn't added to report
 */
