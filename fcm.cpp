#include "fcm.h"

using namespace std;
#define NUM_CHARS 122

double fcm::read_table(char* fname)
{
    int i;    
    char byte,letter;
    string number;
    string aux_sequence;
    map<string,map<char,unsigned int>> sequence_table;
    map<string,map<char,unsigned int>> file_info;
    ifstream readtable = ifstream((char*)filename.data(), ios::binary);

    while(1)
    {
        if(readtable.eof()){
             break;
        }
        aux_sequence = "";
        number = "";
        if(readtable.peek() == -1){
            cout << "no more info in file" << endl;
            break;
        }
        for(i=0; i<k; i++){
            readtable.read(&byte,1);
            aux_sequence += byte;
        }
        readtable.read(&byte,1);
        readtable.read(&letter,1);
        readtable.read(&byte,1);
        while(byte  != '\n'){
            readtable.read(&byte,1);
            number += byte;
            if(readtable.eof())
                break;
        }
        sequence_table[aux_sequence][letter] = atoi(number.data());
    }
    
    string path = "./"+ language + "/" +fname;
    ifstream readfile = ifstream((char*) path.data(), ios::binary);
    
    
    char* letters_history = (char*) malloc(sizeof(char)*k);
    unsigned int history_pointer=k-1;
    for(i=0; i<k; i++)
        readfile.read((char*) &letters_history[i],1);
    unsigned int count_seq=1;
    while(1)
    {
        if(readfile.eof()){
            break;
        }
        readfile.read((char*) &byte,1);
        if(byte == '\n' || byte == '\t' || byte == '\r')
            byte = 0x20;
        if(byte==0x27 || byte==0x2D || byte ==0x20 || (byte >= 0x41 && byte <= 0x5A) || (byte >= 0x61 && byte <= 0x7A) || (byte >= 0xC0)){
            aux_sequence = "";
            for(i=1; i<=k; i++)
            {   
                aux_sequence += letters_history[(history_pointer+i)%k];
            }
            
            if(sequence_table.find(aux_sequence) != sequence_table.end()){
                if(sequence_table[aux_sequence].find(byte) != sequence_table[aux_sequence].end())
                    sequence_table[aux_sequence][byte] += 1;
                else
                    sequence_table[aux_sequence][byte] = 1;
            }else{
                sequence_table[aux_sequence] = map<char,unsigned int>();
                sequence_table[aux_sequence][byte] = 1;
            }
            if(file_info.find(aux_sequence) != file_info.end()){
                if(file_info[aux_sequence].find(byte) != file_info[aux_sequence].end())
                    file_info[aux_sequence][byte] += 1;
                else
                    file_info[aux_sequence][byte] = 1;
            }else{
                file_info[aux_sequence] = map<char,unsigned int>();
                file_info[aux_sequence][byte] = 1;
            }
            history_pointer = (history_pointer+1) % k;
            letters_history[history_pointer] = byte;
            count_seq ++;
        }
    }

    readtable.close();
    readfile.close();

    cout << "count_seq: " << count_seq << endl;
    
    ofstream writetable;
    writetable.clear();
    writetable.open((char*)filename.data());
    
    double global_entropy=0,pi;
    unsigned int sum_table;

    for(map<string,map<char,unsigned int>>::iterator string_iter = sequence_table.begin(); string_iter != sequence_table.end(); ++string_iter)
    {
        sum_table = 0;
        for(map<char,unsigned int>::iterator char_iter = sequence_table[string_iter->first].begin(); char_iter != sequence_table[string_iter->first].end(); ++char_iter)
        {
            writetable << string_iter->first+ " " +char_iter->first+" " +to_string(char_iter->second) +'\n';
            sum_table += char_iter->second;
        }
        for(map<char,unsigned int>::iterator char_iter = file_info[string_iter->first].begin(); char_iter != file_info[string_iter->first].end(); ++char_iter)
        {
            if(sequence_table[string_iter->first].find(char_iter->first) != sequence_table[string_iter->first].end())
                pi = ((double)sequence_table[string_iter->first][char_iter->first]+alpha)/((double)sum_table+NUM_CHARS*alpha);
            else
                pi = alpha/(NUM_CHARS*alpha+(double)sum_table);
            global_entropy -= log2(pi);
        }
    }
    writetable.close();
    return global_entropy/(double)count_seq;
}
int main(int argc, char *argv[])
{
    if(argc < 5){
        cout << "Usage: ./fcm <filename> <language> <k> <a>" << endl;
        return -1;
    }

    string fname = argv[1];
    string language = argv[2];
    int k=atoi(argv[3]);
    int alpha=atoi(argv[4]);
    int i;
    
    fcm fcm_table = fcm(k,alpha,language);
    fcm_table.read_table((char*) fname.data());

    return 0;
}
