#pragma once
#include <map>
#include <string>

class DicHelper
{
public:
    union Store
    {
        std::string*ptr;
        int64_t i64;
        double f64;
    };

    enum class StoreType
    {
        DIC_INTEGER,
        DIC_FLOATING_POINT,
        DIC_STD_STRING
    };

    template<typename T>
    DicHelper(T t);
    ~DicHelper();

    DicHelper(const DicHelper& r);
    DicHelper& operator=(const DicHelper& r);

    DicHelper(DicHelper&& r);
    DicHelper& operator=(DicHelper&& r);

    template <typename T>
    T to(T defaultValue = T()) const
    {
        if (std::is_integral<T>::value)
        {
            if (m_type == StoreType::DIC_INTEGER)
                return static_cast<T>(m_store.i64);
        }
        else if (std::is_floating_point<T>::value)
        {
            if (m_type == StoreType::DIC_FLOATING_POINT)
                return static_cast<T>(m_store.f64);
        }
        return defaultValue;
    }

    template<>
    std::string DicHelper::to(std::string defaultValue) const
    {
        if (m_type == StoreType::DIC_STD_STRING)
        {
            auto pString = m_store.ptr;
            if (pString)
            {
                return *pString;
            }
        }
        return defaultValue;
    }

private:
    Store           m_store;
    const StoreType m_type;
};

class Dic
{
public:

    bool contain(const std::string k) const {
        return m_datas.find(k) != m_datas.end();
    }
    std::map<std::string, DicHelper>::iterator begin() {
        return m_datas.begin();
    }
    std::map<std::string, DicHelper>::iterator end() {
        return m_datas.end();
    }
    std::map<std::string, DicHelper>::const_iterator begin()const {
        return m_datas.begin();
    }
    std::map<std::string, DicHelper>::const_iterator end()const {
        return m_datas.end();
    }

    template<typename T>
    std::pair<std::map<std::string, DicHelper>::iterator, bool> 
        insert(const std::string&k, T v)
    {
        return m_datas.insert(std::make_pair(k, std::move(v)));
    }
    template<typename T>
    T get(const std::string& k, T defaultValue = T()) const {
        auto iter = m_datas.find(k);
        if (iter != m_datas.end()) {
            return iter->second.to<T>(std::move(defaultValue));
        }

        return defaultValue;
    }
    std::map<std::string, DicHelper>::iterator erase(const std::string& k) {
        auto iter = m_datas.find(k);
        if (iter != m_datas.end()) {
            iter = m_datas.erase(iter);
        }

        return iter;
    }
    std::map<std::string, DicHelper>::iterator find(const std::string& k) {
        return m_datas.find(k);
    }
    std::map<std::string, DicHelper>::const_iterator find(const std::string& k)const {
        return m_datas.find(k);
    }


private:
    std::map<std::string, DicHelper> m_datas;
};