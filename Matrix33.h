class Matrix33 {
    double val[9] = { 0.0 };
    public: 
        Matrix33();

        double operator()(size_t i, size_t j) const;
        double& operator()(size_t i, size_t j);

        vec3 operator*(vec3 v) const;

        static Matrix33 Identity();
        static Matrix33 BasisRotation(vec3 u, vec3 v, vec3 a, vec3 b);
};