#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;


int a3_input(void) {
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        if (line.size () > 0)
            cout << line << endl;
    }
    return 0;
}

int main(int argc, char **argv) {
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
    vector<pid_t> kids;

    // creating pipes
    int pipe_1[2]; // Rg to Py
    int pipe_2[2]; // Py to Cpp
    // check pipe conditions
    if (pipe(pipe_1) < 0){
        cerr << "Error: pipe err with Rg to Py\n";
        return 1;
    }
    if (pipe(pipe_2) < 0){
        cerr << "Error: pipe err with Py to Cpp\n";
        return 1;
    }

    // 1. rgen
    pid_t child_1 = fork();
    if (child_1 == 0){
        dup2(pipe_1[1], STDOUT_FILENO);
        close(pipe_1[1]);

        close(pipe_2[0]);
        close(pipe_2[1]);
        close(pipe_1[0]);
        execv("./rgen", argv);
        // execl("/bin/ls", "ls", "-l", nullptr);
        perror("Error: execv rgen");
        return 1;
    } else if (child_1 < 0){
        cerr << "Error: could not fork child 1\n";
        return 1;
    }
    // push back for final kill signals
    kids.push_back(child_1);

    // 2. py
    pid_t child_2 = fork();
    if (child_2 == 0) {
        // dup the relevant
        dup2(pipe_1[0], STDIN_FILENO);
        close(pipe_1[0]);

        dup2(pipe_2[1], STDOUT_FILENO);
        close(pipe_2[1]);
        // close the irrelevant
        close(pipe_1[1]);
        close(pipe_2[0]);
        // exec py
        const char* argv[] = {"python3", "ece650-a1.py", nullptr};
        execv("/usr/bin/python3", const_cast<char* const*>(argv));
        perror("Error: execv py3 failed");
        return 1;
    } else if (child_2 < 0) {
        cerr << "Error: could not fork child 2\n";
        return 1;
    }
    kids.push_back(child_2);

    // 3.cpp
    pid_t child_3 = fork();
    if (child_3 == 0){
        dup2(pipe_2[0], STDIN_FILENO); // 可以在terminal里读取
        close(pipe_2[0]);

        close(pipe_1[0]);
        close(pipe_1[1]);
        //dup2(pipe_2[1], STDOUT_FILENO);
        close(pipe_2[1]);
        

        const char* argv[] = {"ece650-a2", nullptr};
        execv("ece650-a2", const_cast<char* const*>(argv));
        perror("Error: execv cpp failed");
        return 1;
    } else if (child_3 < 0){
        cerr << "Error: could not fork child 3\n";
        return 1;
    }
    kids.push_back(child_3);

    // main process handle input 
    dup2(pipe_2[1], STDOUT_FILENO); // 可以把a2的输出弄到terminal, ok！
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        if (line.size () > 0)
            cout << line << endl;
    }
    close(pipe_2[1]);

    /* important·*/
    // send kill signal to all children
    for (pid_t k : kids){
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}
