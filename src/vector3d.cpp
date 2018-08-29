#include "vector3d.hpp"

/**
 * Copy constructor
 */
Vector3d::Vector3d(const Vector3d& rhs)
{
	if (this == &rhs)	return;
	
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_z = rhs.m_z;
}

/**
 * Operators.
 */
bool Vector3d::operator==(const Vector3d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x != rhs.m_x) ||
	    (m_y != rhs.m_y) ||
	    (m_z != rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector3d::operator!=(const Vector3d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x == rhs.m_x) &&
	    (m_y == rhs.m_y) &&
	    (m_z == rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector3d::operator<(const Vector3d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x >= rhs.m_x) ||
	    (m_y >= rhs.m_y) ||
	    (m_z >= rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector3d::operator<=(const Vector3d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x > rhs.m_x) ||
	    (m_y > rhs.m_y) ||
	    (m_z > rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector3d::operator>(const Vector3d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x <= rhs.m_x) ||
	    (m_y <= rhs.m_y) ||
	    (m_z <= rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector3d::operator>=(const Vector3d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x < rhs.m_x) ||
	    (m_y < rhs.m_y) ||
	    (m_z < rhs.m_z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * Get length of vector.
 */
float Vector3d::length() const
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);	
}

/**
 * Get squared length of vector.
 */
float Vector3d::length2() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

std::ostream &operator<<(std::ostream &os, Vector3d const &v)
{ 
    return os << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
}
