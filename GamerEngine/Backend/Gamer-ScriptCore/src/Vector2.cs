using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamerEngine
{
    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        public static Vector2 operator *(Vector2 vector1, Vector2 vector2)
        {
            return new Vector2(vector1.X * vector2.X, vector1.Y * vector2.Y);
        }

        public float LengthSquared()
        {
            return X * X + Y * Y;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }


        public Vector2 Lerp(Vector2 aVector1, Vector2 aVector2, float aTime)
        {
            return new Vector2(
                aVector1.X + (aTime * (aVector2.X - aVector1.X)),
                aVector1.Y + (aTime * (aVector2.Y - aVector1.Y))
            );

        }
    }
}
