#ifndef CPP_VECTOR_H
#define CPP_VECTOR_H
#include <initializer_list>

using namespace std;

namespace N {
    enum class Category {
        small, medium, large
    };

    inline Category &operator++(Category &c) {
        switch (c) {
            case Category::small:
                return c = Category::medium;
            case Category::medium:
                return c = Category::large;
            case Category::large:
                return c = Category::small;
        }
    }

    class Vector {

    private:
        int length;
        double *items;
        Category category;
    public:
        explicit Vector(int); // no implicit conversion from int to vector

        Vector(int, Category);

        Vector(initializer_list<double>);

        Vector();

        ~Vector();

        // const signifies that this func doesn't modify its object. It can be called by const and non-const objects
        // however non-const funcs cannot be called by const objects (talk about simplicity!)
        [[nodiscard]] int get_size() const;

        double &operator[](int);

        // should never throw and exception but if it does the program will terminate by calling std::terminate()
        const Category get_category() noexcept;
    };
}

#endif //CPP_VECTOR_H
