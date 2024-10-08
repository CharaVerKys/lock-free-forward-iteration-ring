#ifndef RINGQUEUE_H
#define RINGQUEUE_H
#include <vector>
#include <cassert>
#include <std::mutex>

template <typename T>
class Ring
{
public:
    class iterator;
    using iter = typename std::vector<T>::iterator;

    explicit Ring(std::size_t size)
        : mutex_itOfBegin(new std::mutex())
        , m_size(size)
        , m_usedSize(0)
        , m_vector(std::vector<T>(m_size*2))
    {
        m_itOfBegin = m_vector.begin();
        m_itOfEnd = m_itOfBegin;
    }
    ~Ring(){
        if(mutex_itOfBegin){delete mutex_itOfBegin;}
    }

    const std::size_t& size(){
        return m_size;
    }
    const std::size_t& usedSize(){
        return m_usedSize;
    }

    void push_front(const T& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            mutex_itOfBegin->lock();
                ++m_itOfBegin;
                if(m_itOfBegin == m_vector.end()){
                    m_itOfBegin = m_vector.begin();
                }
            mutex_itOfBegin->unlock();
        }
        *m_itOfEnd = value;
        ++m_itOfEnd;
        if(m_itOfEnd == m_vector.end()){
            m_itOfEnd = m_vector.begin();
        }
    }

    void push_front(T&& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            mutex_itOfBegin->lock();
                ++m_itOfBegin;
                if(m_itOfBegin == m_vector.end()){
                    m_itOfBegin = m_vector.begin();
                }
            mutex_itOfBegin->unlock();
        }
        *m_itOfEnd = std::move(value);
        ++m_itOfEnd;
        if(m_itOfEnd == m_vector.end()){
            m_itOfEnd = m_vector.begin();
        }
    }

    void clear(){
        m_itOfBegin = m_vector.end();
        m_itOfEnd = m_itOfBegin;
        m_usedSize = 0;
    }

    template<class func> // only template work with lambda here // even std::function (without template) not work
    void forwardIterationThoughRing(func fnc, std::size_t size){

        mutex_itOfBegin->lock();
            typename std::vector<T>::iterator it = m_itOfBegin;
        mutex_itOfBegin->unlock();

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
    std::mutex* mutex_itOfBegin;
    std::size_t m_size;
    std::size_t m_usedSize;
    typename std::vector<T>::iterator m_itOfBegin;
    typename std::vector<T>::iterator m_itOfEnd;
    typename std::vector<T> m_vector;
};
