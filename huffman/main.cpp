#include"huffman.hpp"
#include"file.hpp"
#include"user.h"
using namespace std;
//压缩
Status Readingangcouting(ifstream& ist);
Status CreateHuffmanTree(HuffmanTree& tree);
Status CreateHuffmanCode(HuffmanTree& tree,string str);
Status show_hfmcode();
Status writehuffmantree(HuffmanTree& root,ofstream& ost);
Status write_compressfile(ifstream& ist,ofstream& ost,HuffmanTree& root);
//解压
Status ReCreateHuffman(ifstream& ist,HuffmanTree& root);
Status HuffmanDecoder(HuffmanTree& root,ifstream& ist, ofstream& ost);

int lastvaildbit=0;
int forks=0;
int codebit=1;
string input_filename;
string output_filename;
headfile headinformation;
findlist freq;
// unordered_map<char,long> freq;

pair<bool, string> getCmdOption(char **begin, char **end, const string &option) {//获取cmd信息
    char **itr = find(begin, end, option);
    if (itr != end && ++itr != end) {
        return make_pair(true, *itr);
    }
    return make_pair(false, "");
}

bool cmdOptionExists(char **begin, char **end, const string &option) {//检测cmd指令是否存在
    return find(begin, end, option) != end;
}


int main(int argc,char **argv){

//help说明 
    if (cmdOptionExists(argv, argv + argc, "-h")) {
        cout << "HUFFMAN comress program" << endl << endl;
        cout << "\e[1mUsage\e[0m" << endl;
        cout << "./main.exe\t\e[1m[OPTION]\e[0m ... \e[1m[FILE]\e[0m ..." << endl << endl;
        cout << "\e[1mOptions\e[0m" << endl;
        cout << "-h : print out help information" << endl;
        cout << "-f : the path for the input file" << endl; //-f 后应接文件路径及文件名
        cout << "-o : the path for the output file" << endl;//-o 接输出路径文件名可不带
        cout << "-c : compress the input file" << endl;     // 压缩时应指定-c(compress)
        cout << "-d : decode the input file" << endl;       //解压时应指定-d(decode)
        cout <<endl;
        return 0;
    }

//获得输入输出文件名称，输入必须有
//输出没有时在压缩时默认为com.yza,存在时为输入文件名加上.yza后缀
//在解压时不指定输出路径默认使用原文件名解压到当前文件夹，这会覆盖原文件会提示注意。

    auto input_info = getCmdOption(argv, argv + argc, "-f");
    auto output_info = getCmdOption(argv, argv + argc, "-o");
    auto cmode = getCmdOption(argv, argv + argc, "-c");
    if (input_info.first) {
        input_filename = input_info.second;
    } 
    else {
        cout<<"No input file specified"<<endl;
        return 0;
    }


    if (cmdOptionExists(argv,argv + argc, "-c")){
        if (output_info.first) {
            output_filename = output_info.second + ".yza";
        } 
        else {
            output_filename = input_filename + ".yza";
        }
    }else if(cmdOptionExists(argv, argv + argc, "-d")){
        if (output_info.first) {
            output_filename = output_info.second;
        } 
        else{
            cout<<"No output filename specified, previous file may lost."<<endl;
            system("pause");
            output_filename="";
        }
    }else 
    {
        cout<<"no function specified, please add \033[1m-c\033[1m or \033[1m-d\033[1m to command line"<<endl;
        exit(1);
    }

    if(cmode.first){
        forks = atoi(cmode.second.c_str());
        if (forks == 2); 
        else if(forks>=3&&forks<=4)codebit=2;
        else if(forks>=5&&forks<=8) codebit=3;
        else if(forks>=9&&forks<=16) codebit=4;
        else{
            cout << "overrange of forks" << endl;
            exit(ERROR);
        }
    }
    else{
        forks = 2;
    }

//压缩与解压缩**************************************************************
    ifstream ist;
    ist=fileget(input_filename);
//压缩
    if (cmdOptionExists(argv, argv + argc, "-c")) {
        int i=0;
        for(i = input_filename.size()-1; i>=0; i--) {
            if(input_filename[i]==92)break;
        }
        headinformation.filename = input_filename.substr(i+1, input_filename.length());
        headinformation.num = forks;
        ofstream ost;
        ost=fileput(output_filename);
        Readingangcouting(ist);
        HuffmanTree root;
        CreateHuffmanTree(root);
        cout<<"creating the huffmancode..."<<endl;
        CreateHuffmanCode(root,"");
        show_hfmcode();
        write_compressfile(ist,ost,root);
    }
//解压缩     
    if (cmdOptionExists(argv,argv + argc, "-d")){
        ofstream ost;
        HuffmanTree root;
        HuffmanDecoder(root,ist,ost);
    }
    return 0;
}//finished




/**************************************************************************************/
//函数定义
/***************************************************************************************/
//function for compression
Status Readingangcouting(ifstream& ist){//统计频率
    char buf;
    cout<<"reading the file..."<<endl;
    ist.seekg(0L,ios::beg);
    while(!ist.eof()){
        ist.read(&buf,sizeof(char));
        freq.ListInsert(buf);
    }
    return OK;
}

Status CreateHuffmanTree(HuffmanTree& tree){
    int addi=0; 
    int weight=0; 
    priorityquene Q;
    if(freq.empty()){cout<<"empty list"<<endl;exit(ERROR);}
    flnode p;
    for (p=freq.head->next;p;p=p->next) {
        HTNode* node=make_node(p->data,p->freq,nullptr);
        Q.push(node);
    }
    addi=(Q.size-1)%(forks-1);
    if(addi)addi = forks-1-addi;
    for(int i=0;i<addi;i++){
        HTNode* node=make_node('~',0,nullptr);
        Q.push(node);
    }
    cout<<"building the huffmantree..."<<endl;
    while(Q.size!=1){
        HTNode** child=new HTNode* [forks];
        for(int i=0;i<forks;i++){
            child[i]=Q.top();
            Q.pop();
            weight+=child[i]->weight;
        }
        HuffmanTree father=make_node('^',weight,child);
        Q.push(father);
    }
    tree=Q.top();
    return OK;
}

Status CreateHuffmanCode(HuffmanTree& tree,string str){
    if(tree==NULL)return OK;
    if(check_leaf(tree)){
        if(str==""){
            flnode p=freq.find(tree->data);
            p->code="1";
        }
        else {
            flnode p=freq.find(tree->data);
            p->code=str;
        }
    }
    else {
        for(int i=0; i<forks ;i++){
            // if(tree->child[i]->data=='~')continue;
            string temp=numtostr(i,codebit);
            CreateHuffmanCode(tree->child[i],str+temp);
        }
    }
    return OK;
}

Status show_hfmcode(){
    if(freq.empty()){cout<<"empty list"<<endl;exit(ERROR);}
    flnode p;
    printline();
    cout<<"|str\t|"<<"code\t|"<<endl;
    for(p=freq.head->next;p;p=p->next){
        cout<<"|"<<p->data<<"\t|"<<p->code<<"\t|"<<endl;
    }
    printline();
    cout<<endl;
    return OK;
}

Status writehuffmantree(HuffmanTree& tree,ofstream& ost){
    if(!tree){
        cout<<"error at writing huffmantree or no huffmantree"<<endl;
        exit(ERROR);
    }
    else{
        ost<<tree->data;
        if(tree->child[0]){
            ost<<"0";
            for(int i=0; i<forks ;i++){
                    writehuffmantree(tree->child[i],ost);
            }
        }else ost<<"1";
        // cout<<root->data;//test
        // if(root->leftchild){
        //     ost<<'0';
        //     // cout<<'0';
        //     writehuffmantree(root->leftchild,ost);
        //     writehuffmantree(root->rightchild,ost);
        // }
        // else{
        //     ost<<'1';
        //     // cout<<'1';
        // }
    }    return OK;
}

Status write_compressfile(ifstream& ist,ofstream& ost,HuffmanTree& root){
    cout<<"compressing the file..."<<endl;
    char buf;
    unsigned char temp;
    int bit=0;
    ist.clear();
    ist.seekg(0,ist.beg);
    cout<<"writing headinformation..."<<endl;
    ost<<headinformation.filename<<"\n"<<headinformation.num<<endl;//写入文件信息
    writehuffmantree(root,ost);//写入huffmantree
    ost<<endl;
    // unordered_map <char,long>::iterator it;
    // for(it=freq.begin();it!=freq.end();it++){
    //     char temp=it->first;
    //     long tempfrq=it->second;
    //     ost<<temp<<tempfrq<<endl;
    // }
    cout<<"encoding the file..."<<endl;
    while(!ist.eof()){
        ist.read(&buf,sizeof(char));
        if(ist.fail()) break;
        flnode p=freq.find(buf);
        for(int i=0;i<p->code.size();i++){
            if(p->code[i]=='0'){
                set_byte0(temp,bit);
            }
            else{
                set_byte1(temp,bit);
            }
            bit++;
            if(bit>=8){
                bit=0;
                ost.write((char*)&temp,sizeof(unsigned char));
            }
        }
    }
    if(bit){
        lastvaildbit=bit;
        headinformation.lastvaildbit=bit;
        ost.write((char*)&temp,sizeof(unsigned char));
        temp=0;
    }else {temp = 0; ost.write((char*)&temp,sizeof(unsigned char));}
    ost<<lastvaildbit;
    ist.close();
    ost.close();
    cout<<"finished compression"<<endl;
    return OK;
}

//funtion to decode
Status ReCreateHuffman(ifstream& ist,HuffmanTree& root){//重新构建huffumantree
    char data;
    char label;
    ist.read(&data,sizeof(char));
    ist.read(&label,sizeof(char));
    if(label=='1'){
        root=make_node(data,0,nullptr);
        return OK;
    }
    else{
        root=new HTNode();
        for(int i=0;i<forks ;i++){
            ReCreateHuffman(ist,root->child[i]);
        }
        return OK;
    }
    return OK;
}

Status HuffmanDecoder(HuffmanTree& tree,ifstream& ist, ofstream& ost){//解压缩
    int i=0;
    char uselesschar;
    unsigned char temp;
    // char tempwrite;
    int bit=0;
    long long filesize;
    long long cuplace=0;
    HuffmanTree p;
    string filename = output_filename;

//ist文件处理及头信息获取和处理***************************

    ist.seekg(0L,ist.end);
    filesize=ist.tellg();
    ist.clear();
    ist.seekg(0L,ist.beg);
    cout<<"Geting head information..."<<endl;
    ist.seekg(-1L,ist.end);
    ist>>headinformation.lastvaildbit;
    lastvaildbit=headinformation.lastvaildbit-48;
    ist.clear();
    ist.seekg(0L,ist.beg);
    ist>>headinformation.filename>>headinformation.num;
    forks = headinformation.num;
    if (forks == 2); 
    else if(forks>=3&&forks<=4)codebit=2;
    else if(forks>=5&&forks<=8) codebit=3;
    else if(forks>=9&&forks<=16) codebit=4;
    else{
        cout << "overrange of forks" << endl;
        exit(ERROR);
    }
    if(filename.empty())
        filename=headinformation.filename;
    else
        filename=output_filename+headinformation.filename;
    ost=fileput(filename);
    // ofstream oost=fileput("test.txt");
    // for(i=0;i<headinformation.num;i++){
    //     char tempch;
    //     long tempfrq;
    //     ist.read(&uselesschar,sizeof(char));
    //     ist.read(&tempch,sizeof(char));
    //     ist>>tempfrq;
    //     freq[tempch] = tempfrq;
    // }
    ist.read(&uselesschar,sizeof(char));
    cout<<"Rebuilding HuffmanTree..."<<endl;
    ReCreateHuffman(ist,tree);
    ist.read(&uselesschar,sizeof(char));
    cuplace=ist.tellg();
    p=tree;
// 调试
    // unordered_map <char,long>::iterator it;
    // for(it=freq.begin();it!=freq.end();it++){
    //     char tempch;
    //     long tempfrq;
    //     tempch=it->first;
    //     tempfrq=it->second;
    //     // oost<<tempch<<"\n"<<tempfrq<<endl;
    // }
    // oost.close();
    // test=freq.size();
//
    // CreateHuffmanTree(root,freq);

//读写译码*****************************************
    int place = 0;
    int getplace = 0;
    cout<<"decoding the file..."<<endl;
    string test;//to be deleted
    ist.read((char*)&temp,sizeof(unsigned char));
    while(!ist.eof()){
        if(check_leaf(p)){
            ost.write(&(p->data),sizeof(char));
            test+=p->data;
            p=tree;
            if(cuplace>=(filesize-1)&&bit>=lastvaildbit){
                break;
            }
        }
        // place = forkdir(temp,bit);
        if(check_byte(temp,bit)){
            place += pow(2,codebit-getplace-1);
        }
        getplace++;
        if(getplace == codebit){
            getplace =0;
            p=p->child[place];
            place = 0;
        }
        bit++;
        if(bit>=8){
            bit=0;
            ist.read((char*)&temp,sizeof(unsigned char));
            cuplace=ist.tellg();
        }
    }
    ist.close();
    ost.close();
    cout<<"success in decoding!\nthe output file is "<<filename<<endl;
    return OK;
}

// int forkdir(char temp,int bit){
//     int place=0;
//     for(int i=0;i<codebit;i++){
//         if(check_byte(temp,bit))place=pow(2,i);
//     }
//     return place;
// }