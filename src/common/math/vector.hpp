#ifndef FLUID_VECTOR_HPP
#define FLUID_VECTOR_HPP

#include <array>
#include <ostream>

/// Vector de tres dimiensiones
namespace sim {
  template <typename DataType>
  struct vec3 {
      DataType x, y, z;

      template <typename T>
      vec3(T _x, T _y, T _z)
        : x(static_cast<DataType>(_x)), y(static_cast<DataType>(_y)), z(static_cast<DataType>(_z)) {
      }

      vec3();

      constexpr vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) { }

      constexpr vec3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }

      vec3<DataType> operator+(vec3<DataType> const & vector) const;

      vec3<DataType> operator+=(vec3<DataType> const & vector);

      vec3<DataType> operator-(vec3 const & vector) const;

      vec3<DataType> operator-=(vec3 const & vector);

      vec3<DataType> operator*(DataType scalar) const;

      vec3<DataType> operator/(DataType scalar) const;

      inline static double SquaredDistance(const vec3<DataType>& v_i, const vec3<DataType>& v_j){
        const double x = v_i.x - v_j.x;
        const double y = v_i.y - v_j.y;
        const double z = v_i.z - v_j.z;
        return x * x + y * y + z * z;
      }

      /**
       * imprimir un objeto vec3 en un flujo de salida (como std::cout)
       *
       * @param os flujo de salida donde se imprimirá el objeto vec3
       * @param v objeto vec3 que se imprimirá
       * @return flujo de salida después de la operación de impresión
       */
      friend std::ostream & operator<<(std::ostream & os, vec3<DataType> const & v) {
        os << v.x << " x " << v.y << " x " << v.z;
        return os;
      }
  };

  using vec3d = vec3<double>;
}  // namespace sim
#endif  // FLUID_VECTOR_HPP
