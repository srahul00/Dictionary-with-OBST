#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class OBST
{
private:
    struct Node
    {
        string key;
        Node *left, *right;
        Node()
        {
            key = "";
            left = right = NULL;
        }
        Node(string s)
        {
            key = s;
            left = right = NULL;
        }
    };
    struct dpNode
    {
        long long cost;
        int root;
        dpNode()
        {
            cost = 0LL;
            root = 0;
        }
    };

    Node* clear(Node *root)
    {
        if(root == NULL)
            return NULL;
        Node *temp;
        temp = clear(root->left);
        temp = clear(root->right);
        delete root;
        return NULL;
    }

    Node* add(string key, Node *t)
    {
        if(t == NULL)
        {
            t = new Node(key);
        }
        else if(key < t->key)
            t->left = add(key, t->left);
        else
            t->right = add(key, t->right);
        return t; 
    }

    Node* find(string key, Node *t)
    {
        if(t == NULL)
            return NULL;
        else if(key < t->key)
            return find(key, t->left);
        else if(key > t->key)
            return find(key, t->right);
        else
            return t;
    }

    Node *root;
public:
    OBST()
    {
        root = NULL;
    }
    void createOBST(string filename)
    {
        fstream f;
        f.open("words.txt", ios::in);
        vector<string> strings;
        vector<long long> probs;
        string word;
        long double freq;
        while(!f.eof())
        {
            f>>word>>freq;
            strings.push_back(word);
            probs.push_back(int(freq*1e7));
        }
        f.close();
        createTree(strings, probs);
    }
    void createTree(const vector<string> strings, const vector<long long> probs)
    {
        int n = strings.size();
        vector<dpNode> matrix[n];
        
        for(int i = 0; i<n; i++)
            matrix[i].resize(n);

        //Lengths of size 1
        for(int i = 0; i<n; i++)
        {
            matrix[i][i].cost = probs[i];
            matrix[i][i].root = i;
        }

        // Weight of probs from i to j can be calculated in constant time
        // Storing the prefix sum of the prob values
        vector<long long> prefixsum(n+1, 0);
        for(int i = 1; i<n; i++)
            prefixsum[i] = prefixsum[i-1] + probs[i-1];

        //Considering all possible lengths from 2 to total length
        for(int len = 2; len<=n; len++)
        {
            //Choosing a subset with length len starting from 0 going on till possible values
            for(int i = 0; i<n-len+1; i++)
            {
                int j = i+len-1;
                matrix[i][j].cost = prefixsum[j+1] - prefixsum[i];
                long long minimum = __LONG_LONG_MAX__;

                //Finding the best k from [i, j]
                for(int k = i; k<=j; k++)
                {
                    long long val = 0;
                    if( k-1 >= i)
                        val += matrix[i][k-1].cost;
                    if(k+1 <= j)
                        val += matrix[k+1][j].cost;

                    if(val < minimum)
                    {
                        minimum = val;
                        matrix[i][j].cost += val;
                        matrix[i][j].root = k;
                    }
                }

            }
        }

        insertTree(strings, matrix, 0, n-1);
    }
    void insertTree(const vector<string> &strings, const vector<dpNode> table[], int i, int j)
    {   
        int index = table[i][j].root;
        root = add(strings[index], root);
        if(i <= index-1)
        {
            insertTree(strings, table, i, index-1);
        }
        if(j >= index+1)
        {
            insertTree(strings, table, index+1, j);
        }
    }
    
    void display(Node *root)
    {
        if(root == NULL)
            return;
        else 
        {
            display(root->left);
            cout<<root->key<<endl;
            display(root->right);
        }
    }
    void disp()
    {
        display(root);
    }

    ~OBST()
    {
        root = clear(root);
    }
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
    obj.createOBST("words.txt");
    obj.disp();
    return 0;
}