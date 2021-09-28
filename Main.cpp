#include "Inventory.h"
#include "ItemRecord.h"

using namespace std;

int main()
{
  Inventory inventory;

  inventory.receive("CORNFLAKES", 8, 1.0);
  inventory.receive("CORNFLAKES", 2, 3.5);

  inventory.sell("CORNFLAKES", 2, 2.5);
  inventory.sell("CORNFLAKES", 4, 3.0);

  inventory.report();
  return 0;
}
