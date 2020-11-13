#include <iostream>
#include "../di.hpp"

using namespace std;
namespace di = boost::di;

struct renderer {
    int device;
};

class view {
public:
    view(string name,renderer& renderer): m_name{std::move(name)}, m_renderer {renderer} { cout << "r.device: " << renderer.device <<endl;}
private:
    renderer m_renderer;
    string m_name;
};

auto Size = []{}; //we use noop lambda for the param name - in a real app this would have to be put in an h file and imported to using code?
struct model {
public:
    // boost:di chooses the cntr with the most args. But with cntrs having the same num of args we need to tell boost:di which cntr we want to use
    model(int height, int width) {cout << "height: " << height << ", width: " << width << endl;}
    // indicating what cntr to use is done using the macro below. Another way is to add ... as the last arg in the cntr
    // signature: model(int size, double precision, ...) {}
    BOOST_DI_INJECT(model, (named = Size) int size, double precision); // we separate the cntr declaration form definition to show that named params need apply only to the declaration
};
model::model(int size, double precision) {cout << "size: " << size << ", precision: " << precision << endl;}

class controller {
public:
    controller(model& model, view& view): m_model{model}, m_view{view} {}
private:
    view m_view;
    model m_model;
};

struct user {};

class app {
public:
    app(user& user, controller& controller): m_user{user}, m_controller{controller} {}
private:
    user m_user;
    controller m_controller;
};

int main() {
    app app1 = di::make_injector(
            di::bind<int>.to(17),
            // this will insure that the int bind in the line above is applied to the renderer.device only
            di::bind<int>.named(Size).to(13), // more syntax acrobatics
            di::bind<double>.to(23.25)
    ).create<app>();
    return 0;
}
