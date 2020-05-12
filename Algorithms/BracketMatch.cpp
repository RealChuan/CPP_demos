#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> match(const std::string& script)
{
    std::vector<std::string> list;
    std::string str = script;

    int count = 0;
    std::string buf;

    for(uint64_t i=0; i<str.size(); i++){
        if(str[i] == '(')
            count++;
        else if(str[i] == ')')
            count--;
        else if(str[i] == ',' && count == 0){
            list.push_back(buf);
            buf.clear();
            continue;
        }
        buf += str[i];
    }

    list.push_back(buf);

    std::cout << "-------------------------------------------------\n";
    for(const std::string&s: list){
        std::cout << s << "\t";
    }
    std::cout << "\n";

    return list;
}

int main()
{
    char buf[] = " B(3,4), B(D(5,8),4), 6, D(A(3, 6)";

    char buf1[] = " B(D(5,8),4), C(5,7), 6, D(A(3, 6), 9)";

    char buf2[] = " A, C(5,7), 6, D(A(3, 6), 9)";

    char buf3[] = "Line(Point(100, 500), Point(300, 800)), Line(Point(800, 300), Point(100, 500))";

    match(buf);
    match(buf1);
    match(buf2);
    match(buf3);

    return 0;
}
