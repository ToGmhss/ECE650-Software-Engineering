#include <iostream>
#include <string>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <unistd.h>

#include <iomanip>
#include <sstream>

using namespace std;


/*
converted from a1 py codes
*/
std::string pp(double x)
{
    // Returns a pretty-print string representation of a number.
    // A float number is represented by an integer, if it is whole,
    // and up to two decimal places if it isn't
    if (std::abs(x - static_cast<int>(x)) < 0.0001)
    {
        return std::to_string(static_cast<int>(x));
    }
    else
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << x;
        return stream.str();
    }
}

class Point
{
    // A point in a two dimensional space
public:
    double x, y;

    Point(double x_val, double y_val) : x(x_val), y(y_val) {}

    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        return os << '(' << pp(p.x) << ',' << pp(p.y) << ')';
    }

    bool operator==(const Point &other) const
    {
        return std::abs(x - other.x) < 0.0001 && std::abs(y - other.y) < 0.0001;
    }

    size_t hash() const
    {
        return std::hash<double>()(x) ^ std::hash<double>()(y);
    }
};

class Line
{
    // A line between two points
public:
    Point src, dst;

    Line(const Point &src_point, const Point &dst_point) : src(src_point), dst(dst_point) {}

    friend std::ostream &operator<<(std::ostream &os, const Line &l)
    {
        return os << '[' << l.src << "-->" << l.dst << ']';
    }
};

/*
 ------  ------ 
    Ignore in our design context for simplicity
        else if (l1.dst == l2.src)  --- to check ------  ------
*/
bool intersect(const Line &l1, const Line &l2)
{
    // Returns whether intersection exists
    double x1 = l1.src.x, y1 = l1.src.y;
    double x2 = l1.dst.x, y2 = l1.dst.y;

    double x3 = l2.src.x, y3 = l2.src.y;
    double x4 = l2.dst.x, y4 = l2.dst.y;

    // 1. for the same coords
    // They can rarely happen
    if (l1.dst == l2.dst || l1.src == l2.src || l1.src == l2.dst)
    {
        return true;
    }
    // Ignore in our design context for simplicity
    else if (l1.dst == l2.src) {
        return false;
    }

    double xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    double xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    double ynum = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4));
    double yden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    if (std::abs(xden) < 0.0001 || std::abs(yden) < 0.0001)
    {
        // Parallel or coincident
        
        // 2. real intersects
        // 2.1 easy cases: x==x or y==y
        if (x1 == x2 and x3 == x4) {
            if (x1 == x3) {
                // Overlap in the same line
                bool cond1 = min(y1, y2) <= y3 <= max(y1, y2);
                bool cond2 = min(y1, y2) <= y4 <= max(y1, y2);
                if (cond1 or cond2) {
                    return true;
                }
            }
        }
        else if (y1 == y2 and y3 == y4) {
            if (y1 == y3) {
                bool cond1 = min(x1, x2) <= x3 <= max(x1, x2);
                bool cond2 = min(x1, x2) <= x4 <= max(x1, x2);
                if (cond1 or cond2) {
                    return true;
                }
            }
        }
        // 2.2 hard cases
        // k1 == k2 not 0;
        float b1 = y1 - (y2-y1)/(x2-x1)*x1;
        float b2 = y3 - (y4-y3)/(x4-x3)*x3;
        // 2.2.1 only parallel
        if (b1 != b2) { 
            return false;
        }
        // 2.2.2 same line
        else{
            bool cond1 = min(x1, x2) <= x3 <= max(x1, x2);
            bool cond2 = min(x1, x2) <= x4 <= max(x1, x2);
            // 2.2.2.a overlap
            if (cond1 or cond2) {
                return true;
            }
            // 2.2.2.b non-overlap
            else{
                return false;
            }
        }
        return false; 
    }

    double xcoor = xnum / xden;
    double ycoor = ynum / yden;

    bool line1_x_cond = std::min(x1, x2) <= xcoor && xcoor <= std::max(x1, x2);
    bool line1_y_cond = std::min(y1, y2) <= ycoor && ycoor <= std::max(y1, y2);
    bool line2_x_cond = std::min(x3, x4) <= xcoor && xcoor <= std::max(x3, x4);
    bool line2_y_cond = std::min(y3, y4) <= ycoor && ycoor <= std::max(y3, y4);

    if (line1_x_cond && line1_y_cond && line2_x_cond && line2_y_cond)
    {
        return true;
    }

    return false; // Or any other invalid point
}

bool overlap(const Line &l1, const Line &l2){
    double x1 = l1.src.x, y1 = l1.src.y;
    double x2 = l1.dst.x, y2 = l1.dst.y;

    double x3 = l2.src.x, y3 = l2.src.y;
    double x4 = l2.dst.x, y4 = l2.dst.y;

    // 1. for the same coords
    // They can rarely happen --// Ignore in our design context for simplicity
    if (l1.dst == l2.dst || l1.src == l2.src || l1.src == l2.dst)
    {
        return false; // diff than the mother function
    }
    // Ignore in our design context for simplicity
    else if (l1.dst == l2.src) {
        return false;
    }

    //double xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    double xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    //double ynum = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4));
    double yden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    if (std::abs(xden) < 0.0001 || std::abs(yden) < 0.0001)
    {
        // Parallel or coincident
        
        // 2. real intersects
        // 2.1 easy cases: x==x or y==y
        if (x1 == x2 and x3 == x4) {
            if (x1 == x3) {
                // Overlap in the same line
                bool cond1 = min(y1, y2) <= y3 <= max(y1, y2);
                bool cond2 = min(y1, y2) <= y4 <= max(y1, y2);
                if (cond1 or cond2) {
                    return true;
                }
            }
        }
        else if (y1 == y2 and y3 == y4) {
            if (y1 == y3) {
                bool cond1 = min(x1, x2) <= x3 <= max(x1, x2);
                bool cond2 = min(x1, x2) <= x4 <= max(x1, x2);
                if (cond1 or cond2) {
                    return true;
                }
            }
        }
        // 2.2 hard cases
        // k1 == k2 not 0;
        float b1 = y1 - (y2-y1)/(x2-x1)*x1;
        float b2 = y3 - (y4-y3)/(x4-x3)*x3;
        // 2.2.1 only parallel
        if (b1 != b2) { 
            return false;
        }
        // 2.2.2 same line
        else{
            bool cond1 = min(x1, x2) <= x3 <= max(x1, x2);
            bool cond2 = min(x1, x2) <= x4 <= max(x1, x2);
            // 2.2.2.a overlap
            if (cond1 or cond2) {
                return true;
            }
            // 2.2.2.b non-overlap
            else{
                return false;
            }
        }
        return false; 
    }

    return false;
}

int GenNum(int n_min, int n_max)
{
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    // read a random unsigned int
    unsigned int num = 42;
    urandom.read((char *)&num, sizeof(unsigned int));
    urandom.close();
    int range = n_max - n_min + 1;
    // cout << "range is:" << range << "max: " << n_max << "min:" << n_min << endl;
    int res = num % range + n_min;
    // cout << "max & min:" << max << min << endl;
    // cout << res << endl;
    return res;
}

// string GenStName() {}

/*
-c k — where k is an integer ≥ 1. Your process should generate (x, y) coordinates such that
every x and y value is in the range [−k, k]. For example, if k = 15,
all of your coordinate values
should be integers between −15 and 15. Default: k = 20.
*/
class Graph
{
private:
    int s, n, c;
    vector<vector<pair<int, int>>> st;
    vector<string> st_names;

public:
    // Graph(int s, int n, int c) {}
    // bug fix using "member initialization list"
    Graph(int s, int n, int c) : s(s), n(n), c(c) {}
    
    bool StIsValid(vector<pair<int, int>> street)
    {
        // bug fix: initial st should be non valid
        if (street.size() == 0) {
            return false;
        }
        for (int i = 0; i < street.size() - 1; i++)
        {
            for (int j = i + 1; j < street.size(); j++)
            {
                // 1. Two pts w same coord (len==0 & self-loop)
                pair<int, int> pt1, pt2;
                pt1 = street[i];
                pt2 = street[j];
                bool cond1 = pt1.first == pt2.first;
                bool cond2 = pt1.second == pt2.second;
                if (cond1 and cond2)
                {
                    //cout << "same coord" << endl;
                    return false;
                }
                // 2. whether intersect
                if ((i + 1 < street.size()) and j + 1 < street.size())
                {
                    Point point1 = Point(pt1.first, pt1.second);
                    Point point2 = Point(pt2.first, pt2.second);
                    Point point3 = Point(street[i + 1].first, street[i + 1].second);
                    Point point4 = Point(street[j + 1].first, street[j + 1].second);
                    Line line1 = Line(point1, point3);
                    Line line2 = Line(point2, point4);
                    if (intersect(line1, line2))
                    {                   
                        //cout << "intersect between " << line1 << " and " << line2 << endl;
                        return false;
                    }
                }
                // further checks
            }
        }
        // bug fix: check st intersections across streets
        for (auto &st_pt : st){
            for (int i = 0; i < st_pt.size()-1; i++){
                pair<int, int> pt1, pt2;
                pt1 = st_pt[i];
                pt2 = st_pt[i+1];
                Point point1 = Point(pt1.first, pt1.second);
                Point point2 = Point(pt2.first, pt2.second);
                Line line1 = Line(point1, point2);
                for (int j = 0; j < street.size(); j++) {
                    pt1 = street[j];
                    pt2 = street[j+1];
                    point1 = Point(pt1.first, pt1.second);
                    point2 = Point(pt2.first, pt2.second);
                    Line line2 = Line(point1, point2);
                    if (overlap(line1, line2))
                    {                   
                        //cout << "overlap between " << line1 << " and " << line2 << endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }
    

    void GenStNames(int st_num)
    {
        for (int i = 1; i <= st_num; i++)
        {
            string cur_name = "Street" + std::to_string(i);
            st_names.push_back(cur_name);
        }
    }

    // genearate st only
    vector<pair<int, int>> GenSt()
    {
        vector<pair<int, int>> street;
        const int attempt_limit = 25;
        int num_seg = GenNum(1, n); // num_cor = num_seg + 1
        // ------ ----- to ------- del ------------------
        //cout << "cur num_seg is: " << num_seg << endl;
        int attempt = 1;
        // Try to gen valid st within specified no.attempts
        while (!StIsValid(street))
        {
            street.clear();
            for (int i = 0; i < num_seg + 1; i++)
            {
                pair<int, int> pt;
                pt.first = GenNum(-c, c);
                pt.second = GenNum(-c, c);
                //cout << "generating coord num: " << i << " " << pt.first << " " << pt.second << endl;
                street.push_back(pt);
            }
            // Err if it's 25
            attempt++;
            if (attempt > attempt_limit)
            {
                //exit(1);
                street.clear();
                cerr << "Error: fail to generate valid input for " << attempt_limit << " simultaneous attempts"  << endl;//<< street.size()
                return street;
            }
        }
        // ------ ----- to ------- del ------------------
        
        return street;
    }

    int GenGraph()
    {
        int num_st = GenNum(2, s);
        //cout << "num of st is: " << num_st << endl;
        GenStNames(num_st);
        //cout << "gen st names done" << endl;
        // gen segs through coords
        // check valid -- 1. no overlap bt segs; 2. no 0-len;


        //-------------------------- to del --------------------------
        for (int i = 0; i < num_st; i++)
        {
            //cout << "gen st " << i << endl;
            vector<pair<int, int>> new_st = GenSt();
            // if empty, indicates err
            if (new_st.size() == 0) {
                return -1;
            }
            st.push_back(new_st);
        }
        return 0;
    }

    void rm()
    {
        if (st.size() == 0)
        {
            return;
        }
        for (int i = 0; i < st.size(); i++)
        {
            cout << "rm \"" << st_names[i] << "\"" << endl;
        }
        st.clear();
        st_names.clear();
    }

    void print()
    {
        string str;
        for (int i = 0; i < st.size(); i++)
        {
            str = "";
            str += "add \"" + st_names[i] + "\" ";
            for (auto &p : st[i])
            {
                // bug fix for a1-py input format
                // ----------------  ---- to del -----------------
                //cout << st[i] << endl;
                str += "(" + to_string(p.first) + "," + to_string(p.second) + ") ";
            }
            cout << str << endl;
        }
        cout << "gg" << endl;
    }
};

int main(int argc, char **argv)
{
    string s_str, n_str, l_str, c_str;
    int s_int = 10, n_int = 5, l_int = 5, c_int = 20; // l = 5
    int option;
    while ((option = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        switch (option)
        {
        case 's':
            s_str = optarg;
            s_int = atoi(s_str.c_str());
            if (s_int < 2)
            {
                cerr << "Error: -s k — where k is an integer ≥ 2" << endl;
                return -1;                
            }
            break;
        case 'n':
            n_str = optarg;
            n_int = atoi(n_str.c_str());
            if (n_int < 1)
            {
                cerr << "Error: -n k — where k is an integer ≥ 1" << endl;
                return -1;
            }
            break;
        case 'l':
            l_str = optarg;
            l_int = atoi(l_str.c_str());
            if (l_int < 5)
            {
                cerr << "Error: -l k — where k is an integer ≥ 5" << endl;
                return -1;
            }
            break;
        case 'c':
            c_str = optarg;
            c_int = atoi(c_str.c_str());
            if (c_int < 1)
            {
                cerr << "Error: -c k — where k is an integer ≥ 1" << endl;
                return -1;
            }
            break;
        case '?':
            cerr << "Error: unknown option: " << optopt << endl;
            return -1;
            break;
        default:
            break;
        }
    }
    //
    // to del -----
    //cout << "start g" << endl;
    //cout << s_int << n_int << c_int << endl;
    Graph g = Graph(s_int, n_int, c_int);
    while (true)
    {
        g.rm();
        g = Graph(s_int, n_int, c_int);
        int g_status = g.GenGraph();
        if (g_status == -1){
            //Terminated after failing to generate valid input affor 25 attempts
            //cerr << "so far so good" << endl;
            exit(1);
        }
        g.print();
        sleep(GenNum(5, l_int)); // should be 5
    }
    return 0;
}