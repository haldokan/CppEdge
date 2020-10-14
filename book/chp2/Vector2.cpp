module;

export module Vector2;

export class Vector2 {

public:
    Vector2(int);
    int get_size();
    double &operator[](int);
private:
    int length;
    double* items;
};

Vector2::Vector2(int _len) {
    length = _len;
    items = new double[_len];
}

int Vector2::get_size() {
    return length;
}

double& Vector2::operator[](int index) {
    return items[index];
}
// supposedly that's how to export non-member funcs -- does not work
//export int get_length(const Vector2& v) { return v.get_size(); }