#include "Vector4D.h"
#include "Maths.h"

float Vector4D::LengthSqr() const
{
    return (x * x + y * y + z * z + w * w);
}

float Vector4D::Length() const
{
    return (Maths::Sqrt(LengthSqr()));
}

void Vector4D::Normalize()
{
    float len = Length();
    x /= len;
    y /= len;
    z /= len;
    w /= len;
}
