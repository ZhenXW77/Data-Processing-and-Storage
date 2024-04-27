#include <string>
#include<iostream>
#include<vector>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class InMemoryDB
{
    public:
    bool start = false;
    int value;
    unordered_map<string, int> dataTracker;
    unordered_map<string, int> tempTracker;

    void begin_transaction();
    void put(string key, int value);
    int get(string key);
    void commit();
    void rollback();
};

//start a new transaction
void InMemoryDB::begin_transaction()
{
    if(start == true)
    {
        cout << "Error: A transaction is already open" << endl;
        return;
    }

    start = true;

    //set temp tracker to the main tracker
    tempTracker = dataTracker;
}

//put key-value pair into the temp tracker
void InMemoryDB::put(string key, int value)
{
    //make sure transaction is started
    if(start)
    {
        //insert the key into temp tracker so it does not effect the main tracker before commit
        tempTracker[key] = value;
    }
    else
    {
        cout << "Error: Please start a transaction" << endl;
    }
}

//get the value of the key
int InMemoryDB::get(string key)
{
    //check the main tracker to see if key exists
    if(dataTracker.find(key) != dataTracker.end())
    {
        return dataTracker[key];
    }

    return NULL;
}

//commit the changes
void InMemoryDB::commit()
{
    if(start == false)
    {
        cout << "Error: No open transaction" << endl;
        return;
    }

    start = false;

    //set main tracker to temp tracker
    dataTracker = tempTracker;
}

//abort changes in the transaction
void InMemoryDB::rollback()
{
    if(start == false)
    {
        cout << "Error: No ongoing transaction" << endl;
        return;
    }

    start = false;

    //clear temp tracker
    tempTracker.clear();
}

int main()
{
    InMemoryDB myDB = InMemoryDB();
    string input;

    //keep checking for user input
    while (input.compare("quit") != 0)
    {
        getline(cin, input);

        string delimiter = " ";
        string token;
        int pos = 0;

        vector<string> userToken;

        //split the input by the spaces
        while (input.find(delimiter) != string::npos)
        {
            pos = input.find(delimiter);
            token = input.substr(0, pos);
            input.erase(0, pos + delimiter.length());

            if(token.length() > 0)
            {
                userToken.push_back(token);
            }
        }

        if(input.length() > 0)
        {
            userToken.push_back(input);
        }
        
        if(userToken[0].compare("begin") == 0)
        {
            myDB.begin_transaction();
        }
        else if (userToken[0].compare("commit") == 0)
        {
            myDB.commit();
        }
        else if (userToken[0].compare("rollback") == 0)
        {
            myDB.rollback();
        }
        else if (userToken[0].compare("put") == 0)
        {
            if(userToken.size() != 3)
            {
                cout << "Error: Please enter a valid input" << endl;
                continue;
            }

            myDB.put(userToken[1], stoi(userToken[2]));
        }
        else if (userToken[0].compare("get") == 0)
        {
            if(userToken.size() != 2)
            {
                cout << "Error: Please enter a valid input" << endl;
                continue;
            }
            
            cout << myDB.get(userToken[1]) << endl;
        }
        else if(userToken[0].compare("quit") == 0)
        {
            continue;
        }
        else
        {
            cout << "Error: Please enter a valid input" << endl;
        }
    }

    return 0;
}



