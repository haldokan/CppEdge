#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

void for_each_with_vec() {
    pair<int, int> p = pair {2, 4};
    vector vec = {pair{2, 4}, pair{1, 3}};
    int val1 = 7;
    int val2 = 1;
    // val1 is captured by ref while val2 is captured by value
    for_each(vec.begin(), vec.end(), ([&val1, val2](pair<int, int> p) {cout << p.first + p.second + val1 + val2<< endl;}));
    cout << endl;
}

void for_each_with_boost_placeholder() {
    using namespace boost::lambda;
    typedef istream_iterator<int> in;
    // the _1 comes from boost::lambda and identifies the first arg; _2 and _3 are defined as well
    cout << "enter int or anything else exit:" << endl;
    for_each(in(cin), in(), cout << (_1 * 3) << " " );
    cout << endl;
}

int main() {
    for_each_with_boost_placeholder();
    for_each_with_vec();
}
