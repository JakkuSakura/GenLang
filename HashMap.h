//
// Created by Lenovo on 2018/9/27.
//

#ifndef GENLANG_HASHMAP_H
#define GENLANG_HASHMAP_H

#include <math.h>
#include <stdio.h>
#include "Node.h"
typedef struct __pair{
    pNode val;
    pNode key;
}Pair;
typedef struct __hashmap{
    int size;
    int max_length;
    Pair *a;
}HashMap;
int hash(HashMap *hashMap, pNode key)
{
    double tmp = (int)key * 0.618165161;
    return (int)((tmp - floor(tmp)) * hashMap->max_length);
}
HashMap *new_HashMap(int max_length){
    HashMap *hashMap = (HashMap *)malloc(sizeof(HashMap));
    hashMap->max_length = max_length;
    hashMap->size = 0;
    hashMap->a = (Pair *) malloc(sizeof(Pair) * max_length);
    return hashMap;
}
int get_index(HashMap *hashMap, pNode key)
{
    int h = hash(hashMap, key);
    int maxtime = (int)ceil(2 + 5.0 * hashMap->size / hashMap->max_length);
    while(maxtime > 0 && hashMap->a[h].key != key)
    {
        h += 1;
        if (h == hashMap->max_length)
            h = 0;
    }
    return h;
}
int hasKey(HashMap *hashMap, pNode key)
{
    int i = get_index(hashMap, key);
    return hashMap->a[i].key == key;
}
void put_into(HashMap *hashMap, Pair pair)
{
    int i = get_index(hashMap, pair.key);
    if(hashMap->a[i].key == pair.key)
    {
        hashMap->a[i].val = pair.val;
    }
    else if(hashMap->a[i].key == 0)
    {
        hashMap->a[i] = pair;
        hashMap->size += 1;
    }
    else
    {
        fprintf(stderr, "HashMap: Key conflicts %d in %d\n", hashMap->size, hashMap->max_length);
    }
}
HashMap *clear_hashmap(HashMap *hashMap)
{
    free(hashMap->a);
    free(hashMap);
    return 0;
}

#endif //GENLANG_HASHMAP_H
