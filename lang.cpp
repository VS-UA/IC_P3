#include "lang.h"

using namespace std;
bool lang::set_file(char* fname)
{
    lang_file = fname;
    readtable = ifstream(fname, ios::binary);
    return readtable.good();
}

void lang::read_table()
{
    char byte,letter;
    string aux_sequence;
    string number;
    int i,num;
    readtable = ifstream(table_file,  ios::binary);
    readfile = ifstream(lang_file, ios::binary);
    map<string,map<char,unsigned int>> sequence_table;
    
    if(!(readtable.good() && readfile.good())){
        cout << "Goodbit is zero" << endl;
        return;
    }
    while(1)
    {
        if(readtable.eof()){
             break;
        }
        
        aux_sequence = "";
        number = "";
        if(readtable.peek() == -1){
            std::cout << "no more info in file" << endl;
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
        num = atoi(number.data());
        sequence_table[aux_sequence][letter] = num;
    }
    this->table = sequence_table;
}
void lang::get_sequences(char* fname)
{
    int i;
    char byte;
    string aux_sequence;
    ifstream readfile = ifstream(lang_file, ios::binary);
    map<string,map<char,unsigned int>> file_table;
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
        if(byte==0x27 || byte==0x2D || byte==0x20 || (byte >= 0x41 && byte <= 0x5A) || (byte >= 0x61 && byte <= 0x7A) || (byte >= 0xC0)){
            aux_sequence = "";
            for(i=1; i<=k; i++)
            {   
                aux_sequence += letters_history[(history_pointer+i)%k];
            }
            if(file_table.find(aux_sequence) != file_table.end()){
                if(file_table[aux_sequence].find(byte) != file_table[aux_sequence].end())
                    file_table[aux_sequence][byte] += 1;
                else
                    file_table[aux_sequence][byte] = 1;
            }else{
                file_table[aux_sequence] = map<char,unsigned int>();
                file_table[aux_sequence][byte] = 1;
            }
            
            history_pointer = (history_pointer+1) % k;
            letters_history[history_pointer] = byte;
            count_seq ++;
        }
    }
    this->file_info = file_table;
}

double lang::check_compression(char* fname, bool read_sequences)
{
    if(read_sequences)
        get_sequences(fname);
    read_table();
    double global_entropy=0,pi;
    unsigned int sum_table;
    
    for(map<string,map<char,unsigned int>>::iterator string_iter = file_info.begin(); string_iter != file_info.end(); ++string_iter)
    {
        sum_table = 0;
        for(map<char,unsigned int>::iterator char_iter = table[string_iter->first].begin(); char_iter != table[string_iter->first].end(); ++char_iter)
        {
            sum_table += char_iter->second;
        }
        for(map<char,unsigned int>::iterator char_iter = file_info[string_iter->first].begin(); char_iter != file_info[string_iter->first].end(); ++char_iter)
        {
            if(table[string_iter->first].find(char_iter->first) != table[string_iter->first].end())
                pi = ((double)table[string_iter->first][char_iter->first]+alpha)/((double)sum_table+NUM_CHARS*alpha);
            else
                pi = alpha/(NUM_CHARS*alpha+(double)sum_table);
            global_entropy -= log2(pi);
        }
    }
    return global_entropy;
}

char* lang::find_lang(char*fname)
{
    char* language;
    unsigned int compression_size;
    unsigned int min_size=0xffffffff;
    string lang_name;
    string path;
    get_sequences(fname);
    for(int i=0 ; i<NUM_LANGS ; i++)
    {
        lang_name = languages[i];
        path = "./"+lang_name+"/table_"+to_string(k)+".txt";
        table_file = (char*)path.data();
        cout << table_file << endl;
        compression_size = check_compression(fname,false);
        cout << "compression: " << compression_size << endl;
        if(compression_size<min_size)
        {
            min_size = compression_size;
            language = (char*)(languages[i]);
        } 
    }
    cout << "min_size: " << min_size << endl;
    cout << "language: " << language << endl;
    return language;
}

unsigned int lang::check_compression_size(char* fname)
{
    char byte,letter;
    string aux_sequence;
    string number;
    int i,num;
    readtable = ifstream(table_file,  ios::binary);
    readfile = ifstream(lang_file, ios::binary);
    
    if(!(readtable.good() && readfile.good())){
        cout << "Goodbit is zero" << endl;
        return 0;
    }
    
    map<string,map<char,unsigned int>> sequence_table;
    while(1)
    {
        if(readtable.eof()){
             break;
        }
        
        aux_sequence = "";
        number = "";
        if(readtable.peek() == -1){
            std::cout << "no more info in file" << endl;
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
        num = atoi(number.data());
        sequence_table[aux_sequence][letter] = num;

    }

    map<string,map<char,unsigned int>> file_table;
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

        if(byte==0x27 || byte==0x2D || byte==0x20 || (byte >= 0x41 && byte <= 0x5A) || (byte >= 0x61 && byte <= 0x7A) || (byte >= 0xC0)){
            aux_sequence = "";
            for(i=1; i<=k; i++)
            {   
                aux_sequence += letters_history[(history_pointer+i)%k];
            }
            if(file_table.find(aux_sequence) != file_table.end()){
                if(file_table[aux_sequence].find(byte) != file_table[aux_sequence].end())
                    file_table[aux_sequence][byte] += 1;
                else
                    file_table[aux_sequence][byte] = 1;
            }else{
                file_table[aux_sequence] = map<char,unsigned int>();
                file_table[aux_sequence][byte] = 1;
            }
            
            history_pointer = (history_pointer+1) % k;
            letters_history[history_pointer] = byte;
            count_seq ++;
        }
    }

    double global_entropy = 0;
    unsigned int sum_table;
    double pi;
    for(map<string,map<char,unsigned int>>::iterator string_iter = file_table.begin(); string_iter != file_table.end(); ++string_iter)
    {
        sum_table = 0;
        for(map<char,unsigned int>::iterator char_iter = sequence_table[string_iter->first].begin(); char_iter != sequence_table[string_iter->first].end(); ++char_iter)
        {
            sum_table += char_iter->second;
        }
        for(map<char,unsigned int>::iterator char_iter = file_table[string_iter->first].begin(); char_iter != file_table[string_iter->first].end(); ++char_iter)
        {
            if(sequence_table[string_iter->first].find(char_iter->first) != sequence_table[string_iter->first].end())
                pi = ((double)sequence_table[string_iter->first][char_iter->first]+alpha)/((double)sum_table+NUM_CHARS*alpha);
            else
                pi = alpha/(NUM_CHARS*alpha+(double)sum_table); 
            
            global_entropy -= log2(pi);
        }
    }
    
    cout << global_entropy << endl;
    cout << count_seq << endl;
    return global_entropy;
}
int main(int argc, char *argv[])
{
    if(argc < 5){
        cout << "Usage: ./lang <filename> <language/table_k.txt> <k> <a>" << endl;
        return -1;
    }

    string fname = argv[1];
    string language = argv[2];
    unsigned int k=atoi(argv[3]);
    double alpha=stod(argv[4]);
    
    lang lang_test = lang((char*)fname.data(),(char*)language.data(),k,alpha);
    unsigned int compression_size = lang_test.check_compression_size((char*)language.data());
    cout << "compression -> " << compression_size << endl;
    return 0;
}
