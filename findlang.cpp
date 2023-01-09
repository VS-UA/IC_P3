#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#define NUM_CHARS 122

using namespace std;

class fcm
{
private:
    int k; // order of the model
    double alpha; // smoothing parameter
    map<string, map<char, int>> sequence_counts; // counts of each sequence and its following character
    map<string, int> sequence_totals; // totals for each sequence

public:
    fcm(int k, double alpha)
    {
        this->k = k;
        this->alpha = alpha;
    }

    // function to read a text file and build the finite-context model
    void read_file(string filename)
    {
        char c;
        string sequence = "";

        // open the text file
        ifstream text_file(filename);

        // read the first k characters of the file
        for (int i = 0; i < k; i++)
        {
            text_file.get(c);
            sequence += c;
        }

        // read the rest of the file character by character
        while (text_file.get(c))
        {
            // update the counts for this sequence and the following character
            if (sequence_counts.find(sequence) == sequence_counts.end())
            {
                // this is a new sequence, add it to the map
                sequence_counts[sequence] = map<char, int>();
            }
            if (sequence_counts[sequence].find(c) == sequence_counts[sequence].end())
            {
                // this is a new character following this sequence, add it to the map
                sequence_counts[sequence][c] = 1;
            }
            else
            {
                // increment the count for this character following this sequence
                sequence_counts[sequence][c]++;
            }

            // update the total count for this sequence
            if (sequence_totals.find(sequence) == sequence_totals.end())
            {
                // this is a new sequence, add it to the map
                sequence_totals[sequence] = 1;
            }
            else
            {
                // increment the total count for this sequence
                sequence_totals[sequence]++;
            }

            // update the sequence for the next iteration
            sequence = sequence.substr(1) + c;
        }
    }

    // function to calculate the compressed size of a text file using the finite-context model
    double calc_compressed_size(string filename)
    {
        double size = 0;

        // read the text file character by character
        char c;
        string sequence = "";
        ifstream text_file(filename);
        for (int i = 0; i < k; i++)
        {
            text_file.get(c);
            sequence += c;
        }
        while (text_file.get(c))
        {
            // calculate the probability of the following character given the sequence
            double p = (sequence_counts[sequence][c] + alpha) / (sequence_totals[sequence] + NUM_CHARS*alpha);

            // calculate the size of this sequence and the following character using the formula:
            size += -log2(p);
	    
	    // update the total count for this sequence
            if (sequence_totals.find(sequence) == sequence_totals.end())
            {
                // this is a new sequence, add it to the map
                sequence_totals[sequence] = 1;
            }
            else
            {
                // increment the total count for this sequence
                sequence_totals[sequence]++;
            }

            // update the sequence for the next iteration
            sequence = sequence.substr(1) + c;
        }

        return size;
    }
};

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        cerr << "Usage: findlang t ri_dir k alpha" << endl;
        return 1;
    }

    string t_file = argv[1];
    string ri_dir = argv[2];
    int k = atoi(argv[3]);
    double alpha = atof(argv[4]);

    // create a map to store the compressed sizes for each language
    map<string, double> language_sizes;

    // read all the files in the ri directory
    vector<string> ri_files;
    ifstream dir_listing(ri_dir);
    string line;
    while (getline(dir_listing, line))
    {
        ri_files.push_back(line);
    }

    // for each language, build the model and calculate the compressed size of t
    for (string ri_file : ri_files)
    {
        // extract the language from the filename
        int pos = ri_file.find(".");
        string language = ri_file.substr(0, pos);

        // build the model for this language
        fcm model(k, alpha);
        model.read_file(ri_dir + "/" + ri_file);

        // calculate the compressed size of t using the model
        double size = model.calc_compressed_size(t_file);

        // store the compressed size for this language
        language_sizes[language] = size;
    }

    // find the language with the smallest compressed size
    string smallest_language;
    double smallest_size = 1e10; // a large number
    for (auto const& x : language_sizes)
    {
        if (x.second < smallest_size)
        {
            smallest_language = x.first;
            smallest_size = x.second;
        }
    }
    
    cout << "Language of t: " << smallest_language << endl;

    return 0;
}


