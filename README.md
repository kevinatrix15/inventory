# Instructions:
## Building:
  * Find and open the Inventory.sln file in VisualStudio
    - NOTE: Requires at least ISO C++ 2017 capability
  * Build the Inventory project within Visual Studio

## Running:
  * The main() function in Main.cpp runs the simple example from the email
    instructions.
  * More extensive unit testsing is found in the Inventory.Test project for both
    the Inventory and ItemRecord classes.
  * Tests may be run using the TestExplorer in VisualStudio.


# Solution Design
The inventory management system is represented completely between two classes-
Inventory and ItemRecord. They were designed with the following considerations.

## Inventory Class
### Overview
This class serves to provide the desired API to satisfy the basic business
functions:
  * Receive- receives a specified quantity of a item (represented by its
      SKU) at a given unit cost.
  * Sell- sell a specified quantity of a item (represented by its SKU) at a
    given unit price.
  * Report- Produces a report that contains, for each item:
      - Quantity sold
      - Quantity currently in stock
      - Profit or loss to date, using the weighted average coast of the item
      - Cost of unsold stock, using the weighted average cost of the item

No additional public methods were added to this class in order to keep the
exposed interface simple. It is composed of a map which stores an item record
for each item for efficient accessing of ItemRecords by SKU.

### Assumptions / Edge Cases
  * Sell of an unregistered item / out of stock item- Report a warning message to
    stderr.
  * Sell in which the requested quantity exceeds the available stock- Rather
    than causing the program to error at this point, it reduces the sold quantity
    to the available stock. This was done to keep the API user friendly without
    adding extra burden to the client to maintain its own accounting of available
    stock. A warning message is reported to stderr to notify the client to avoid
    unexpected behavior.

### Future Considerations
  * It might be desirable to make the 'sell' method more interactive in the
    event the requested quantity is unavailable. This might involve allowing the
    client to decide whether or not to execute the sell.
  * It might also be of interest to configure the Inventory class to maintain a
    minimum inventory of a given item in stock.
  * Ideally, dollar amounts included in the report should be formatted with two
    decimals.

## ItemRecord Class
### Overview
This class is responsible for maintaining the pertinent transaction history and
derived data (available stock, profit / loss, unsold cost) for a given item.
All of the computations are done here to separate responsibility from the
business logic of the Inventory class. This separation enables more thorough
testing of the underlying calculations through its class interface without
cluttering the API of the Inventory class.

### Assumptions / Edge Cases
Given the parity of transaction methods with the Inventory class, similar edge
cases are encountered here, but their handling is different.
  * Sell of an out of stock item- I assume here that negative stock of an item
    is unacceptable, as it invalidates other calculations. As such, it throws an
    exception to terminate execution to avoid silent failures.
  * Sell in which the requested quantity exceeds the available stock- Same
    behavior as the above item for the same reasons.

### Algorithmic Considerations
The primary results of interest are the profit and cost of unsold stock at a
given time, using the weighted average purchase price. The naive approach might
involve appending each receipt and sell transaction to a container, and then
calculating the average (mean) at the time the it is needed. While simple to
implement, the drawbacks of this approach include the following:
  * The calculation of the average will require O(N) operations each time it is
    needed, where N is the number of transactions. It is reasonable to assume
    multiple queries resulting in duplicated calculations of the mean.
  * The memory requirement will be O(N) for a container tracking the receipts.
    As the number of transactions is not known ahead of time, this may require
    multiple inefficient resizing operations.

I decided to go with an on-the-fly approach for calculating the weighted
average for both purchase and sell transactions. The benefits of this approach
include the following:
  * While calculating the weighted average on-the-fly still requires O(N)
    operations, it does not require re-calculation if queried multiple times at
    the same N. Additionally, this cost is incurred in constant time at each
    transaction, so the perceived cost when queried is O(1).
  * The memory requirement is now O(1) for both purchase and sell transactions.
