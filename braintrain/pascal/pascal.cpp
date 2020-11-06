#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "headers/EnglishDictionary.h"

using namespace std;

struct School {
    string name;
    int capacity = 0;

    // instead of passing name by ref (const &name), Clang-Tidy suggested passing by value and 'move'
    School(string name, int capacity) : name{move(name)}, capacity{capacity} {}

    bool operator==(const School &other) const {
        return name == other.name;
    }
};

bool operator<(const School &first, const School &second) {
    return first.capacity < second.capacity;
}

ostream &operator<<(ostream &os, const School &b) {
    return os << "{\"" << b.name << "\", " << b.capacity << "}";
}

// Dictionary is passed polymorphically to this func
void static_class_create_callee(Dictionary &dict, string word) {
    string w = std::move(word); // the move is suggested by Clang-Tidy
    cout << w << " :" << dict.lookup(w) << endl;
}

void static_class_create() {
    cout << "static_class_create" << endl;
    // unlike Java we cannot use Dictionary as the type of the variable
    EnglishDictionary dict_french = EnglishDictionary::create(Translated::French);
    cout << "health: " << dict_french.lookup("health") << endl;

    EnglishDictionary dict_german = EnglishDictionary::create(Translated::German);
    cout << "hope: " << dict_german.lookup("hope") << endl;

    static_class_create_callee(dict_french, "money");
    static_class_create_callee(dict_german, "health");
}

void stream_formatting() {
    cout << "stream_formatting" << endl;
    cout.precision(4);  // be careful, this trash is not what it looks like: it is NOT the length of the decimal part
    cout << 1232.567
         << endl; // prints 1233 (rounded) - but again depends on large the number it may print it in scientific format
    cout << 123.71356 << endl; // prints 123.7
    cout << 1234567 << endl; // precision does not change integers

    constexpr double val = 123.4567;
    //These floating-point manipulators are “sticky”; that is, their effects persist for subsequent floating-point operations
    // and this can screw you up grandly... not sure how to reset this crap
    cout << val << ", " << fixed << val << ", " << scientific << val << ", " << defaultfloat << val << ", " << hexfloat
         << val << endl;

    constexpr int val2 = 123; // has to be int for these formatters (c++ style if you provide double vals it silently fails to convert)
    cout << hex << val2 << ", " << oct << val2 << endl;

    cout.precision(4); // note that what is printed bellow is totally screwed due to the sticky ops above
    cout << 123.45658 << endl;
    cout << 123.71356 << endl;
}

void string_streams() {
    cout << "string_streams" << endl;
    ostringstream oss;
    oss << "today's temp 68F in hex is: " << hex << 68;
    cout << oss.str() << endl;

    stringstream ss; // read from & write to stream
    ss << "today's temp 68F in octal is: " << oct << 68;
    cout << ss.str() << endl;
    // read ss to a var: good luck with that... this will read only up to the first space; we have to loop to get the whole str
    string str;
    ss >> str;
    cout << str << endl; // prints: today's
}

// this template is what it looks like: if caller doesn't specify the types in the call (example convert(1.2) default to strings
template<typename source =string, typename target =string>
target convert(source arg)      // convert source to target
{
    stringstream converter;
    target result;

    if (!(converter << arg)                 // write arg into stream
        || !(converter >> result)           // read result from stream
        || !(converter >> std::ws).eof())   // stuff left in stream?
        throw runtime_error{"convert<>() failed"};

    return result;
}

void c_style_io() {
    // B.S: Many uses of this library are unsafe from a type and security point-of-view, so I don’t recommend its use
    // B.S: If you don’t use C-style I/O and care about I/O performance, call:
    ios_base::sync_with_stdio(false);     // avoid significant overhead
    // B.S: Without that call, iostreams can be significantly slowed down to be compatible with the C-style I/O.
}

void convert_caller() {
    cout << "convert_caller" << endl;
    // this will fail if we did not default the template types to string
    cout << convert(7.83) << ", " << convert<>(7.83) << endl;

    // source is double and target is deduced to be double
    cout << convert<double>(7.83) << endl;
    // source is string and target is deduced as string
    cout << convert<string>("7.83") << endl;
    cout << fixed << convert<string, double>("7.83") << endl; // converts string to double - used fixed to avoid printing it in hex
}

void file_system_path() {
    cout << "file_system_path" << endl;
    using namespace filesystem; // need this here
    const path file {"/Users/haldokanji/misc/cpp-test-file.txt"};
    assert(exists(file));

    // is_fifo checks if file is a pipe
    cout << is_block_file(file) << ", " << ", " << is_fifo(file) << ", " << is_symlink(file) << ", " << is_directory(file) << ", " << is_socket(file) << endl;
    if (is_regular_file(file)) {
        // it prints the file size rounded in kbytes so the file size shows as 0!
        cout << "regular file: [" << file << "] of size: " << file_size(file) << endl;
    }
    ofstream os { file }; // open the file using path
    os.close();
    // stem is the file name w/o the ext
    cout << "stem: " << file.stem().string() << ", file: " << file.filename().string() << ", ext: " << file.extension().string() << endl;

    // iterate over the file path parts
    for (auto p = file.begin(); p != file.end(); p++) {
        cout << *p << ", ";
    }
    cout << endl;
}

void iterate_over_directory() {
    cout << "iterate_over_directory" << endl;
    using namespace filesystem; // need this here
    path dir {"/Users/haldokanji/misc"};
    try {
        if (is_directory(dir)) {
            // we can also use the recursive_directory_iterator to traverse directories the their sub-dirs
            for (const directory_entry &entry : directory_iterator(dir)) {
                cout << entry.path() << ", " << entry.file_size() << endl;
                // this is curious: it refers to the path part of the entry (needs const to work):
                // it is actually and operator: (operator const path&) defined in directory_entry and it returns the path
                const path& path1 = entry;
                cout << path1.string() << endl;
            }
        }
    } catch (const filesystem_error &error) {
        cerr << error.what() << endl;
    }
}

void copy_files_and_dirs() {
    cout << "copy_files_and_dirs" << endl;
    using namespace filesystem; // need this here

    path file1 {"/Users/haldokanji/misc/cpp-test-file.txt"};
    path file2 {"/Users/haldokanji/misc/cpp-test-file2.txt"};

    bool removed = remove(file2); // remove file if exist (will not overwrite by default)
    cout << "removed: " << removed << endl;

    bool nominal = copy_file(file1, file2); // this will throw if failed
    cout << "nominal: " << nominal << endl; // prints 1
    remove(file2);

    // B.S: We use the error codes when operations are expected to fail frequently in normal use and the throwing
    // operations when an error is considered exceptional
    error_code err_code;
    nominal = copy_file(file1, file2, err_code); // this will NOT throw if failed, rather return and error code
    cout << "nominal: " << nominal << ", err_code: " << err_code << endl;

    // can use create_directory and create_directories; the latter creates the all dirs in the path - will not do that here

    cout << "cwd: " << current_path() << endl; // interestingly this points to where the executable are: /Users/haldokanji/CLionProjects/CppEdge/cmake-build-debug
    // current_path(some_path); this changes the cwd to some_path
}

int main() {
    static_class_create();
    stream_formatting();
    string_streams();
    convert_caller();
    file_system_path();
    iterate_over_directory();
    copy_files_and_dirs();

    return 0;
}