#ifndef VECCORE_BACKEND_VC_VECTOR_H
#define VECCORE_BACKEND_VC_VECTOR_H

namespace vecCore {

template <typename T, class Abi>
struct TypeTraits<Vc::Mask<T, Abi>> {
  using IndexType  = size_t;
  using ScalarType = Bool_s;
};

template <typename T, class Abi>
struct TypeTraits<Vc::Vector<T, Abi>> {
  using ScalarType = T;
  using MaskType   = typename Vc::Vector<T, Abi>::MaskType;
  using IndexType  = typename Vc::Vector<T, Abi>::IndexType;
};

namespace backend {

template <typename T = Real_s, class Abi = Vc::VectorAbi::Best<T>>
class VcVectorT {
public:
  using Real_v   = Vc::Vector<T, Abi>;
  using Float_v  = Vc::Vector<Float_s, Abi>;
  using Double_v = Vc::Vector<Double_s, Abi>;

  using Int_v   = Vc::Vector<Int_s, Abi>;
  using Int16_v = Vc::Vector<Int16_s, Abi>;
  using Int32_v = Vc::Vector<Int32_s, Abi>;
  using Int64_v = Vc::Vector<Int64_s, Abi>;

  using UInt_v   = Vc::Vector<UInt_s, Abi>;
  using UInt16_v = Vc::Vector<UInt16_s, Abi>;
  using UInt32_v = Vc::Vector<UInt32_s, Abi>;
  using UInt64_v = Vc::Vector<UInt64_s, Abi>;
};

using VcVector = VcVectorT<>;

} // namespace backend

template <typename T, class Abi>
VECCORE_FORCE_INLINE
Bool_s MaskEmpty(const Vc::Mask<T, Abi> &mask)
{
  return mask.isEmpty();
}

template <typename T, class Abi>
VECCORE_FORCE_INLINE
Bool_s MaskFull(const Vc::Mask<T, Abi> &mask)
{
  return mask.isFull();
}

template <typename T, class Abi>
struct IndexingImplementation<Vc::Mask<T, Abi>> {
  using M = Vc::Mask<T, Abi>;
  static inline Bool_s Get(const M &mask, size_t i) { return mask[i]; }

  static inline void Set(M &mask, size_t i, const Bool_s val) { mask[i] = val; }
};

template <typename T, class Abi>
struct LoadStoreImplementation<Vc::Vector<T, Abi>> {
   using V = Vc::Vector<T, Abi>;
  template <typename S = Scalar<V>>
  static inline void Load(V &v, S const *ptr)
  {
    v.load(ptr);
  }

  template <typename S = Scalar<V>>
  static inline void Store(V const &v, S *ptr)
  {
    v.store(ptr);
  }
};

template <typename T, class Abi>
struct LoadStoreImplementation<Vc::Mask<T, Abi>> {
  using M = Vc::Mask<T, Abi>;

  template <typename S = Scalar<T>>
  static inline void Load(M &mask, Bool_s const *ptr)
  {
    mask.load(ptr);
  }

  template <typename S = Scalar<T>>
  static inline void Store(M const &mask, S *ptr)
  {
    mask.store(ptr);
  }
};

template <typename T, class Abi>
struct MaskingImplementation<Vc::Vector<T, Abi>> {
  using M = Vc::Mask<T, Abi>;
  using V = Vc::Vector<T, Abi>;

  static inline void Assign(V &dst, M const &mask, V const &src) { dst(mask) = src; }

  static inline void Blend(V &dst, M const &mask, V const &src1, V const src2)
  {
    dst       = src2;
    dst(mask) = src1;
  }
};

namespace math {

template <typename T, class Abi>
VECCORE_FORCE_INLINE
Vc::Vector<T, Abi> CopySign(const Vc::Vector<T, Abi> &x, const Vc::Vector<T, Abi> &y)
{
  return Vc::copysign(x, y);
}

#define VC_MATH_UNARY_FUNCTION(F, f)                \
template <typename T, class Abi>                    \
VECCORE_FORCE_INLINE                                \
Vc::Vector<T, Abi> F(const Vc::Vector<T, Abi> &x)   \
{ return Vc::f(x); }                                \

VC_MATH_UNARY_FUNCTION(Abs, abs)
VC_MATH_UNARY_FUNCTION(Exp, exp)
VC_MATH_UNARY_FUNCTION(Log, log)
VC_MATH_UNARY_FUNCTION(Log2, log2)
VC_MATH_UNARY_FUNCTION(Log10, log10)
VC_MATH_UNARY_FUNCTION(Sqrt, sqrt)

VC_MATH_UNARY_FUNCTION(Sin, sin)
VC_MATH_UNARY_FUNCTION(Cos, cos)
VC_MATH_UNARY_FUNCTION(ASin, asin)
// VC_MATH_UNARY_FUNCTION(ATan, atan) // slower than std::atan()

// VC_MATH_UNARY_FUNCTION(Floor, floor) // broken
// VC_MATH_UNARY_FUNCTION(Ceil, ceil)   // broken
// VC_MATH_UNARY_FUNCTION(Trunc, trunc) // broken

#undef VC_MATH_UNARY_FUNCTION

template <typename T, class Abi>
VECCORE_FORCE_INLINE
Vc::Vector<T, Abi> Tan(const Vc::Vector<T, Abi> &x)
{
  Vc::Vector<T, Abi> s, c;
  Vc::sincos(x, &s, &c);
  return s / c;
}

template <typename T, class Abi>
VECCORE_FORCE_INLINE
Vc::Mask<T, Abi> IsInf(const Vc::Vector<T, Abi> &x)
{
  return Vc::isinf(x);
}

}

} // namespace vecCore

#endif
