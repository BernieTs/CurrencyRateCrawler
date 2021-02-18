#ifndef CURRENCYRATEMANAGER_H
#define CURRENCYRATEMANAGER_H

#include <unordered_map>

/**
 * @brief 匯率資訊結構
 */
struct CurrencyRate
{
    std::string CurrName;
    std::string sDescription;
    float buyValue;
    float sellValue;
};

/**
 * @brief 儲存匯率資訊類別
 * 此類別提供匯率資訊紀錄，查找等功能。
 */
class CurrencyRateManager
{
public:
    CurrencyRateManager();
    void AddCurrencyRate(CurrencyRate rate);
    bool GetCurrencyRate(const std::string &name, CurrencyRate &currencyRate);
    std::vector<CurrencyRate> GetAllCurrencyRateInfo();
    void RemoveAllCurrencyRate();

private:
    std::unordered_map<std::string, CurrencyRate> currencyMap;
};

#endif // CURRENCYRATEMANAGER_H
