#include "Dic.h"

template<typename T>
DicHelper::DicHelper(T t)
    :m_type(std::is_floating_point<T>::value ? StoreType::DIC_FLOATING_POINT : StoreType::DIC_INTEGER)
{
    m_store.i64 = 0;

    if (std::is_integral<T>::value)
    {
        m_store.i64 = static_cast<int64_t>(t);
    }
    else if (std::is_floating_point<T>::value)
    {
        m_store.f64 = static_cast<double>(t);
    }
}

template<>
DicHelper::DicHelper(std::string t)
    :m_type(StoreType::DIC_STD_STRING)
{
    m_store.ptr = new std::string(std::move(t));
}

template<>
DicHelper::DicHelper(const char* t)
    :m_type(StoreType::DIC_STD_STRING)
{
    m_store.ptr = new std::string(t);
}

DicHelper::~DicHelper()
{
    switch (m_type)
    {
    case StoreType::DIC_STD_STRING:
        if(m_store.ptr)
        {
            delete m_store.ptr;
            m_store.ptr = nullptr;
        }
        break;
    default:
        break;
    }
}

DicHelper::DicHelper(const DicHelper& r) : m_type(r.m_type)
{
    m_store.i64 = 0;
    *this = r;
}

DicHelper& DicHelper::operator=(const DicHelper& r)
{
    if (this != &r)
    {
        if (m_type == r.m_type)
        {
            switch (m_type)
            {
            case StoreType::DIC_STD_STRING:
            {
                if (m_store.ptr)
                {
                    delete m_store.ptr;
                    m_store.ptr = nullptr;
                }

                if (r.m_store.ptr)
                {
                    m_store.ptr = new std::string(*r.m_store.ptr);
                }
            }
            break;
            default:
                m_store.i64 = r.m_store.i64;
                break;
            }
        }
    }
    return *this;
}

DicHelper::DicHelper(DicHelper&& r) : m_type(r.m_type)
{
    m_store.i64 = r.m_store.i64;
    r.m_store.i64 = 0;
}

DicHelper& DicHelper::operator=(DicHelper&& r)
{
    if (this != &r)
    {
        if (r.m_type == m_type)
        {
            if (m_type == StoreType::DIC_STD_STRING)
            {
                if (m_store.ptr)
                {
                    delete m_store.ptr;
                    m_store.ptr = nullptr;
                }

                m_store.ptr = r.m_store.ptr;
                r.m_store.ptr = nullptr;
            }
            else
            {
                m_store.i64 = r.m_store.i64;
            }
        }
    }
    return *this;
}

// https://learn.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=msvc-170
// c++ template class separate implementation
void MyTestDic()
{
    int32_t v1 = 10;
    uint32_t v2 = 11;
    int64_t v3 = 12;
    uint64_t v4 = 14;
    int16_t v5 = 15;
    uint16_t v6 = 16;
    int8_t v7 = 17;
    uint8_t v8 = 18;
    float v9 = 19.0f;
    double v10 = 20.0;
    std::string v11 = "21";
    const char* v12 = "22";

    DicHelper d1(v1);
    DicHelper d2(v2);
    DicHelper d3(v3);
    DicHelper d4(v4);
    DicHelper d5(v5);
    DicHelper d6(v6);
    DicHelper d7(v7);
    DicHelper d8(v8);
    DicHelper d9(v9);
    DicHelper d10(v10);

    DicHelper d11(v11);
    DicHelper d12(v12);

    DicHelper d13(d11);
    DicHelper d14(std::move(d12));

    d13 = d14;
}