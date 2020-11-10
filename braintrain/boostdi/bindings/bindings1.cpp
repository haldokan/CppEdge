#include "../di.hpp"

using namespace std;
namespace di = boost::di;

struct renderer {
    string device;
};

class view {
public:
    view(string name,renderer& renderer): m_name{std::move(name)}, m_renderer {renderer} {}
private:
    renderer m_renderer;
    string m_name;
};

struct model {};

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
    // just like that! it creates the dependency graph
    app app1 = di::make_injector().create<app>();
    return 0;
}
