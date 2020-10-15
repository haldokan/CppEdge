class Container {
    // const indicate that this func will not change its object
    virtual int length() const = 0; // the = 0 indicate pure virtual (must be implemented in a derived class)
    virtual double& operator[](int)  = 0; // return a ref to double so that we can access & change vals at subscript
    virtual ~Container() {};
};
