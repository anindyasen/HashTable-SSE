#include<bits/stdc++.h>
using namespace std;

enum Ctrl {
    Empty     = -128,
    Deleted   = -2,
    StopCtrl  = -1,
};


//template for generic type
template<typename K, typename V>
 
//Hashnode class
class HashNode
{
    public:
    V value;
    K key;
     
    //Constructor of hashnode 
    HashNode(K key, V value)
    {
        this->value = value;
        this->key = key;
    }
};
 
//template for generic type
template<typename K, typename V>
 
//Our own Hashmap class
class HashMap
{
    //hash element array
    HashNode<K,V> **arr;
    int capacity;
    // no of group
    int num_groups_;
    //Is rehash hash table
    bool isRehash;
    //current size
    int size;
    //dummy node
    HashNode<K,V> *dummy;
    //cntl byte
    char *cntl_byte;
 
    public:
    HashMap(int _capacity, bool _isRehash)
    {
        //Initial capacity of hash array
        capacity = _capacity;
        isRehash = _isRehash;
        num_groups_ = _capacity / 16;
        size=0;
        arr = new HashNode<K,V>*[capacity];
        cntl_byte = new char[capacity]; 
        for(int i=0;i<capacity;i++)
            cntl_byte[i] = Empty;
        //Initialise all elements of array as NULL
        for(int i=0 ; i < capacity ; i++)
            arr[i] = NULL;
         
        //dummy node with value and key -1
        dummy = new HashNode<K,V>(-1, -1);
    }
    // This implements hash function to find index
    // for a key
    int hashCode(K key, int _capacity)
    {
        return key % _capacity;
    }

    uint64_t hashfn(K key)
    {
        return key;
    }

    int h1(uint64_t hash) {
        return (hash >> 7);
    }

    int h2(uint64_t hash) {
        return (hash & 0x7f);
    }

    void rehash()
    {
        int new_capacity = capacity*2;
        int new_num_group = new_capacity / 16;
        int new_size=0;
        HashNode<K,V> **new_arr = new HashNode<K,V>*[new_capacity];

        //Initialise all elements of array as NULL
        for(int i=0 ; i < new_capacity ; i++)
            new_arr[i] = NULL;
        
        char *new_cntl_byte = new char[new_capacity]; 
        for(int i=0;i<new_capacity;i++)
            new_cntl_byte[i] = Empty;
        
        //rehash

        for(int i=0 ; i<capacity ; i++)
        {
            if(arr[i] != NULL && arr[i]->key != -1) {
                 HashNode<K,V> *temp = new HashNode<K,V>(arr[i]->key , arr[i]->value);
                  
                 // Apply hash function to find index for given key
                 uint64_t hashIndex = hashfn(arr[i]->key);
                 uint64_t pos = 0;
                 uint64_t group_pos = hashIndex % new_num_group;
                 //int hashIndex = hashCode(arr[i]->key,new_capacity);
                 int hashSize = 0;
                 int group_cnt = 0;
                  while(new_cntl_byte[pos + group_pos*16] != Empty && 
                        new_cntl_byte[pos + group_pos*16] != Deleted && 
                        new_cntl_byte[pos + group_pos*16] !=  h2(hashIndex) &&
                        arr[pos + group_pos*16]->key != temp->key) {
                      pos = (pos + 1) % 16;
                      hashSize++;
                      if(hashSize == 16) {
                          cout<< "Completely filled group : "<< group_pos<< endl;
                          group_pos = (group_pos+1) % new_num_group;
                          pos = 0;
                          hashSize = 0;
                          group_cnt++;
                          if(group_cnt == new_num_group) {
                             cout<< "No More ReHash"<<endl;
                             return;
                          }
                      }
                  }
                     
                  
                 //if new node to be inserted increase the current size
                 if(new_arr[pos + group_pos*16] == NULL || new_arr[pos + group_pos*16]->key == -1)
                     new_size++;
                 new_cntl_byte[pos + group_pos*16] = h2(hashIndex);
                 new_arr[pos + group_pos*16] = temp;
            }
        }
        delete arr;
        arr = new_arr;
        delete cntl_byte;
        cntl_byte = new_cntl_byte;
        capacity = new_capacity;
        num_groups_ = new_num_group;
        size = new_size;
    }
     
    //Function to add key value pair
    void insertNode(K key, V value)
    {
        HashNode<K,V> *temp = new HashNode<K,V>(key, value);
         
        // Apply hash function to find index for given key
        //int hashIndex = hashCode(key,capacity);
        uint64_t hashIndex = hashfn(key);
        uint64_t pos = 0;
        uint64_t group_pos = h1(hashIndex) % num_groups_;
        int group_cnt = 0;
        int hashSize = 0; 
        //find next free space
        while(cntl_byte[pos + group_pos*16] != Empty && 
              cntl_byte[pos + group_pos*16] != Deleted && 
              cntl_byte[pos + group_pos*16] !=  h2(hashIndex) &&
              arr[pos + group_pos*16]->key != temp->key) {
            pos = (pos + 1) % 16;
            hashSize++;
            if(hashSize == 16) {
                cout<< "Completely filled group : "<< group_pos<< endl;
                group_pos = (group_pos+1) % num_groups_;
                pos = 0;
                hashSize = 0;
                group_cnt++;
                if(group_cnt == num_groups_) {
                    if(isRehash == 1) {
                        cout<<"ReHashing"<<endl;
                        rehash();
                        hashSize = 0;
                        group_cnt = 0;
                    } else {
                        cout <<"Hash Full";
                        return;
                    }
                }
            }
        }

        //if new node to be inserted increase the current size
        if(cntl_byte[pos + group_pos*16] == Empty || cntl_byte[pos + group_pos*16] == Deleted)
            size++;
        cntl_byte[pos + group_pos*16] = h2(hashIndex);
        arr[pos + group_pos*16] = temp;
        cout << "pos : "<<pos << " group : "<< group_pos << " key : "<< temp->key << " value : "<< temp->value<<endl;
    }
     
    //Function to delete a key value pair
    V deleteNode(int key)
    {
        // Apply hash function to find index for given key
        //int hashIndex = hashCode(key,capacity);
        uint64_t hashIndex = hashfn(key);
        uint64_t pos = 0;
        uint64_t group_pos = h1(hashIndex) % num_groups_;
        int hashSize = 0;
        int group_cnt = 0; 
        //finding the node with given key
        while(cntl_byte[pos + (group_pos * 16)] != Empty) {
            //if node found
           if(cntl_byte[pos + (group_pos * 16)] == h2(hashIndex) && arr[pos + (group_pos * 16)]->key == key) {
               HashNode<K,V> *temp = arr[pos + (group_pos * 16)];
                //Insert dummy node here for further use
               arr[pos] = dummy;
               cntl_byte[pos + (group_pos * 16)] = Deleted;
                // Reduce size
               size--;
               return temp->value;
           }
           pos = (pos + 1) % capacity;
           hashSize++;
           if(hashSize == 16) {
               group_pos = (group_pos+1) % num_groups_;
               pos = 0;
               hashSize = 0;
               group_cnt++;
               if(group_cnt == num_groups_) {
                  cout<< "Element Not Present\n"<<endl;
                  return V(0);
               }
           }
        }
         
        //If not found return null
        return V(0);
    }
    
    // compare 2 arr of size capacity
    void _mm_cmp_epi8(char* cntl_byte, char* cmp_key) {
        for(int i =0; i<16; i++) {
           if(cntl_byte[i] == cmp_key[i])
              cmp_key[i] = 0xFF; 
           else
              cmp_key[i] = 0x0;
        }
    }
   
    // mark the matching position as 1 else 0 of a arr of len capacity
    void _mm_movemask_epi8(char* cmp_key) {
        char val = 0xFF;
        for(int i =0;i<16;i++) {
            if(cmp_key[i] == val) {
                cmp_key[i] = 1;
            } else {
                cmp_key[i] = 0;
            }
        }
    }

    //Function to search the value for a given key
    V get(int key)
    {
        // Apply hash function to find index for given key

        //int hashIndex = hashCode(key,capacity);
        uint64_t hashIndex = hashfn(key);
        //uint64_t pos = h1(hashIndex) % capacity;
        uint64_t group_pos = h1(hashIndex) % num_groups_;
        int group_cnt = 0;
        char cmp_key[16];
        for(int i=0;i<16;i++) {
            cmp_key[i] = h2(hashIndex);
        }
        while(1){
            int hashSize = 0;
            _mm_cmp_epi8(cntl_byte+(group_pos*16), cmp_key);
            _mm_movemask_epi8(cmp_key);
            for (int i= 0;i<16; i++) {
                if(cmp_key[i] == 1 && arr[i + (group_pos*16)]->key == key) {
                    return arr[i + (group_pos*16)]->value;
                }
            } 
            group_cnt++;
            group_pos = (group_pos + 1)  % num_groups_;
            if (group_cnt == num_groups_) {
                break;
            }
        }
        cout << "Val Not Found" << endl;
        return V(0);
    }
     
    //Return current size 
    int sizeofMap()
    {
        return size;
    }

    int getCapacity() {
        return capacity;
    }
     
    //Return true if size is 0
    bool isEmpty()
    {
        return size == 0;
    }
     
    //Function to display the stored key value pairs
    void display()
    {
        for(int i=0 ; i<capacity ; i++)
        {
            if(arr[i] != NULL && arr[i]->key != -1)
                cout << "key = " << arr[i]->key 
                     <<"  value = "<< arr[i]->value << endl;
        }
    }
};
 
