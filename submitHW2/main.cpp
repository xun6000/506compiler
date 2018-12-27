#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include "code.h"
#include <cctype>
#include<map>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <math.h>
#include <algorithm>
using namespace std;
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include<vector>
struct student{
    string label;
    string opCode;
    vector<string> opSource;

    vector<string> opTargets;
    string arrow;
};


vector<int> leader;
vector<int> last;
int newRegister =0;
int newLabel =0;
int str2num(string s)
{
    int num;
    stringstream ss(s);
    ss>>num;
    return num;
}
string num2str(double i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}


void newvalue(vector<struct student> &hef){

    for(int i=0; i<hef.size();i++){
        //cout << "label=" << hef[i].label  << endl;
        if (hef[i].label.length()>0){
            int labelNum;
            //sscanf("123","%d",&labelNum);
            //cout << hef[i].label.substr(1,hef[i].label.length())<< endl;
            //string newbug=hef[i].label.substr(1,hef[i].label.length());
            labelNum=str2num(hef[i].label.substr(1,hef[i].label.length()));
            //sscanf(newbug,"%d",&labelNum);
            if (labelNum>newLabel){
                newLabel=labelNum;

            }



        }
        for (string r:hef[i].opSource){
            if (r.length()>0 &&r[0]=='r'){
                int registNum;
                registNum=str2num(r.substr(1,r.length()));
                newRegister=registNum>newRegister? registNum:newRegister;


            }

        }
        for (string r:hef[i].opTargets){
            if (r.length()>0 && r[0]=='r'){
                int registNum;
                registNum=str2num(r.substr(1,r.length()));
                newRegister=registNum>newRegister? registNum:newRegister;


            }

        }




    }








}
void unrolling(vector<struct student> &hef){
    newvalue(hef);
    newRegister++;
    newLabel++;
    vector<struct student> newInstructions;
    for(int i=0;i<leader.size();i++){
        int begin=leader[i];
        int end = last[i];
        if ((hef[begin].label.length()>0)&&(hef[end].opCode=="cbr")){
            string loopLabel = hef[begin].label;

            string loopTarget = hef[end].opTargets[0];
            if ((loopLabel==loopTarget)&&hef[end-2].opCode=="addI") {

                struct student newInstruction1;

                newInstruction1.label = hef[begin].label;
                newInstruction1.opCode = "nop";
                newInstructions.push_back(newInstruction1);

                struct student newInstructionL ;
                newInstructionL.opCode = "sub";
                newInstructionL.opSource.push_back(hef[end - 1].opSource[1]);
                newInstructionL.opSource.push_back(hef[end - 1].opSource[0]);
                string newString = "r" + num2str(newRegister);
                newRegister += 1;

                newInstructionL.arrow = "=>";
                newInstructionL.opTargets.push_back(newString);
                newInstructions.push_back(newInstructionL);
//
                struct student  newInstruction2;
                newInstruction2.opCode = "rshiftI";


                newInstruction2.opSource.push_back(newString);
                newInstruction2.opSource.push_back("2");
                newInstruction2.arrow = "=>";

                newInstruction2.opTargets.push_back(newString);
                newInstructions.push_back(newInstruction2);

                struct student newInstruction3 ;
                newInstruction3.opCode = "lshiftI";
                newInstruction3.opSource.push_back(newString);
                newInstruction3.opSource.push_back("2");
                newInstruction3.arrow = "=>";
                newInstruction3.opTargets.push_back(newString);
                newInstructions.push_back(newInstruction3);

                struct student newInstructionA ;
                newInstructionA.opCode = "add";
                newInstructionA.opSource.push_back(hef[end - 1].opTargets[0]);
                newInstructionA.opSource.push_back(newString);
                newInstructionA.arrow = "=>";
                string register2 = "r" + num2str(newRegister);
                newRegister += 1;
                newInstructionA.opTargets.push_back(register2);
                newInstructions.push_back(newInstructionA);


                struct student newInstruction4 ;
                newInstruction4.opCode = "cbr";
                newInstruction4.opSource.push_back(newString);
                newInstruction4.arrow = "->";
                string label1 = "L" + num2str(newLabel);
                newLabel += 1;
                newInstruction4.opTargets.push_back(label1); //label 1 is used to do the unrolling
                string label2 = "L" + num2str(newLabel);  //label 2 is used to do the rest looping
                newLabel += 1;
                newInstruction4.opTargets.push_back(label2);
                newInstructions.push_back(newInstruction4);

                struct student newInstruction5 ;
                newInstruction5.label = label1;
                newInstruction5.opCode = "nop";
                newInstructions.push_back(newInstruction5);

                for (int m = 0; m < 4; m ++) {
                    for (int j = begin + 1; j <= end - 2; j++) {
                        newInstructions.push_back(hef[j]);
                    }

                }
                struct student newInstruction6;
                if (hef[end - 1].opCode=="cmp_LE") {
                    newInstruction6.opCode = "cmp_LT";
                } else if (hef[end - 1].opCode=="cmp_LT") {
                    newInstruction6.opCode = "cmp_LE";
                }
                newInstruction6.opSource.push_back(hef[end - 1].opSource[0]);
                newInstruction6.opSource.push_back(register2);
                newInstruction6.arrow = "=>";
                newInstruction6.opTargets.push_back(hef[end - 1].opTargets[0]);
                newInstructions.push_back(newInstruction6);


                struct student newInstruction7 ;
                newInstruction7.opCode = "cbr";
                newInstruction7.opSource.push_back(hef[end - 1].opTargets[0]);
                newInstruction7.arrow = "->";
                newInstruction7.opTargets.push_back(label1); //running in the loop
                newInstruction7.opTargets.push_back(label2); //run the rest code
                newInstructions.push_back(newInstruction7);

                struct student newInstruction8 ;
                newInstruction8.label = label2;
                newInstruction8.opCode = "nop";
                newInstructions.push_back(newInstruction8);

                newInstructions.push_back(hef[end - 1]);



                struct student newInstruction9 ;
                newInstruction9.opCode = "cbr";
                newInstruction9.opSource.push_back(hef[end - 1].opTargets[0]);
                newInstruction9.arrow = "->";
                string label3 = "L" + num2str(newLabel);
                newLabel += 1;
                newInstruction9.opTargets.push_back(label3); //do the second loop
                newInstruction9.opTargets.push_back(hef[end].opTargets[1]); // do the rest of the code
                newInstructions.push_back(newInstruction9);

                //do the loop
                struct student newInstruction0 ;
                newInstruction0.label = label3;
                newInstruction0.opCode = "nop";
                newInstructions.push_back(newInstruction0);

                for (int j = begin + 1; j <= end - 2; j ++) {
                    newInstructions.push_back(hef[j]);
                }

                newInstructions.push_back(hef[end - 1]);
                newInstructions.push_back(newInstruction9);
            } else {
                for (int m = begin; m <= end; m ++) {
                    newInstructions.push_back(hef[m]);
                }
                continue;
            }

        } else {
            for (int m = begin; m <= end; m ++) {
                newInstructions.push_back(hef[m]);
            }
            continue;
        }
    }
    hef = newInstructions;
}

void LocalValueNumbering(vector<struct student> &hef,vector<int> leader, vector<int> last){
    int blockNum = leader.size();
    //cout<<"LVNSIZE"<<leader.size()<<endl;
    int value = 0;
    /*
     * Work on each block
     */
    for (int i = 0; i < blockNum; i ++) {
        int start = leader[i];
        int end = last[i];
        /*
         * operation string hash
         */
        map<string, int> lvnStorage ;
        /*
         * Single character hash
         */
        map<string, int> source;
        /*
         * used to get key from value, opposite of source
         */
        map<int,string> key2name;
        for (int j = start; j <= end; j ++) {
            if (hef[j].opSource.size() == 2) {
                if (source.count(hef[j].opSource[0])==0) {

                    source[hef[j].opSource[0]]=value;
                    value ++;
                }
                if (source.count(hef[j].opSource[1])==0) {

                    source[hef[j].opSource[1]]=value;
                    value ++;
                }
                string key = hef[j].opCode + "," + num2str(source[hef[j].opSource[0]]) + "," + num2str(source[hef[j].opSource[1]]);
                //cout<<"___________"<<key<<end;
                if (lvnStorage.count(key)==0) {
                    source[hef[j].opTargets[0]]= value;

                    key2name[value]=hef[j].opTargets[0];
                    lvnStorage[key]=value;
                    value ++;
                } else {
                    if ((source[key2name[lvnStorage[key]]]) == lvnStorage[key]) {
                        struct student newDS;
                        newDS.opCode = "i2i";
                        newDS.opSource.push_back(key2name[lvnStorage[key]]);
                        newDS.opTargets.push_back(hef[j].opTargets[0]);
                        newDS.arrow = "=>";
                        hef[j]=newDS;
                    }
                    source[hef[j].opTargets[0]]= lvnStorage[key];
                }
            }
        }
    }
}
void LVNConstant(vector<struct student> &hef){
    map<string,int> dict2;
    dict2["multI"]=1;
    dict2["divI"]=2;
    dict2["addI"]=3;
    dict2["subI"]=4;
    for (int i = 0; i < hef.size(); i ++) {
        int r0, r1;
        struct student content = hef[i];
        switch(dict2[content.opCode]) {//
            case 1:
                r1 = str2num(content.opSource[1]);
                if((r1&(r1-1)) == 0 ){
                    content.opCode = "lshiftI";
                    content.opSource[1]=num2str(int (sqrt(str2num(content.opSource[1]))));
                    hef[i]=content;
                } else if (r1 == 0) {
                    content.opCode = "loadI";
                    vector <string> sources ;
                    sources.push_back("0");
                    content.opSource = sources;
                    hef[i]=content;
                } else if (r1 == 1) {
                    content.opCode = "i2i";
                    vector<string> sources ;
                    sources.push_back(content.opSource[0]);
                    content.opSource = sources;
                    hef[i]=content;
                }
                break;
            case 2:
                r1 = str2num(content.opSource[1]);
                if((r1&(r1-1)) == 0 ){
                    content.opCode = "rshiftI";
                    content.opSource[1]=num2str(int(sqrt(str2num(content.opSource[1]))));
                    hef[i]=content;
                } else if (r1 == 1) {
                    content.opCode = "i2i";
                    vector<string> sources ;
                    sources.push_back(content.opSource[0]);
                    content.opSource = sources;
                    hef[i]=content;
                } else if (content.opSource[0]==(content.opSource[1])) {
                    content.opCode = "loadI";
                    vector<string> sources ;
                    sources.push_back("1");
                    content.opSource = sources;
            hef[i]=content;
                }
                break;
            case 3:
                r1 = str2num(content.opSource[1]);
                if(r1 == 0) {
                    content.opCode = "i2i";
                    vector<string> sources ;
                    sources.push_back(content.opSource[0]);
                    content.opSource = sources;
                    hef[i]=content;
                }
                break;
            case 4:
                r1 = str2num(content.opSource[1]);
                if(r1 == 0) {
                    content.opCode = ("i2i");
                    vector<string> sources ;
                    sources.push_back(content.opSource[0]);
                    content.opSource = sources;
                    hef[i]=content;
                } else if (content.opSource[0]==(content.opSource[1])) {
                    content.opCode = "loadI";
                vector<string> sources ;
                    sources.push_back("0");
                    content.opSource = sources;
                    hef[i]=content;
                }
                break;
            default:
                break;
        }
    }









}
void getNewFile(vector<struct student> &hef,string filename,string opt){
    ofstream stream;
    stream.open ("output"+filename+opt+".i", ios::app);

    for(int i = 0; i < hef.size(); i ++) {
        string put = "";
        struct student content = hef[i];
        if (content.label.length()>0) {
            put = put + content.label + ":	";
        } else {
            put = put + "	";
        }
        put = put + content.opCode+" ";
        if (content.opSource .size()>0) {
            for (int j = 0; j < content.opSource.size(); j ++) {
                put = put + content.opSource[j];
                if (j < content.opSource.size() - 1) {
                    put = put + ",";
                }
                put = put + " ";
            }
        }
        if (content.arrow.length()>0) {
            put = put + content.arrow +" ";
        }
        if (content.opTargets.size()>0) {
            for (int j = 0; j < content.opTargets.size(); j ++) {
                put = put + content.opTargets[j];
                if (j < content.opTargets.size() - 1) {
                    put = put + ", ";
                }
            }
        }

        stream << put << endl;
    }










}
void LVN(vector<struct student> &hef){
    LVNConstant(hef);
    LocalValueNumbering(hef,leader, last);


}









void CFG(vector<struct student> &hef){
//
    map<string,int> temp;
    vector<int> ready;




    //cout<<hef[100].opCode<<endl;
    for(int i=0; i<hef.size();i++){
        if (hef[i].label.length()>0){
            temp[hef[i].label]=i;


            //cout<<"dictis "<<hef[i].label<<"  " <<i<<endl;

        }


    }






    leader.push_back(0);
    //cout<<"heris the size"<<leader.size()<<endl;
    for (int i = 0; i<hef.size();i++){
        //cout<<(hef[i].label.length()>0)<<(find(ready.begin(), ready.end(), i) != ready.end())<<endl;

            vector<int>::iterator ret;
            ret = std::find(ready.begin(), ready.end(), i);
            if(hef[i].label.length()>0 && (ret != ready.end())){
            leader.push_back(i);


        }
        int j=i+1;
        if((hef[i].opCode=="br")){
            ready.push_back(temp[hef[i].opTargets[0]]);





        }else if(hef[i].opCode=="cbr"){
            //cout<<"_________________"<<hef[i].opTargets[1]<<"    "<<temp[hef[i].opTargets[1]]<<endl;
            //vector<int>::iterator ret;
            //ret = std::find(leader.begin(), leader.end(), temp[hef[i].opTargets[0]]);
            if ( find(leader.begin(), leader.end(), temp[hef[i].opTargets[0]])== leader.end()){
                //cout<<temp[hef[i].opTargets[0]]<<temp[hef[i].opTargets[0]]<<endl;
                ready.push_back(temp[hef[i].opTargets[0]]);

            }
            //vector<int>::iterator ret2;
            //ret2 = std::find(leader.begin(), leader.end(), temp[hef[i].opTargets[1]]);

            if (find(leader.begin(), leader.end(), temp[hef[i].opTargets[1]]) == leader.end()){
                //cout<<temp[hef[i].opTargets[0]]<<temp[hef[i].opTargets[0]]<<endl;
                ready.push_back(temp[hef[i].opTargets[1]]);

            }

        }



   }
        for (int i=0; i<leader.size();i++){
        int j =1;
        while((leader[i]+j)<hef.size()&& (find(leader.begin(), leader.end(), leader[i]+j) == leader.end())){

            j++;



        }

        last.push_back(leader[i]+j-1);
    }
   // cout<<"readysizellll"<<ready.size()<<endl;
//    for (int i = 0; i < ready.size(); i ++){
//        cout<<"readysize"<<ready[i]<<endl;
//        //cout<<"leadersize"<<leader[i]<<endl;
//
//
//        ;}
//    for (int i = 0; i < leader.size(); i ++){
//        cout<<"leadersize"<<leader[i]<<endl;
//        //cout<<"leadersize"<<leader[i]<<endl;
//
//
//        ;}



}




string trim(const string& str)
{
    size_t first = max(str.find_first_not_of(' '),str.find_first_not_of('\t'));
    if (string::npos == first)
    {
        return str;
    }
    size_t last = min(str.find_last_not_of(' '),str.find_last_not_of('\t'));
    return str.substr(first, (last - first + 1));
}
vector<string> split( string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc>=5) {
        cout << "wrong command line" << endl;
        return -1;
    }

    string runType = argv[1];



    string filename = argv[argc-1];
    ifstream infile;

    infile.open(filename);
    string line = "";
    int totallinnumber = 0;
    vector<struct student> data ;
    while (getline(infile, line)){
        totallinnumber++;
        int poscome = line.find("/");
        if (poscome!=-1){

            line =line.substr(0,poscome);
        }
        struct student stu;
        if (line.find(":")!=-1)
        {stu.label=trim(line.substr(0,line.find(":")));
            line = trim(line.substr(line.find(":")+1,line.length()));
            //std::cout << line << std::endl;
        }

        vector<string> tmp = split(line,' ');
        stu.opCode=trim(tmp[0]);
        if (line.find(">")!=-1){
            int j=1;
            while(tmp[j]!="=>"&&tmp[j]!="->"){
                if (tmp[j][tmp[j].length()-1]==','){
                    tmp[j]=tmp[j].substr(0,tmp[j].length()-1);



                }
                stu.opSource.push_back(trim(tmp[j]));
                j++;

            }
            stu.arrow=(tmp[j]=="=>"? "=>":"->");
            j++;
            while(j<tmp.size()){
                if (tmp[j][tmp[j].length()-1]==','){
                    tmp[j]=tmp[j].substr(0,tmp[j].size()-1);

                }
                stu.opTargets.push_back(trim(tmp[j]));
                j++;


            }


        }else{
            if (tmp.size()>1){
                stu.opSource.push_back(trim(tmp[1]));
                //std::cout << stu.opSource[0] << std::endl;

            }


        }

        data.push_back(stu);



    }
    if (argc==3 &&runType=="-u"){
        CFG(data);
        unrolling(data);
        //LVN(data);
//        for (int l=0;l<totallinnumber;l++) {
//            //cout<<"data of lint"<<l<< endl;
//            if (data[l].label.length() > 0) {
//                cout << "label=" << data[l].label  << endl;
//
//            }
//            if (data[l].opCode.length() >0) {
//                cout << "opCode=" << data[l].opCode << endl;
//
//            }
//
//
//            if (data[l].opSource.size() ==1) {
//                cout << "opSource=" << data[l].opSource[0]<< endl;
//
//            }
//            if (data[l].opSource.size() ==2) {
//                cout << "opSource=" << data[l].opSource[0]<< endl;
//                cout << "opSource=" << data[l].opSource[1]<< endl;
//
//            }
//            if (data[l].opTargets.size() ==1) {
//                cout << "opTargets=" << data[l].opTargets[0] << endl;
//
//            }
//            if (data[l].opTargets.size() ==2) {
//                cout << "opTargets=" << data[l].opTargets[0] << endl;
//                cout << "opTargets=" << data[l].opTargets[1] << endl;
//
//            }
//            cout<<"---------------"<< endl;
//
//        }

        cout<<"compeleted unrolling"<<endl;


    }
    else if (argc==3 &&runType=="-v"){
        CFG(data);
        LVN(data);
        cout<<"compeleted LVN"<<endl;

    }
    else if (argc==4 &&runType=="-v"){
        CFG(data);
        LVN(data);
        unrolling(data);
        cout<<"compeleted LVN and unrolling"<<endl;
        runType="-vu";

    }
    else if (argc==4 &&runType=="-u"){
        CFG(data);

        unrolling(data);
        LVN(data);
        runType="-uv";
        cout<<"compeleted unrolling and LVN"<<endl;
    }
    else {
        cout<<"we only accept -u and -v"<<endl;
        return 0;

    }

    getNewFile(data,filename.substr(0,filename.length()-2),runType);
    return 0;
}