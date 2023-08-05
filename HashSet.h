//
// Created by ethan on 4/4/2023.
//

#ifndef UNTITLED30_HASHSET_H
#define UNTITLED30_HASHSET_H
#include <list>
#include <algorithm>
#include <sstream>
using namespace std;

const size_t INITIAL_CAPACITY = 11;
const double MAX_LOAD = 1;

template <class T>
class HashSet {
public:
    HashSet(){}
    ~HashSet(){
        delete [] m_data;
    }
    bool add(T & value) {
        size_t hashcode = hash(value);
        size_t index = hashcode % m_capacity;
        if(count(m_data[index].begin(), m_data[index].end(), value) != 0){
            return false;
        }
        m_data[index].push_back(value);
        m_size++;
        if(static_cast<double>(m_size)/m_capacity > MAX_LOAD){
            grow();
        }
        return true;
    }
    bool remove(T & value){
        if (!contains(value)){
            return false;
        }
        size_t index = hash(value) % m_capacity;
        m_data[index].remove(value);
        m_size--;
        return true;
    }
    bool contains(T & value) const {
        size_t index = hash(value) % m_capacity;
        if (m_data[index].size() != 0){
            if(count(m_data[index].begin(), m_data[index].end(), value) != 0){
                return true;
            }
        }
        return false;
    }
    size_t size() const {return m_size;}
    std::string toString() const {
        std::ostringstream os;
        for (int i = 0; i < m_capacity; i++){
            if (m_data[i].size() == 0) continue;
            os << "[" << i << "] = ";
            for (T value : m_data[i]){
                os << value << " ";
            }
            os << endl;
        }
        return os.str();
    }
private:
    size_t hash(T & value) const { // Write multiple versions for different data types
        return static_cast<size_t>(value);
    }
    size_t hash(string & value) const {
        size_t sum = 0;
        for (char c : value){
            sum = sum * 31 + c;
        }
        return sum;
    }
    size_t myHash(T & value){
        unsigned char * itr = (unsigned char *) &value;
        size_t sum = 0;
        for(size_t i = 0; i < sizeof(value); i++){
            sum = sum * 31 + *itr;
            itr++;
        }
        return sum;
    }

    void grow(){
        list<T>* old= m_data;
        size_t oldSize = m_capacity;
        m_capacity = m_capacity * 2 -1;
        m_data = new list<T>[m_capacity];
        m_size = 0;
        for (int i = 0; i < oldSize; i++){
            if (old[i].size() != 0){
                for(T value : old[i]){
                    add(value);
                }
            }
        }
        delete [] old;
    }

    list<T>* m_data = new list<T>[INITIAL_CAPACITY];
    size_t m_capacity = INITIAL_CAPACITY;
    size_t m_size = 0;
};


#endif //UNTITLED30_HASHSET_H
