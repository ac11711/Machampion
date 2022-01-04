#pragma once
#include <math.h>

//A class that allows us to work with the dimensions and coordinates of the game
class Vector2D {
public:

	//Constructor

	Vector2D() {
		m_x = 0;
		m_y = 0;
	}
	Vector2D(float x, float y) {
		m_x = x;
		m_y = y;
	}

	//Getters

	float getX() { return m_x; }
	float getY() { return m_y; }

	//Setters

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	//Length
	float length() { return sqrt(m_x * m_x + m_y * m_y); }

	//Overload operators

	Vector2D operator+(const Vector2D& v2) {
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
	}
	Vector2D operator-(const Vector2D& v2) {
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
	}
	Vector2D operator*(float scalar) {
		return Vector2D(m_x * scalar, m_y * scalar);
	}
	Vector2D operator/(float scalar) {
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	//Overload quick operators

	friend Vector2D& operator+=(Vector2D& v1, Vector2D& v2) {
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;

		return v1;
	}
	friend Vector2D& operator-=(Vector2D& v1, Vector2D& v2) {
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;

		return v1;
	}
	Vector2D& operator*=(float scalar) {
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}
	Vector2D& operator/=(float scalar) {
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

	//Variables

	float m_x;
	float m_y;
};