#include"user.h"
extern int forks;
extern int byunit;
typedef class Node{
public:
    unsigned char* data;
    int weight;
    Node** child;
    Node* next;
    Node(){data=new unsigned char [byunit];
            for(int i=0;i<byunit;i++){data[i]=0;}
            weight=0;
            child=new Node* [forks];
            for(int i=0;i<forks;i++){
            child[i]=nullptr;}
            next = nullptr;}
    Node* make_node(unsigned char* data, int weight, Node* leftchild, Node* rightchild);
}HTNode,*HuffmanTree;

// typedef char**HuffmanCode;
// typedef unordered_map<char,string> HuffmanCode;//原stl方法
    
inline Node* make_node(unsigned char* data, int weight, Node** child){
    Node* node=new Node();
    if(data!=nullptr){
        node->data= data;
    }
    node->weight = weight;
    if(child!=nullptr){
        for(int i=0; i<forks ;i++){
            node->child[i]=child[i];
        }
    }
    node->next=nullptr;
    return node;
}

bool check_leaf(HuffmanTree node){
    if(!node){
        cout << "huffmantree error: node is NULL" << endl;
        exit(0);
    }
    if(node->child[0] == nullptr)
    return 1;
    else
    return 0;
}

class priorityquene{//优先队列
public:
    HuffmanTree head;
    HuffmanTree tail;
    int size;

    priorityquene(){head=new Node();tail=new Node();size=0;}
    bool empty(){if(!head->next)return 1;else return 0;}
    HTNode* top();
    HTNode* pop();
    Status push(HTNode* node);
};

HTNode* priorityquene::top(){
    if(empty())return nullptr;
    else return head->next;
}

HTNode* priorityquene::pop(){
    HTNode* p;
    if(empty()){
        cout<<"error at popping"<< endl;
        exit(ERROR);
    }
    else{
        p=head->next;
        head->next=p->next;
        p->next=nullptr;
        size--;
    }
    return p;
}

Status priorityquene::push(HTNode* node){
    size++;
    if(empty()){
        head->next=node;
        tail->next=node;
        return OK;
    }
    else{
        HTNode* pinsert=head;
        HTNode* p=head->next;
        for(;p;p=p->next,pinsert=pinsert->next){
            if(p->weight>=node->weight){
                pinsert->next=node;
                node->next=p;
                return OK;
            }
        }
        pinsert->next=node;
    }
    return OK;
}