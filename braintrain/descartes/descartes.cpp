#include <iostream>
#include <string>
#include <utility>

using namespace std;

// strings are mutable
void mutable_string() {
    cout << "string_is_mutable" << endl;
    string s = "modern c++";

    s[0] = toupper(s[0]);
    cout << s << endl;

    s.replace(8, 10, "pp");
    cout << s << endl;

    cout << s.substr(7, 10) << endl;
}

// strings views are immutable: {pointer-to-begin, size}
void immutable_string() {
    string_view sv1 = "hello-"sv; // adding the sv suffix is not necessary but it helps calculating the string_view size at compile time since it is immutable
    string_view sv2 = "world"sv;

    string s3 {sv1};
    s3 += sv2; // append from sv s2; note that s3 + s2 will not work (I guess the operator+() is not implemented)
    cout << s3 << endl;

    // they can be constructed from a string with a {pointer, size} cntr
    string_view sv3 = {&s3[2], 5};
    cout << sv3 << endl;

    // iterate over sv chars (we can also do that with string)
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

// note this class and the testing done on it is improvisation on my part so it is not the bible especially with a complicated language like this
class village {
public:
    village(string name, int population): name{std::move(name)}, population{population} {}
    // move-cntr
    village(village&& other) noexcept: name{move(other.name)}, population{other.population} {
        cout << "called move-cntr" << endl;
        // set moved-from to sensible values
        other.name = "";
        other.population = 0;
    }

    village& operator=(village&& other) noexcept {
        cout << "called move-assignment" << endl;

        this-> name = other.name;
        this->population = other.population;

        other.name = "";
        other.population = 0;

        return *this;
    }
    [[nodiscard]] string get_name() const {return name;}
    [[nodiscard]] int get_population() const {return population;}

    void set_name(string nm) { // using 'name' gives a warning that it shadows the member field (that's not a problem in Java)
        this->name = std::move(nm);
    }

    void set_population(int pop) {
        this->population = pop;
    }
private:
    string name;
    int population;
};

ostream &operator<<(ostream &os, const village &v) {
    return os << "{\"" << v.get_name() << "\", " << v.get_population() << "}";
}

// this would create a village then copies it again when it returns it had we not defined move semantics for class village
village create_village1(string name, int population) {
    return village{std::move(name), population};
}

// this should only copy the value of the pointer at return (I think): must mean that the unique_ptr creates the village on the heap (cannot be on the func stack)
unique_ptr<village> create_village2(string name, int population) {
    auto village_ptr = make_unique<village>(village{std::move(name), population});
    cout << "v2_ptr: "<< village_ptr << endl;
    return village_ptr;
}

unique_ptr<village> create_village3(string name, int population) {
    // this is equivalent to what is done in create_village2 (Clang-Tidy urges using make_unique tho so we should not do that)
    auto village_ptr = unique_ptr<village>(new village(std::move(name), population));
    cout << "v3_ptr: "<< village_ptr << endl;
    return village_ptr;
}

void village_tester() {
    cout << ">>>>village_tester" << endl;
    village v1 = create_village1("Cold Spring", 10000);
    cout << "village1: " << v1 << endl;

    unique_ptr<village> v2 = create_village2("Peeks Kill", 4500);
    cout << "v2_ptr: " << v2 << endl; // this prints the same memory address as in create_village2
    cout << "village2: " << *v2 << endl;

    unique_ptr<village> v3 = create_village3("Haverstraw", 6000);
    cout << "v3_ptr: " << v3 << endl; // this prints the same memory address as in create_village3
    cout << "village3: " << *v3 << endl;

    //this does not compile and we get (for once) a good error message:
    //Call to implicitly-deleted copy constructor of 'village'
    //copy constructor is implicitly deleted because 'village' has a user-declared move constructor
    // village v4 = v1;

    village v4 = move(v1); // this calls the move-cntr (not the move-assignment)
    cout << "village4: " << v4 << endl;
    // trying to access v1 after it was moved can blow (it does not here!) - Clang-Tidy detects that and warn of using v1 after move
    cout << "moved-village1: " << v1 << endl;
    // so the moved-from can be used it seems (I think I read somewhere that this is expected/done in the std-lib)
    v1.set_name("Jersey City");
    v1.set_population(1000'000);
    cout << "moved-village1-after-assignments: " << v1 << endl; // this works
    cout << "village4: " << v4 << endl; // it is not affected by v1 as expected
}

void string_style_c_vs_cpp() {
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

int main() {
    mutable_string();
    string_style_c_vs_cpp();
    immutable_string();
    village_tester();

    return 0;
}
