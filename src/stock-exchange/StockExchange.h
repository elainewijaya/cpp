#pragma once

#include "IStockExchange.h"

class StockExchange : public IStockExchange {
public:
  StockExchange();
  virtual ~StockExchange() override;
  virtual void Test(TestCallback p_Callback) override;
  virtual void Process(Order&& p_Order) override;
  virtual void DisplayOrders() override;

private:
  // data
};
