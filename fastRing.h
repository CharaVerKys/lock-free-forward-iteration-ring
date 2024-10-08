#ifndef RINGQUEUE_H
#define RINGQUEUE_H
#include <iostream>
#include <vector>
#include <cassert>
#include <atomic>
#include <cstdint>

enum Size{
    s128 = 7,
    s32768 = 15,
    s2147483648 = 31
};

template <typename T, typename uint_type>
class RingBase
{
public:

    using iter = typename std::vector<T>::iterator;

    explicit RingBase(std::size_t sizeInPowOfTwo)
        : m_usedSize(0)
        , m_size(sizeInPowOfTwo)
        , m_vector(std::vector<T>(m_size*2))    
        , m_itOfBegin(0)
        , m_itOfEnd(0)
    {
        assert((
            m_size == (1 << s128)  or
            m_size == (1 << s32768) or
            m_size == (2147483648)
        )&& "wrong size");
    }

    const std::size_t& maxSize(){
        return m_size;
    }
    const std::size_t& usedSize(){
        return m_usedSize;
    }

    void push_front(const T& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            ++m_itOfBegin; // ++ and move to begin if end, cuz uint8_t {255} +1 == uint8_t {0}
        }
        m_vector[m_itOfEnd] = value;
        ++m_itOfEnd;
    }

    void push_front(T&& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            ++m_itOfBegin; // ++ and move to begin if end, cuz uint8_t {255} +1 == uint8_t {0}
        }
        m_vector[m_itOfEnd] = std::move(value);
        ++m_itOfEnd;
    }

    void clear(){
        m_itOfBegin.store(0);
        m_itOfEnd.store(0);
        m_usedSize = 0;
    }

    template<class func> // only template work with lambda here // even std::function (without template) not work
    void forwardIterationThoughRing(func fnc, std::size_t size){

        typename std::vector<T>::iterator it = m_vector.begin() + m_itOfBegin.load();

        if(it + size-1 < m_vector.end()){
            it += size-1;
            assert(it!=m_vector.end());
        }else{
            std::size_t i = 0;
            while(it + i != m_vector.end()){++i;} // find end
            it += i;
            assert(it==m_vector.end());
            it = m_vector.begin() + size -1 -i;
        }

        for(std::size_t i = 0; i<size; ++i){
            assert(it!=m_vector.end());
            fnc(it);
            if(it == m_vector.begin()){it = m_vector.end();}
            --it;
        }
    }

private:
    const std::size_t m_size;
    std::size_t m_usedSize;
    std::atomic<uint_type> m_itOfBegin;
    std::atomic<uint_type> m_itOfEnd;
    typename std::vector<T> m_vector;

};

template <typename T, Size size>
class Ring : public RingBase<T,int>{
    Ring():RingBase<T,int>(0){
        throw("using unspecified container");
    }
};

template <typename T>
class Ring<T, s128> : public RingBase<T,uint8_t> {
public:
    Ring():RingBase<T, uint8_t>(1<<s128){}
}; // end of s128

template <typename T>
class Ring<T, s32768> : public RingBase<T,uint16_t> {
public:
    Ring():RingBase<T, uint16_t>(1<<s32768){}
}; // end of s32768

template <typename T>
class Ring<T, s2147483648> : public RingBase<T,uint32_t> {
public:
    Ring():RingBase<T, uint32_t>(2147483648){}
}; // end of s2147483648

#endif // RINGQUEUE_H