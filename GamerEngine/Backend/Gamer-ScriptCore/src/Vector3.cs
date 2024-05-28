using System;

namespace GamerEngine
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y,a.Z + b.Z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar , vector.Z * scalar);
        }

        public static Vector3 operator *(Vector3 vector1, Vector3 vector2)
        {
            return new Vector3(vector1.X * vector2.X, vector1.Y * vector2.Y, vector1.Z * vector2.Z);
        }

        public float LengthSquared()
        {
            return X * X + Y * Y + Z * Z;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }


        public Vector3 Lerp(Vector3 aVector1, Vector3 aVector2, float aTime)
        {
            return new Vector3(
                aVector1.X + (aTime * (aVector2.X - aVector1.X)),
                aVector1.Y + (aTime * (aVector2.Y - aVector1.Y)),
                aVector1.Z + (aTime * (aVector2.Z - aVector1.Z))
            );

        }
    }
}