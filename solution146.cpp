/**
 * System design: LRU cache (least recently used).
 *
 * cpselvis(cpselvis@gmail.com)
 * Oct 8th, 2016
 */
#include<iostream>
#include<map>
/**
LRU Cache具备的操作：
set(key,value)：如果key在hashmap中存在，则先重置对应的value值，然后获取对应的节点cur，将cur节点从链表删除，并移动到链表的头部；若果key在hashmap不存在，
则新建一个节点，并将节点放到链表的头部。当Cache存满的时候，将链表最后一个节点删除即可。
get(key)：如果key在hashmap中存在，则把对应的节点放到链表头部，并返回对应的value值；如果不存在，则返回-1。
LRU的c++实现
LRU实现采用双向链表 + Map 来进行实现。这里采用双向链表的原因是：如果采用普通的单链表，则删除节点的时候需要从表头开始遍历查找，效率为O(n)，
采用双向链表可以直接改变节点的前驱的指针指向进行删除达到O(1)的效率。使用Map来保存节点的key、value值便于能在O(logN)的时间查找元素,对应get操作。
*/
using namespace std;

/**
 * Definition of cachelist node, it's double linked list node.
 */
struct CacheNode {
  int key;
  int value;
  CacheNode *pre, *next;
  CacheNode(int k, int v) : key(k), value(v), pre(NULL), next(NULL) {}
};

class LRUCache{
private:
  int size;                     // Maximum of cachelist size.
  CacheNode *head, *tail;
  map<int, CacheNode *> mp;          // Use hashmap to store
public:
  LRUCache(int capacity)
  {
    size = capacity;
    head = NULL;
    tail = NULL;
  }

  int get(int key)
  {
    map<int, CacheNode *>::iterator it = mp.find(key);
    if (it != mp.end())
    {
      CacheNode *node = it -> second;
      remove(node);
      setHead(node);
      return node -> value;
    }
    else
    {
      return -1;
    }
  }
  /*
  set(key, value)操作需要分情况判断。如果当前的key值对应的节点已经存在，则将这个节点取出来，并且删除节点所处的原有的位置，
  并在头部插入该节点；如果节点不存在节点中，这个时候需要在链表的头部插入新节点，插入新节点可能导致容量溢出，
  如果出现溢出的情况，则需要删除链表尾部的节点。
  */
  void set(int key, int value)
  {
    map<int, CacheNode *>::iterator it = mp.find(key);
    if (it != mp.end())
    {
      CacheNode *node = it -> second;
      node -> value = value;
      remove(node);
      setHead(node);
    }
    else
    {
      CacheNode *newNode = new CacheNode(key, value);
      if (mp.size() >= size)
      {
	map<int, CacheNode *>::iterator iter = mp.find(tail -> key);
      	remove(tail);
	mp.erase(iter);
      }
      setHead(newNode);
      mp[key] = newNode;
    }
  }

  void remove(CacheNode *node)
  {
    if (node -> pre != NULL)
    {
      node -> pre -> next = node -> next;
    }
    else
    {
      head = node -> next;
    }
    if (node -> next != NULL)
    {
      node -> next -> pre = node -> pre;
    }
    else
    {
      tail = node -> pre;
    }
  }

  void setHead(CacheNode *node)
  {
    node -> next = head;
    node -> pre = NULL;

    if (head != NULL)
    {
      head -> pre = node;
    }
    head = node;
    if (tail == NULL)
    {
      tail = head;
    }
  }
};


int main(int argc, char **argv)
{
  LRUCache *lruCache = new LRUCache(2);
  lruCache -> set(2, 1);
  lruCache -> set(1, 1);
  cout << lruCache -> get(2) << endl;
  lruCache -> set(4, 1);
  cout << lruCache -> get(1) << endl;
  cout << lruCache -> get(2) << endl;
}
