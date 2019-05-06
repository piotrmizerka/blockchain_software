#include "stdafx.h"

void drawText(const char * text, int length, double x, double y, double r, double g, double b)
{
	/*glColor3f(r, g, b);
	glRasterPos2d(x, y);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
	}*/
}

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

/// https://www.opengl.org/discussion_boards/showthread.php/167993-draw-an-arc-in-opengl - Dastagir 07-07-2009 4:56 AM
void drawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
	float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

	float tangetial_factor = tanf(theta);

	float radial_factor = cosf(theta);


	float x = r * cosf(start_angle);//we now start at the start angle
	float y = r * sinf(start_angle);

	glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + cx, y + cy);

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

