#include <iostream>
#include "../di.hpp"

using namespace std;
namespace di = boost::di;

// If a project has hundreds of interfaces and one would like to split the bindings into separate components,
// injectors can be split for different hierarchies then composed into one injector

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% card hierarchy %%%%%%%%%%%%%%%%%%%%%%%%

class icard {
public:
    virtual ~icard() = default;

    virtual double charge(double) = 0; // charge amt and return balance
};

class credit_card : public icard {
public:
    double charge(double amt) override {return 0;}
};

class debit_card : public icard {
public:
    double charge(double amt) override {return 0;}
};

class cards {
public:
    explicit cards(vector<shared_ptr<icard>> cards) {}
};

auto cards_injector() {
    cout << "cards_injector" << endl;
    return di::make_injector(
            di::bind<icard *[]>().to<credit_card, debit_card, credit_card, debit_card>() // this will inject a vector into class cards with 2 cards: once credit and one debit
    );
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% mvc hierarchy %%%%%%%%%%%%%%%%%%%%%%%%

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
    void refresh() override {}
};

class gui_view : public iview {
public:
    gui_view(const string name, renderer &renderer) {}
    void refresh() override { cout << "view-text-refreshed"; }
};

struct model {};

class controller {
public:
    controller(model &model, iview &view) {}
};

struct user {};

class app {
public:
    app(user &user, controller &controller) {}
};

auto mvc_injector() {
    cout << "mvc_injector" << endl;
    return di::make_injector(
            di::bind<string>.to("graphic_card"), // the renderer device
            di::bind<iview>.to<gui_view>()
    );
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% composite hierarchy %%%%%%%%%%%%%%%%%%%%%%%%

void composite_injector() {
    cout << "composite_injector" << endl;
    const auto injector = di::make_injector(
            cards_injector(),
            mvc_injector()
    );
    // that's actually cool... the composite injector can create the hierarchies of its composed injectors
    auto cardz = injector.create<cards>();
    auto mvc = injector.create<app>();
}

int main() {
    composite_injector();

    return 0;
}
