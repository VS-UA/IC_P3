#ifndef H_FCM
#define H_FCM

#include <iostream>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <map>
#include "math.h"
#include <set>

class fcm
{
    public:
    /**
     * @brief Construct a new fcm object
     * 
     * @param k order that will be used in the construction of the model.
     * @param alpha alpha value.
     * @param language name of the language (one of the language directories).
     */
    fcm(unsigned int k, double alpha, std::string language)
    {
        this->k = k;
        this->alpha = alpha;
        filename = "./"+ language + "/table_"+std::to_string(k)+".txt";
        this->language = language;
        std::ofstream writetable;
        writetable = std::ofstream((char*)filename.data(), std::ios::binary|std::ios_base::app);
        writetable.close();
    };
    fcm(){};
    /**
     * @brief read information about a existing table of a language and order K.
     * 
     * @param fname name of the file that contains the table about a language.
     * @returns entropy;
     */
    double read_table(char* fname);

    private:
        std::string filename;
        unsigned int k;
        double alpha;
        std::string language;
};


#endif
