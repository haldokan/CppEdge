#include <iostream>
#include "../di.hpp"

using namespace std;
namespace di = boost::di;

class icard {
public:
    virtual ~icard() = default;

    virtual double charge(double) = 0; // charge amt and return balance
};

class credit_card : public icard {
public:
    double charge(double amt) override {
        cout << "charging: " << amt << " to credit card" << endl;
        return amt * 5;
    }
};

class debit_card : public icard {
public:
    double charge(double amt) override {
        cout << "charging: " << amt << " to debit card" << endl;
        return amt * 3;
    }
};

class cards {
public:
    explicit cards(vector<unique_ptr<icard>> cards) : m_cards{move(cards)} {
        // note that we should not use 'cards' here because it was moved to m_cards
        cout << "cards size: " << m_cards.size() << endl;
        assert(3 == m_cards.size() || 2 == m_cards.size()); // bcz we do 2 injections one with 3 and the other with 2
        assert(dynamic_cast<credit_card*>(m_cards[0].get()));
        assert(dynamic_cast<debit_card*>(m_cards[1].get()));
    }
    [[nodiscard]] double charge(int index, double amt) const {return m_cards.at(index)->charge(amt);} // using vector.at bcz it throws for out_of_range access

private:
    // note that prefixing this with const screws things up grandly because we do subscript access in func 'charge'.
    // compile errors I got for that suck donkey.
    vector<unique_ptr<icard>> m_cards;
};

void container_injection() {
    cout << "container_injection" << endl;
    const auto injector = di::make_injector(
            // for some devil he throw in brackets after the bind - I can remove them and the crap still works
            di::bind<icard *[]>().to<credit_card, debit_card, credit_card>() // this will inject a vector into class cards with 2 cards: once credit and one debit
    );
    const auto cardz = injector.create<cards>(); // note that using var name that's the same as the class name wont compile
    cout << cardz.charge(0, 17) << endl;
}

// this can be useful in testing
void override_injection() {
    cout << "override_injection" << endl;
    const auto injector = di::make_injector(
            // for some devil he throw in brackets after the bind - I can remove them and the crap still works
            di::bind<icard *[]>().to<credit_card, debit_card, credit_card>(), // this will inject a vector into class cards with 2 cards: once credit and one debit
            di::bind<icard *[]>().to<credit_card, debit_card>() [di::override] // more mind boggling syntax
    );
    const auto cardz = injector.create<cards>(); // note that using var name that's the same as the class name wont compile
    cout << cardz.charge(0, 17) << endl;
}

int main() {
    container_injection();
    override_injection();

    return 0;
}
