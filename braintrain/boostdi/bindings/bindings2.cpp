#include <iostream>
#include "../di.hpp"

using namespace std;
namespace di = boost::di;

struct renderer {
    string device;
};

class iview {
public:
    virtual ~iview() noexcept = default;

    virtual void refresh() = 0;
};

class text_view : public iview {
public:
    void refresh() override { cout << "view-text-refreshed"; }
};

class gui_view : public iview {
public:
    gui_view(string name, renderer &renderer) : m_name{std::move(name)}, m_renderer{renderer} {
        assert("graphic_card" == renderer.device);
        assert(name.empty()); // so this is not set - later we'll see how named params are used for that
    }

    void refresh() override { cout << "view-text-refreshed"; }

    ~gui_view() override = default;

private:
    renderer &m_renderer;
    string m_name;
};

struct model {
};

class controller {
public:
    controller(model &model, iview &view) : m_model{model}, m_view{view} {}

private:
    iview &m_view; // iview must be a reference since it is an interface
    model &m_model; // I think if this is not a ref it i will be copied (will to DI in c++ where nothing is straightforward)
};

struct user {
};

class app {
public:
    app(user &user, controller &controller) : m_user{user}, m_controller{controller} {}

private:
    user &m_user;
    controller &m_controller;
};

int main() {
    auto injector = di::make_injector(
            di::bind<string>.to("graphic_card"), // the renderer device
            di::bind<iview>.to<gui_view>()
    );
    app app1 = injector.create<app>();
    return 0;
}
