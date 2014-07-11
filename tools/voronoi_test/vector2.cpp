#include "vector2.h"

using namespace BLAH;

/** Magnitude returns the calculated magnitude of the
 *  vector. Calculated using the traditional pyth-theorem.
 */
pos_type vector2::magnitude(void)
{
	return sqrt(mX*mX + mY*mY);
}

/** Normalizes this vector.
 *  Finds magnitude based on magnitude function.
 */
void vector2::normalize(void)
{
	pos_type mag = magnitude();

	mX /= mag;
	mY /= mag;
}

/** Returns the normalized version of this vector.
 *  Finds magnitude based on magnitude function.
 */
vector2 vector2::getNormalized(void)
{
	pos_type mag = magnitude();

	return vector3(mX/mag, mY/mag);
}