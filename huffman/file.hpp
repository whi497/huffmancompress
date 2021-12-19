#include"user.h"

class headfile{
public:
    string filename;
    unsigned char lastvaildbit;
    int num;
};

struct frequency{
    char data;
    int freq;
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
    char data;
    long freq;
    string code;
    FLNode* next;
    FLNode(char da, long fr,string co){data=da;freq=fr;code=co;next=NULL;}
}* flnode;

class findlist{
public:
    flnode head;
    int size;
    findlist(){head=new FLNode('\0',0,"");size=0;}
    bool empty(){if(!head->next)return 1;else return 0;}
    flnode find(char elem);
    Status ListInsert(char elem);
};

flnode findlist::find(char elem){
    if(empty())return NULL;
    else{
        for(flnode p=head->next;p;p=p->next){
            if(p->data==elem){
                return p;
            }
        }
    }
    return NULL;
}

Status findlist::ListInsert(char elem){
    flnode p=find(elem);
    if(!p){
        size++;
        p=new FLNode(elem,1,"");
        p->next=head->next;
        head->next=p;
    }
    else{
        p->freq++;
    }
    return OK;
}