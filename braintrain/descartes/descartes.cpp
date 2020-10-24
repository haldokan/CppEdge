#include <iostream>
#include <string>

using namespace std;

void read_only_string() {
    // strings views are read-only: {point-to-start, size}
    string_view sv1 = "hello-"sv; // adding the sv suffix is not necessary but it helps calculating the string_view size at compile time since it is immutable
    string_view sv2 = "world"sv;

    string s3 {sv1};
    s3 += sv2; // append from sv s2; note that s3 + s2 will not work (I guess the operator+() is not implemented)
    cout << s3 << endl;

    // they can be constructed from a string with a {pointer, size} cntr
    string_view sv3 = {&s3[2], 5};
    cout << sv3 << endl;

    // iterate over sv chars (we can also do that we string)
    for (char c : sv1) {
        cout << "[" << c << "]";
    }
    cout << endl;
    // string_view is read-only
    // sv1[0] = '7'; compile error: cannot assign to return value because function 'operator[]' returns a const value

    cout << sv1[70] << endl; // accessing out of ranges indexes is undefined (here it returns a random char
    cout << s3[70] << endl; // same for normal string

    // use at() to guard against out or range access for string and string_view:
    try {
        cout << sv1.at(70) << endl;
    } catch (exception &e) {
        cout << "testing exception: " << e.what() << endl;
    }
}

void string_style_c_cpp() {
    cout << "string_style_c_cpp" << endl;

    auto s1 = "12356"; // this is c-style string: char*
    cout << s1 << endl;
    cout << s1[0] << endl;

    auto s2 = "1234"s; //c++ string
    cout << s2.substr(0, 2) << endl;

    // we can get c-style string from c++ string: there is some sort of incestuous relationship b/w these 2 languages
    auto s3 = s2.c_str();
    cout << s3[0] << endl;
}

void string_is_mutable() {
    cout << "string_is_mutable" << endl;
    string s = "modern c++";

    s[0] = toupper(s[0]);
    cout << s << endl;

    s.replace(8, 10, "pp");
    cout << s << endl;

    cout << s.substr(7, 10) << endl;
}

int main() {
    string_is_mutable();
    string_style_c_cpp();
    read_only_string();

    return 0;
}
