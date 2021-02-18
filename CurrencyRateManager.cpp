#include "CurrencyRateManager.h"

CurrencyRateManager::CurrencyRateManager()
{

}

void CurrencyRateManager::AddCurrencyRate(CurrencyRate rate)
{
    if(GetCurrencyRate(rate.CurrName, CurrencyRate())) //object已經在map中了
        return;

    currencyMap.insert(std::make_pair(rate.CurrName, rate));
}

/**
 * @brief CurrencyRateManager::GetCurrencyRate
 * @param name 查詢的幣別名稱
 * @param currencyRate 回傳查詢到的currencyRate物件
 * @return 資料存在於map中則回傳true，反之回傳false
 */
bool CurrencyRateManager::GetCurrencyRate(const std::string &name, CurrencyRate &currencyRate)
{
    std::unordered_map<std::string, CurrencyRate>::iterator iter;
    iter = currencyMap.find(name);
    if(iter!=currencyMap.end())
    {
        currencyRate = iter->second;
        return true;
    }
    return false;
}

/**
 * @brief CurrencyRateManager::GetAllCurrencyRateInfo
 * 取得currencyRateMap中所有物件
 * @return vecotor形式記錄的物件
 */
std::vector<CurrencyRate> CurrencyRateManager::GetAllCurrencyRateInfo()
{
    std::vector<CurrencyRate> vCurrencyRates;
    std::unordered_map<std::string, CurrencyRate>::iterator iter;
    for(iter = currencyMap.begin(); iter!=currencyMap.end(); iter++)
    {
        vCurrencyRates.push_back(iter->second);
    }
    return vCurrencyRates;
}

/**
 * @brief CurrencyRateManager::RemoveAllCurrencyRate
 * 移除所有currencyMap資料
 */
void CurrencyRateManager::RemoveAllCurrencyRate()
{
    currencyMap.clear();
}
