#include "CurrencyRateManager.h"
#include <QDebug>

CurrencyRateManager::CurrencyRateManager()
{

}

/**
 * @brief CurrencyRateManager::AddCurrencyRate
 * 將CurrencyRate物件加入至CurrencyRateManager map中
 * @param rate 預加入的CurrencyRate物件
 */
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

/**
 * @brief CurrencyRateManager::DownloadCurrencyRateByName
 * 將map中特定name的CurrencyRate資料輸出成.txt檔案
 * @param name 預輸出的貨幣名稱
 * @param filePath 輸出的檔案位置
 * @return 如果map沒有該筆資料或檔案輸出不成功則返回false
 */
bool CurrencyRateManager::DownloadCurrencyRateByName(const std::string &name, QFileInfo filePath)
{
    CurrencyRate cr;
    if(!GetCurrencyRate(name, cr)) //object不在map中時
    {
        qDebug()<<"!GetCurrencyRate(name, cr)";
        return false;
    }

    qDebug()<<filePath.absoluteFilePath();
    QFile file(filePath.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        qDebug()<<"Cannot open file for writing";
        return false;
    }

    QTextStream out( &file );
    out << QString("幣別：") << QString::fromStdString(cr.CurrName) << "\n";
    out << QString("匯率：") << QString::fromStdString(cr.sDescription) << "\n";
    out << QString("現金匯率：") << QString::number(cr.buyValue) << "\n";
    out << QString("即期匯率：") << QString::number(cr.sellValue) << "\n";

    file.close();
    return true;
}
