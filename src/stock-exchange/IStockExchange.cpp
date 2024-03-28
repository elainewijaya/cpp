#include "IStockExchange.h"
#include "StockExchange.h"

std::unique_ptr<IStockExchange> IStockExchange::Create() {
  return std::make_unique<StockExchange>();
}
