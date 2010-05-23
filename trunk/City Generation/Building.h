

#ifndef _BUILDING_H_
#define _BUILDING_H_

#define WINDOW_OFFSET 1000000
#define SIDEWALK_OFFSET 2000000

#include "BuildingType.h"
#include "Texture.h"
#include "SidewalkTexture.h"
#include "MyUtil.h"

class Building
{
private:
	Texture windows;
	SidewalkTexture sidewalk;
	Mat4x4 trans;
	GLint id;
	int levels;
	int width, depth; // Of the base to work with
	void generate();
	void generateStandard();

public: 
	BuildingType type;
	void draw() const;
	void increaseHeight();
	void decreaseHeight();

	inline Building(BuildingType t, int width, int depth, Mat4x4 position, GLuint ident);
};

void Building::draw() const
{
	glPushMatrix();
	glMultMatrix(trans);
	glCallList(id);
	glPopMatrix();
}

void Building::generateStandard()
{
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricNormals( qobj, GL_TRUE );
	glNewList( id, GL_COMPILE );
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, windows.id);
	
	double w = (width / 2) - ((rand() % (width / 2)) + 1) / 2;
	double d = (depth / 2) - ((rand() % (depth / 2)) + 1) / 2;

	glColor(Color(0, 0, 0));

	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 0.0);	glVertex3f(w, -d, 0);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-w, -d, 0);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-w, -d, levels);
	glTexCoord2f(1.0, 1.0);	glVertex3f(w, -d, levels);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0);	glVertex3f(w, d, levels);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-w, d, levels);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-w, d, 0);
	glTexCoord2f(1.0, 0.0);	glVertex3f(w, d, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(w, -d, 0);
	glTexCoord2f(0.0, 1.0);	glVertex3f(w, -d, levels);
	glTexCoord2f(1.0, 1.0);	glVertex3f(w, d, levels);
	glTexCoord2f(1.0, 0.0);	glVertex3f(w, d, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-w, d, 0);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-w, d, levels);
	glTexCoord2f(1.0, 1.0);	glVertex3f(-w, -d, levels);
	glTexCoord2f(1.0, 0.0);	glVertex3f(-w, -d, 0);
	glEnd();

	// Roof
	glBegin(GL_POLYGON);
	glVertex3f(w, -d, levels);
	glVertex3f(-w, -d, levels);
	glVertex3f(-w, d, levels);
	glVertex3f(w, d, levels);
	glEnd();

	// Base/Sidewalk
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sidewalk.stexId);
	//glDisable(GL_LIGHTING);
	w = (width / 2.0) + 0.2;
	d = (depth / 2.0) + 0.2;
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex3f(w, d, 0.0001);
	glTexCoord2f(1.0, 0.0); glVertex3f(w, -d, 0.0001);
	glTexCoord2f(0.0, 0.0); glVertex3f(-w, -d, 0.0001);
	glTexCoord2f(0.0, 1.0);glVertex3f(-w, d, 0.0001);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEndList();
	gluDeleteQuadric( qobj );
}

void Building::generate()
{
	switch(type)
	{
	case STANDARD:
		levels = rand() % 40 + 6;
		generateStandard();
		break;
	}
}

inline Building::Building(BuildingType t, int w, int d, Mat4x4 position, GLuint ident)
{
	windows = Texture(ident + WINDOW_OFFSET, WINDOWS);
	sidewalk = SidewalkTexture();
	type = t;
	width = w;
	depth = d;
	trans = position;
	id = ident;

	generate();
}

#endif