王佳妮2014 fall FM  12:16:19 PM
//
//  main.cpp
//  test111
//
//  Created by Anny Wang on 14-10-1.
//  Copyright (c) 2014年 Anny Wang. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <memory.h>
using namespace std;

class Array{
public:
    Array(){
        num = 0;
        capacity = 0;
    }
    Array(int n,int val){
        allocate_and_fill(n,val);
    }
    Array(int n){
        allocate_and_fill(n,0);
    }
    Array( Array& source){
        allocate_and_fill(source,0,source.size());
    }
    ~Array(){
        deallocate(data);
    }
    void allocate_and_fill(int n,int val){
        data = new int[n];
        num = n;
        capacity = n;
        while( --n >=0 ){
            data[n] = val;
        }
    }
    void allocate_and_fill(const Array& source,int start,int end){
        allocate_and_fill(source,start,end,start-end);
    }
    void allocate_and_fill(const Array& source,int start,int end,int new_capacity){
        num = end - start;
        data = new int[new_capacity];
        capacity = new_capacity;
        for(int i=0;i<num;i++){
            data[i] = source[start+i];
        }
    }
    void allocate_and_fill(int* source,int start,int end,int new_capacity){
        num = end - start;
        data = new int[new_capacity];
        capacity = new_capacity;
        for(int i=0;i<num;i++){
            data[i] = source[start+i];
        }
    }
    void deallocate(int* arr){
        if(arr){
            delete[] arr;
        }
    }
    int size() const {
        return num;
    }
    int capa() const {
        return capacity;
    }
    bool full() const {
        return num == capacity;
    }
    int operator[]  (int index) const{
        if(0<=index && index<num){
            return data[index];
        }else{
            return 0;
        }
    }
    
    int first(){
        if(num>=0){
            return data[0];
        }else{
            return 0;
        }
    }
    int last(){
        if(num>=0){
            return data[num-1];
        }else{
            return 0;
        }
    }
    void push_back(const int val){
        if(!full()){
            data[num++] = val;
        }else {
            insert(val,num);
        }
    }
    void pop_back(){
        if(num>0){
            num--;
        }
    }
    void remove(int start,int end){ //remove [start,end)
        int j = num - end;
        for(int i=0;i<j;i++){
            data[start+i] = data[end+i];
        }
        num -= (end - start);
    }
    void remove(int index) {
        remove(index,index+1);
    }
    void clear(){
        num = 0;
    }
    
    void insert(int val,int index){
        if(capacity == 0){
            allocate_and_fill(1,val);
        }
        else if(num+1>capacity){
            int* old_data = data;
            
            allocate_and_fill(old_data,0,num,capacity*2);
            for(int i=num;i>index;i--){
                data[num] = data[num-1];
            }
            data[index] = val;
            num++;
            
            deallocate(old_data);
        }else{
            for(int i=num;i>index;i--){
                data[num] = data[num-1];
            }
            data[index] = val;
            num++;
        }
    }
    
private:
    int num;
    int* data;
    int capacity;
};

Array array(3,2);

void print(){
    cout <<"size: "<< array.size() << "\t" <<"capacity: "<< array.capa() << endl;
    int length = array.size();
    for(int i=0;i<length;i++){
        cout << array[i] << endl;
    }
}

int main(){
    
    print();
    array.push_back(4);
    print();
    array.push_back(5);
    print();
    array.push_back(6);
    array.push_back(7);
    print();
    
    array.remove(5);
    print();
    
    array.insert(11,1);
    print();
    
    array.pop_back();
    print();
    
    array.clear();
    print();
    
    return 0;
    
}
