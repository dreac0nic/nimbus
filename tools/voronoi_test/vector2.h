#ifndef BLAH_VECTOR2
#define BLAH_VECTOR2

namespace BLAH
{
	/** Class for storing a three-dimensional vector.
	 * @param pos_type The type used for positional components of the vector.
	 */
	template<typename pos_type>
 	 class vector2
	{
	private:
		pos_type mX, mY;

	public:
		// CONSTRUCTORS
		/** Default constructor, can be provided a single scalar.
		 * @param position [pos_type] Single scalar for unit vector. (Def = 0.0)
		 */
		vector2(pos_type position = pos_type(0.0f)):
			mX(position), mY(position) {}

		/** Simple constructor, given three coordinate values.
		 * @param x [pos_type] X-coord for vector.
		 * @param y [pos_type] Y-coord for vector.
		 * @param z [pos_type] Z-coord for vector.
		 */
		vector2(pos_type x, pos_type y):
			mX(x), mY(y) {}

		/** Copy constructor, duplicates given vector across this vector.
		 * @param vector [vector2] Vector to duplicate.
		 */
		vector2(const vector2& vector):
			vector2(vector.mX, vector.mY) {}

		// DESTRUCTOR
		/** Default destructor, yo.
		 */
		virtual ~vector2(void) {}

		// OPERATORS
		// -- Negated
		vector2 operator-() const { return vector2(-mX, -mY); }

		// -- Vector operations
		// NOTE: COMPONENT MULIPLICATION IS IMPLEMENTED FOR THE (*) OPERATOR FOR SIMPLICITY.
		//       Dot and cross product operators should be implmeneted in the future using functions.
		vector2 operator+(const vector2& vector) const { return vector2(mX + vector.mX, mY + vector.mY); }
		vector2 operator-(const vector2& vector) const { return vector2(mX - vector.mX, mY - vector.mY); }
		vector2 operator*(const vector2& vector) const { return vector2(mX * vector.mX, mY * vector.mY); }

		vector2& operator+=(const vector2& vector) { mX += vector.mX; mY += vector.mY; return (*this); }
		vector2& operator-=(const vector2& vector) { mX -= vector.mX; mY -= vector.mY; return (*this); }
		vector2& operator*=(const vector2& vector) { mX *= vector.mX; mY *= vector.mY; return (*this); }

		// -- Scalar operators
		vector2 operator*(const pos_type scalar) const { return vector2(mX*scalar, mY*scalar); }
		vector2 operator/(const pos_type scalar) const { pos_type realScale = 1.0f/scalar; return (*this)*realScale; } // NOTE TWO CALLS, CARE

		vector2 operator*=(const pos_type scalar) { mX *= scalar; mY *= scalar; return (*this); }
		vector2 operator/=(const pos_type scalar) { pos_type realScale = 1.0f/scalar; mX *= realScale; mY *= realScale; return (*this); }

		// METHODS
		pos_type magnitude(void)
		{
			return sqrt(mX*mX + mY*mY);
		}

		void normalize(void)
		{
			pos_type mag = magnitude();

			mX /= mag;
			mY /= mag;
		}
		
		vector2 getNormalized(void)
		{
			pos_type mag = magnitude();

			return vector3(mX/mag, mY/mag);
		}

		// RELATIONSHIPS
		friend class vector2; // A lonely world, being friends with yourself...
	};
}

#endif