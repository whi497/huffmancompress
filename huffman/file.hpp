#include"user.h"

extern int byunit;

inline bool is_samech(unsigned char* a, unsigned char* b){
    bool res=1;
    for(int i=0;i<byunit ;i++){
        if(a[i]!=b[i])res=0;
    }
    return res;
}

class headfile{
public:
    string filename;
    unsigned char lastvaildbit;
    unsigned char laststraghtbit;
};

struct frequency{
    unsigned char* data;
    int freq;
    frequency(){data = new unsigned char [byunit];}
};

ifstream& open_ifile(ifstream& in,const string& name){
    in.close();
    in.clear();
    in.open(name.c_str(),ios::binary);
    return in;
}

ofstream& open_ofile(ofstream& out,const string& name){
    out.close();
    out.clear();
    out.open(name.c_str(),ios::binary);
    return out;
}

ifstream fileget(const string& filename){
    ifstream ist;
    open_ifile(ist,filename);
    if(!ist){
        cout<<"wrong at input_filename"<< endl;
        exit(ERROR);
    }
    return ist;
}

ofstream fileput(const string& filename){
    ofstream ost;
    open_ofile(ost,filename);
    if(!ost){
        cout<<"wrong at output_filename"<< endl;
        exit(ERROR);
    }
    return ost;
}

typedef class FLNode{
public:
    unsigned char* data;
    long freq;
    string code;
    FLNode* next;
    FLNode(){data = nullptr; freq =0;code="";next=nullptr;}
    FLNode(unsigned char* da, long fr,string co){data=da;freq=fr;code=co;next=NULL;}
}* flnode;

class findlist{
public:
    flnode head;
    int size;
    findlist(){head=new FLNode(nullptr,0,"");size=0;}
    bool empty(){if(!head->next)return 1;else return 0;}
    flnode find(unsigned char* elem);
    Status ListInsert(unsigned char*& elem);
};

flnode findlist::find(unsigned char* elem){
    if(empty())return NULL;
    else{
        for(flnode p=head->next;p;p=p->next){
            if(is_samech(p->data,elem)){
                return p;
            }
        }
    }
    return NULL;
}

Status findlist::ListInsert(unsigned char*& elem){
    flnode p=find(elem);
    if(!p){
        size++;
        p=new FLNode(elem,1,"");
        p->next=head->next;
        head->next=p;
    }
    else{
        p->freq++;
        free(elem);
    }
    return OK;
}
