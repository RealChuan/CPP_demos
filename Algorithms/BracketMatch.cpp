#include <string>
#include <vector>
#include <iostream>

std::string trimmed(std::string &str)
{
    std::string::iterator begin = str.begin();
    std::string::iterator end = str.end();

    // skip white space from end
    while (begin < end && end[-1] == ' ')
        --end;
    // skip white space from start
    while (begin < end && *begin == ' ')
        begin++;

    return std::string(begin, end);
}

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
            list.push_back(trimmed(buf));
            buf.clear();
            continue;
        }
        buf += str[i];
    }

    list.push_back(trimmed(buf));

    std::cout << "-------------------------------------------------\n";
    for(const std::string&s: list){
        std::cout << s << "\t";
    }
    std::cout << "\n";

    return list;
}

// 删除多余空客 只保留一个
std::string removeExtraSpaces(const std::string& script)
{
    std::string result;
    for(uint64_t i=0; i<script.size(); i++){
        result += script[i];
        if(script[i] == ' ' && script[i+1] == ' '){
            for(uint64_t j=i; j<script.size(); j++){
                if(script[j] != ' '){
                    i = --j;
                    break;
                }
            }
        }
    }

    std::cout << trimmed(result) << std::endl;
    return result;
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

    char buf4[] = "   He ll o world,  ni ha     o wa  ! ";
    removeExtraSpaces(buf4);

    return 0;
}
