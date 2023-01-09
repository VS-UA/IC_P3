#ifndef H_LANG
#define H_LANG

#include <iostream>
#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <map>
#include "math.h"
#include <set>
#include "fcm.h"
#define NUM_LANGS 10
#define NUM_CHARS 122
class lang
{
    public:
    /**
     * @brief Construct a new lang object
     * 
     * @param file name of the file that will be compared with the table.
     * @param table file of the table that contains information about a language with order k.
     * @param k k value as argument
     * @param a alpha value as argument
     */
    lang(char* file, char* table, unsigned int k, double a)
    {
        lang_file = file;
        table_file = table;
        alpha = a;
        this->k = k;
    };
    lang(){};
    /**
     * @brief check the compression size, this function reads the table, the file and checks the compression size.
     * 
     * @param fname name of the file that will be compared with the table.
     * @return unsigned int 
     */
    unsigned int check_compression_size(char* fname);
    /**
     * @brief find the language of the file given to the function.
     * 
     * @param fname name of the file that will be compared with the table.
     * @return char* 
     */
    char* find_lang(char* fname);
    /**
     * @brief Set the file object
     * 
     * @param filename name of the file that will be compared with the table.
     * @return true 
     * @return false 
     */
    bool set_file(char* filename);
    /**
     * @brief Get the sequences object
     * 
     * @param fname filename
     */
    void get_sequences(char* fname);
    /**
     * @brief read the created table from the file.
     * 
     */
    void read_table();
    /**
     * @brief 
     * 
     * @param fname name of the file that will be compared with the table.
     * @param read_sequences if true the function will read the text file given as argument to the function, 
     * if false the function will the information in the file_info.
     * @return double 
     */
    double check_compression(char* fname, bool read_sequences);

    private:
        char* table_file;
        char* lang_file;
        double alpha;
        std::ifstream readfile;
        std::ifstream readtable;
        unsigned int k;
        const char* languages[10] = {"en","pt","french","spanish","german","danish","greek","italian","finnish","dutch"};
        std::map<std::string,std::map<char,unsigned int>> table;
        std::map<std::string,std::map<char,unsigned int>> file_info;
};


#endif
