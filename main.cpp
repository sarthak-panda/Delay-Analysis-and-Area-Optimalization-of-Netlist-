#include <bits/stdc++.h>
#include <fstream>
using namespace std;
void splitstrtovec(string s, vector<string> &v)
{
    int start, end;
    start = end = 0;
    char dl = ' ';
    while ((start = s.find_first_not_of(dl, end)) != string::npos)
    {
        end = s.find(dl, start);
        v.push_back(s.substr(start, end - start));
    }
}
// partA
class Graph_A
{
private:
    int vertex;
    int edge;
    unordered_map<string, int> indegree;
    unordered_map<string, unordered_map<string, double>> adjlist;

public:
    Graph_A()
    {
        vertex = 0;
        edge = 0;
    }
    void addvertex(const string &signal)
    {
        if (indegree.find(signal) == indegree.end())
        {
            vertex++;
            indegree[signal] = 0;
            adjlist[signal] = unordered_map<string, double>();
        }
    }
    void addedge(const string &sig1, const string &sig2, const string &gate, const unordered_map<string, double> &m)
    {   
        if(adjlist[sig1].find(sig2)==adjlist[sig1].end()){
        indegree[sig2]++;
        edge++;
        adjlist[sig1][sig2] = m.at(gate);
        }
    }
    double longestpath()
    {
        unordered_map<string, double> lpath;
        queue<string> zerodegq;
        for (auto x : adjlist)
        {   
            if (indegree.at(x.first) == 0)
            {
                zerodegq.push(x.first);
            }
        }
        while (zerodegq.empty() == false)
        {
            string j = zerodegq.front();
            zerodegq.pop();
            indegree[j]--;
            for (auto k : adjlist[j])
            {
                indegree[k.first]--;
                lpath[k.first] = std::max(lpath[k.first], lpath[j] + adjlist[j][k.first]);
                if (indegree[k.first] == 0)
                {
                    zerodegq.push(k.first);
                }
            }
        }
        double maxm = 0;
        for (auto y : lpath)
        {
            if (y.second > maxm)
            {
                maxm = y.second;
            }
        }
        return maxm;
    }
};
void hashgengates(string g, unordered_map<string, double> &m)
{
    ifstream file(g);
    string line;
    while (!file.eof() and file.peek() != '\n' and getline(file, line))
    {
        vector<string> v; 
        splitstrtovec(line, v);
        if (m.find(v[1]) == m.end())
        {
            m[v[1]] = stod(v[2]);
        }
        else if (stod(v[2]) < m[v[1]])
        {
            m[v[1]] = stod(v[2]);
        }
    }
}
void Graph_Agenrator(const string &f, const unordered_map<string, double> &m, Graph_A &g)
{
    ifstream fl(f);
    string line;
    while (!fl.eof() and fl.peek() != '\n' and getline(fl, line))
    {
        vector<string> v;
        splitstrtovec(line, v);
        if (v[0] == "PRIMARY_INPUTS" or v[0] == "PRIMARY_OUTPUTS" or v[0] == "INTERNAL_SIGNALS")
        {
            for (int i = 1; i < v.size(); i++)
            {
                g.addvertex(v[i]);
            }
        }
        else if (v[0] != "DFF" and m.find(v[0]) != m.end())
        {
            int sz = v.size();
            for (int i = 1; i < sz - 1; i++)
            {
                g.addedge(v[i], v[sz - 1], v[0], m);
            }
        }
    }
}

// partB
class Graph
{
private:
    int vertex;
    int edge;
    unordered_map<string, int> indegree;
    unordered_map<string, unordered_map<string, double>> adjlist;

public:
    Graph()
    {
        vertex = 0;
        edge = 0;
    }
    void addvertex(const string &signal)
    {
        if (indegree.find(signal) == indegree.end())
        {
            vertex++; // to take care of fact a signal can be present both in primary inputs as well as outputs by using find implemplent on part 1 too.
            indegree[signal] = 0;
            adjlist[signal] = unordered_map<string, double>();
        }
    }
    void addedge(const string &sig1, const string &sig2, const string &gate, const unordered_map<string, vector<pair<double, double>>> &m, const vector<int> &nums, int i)
    {   
        if(adjlist[sig1].find(sig2)==adjlist[sig1].end()){
        indegree[sig2]++;
        edge++;
        adjlist[sig1][sig2] = m.at(gate)[nums[i]].second; 
        }
    }
    double longestpath()
    {
        unordered_map<string, double> lpath;
        queue<string> zerodegq;
        for (auto x : adjlist)
        {
            if (indegree.at(x.first) == 0)
            {
                zerodegq.push(x.first);
            }
        }
        while (zerodegq.empty() == false)
        {
            string j = zerodegq.front();
            zerodegq.pop();
            indegree[j]--;
            for (auto k : adjlist[j])
            {
                indegree[k.first]--;
                lpath[k.first] = std::max(lpath[k.first], lpath[j] + adjlist[j][k.first]);
                if (indegree[k.first] == 0)
                {
                    zerodegq.push(k.first);
                }
            }
        }
        double maxm = 0;
        for (auto y : lpath)
        {
            if (y.second > maxm)
            {
                maxm = y.second;
            }
        }
        return maxm;
    }
};

double delayconstraint(string h)
{
    ifstream file(h);
    string line;
    double d;
    while (!file.eof() and file.peek() != '\n' and getline(file, line))
    {
        d = stod(line);
    }
    return d;
}

void hash3gen(string g, unordered_map<string, vector<pair<double, double>>> &m)
{
    ifstream file(g);
    string line;
    while (!file.eof() and file.peek() != '\n' and getline(file, line))
    {
        vector<string> v; 
        splitstrtovec(line, v);
        if (m.find(v[1]) == m.end())
        {
            pair<double, double> n(0, 0);
            pair<double, double> p(stod(v[3]), stod(v[2])); // first we store area then we store delay
            vector<pair<double, double>> vec;
            vec.push_back(n); // to use brute force you need to comment this out
            vec.push_back(p);
            m[v[1]] = vec;
        }
        else
        {
            pair<double, double> p(stod(v[3]), stod(v[2]));
            m.at(v[1]).push_back(p);
            sort(m.at(v[1]).begin(), m.at(v[1]).end());
        }
    }
}

int gatecounter(const string &f, const unordered_map<string, vector<pair<double, double>>> &m)
{
    ifstream fl(f);
    string line;
    int len = 0;
    while (!fl.eof() and fl.peek() != '\n' and getline(fl, line))
    {
        vector<string> v;
        splitstrtovec(line, v); // aviods blank lines in betweeen
        if (v[0] != "PRIMARY_INPUTS" and v[0] != "PRIMARY_OUTPUTS" and v[0] != "INTERNAL_SIGNALS" and v[0] != "DFF" and m.find(v[0]) != m.end())
        {
            len++;
        }
    }
    return len;
}
// willl return area after genrating the graph
double graphgenrator(const string &f, const unordered_map<string, vector<pair<double, double>>> &m, Graph &g, const vector<int> &nums)
{
    ifstream fl(f);
    string line;
    int cnt = 0;
    double area = 0;
    while (!fl.eof() and fl.peek() != '\n' and getline(fl, line))
    {
        vector<string> v;
        splitstrtovec(line, v);
        if (v[0] == "PRIMARY_INPUTS" or v[0] == "PRIMARY_OUTPUTS" or v[0] == "INTERNAL_SIGNALS")
        {
            for (int i = 1; i < v.size(); i++)
            {
                g.addvertex(v[i]);
            }
        }
        else if (v[0] != "DFF" and m.find(v[0]) != m.end())
        {
            int sz = v.size();
            for (int i = 1; i < sz - 1; i++)
            {
                g.addedge(v[i], v[sz - 1], v[0], m, nums, cnt);
            }
            area += m.at(v[0])[nums[cnt]].first;
            cnt++;
        }
    }
    return area;
}

void print(const double &d, string path_print_file)
{
    ofstream out;
    out.open(path_print_file, ios::app);
    out << d << "\n";
    out.close();
}
// brute force approach helper
void resize_vector(vector<int> &vec, int new_size)
{ // stl resize adds zero at end
    int current_size = vec.size();
    if (new_size > current_size)
    {
        // Calculate the number of zeros to be added
        int num_zeros = new_size - current_size;
        // Add the necessary number of zeros at the beginning of the vector
        vec.insert(vec.begin(), num_zeros, 0);
    }
}
double brute_area_finder(string path_delay_constraint, string path_circuit,unordered_map<string, vector<pair<double, double>>> &map,int &length)
{ // before using this remember to uncomment null gates in hash3gen
    double dp, db;
    double area = INT_MAX, ar;
    dp = delayconstraint(path_delay_constraint);
    for (int j = 0; j < pow(3, length); j++)
    {
        vector<int> nums;
        int k = j;
        if (j > 0)
        {
            while (k > 0)
            {
                nums.push_back(k % 3);
                k = k / 3;
            }
        }
        else
        {
            nums.push_back(0);
        }
        reverse(nums.begin(), nums.end());
        resize_vector(nums, length);
        for (int i = 0; i < nums.size(); i++)
        {
            nums[i] = nums[i] + 1;
        }
        Graph gh;
        ar = graphgenrator(path_circuit, map, gh, nums);
        db = gh.longestpath();
        if (db <= dp and ar < area)
        {
            area = ar;
        }
    }
    return area;
}
// some optimization to reach a nearer answer not exact using dynamic programming concept
double optimal1_area_finder(string path_delay_constraint, string path_circuit,unordered_map<string, vector<pair<double, double>>> &map,int &length)
{
    double dc, delay;
    double ar;
    string flag="";
    dc = delayconstraint(path_delay_constraint);
    vector<int> nums(length, 0);
    int i = 0, j;
    while (i < length)
    {
        if (i < 0)
        {
            break;
        }
        if (nums[i] == 0)
            j = 3;
        while (j >= 1)
        {
            if (j == 4)
            {
                nums[i]=0;
                i--;
                j = nums[i] + 1;
                flag="back_tracking";
                break;
            }
            nums[i] = j;
            Graph gh;
            ar = graphgenrator(path_circuit, map, gh, nums);
            delay = gh.longestpath();
            // graphs are on stack no need to delete
            if (delay > dc and j == 3)
            {
                nums[i]=0;
                i--;
                j = nums[i] + 1;
                flag="back_tracking";
                break;
            }
            else if (delay > dc and j < 3)
            {
                j++;
                nums[i] = nums[i] + 1; // j increment optional
                i++;
                break;
            }
            else if (flag!="back_tracking" and delay <= dc and j > 1)
            {
                j--;
            }
            else if (delay <= dc and (j == 1 or flag=="back_tracking"))
            {
                flag="off";
                i++;
                break; // equivalenty j--
            }
        }
    }
    Graph gh;
    ar = graphgenrator(path_circuit, map, gh, nums);
    delay = gh.longestpath();
    if (delay > dc)
    {
        ar = INT_MAX;
    }
    return ar;
}
double optimal2_area_finder(string path_delay_constraint, string path_circuit,unordered_map<string, vector<pair<double, double>>> &map,int &length)
{
    double dc, delay;
    double area, ar = INT_MAX;
    string flag="";
    dc = delayconstraint(path_delay_constraint);
    vector<int> nums(length);
    for (int k = 0; k < length; k++)
    {   
        for (int u = 0; u < length; u++)
        {
            nums[u] = 0;
        }
        int i = k, j, count = 0;
        while (count < length)
        {  
            if (count < 0)
            {
                break;
            }
            if (nums[i] == 0)
                j = 3;
            while (j >= 1)
            {   
                if (j == 4)
                {   
                    nums[i]=0;
                    i = ((i - 1) % length + length) % length;
                    j = nums[i] + 1;
                    count--;
                    flag="back_tracking";
                    break;//flag is backtracking
                }
                nums[i] = j;
                Graph gh;
                area = graphgenrator(path_circuit, map, gh, nums);
                delay = gh.longestpath();
                // graphs are on stack no need to delete
                if (delay > dc and j == 3)
                {   
                    nums[i]=0;
                    i = ((i - 1) % length + length) % length;
                    j = nums[i] + 1;
                    count--;
                    flag="back_tracking";
                    break;
                }
                else if (delay > dc and j < 3)//not possible during backtracking
                {
                    j++;
                    nums[i] = nums[i] + 1; // j increment optional
                    i = (i + 1) % length;
                    count++;
                    break;
                }
                else if (flag!="back_tracking" and delay <= dc and j > 1)
                {
                    j--;
                }
                else if (delay <= dc and (j == 1 or flag=="back_tracking"))
                {
                    flag="off";
                    i = (i + 1) % length;
                    count++;
                    break; // equivalenty j--
                }
            }
        }
        Graph gh;
        area = graphgenrator(path_circuit, map, gh, nums);
        delay = gh.longestpath();
        if (delay > dc)
        {
            ar = INT_MAX;
            return ar;
        }
        else if (area < ar)
        {
            ar = area;
        }
    }
    return ar;
}
int main(int argc, char *argv[])
{
    if (*argv[1] == 'A')
    {
        string path_circuit = argv[2];
        string path_gate = argv[3];
        string path_delay = argv[4];
        unordered_map<string, double> map;
        Graph_A gh;
        double db;
        hashgengates(path_gate, map);
        Graph_Agenrator(path_circuit, map, gh);
        db = gh.longestpath();
        print(db, path_delay);
    }
    else if (*argv[1] == 'B')
    {
        string path_circuit = argv[2];
        string path_gate = argv[3];
        string path_delay_constraint = argv[4];
        string path_minimum_area = argv[5];
        double area;
        unordered_map<string, vector<pair<double, double>>> map;
        hash3gen(path_gate, map);
        int length = gatecounter(path_circuit, map);
        if(length<=12){
            area=brute_area_finder(path_delay_constraint, path_circuit,map,length);
        }
        else{
            area=optimal1_area_finder(path_delay_constraint, path_circuit,map,length);
        }
        print(area, path_minimum_area);
    }
    return 0;
}
