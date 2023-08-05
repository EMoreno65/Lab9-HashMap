#ifndef UNTITLED30_HASHMAP_H
#define UNTITLED30_HASHMAP_H

#include <list>
#include <algorithm>
#include <sstream>
using namespace std;
#include <string>

#include "HashmapInterface.h"

struct KeyValuePair {
    KeyValuePair(string key, int value){
       this->key = key;
       this->value = value;
    };
    std::string key;
    int value;
};

const size_t INITIAL_CAPACITY = 10;
const double MAX_LOAD = 0.8;

class Hashmap : public HashmapInterface {
private:
    // put any private data members or methods here
    list<KeyValuePair>* m_data = new list<KeyValuePair>[INITIAL_CAPACITY];
    size_t m_capacity = INITIAL_CAPACITY;
    size_t m_size = 0;
public:
    Hashmap(){
    }
    Hashmap(string basicString, int i) {
        m_data = new std::list<KeyValuePair>[INITIAL_CAPACITY];
        m_capacity = INITIAL_CAPACITY;
        m_size = 0;
    }

    ~Hashmap(){
        for (size_t i = 0; i < m_capacity; i++) {
            m_data[i].clear();
        }
        delete[] m_data;
    }





    void insert(std::string key, int value) override {
        size_t hashcode = std::hash<std::string>{}(key); // 1. Compute Hashcode using hash function
        size_t index = hashcode % m_capacity; // 2. Map hashcode to an index
        if(m_data[index].empty()){
            m_data[index] = std::list<KeyValuePair>();
        }

        for (auto& node : m_data[index]) {
            if (node.key == key) {
                node.value = value;
                return;
            }
        }

        m_data[index].push_back(KeyValuePair(key, value)); // 3. Check if index has a linked list, if not, make one
        m_size++;

        // 4. Check the load factor and resize if necessary
        double loadFactor = static_cast<double>(m_size) / m_capacity;
        if (loadFactor > MAX_LOAD_FACTOR) {
            size_t newCapacity = m_capacity * 2;
            list<KeyValuePair>* newData = new list<KeyValuePair>[newCapacity];
            for (size_t i = 0; i < m_capacity; ++i) {
                for (const auto& node : m_data[i]) {
                    size_t newHashcode = std::hash<std::string>{}(node.key);
                    size_t newIndex = newHashcode % newCapacity;
                    newData[newIndex].push_back(node);
                }
            }
            delete[] m_data;
            m_data = newData;
            m_capacity = newCapacity; // Update the capacity
            // Update the hash codes and indices for each element
            for (size_t i = 0; i < m_capacity; ++i) {
                auto& bucket = m_data[i];
                for (auto& node : bucket) {
                    size_t newHashcode = std::hash<std::string>{}(node.key);
                    size_t newIndex = newHashcode % m_capacity;
                    node.key = node.key; // No need to update the key
                    node.value = node.value; // No need to update the value
                }
            }
        }
    }



    bool contains(const std::string &key) const override {
        size_t hashcode = std::hash<std::string>{}(key);
        size_t index = hashcode % m_capacity;
        for (const auto& node : m_data[index]) {
            if (node.key == key) {
                return true;
            }
        }
        return false;
        //Compute the hashcode of the key using std::hash<std::string>{}(key)
        //Map the hashcode to an index using hashcode % m_capacity
        //Check if the linked list at the index contains a node with the given key
        //If a node with the key is found, return true. Otherwise, return false.
        /*
     * Return the value associated with the given key in the map
     *  Throw invalid_argument exception if key is not in map
     */
    }

    int get(const std::string &key) const override {
        size_t hashcode = std::hash<std::string>{}(key); // 1. Compute Hashcode using hash function
        size_t index = hashcode % m_capacity; // 2. Map hashcode to an index
        const auto& bucket = m_data[index];
        for (const auto& node : bucket){
            if (node.key == key){
                return node.value;
            }
        }
        throw std::invalid_argument("Key not found in map");
    }


    int &operator[](const std::string &key) override {
        size_t hashcode = std::hash<std::string>{}(key);
        size_t index = hashcode % m_capacity;

        // If the bucket is empty, create a new list
        if (m_data[index].empty()) {
            m_data[index] = std::list<KeyValuePair>();
        }

        // Search for an existing node with the given key
        for (auto& node : m_data[index]) {
            if (node.key == key) {
                return node.value;
            }
        }

        // If no existing node is found, create a new node and add it to the bucket
        m_data[index].push_back(KeyValuePair(key, 0));
        m_size++;

        // Check the load factor and resize if necessary
        double loadFactor = static_cast<double>(m_size) / m_capacity;
        if (loadFactor > MAX_LOAD_FACTOR) {
            size_t newCapacity = m_capacity * 2;
            list<KeyValuePair>* newData = new list<KeyValuePair>[newCapacity];
            for (size_t i = 0; i < m_capacity; ++i) {
                for (const auto& node : m_data[i]) {
                    size_t newHashcode = std::hash<std::string>{}(node.key);
                    size_t newIndex = newHashcode % newCapacity;
                    newData[newIndex].push_back(node);
                }
            }
            delete[] m_data;
            m_data = newData;
            m_capacity = newCapacity;
            index = hashcode % m_capacity;
        }

        return m_data[index].back().value;
    }


    bool remove(const std::string &key) override {
        size_t hashcode= std::hash<std::string>{}(key);
        size_t index = hashcode % m_capacity;
        auto& bucket = m_data[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                --m_size;
                return true;
            }
            return false;
        }
        //Compute the hash code for the input key.
        //Use the hash code to calculate the index of the bucket in which the key should be located.
        //Traverse the linked list of key-value pairs in the bucket at the calculated index.
        //If the key is found, remove the key-value pair from the linked list.
        //Decrement the size of the hash map by 1 to reflect the removal.
        //If the bucket is now empty, set it to null to free up the memory.
        //If the key is not found, return false to indicate that the removal was unsuccessful.
        //Return true to indicate that the removal was successful.
        /*
     * Reset map to empty
     */
    }

    void clear() override {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_data[i].clear();
        }
        m_size = 0;
    }

    int numBuckets() const override {
        return m_capacity;
    }

    int size() const override {
        return m_size;
    }
};

#endif //UNTITLED30_HASHMAP_H
