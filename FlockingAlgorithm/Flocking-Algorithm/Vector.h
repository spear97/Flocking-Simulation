#ifndef VECTOR_H_
#define VECTOR_H_

#include "Basic.h" // Include necessary dependencies

namespace mathtool {

    // Class representing a 3-dimensional vector
    class Vector3d {
    public:

        /////////////Constructors/////////////

        // Default constructor with optional parameters for x, y, and z components
        Vector3d(const double& _x = double(), const double& _y = double(), const double& _z = double()) {
            // Initialize vector components
            m_v[0] = _x; m_v[1] = _y; m_v[2] = _z;
        }

        // Copy constructor
        Vector3d(const Vector3d& _v) {
            // Copy values from another vector
            m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
        }

        // Constructor from array
        Vector3d(const double(&_t)[3]) {
            // Initialize vector components from an array
            m_v[0] = _t[0]; m_v[1] = _t[1]; m_v[2] = _t[2];
        }

        /////////////Assignment operators/////////////

        // Assignment operator with new x, y, and z values
        Vector3d& operator()(const double& _x, const double& _y, const double& _z) {
            // Assign new values to vector components
            m_v[0] = _x; m_v[1] = _y; m_v[2] = _z;
            return *this;
        }

        // Assignment operator from another vector
        Vector3d& operator=(const Vector3d& _v) {
            // Assign values from another vector
            m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
            return *this;
        }

        // Assignment operator from an array
        Vector3d& operator=(const double _v[3]) {
            // Assign values from an array
            m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
            return *this;
        }

        /////////////Comparison operators/////////////

        // Equality comparison operator
        bool operator==(const Vector3d& _v) const {
            // Check if components are equal
            return m_v[0] == _v[0] && m_v[1] == _v[1] && m_v[2] == _v[2];
        }

        // Inequality comparison operator
        bool operator!=(const Vector3d& _v) const {
            // Check if components are not equal
            return !(*this == _v);
        }

        /////////////Compound assignment operators/////////////

        // Addition assignment operator
        Vector3d& operator+=(const Vector3d& _v) {
            // Add components of another vector
            m_v[0] += _v[0]; m_v[1] += _v[1]; m_v[2] += _v[2];
            return *this;
        }

        // Subtraction assignment operator
        Vector3d& operator-=(const Vector3d& _v) {
            // Subtract components of another vector
            m_v[0] -= _v[0]; m_v[1] -= _v[1]; m_v[2] -= _v[2];
            return *this;
        }

        // Scalar multiplication assignment operator
        Vector3d& operator*=(const double& _d) {
            // Multiply components by a scalar value
            m_v[0] *= _d; m_v[1] *= _d; m_v[2] *= _d;
            return *this;
        }

        // Scalar division assignment operator
        Vector3d& operator/=(const double& _d) {
            // Divide components by a scalar value
            m_v[0] /= _d; m_v[1] /= _d; m_v[2] /= _d;
            return *this;
        }

        /////////////Unary operators/////////////

        // Unary negation operator
        Vector3d operator-() const {
            // Negate components of the vector
            return Vector3d(-m_v[0], -m_v[1], -m_v[2]);
        }

        /////////////Binary arithmetic operators/////////////

        // Addition operator
        Vector3d operator+(const Vector3d& _v) const {
            // Add components of another vector
            return Vector3d(m_v[0] + _v[0], m_v[1] + _v[1], m_v[2] + _v[2]);
        }

        // Subtraction operator
        Vector3d operator-(const Vector3d& _v) const {
            // Subtract components of another vector
            return Vector3d(m_v[0] - _v[0], m_v[1] - _v[1], m_v[2] - _v[2]);
        }

        // Scalar multiplication operator
        Vector3d operator*(const double& _d) const {
            // Multiply components by a scalar value
            return Vector3d(m_v[0] * _d, m_v[1] * _d, m_v[2] * _d);
        }

        // Scalar division operator
        Vector3d operator/(const double& _d) const {
            // Divide components by a scalar value
            return Vector3d(m_v[0] / _d, m_v[1] / _d, m_v[2] / _d);
        }

        /////////////Utility methods/////////////

        // Dot product of vectors
        double operator*(const Vector3d& _v) const {
            // Compute dot product of two vectors
            return m_v[0] * _v[0] + m_v[1] * _v[1] + m_v[2] * _v[2];
        }

        // Magnitude of the vector
        double norm() const {
            // Compute the magnitude of the vector
            return std::sqrt(normsqr());
        }

        // Squared magnitude of the vector
        double normsqr() const {
            // Compute the squared magnitude of the vector
            return (*this) * (*this);
        }

        // Normalize the vector in place
        Vector3d& selfNormalize() {
            // Normalize the vector
            double n = norm();
            if (n < std::numeric_limits<double>::epsilon())
                return *this = Vector3d();
            return *this /= n;
        }

        // Return a normalized copy of the vector
        Vector3d normalize() const {
            // Return a normalized copy of the vector
            double n = norm();
            if (n < std::numeric_limits<double>::epsilon())
                return Vector3d();
            return *this / n;
        }

        /////////////Accessors/////////////

        // Accessor for individual components (const)
        double operator[](int i) const {
            // Access individual components of the vector (const)
            return m_v[i];
        }

        // Accessor for individual components (mutable)
        double& operator[](int i) {
            // Access individual components of the vector (mutable)
            return m_v[i];
        }

        /////////////Getters/////////////

        // Get the X-Value of the Vector
        double GetX() {
            return m_v[0];
        }

        // Get the Y-Value of the Vector
        double GetY() {
            return m_v[1];
        }

        // Get the Z-Value of the Vector
        double GetZ() {
            return m_v[2];
        }

        /////////////Setters/////////////

        // Set the X-Value of the Vector
        void SetX(double _d) {
            m_v[0] = _d;
        }

        // Set the Y-Value of the Vector
        void SetY(double _d) {
            m_v[1] = _d;
        }

        // Set the Z-Value of the Vector
        void SetZ(double _d) {
            m_v[2] = _d;
        }

    private:
        double m_v[3]; // Array to store vector components
    };

    /////////////Additional utility functions/////////////

    // Scalar multiplication with vector
    inline Vector3d operator*(const double& _d, const Vector3d& _v) {
        // Scalar multiplication with a vector
        return _v * _d;
    }

    // Output operator for vectors
    inline std::ostream& operator<<(std::ostream& _os, const Vector3d& _v) {
        // Output vector components to a stream
        for (size_t i = 0; i < 3; ++i) _os << _v[i] << " ";
        return _os;
    }

} // End of namespace mathtool

#endif // VECTOR_H_
