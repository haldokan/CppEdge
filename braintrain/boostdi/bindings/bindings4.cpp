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
        blnc += amt;
        return blnc;
    }
private:
    double blnc = 500;
};

class debit_card : public icard {
public:
    double charge(double amt) override {
        cout << "charging: " << amt << " to debit card" << endl;
        blnc -= amt;
        return blnc;
    }
private:
    double blnc = 1000;
};

class cards {
public:
    explicit cards(vector<shared_ptr<icard>> cards) : m_cards{move(cards)} {
        // note that we should not use 'cards' here because it was moved to m_cards
        assert(4 == m_cards.size());
        assert(dynamic_cast<credit_card*>(m_cards[0].get()));
        assert(dynamic_cast<debit_card*>(m_cards[1].get()));
    }
    [[nodiscard]] double charge(int index, double amt) const {return m_cards.at(index)->charge(amt);} // using vector.at bcz it throws for out_of_range access

private:
    // note that prefixing this with const screws things up grandly because we do subscript access in func 'charge'.
    // compile errors I got for that suck donkey.
    vector<shared_ptr<icard>> m_cards;
};

void container_injection_with_scopes() {
    cout << "container_injection_with_scopes" << endl;
    const auto injector = di::make_injector(
            // we explicitly bind this to unique-scope: different per request (even tho we use shared_ptr which deduced to be a singleton)
            di::bind<debit_card>.in(di::unique),
            // all the credit_card(s) injected here are the pointed to by the same shared pointer, while the debit_card(s) are different objects
            di::bind<icard *[]>().to<credit_card, debit_card, credit_card, debit_card>() // this will inject a vector into class cards with 2 cards: once credit and one debit
    );
    const auto cardz = injector.create<cards>(); // note that using var name that's the same as the class name wont compile
    cout << ">>>>credit transactions" << endl;
    cout << cardz.charge(0, 50) << endl; // prints 550
    cout << cardz.charge(0, 50) << endl; // prints 600
    cout << cardz.charge(2, 50) << endl; // prints 650: because all credit_card(s) are pointed to by the same shared_ptr (actually one object: singleton)

    cout << ">>>debit transactions" << endl;
    cout << cardz.charge(1, 50) << endl; // prints 950
    cout << cardz.charge(1, 50) << endl; // prints 900
    cout << cardz.charge(3, 50) << endl; // prints 950: the 3rd debit_card is a different object and is accessed by a different unique_ptr
}

int main() {
    container_injection_with_scopes();
    return 0;
}
