#pragma once

class ItemRecord
{
  public:
  ItemRecord()
      : m_numBought(0),
        m_numSold(0),
        m_avgPurchasePrice(0.0),
        m_avgSalesPrice(0.0)
  {
  }

  void buy(const size_t quantity, const double price)
  {
    updateAverage(quantity, price, m_numBought, m_avgPurchasePrice);
    m_numBought += quantity;
  }

  void sell(const size_t quantity, const double price)
  {
    updateAverage(quantity, price, m_numSold, m_avgSalesPrice);
    m_numSold += quantity;
  }

  int numAvailable() const
  {
    return static_cast<int>(m_numBought) - static_cast<int>(m_numSold);
  }

  size_t numBought() const
  {
    return m_numBought;
  }

  size_t numSold() const
  {
    return m_numSold;
  }

  double avgPurchasePrice() const
  {
    return m_avgPurchasePrice;
  }

  double avgSalesPrice() const
  {
    return m_avgSalesPrice;
  }

  double profit() const
  {
    return m_numSold * (m_avgSalesPrice - m_avgPurchasePrice);
  }

  double unsoldCost() const
  {
    return (this->numAvailable() * m_avgPurchasePrice);
  }

  private:
  // https://stackoverflow.com/questions/28820904/how-to-efficiently-compute-average-on-the-fly-moving-average
  void updateAverage(const size_t quantity,
                     const double val,
                     const size_t startCount,
                     double& currAvgRef) const
  {
    const size_t totalCount = startCount + quantity;
    for (size_t count = startCount; count < totalCount; ++count) {
      currAvgRef += (val - currAvgRef) / (count + 1);
    }
  }

  size_t m_numBought;
  size_t m_numSold;
  double m_avgPurchasePrice;
  double m_avgSalesPrice;
};
