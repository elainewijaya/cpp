#include "StockExchange.h"

#include <iostream>

StockExchange::StockExchange() {}

StockExchange::~StockExchange() {}

void StockExchange::Test(TestCallback p_Callback) {
  std::cout << "StockExchange::Test START" << std::endl;
  p_Callback();
  return;
}

void StockExchange::Process(Order&& p_Order) {
  // do something that could take a while
  // callback when done to fire event

}

void StockExchange::DisplayOrders() {

}
