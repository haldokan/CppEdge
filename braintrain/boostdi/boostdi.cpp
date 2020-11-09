#include <iostream>
#include <utility>
#include "di.hpp"

using namespace std;
namespace di = boost::di;

class ctor {
public:
    ctor(int i, string s) : i(i), s{std::move(s)} {}

    int i;
    string s;
};

struct aggregate {
    double d;
    int i;
};

class example1 {
public:
    example1(aggregate a, const ctor &c) {
        assert(30.0 == a.d);
        assert("boostdi" == c.s);
        assert(50 == c.i);
    };
};

void create_object_graph() {
    cout << "create_object_graph" << endl;
    // binding of types allows only one of each in make_injector. A type that's bound to a value applies to *all* the objects that has the type
    // for example ctor.i and aggregate.i will have the value 50 in this example
    const auto injector = di::make_injector(
            di::bind<int>.to(50),
            di::bind<double>.to(30.0),
            di::bind<string>.to("boostdi")
    );
    injector.create<example1>();
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// interface classes should not have cntr and the dstr should be virtual
struct shape {
    virtual ~shape() noexcept = default;

    [[nodiscard]] virtual string draw() const = 0;
};

class circle : public shape {
public:
    [[nodiscard]] string draw() const override { return "circle"; }
};

struct draw_kit {
    // won't compile with unique (instead of shared) ptr. Also we can't pass shape by value (boost expect a shared ptr)
    explicit draw_kit(const shared_ptr<shape> &shape) {
        assert("circle" == shape->draw());
    }
};

void bind_interfaces() {
    cout << "bind_interfaces" << endl;
    const auto injector = di::make_injector(
            di::bind<shape>.to<circle>()
    );
    injector.create<unique_ptr<draw_kit>>();
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template<typename Language = class TLanguage>
class linux_compiler {
public:
    void compile() const {
        Language::compile();
    }
};

template<typename Compiler = class TCompiler>
class compilation {
public:
    explicit compilation(const Compiler &compiler) : m_compiler{compiler} {}

    void compile() {
        m_compiler.compile();
    }

private:
    const Compiler &m_compiler;
};

struct CPP {
    string name = "cpp";
    static void compile() { cout << "cpp-compile" << endl; }
};

void bind_templates() {
    cout << "bind_templates" << endl;
    const auto injector = di::make_injector(
            di::bind<class TLanguage>.to<CPP>(),
            di::bind<class TCompiler>.to<linux_compiler>()
    );
    injector.create<compilation>().compile();
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct width {
    double width;
    // this must be the cast operator since I can do: double d = (double) width {1.2}
    constexpr explicit operator double() const {return width;}
};

struct height {
    double height;
    constexpr explicit operator double() const {return height;}
};

// passing 2 doubles to the cntr is error prone so we strong-typedef them
class Rectangle {
public:
    Rectangle(width w, height h): m_width {w}, m_height {h} {}
    [[nodiscard]] double area() const {
        return m_width * m_height;
    }
private:
    double m_width;
    double m_height;
};

void strong_typedefs() {
    Rectangle rectangle {width{5.2}, height{2.1}};
    cout << rectangle.area() << endl;
    // Rectangle rectangle2 {height{5.2}, width{2.1}}; now this will give a compile error

    // I can cast struct width to double bcz the cast operator is defined in the struct
    double d = (double) width{2.1};
    cout << d << endl;
}

int main() {
    create_object_graph();
    bind_interfaces();
    bind_templates();
    strong_typedefs(); // has nothing todo with boost::di but a nice technique described by its author and used in the framework

    return 0;
}
