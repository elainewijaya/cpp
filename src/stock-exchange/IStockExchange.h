#pragma once

#include <string>

struct Order {
  enum class Operation {
    BUY, SELL
  };

  std::string symbol;
  double price;
  int volume;
  Operation operation;
};

using TestCallback = std::function<void()>;

class IStockExchange {
public:
  // Factory function
  static std::unique_ptr<IStockExchange> Create();

  virtual ~IStockExchange() {}
  virtual void Test(TestCallback p_Callback) = 0;
  virtual void Process(Order&& p_Order) = 0;
  virtual void DisplayOrders() = 0;
};

