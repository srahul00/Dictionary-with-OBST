#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class OBST
{

};

//Utility function to get a percentage of words (uniformly skipping rows)
//Creates a file with format: word " " probability
void getWords(string inFilename, string outFilename, float percentage = 10.0/100)
{
    string word;
    int freq;
    vector<pair<string, int>> words; //Storing the file data in a vector
    long long totalcount = 0; //Total count to get the probability value by dividing
    int i = 0;
    int counter = 1.0/percentage;

    //Reading from the in file
    //Storing 1 line every 'counter' no.of lines
    fstream f;
    f.open(inFilename, ios::in);

    while(f>>word)
    {
        f>>freq;
        i++;
        i %= counter;

        if(i % counter)
            continue;

        words.push_back({word, freq});
        totalcount += freq;
    }
    f.close();

    //Writing into the out file
    fstream g;
    g.open(outFilename, ios::out);
    for(auto p: words)
    {
        g<<p.first<<" "<< (long double) 1.0 * p.second / totalcount <<"\n";
    }
    g.close();

}

int main()
{
    //getWords("count_big.txt", "words.txt", 3.0/100);
    OBST obj = OBST();
    return 0;
}