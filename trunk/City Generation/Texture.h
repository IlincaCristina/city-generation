#ifndef TEXTURE_H
#define TEXTURE_H

#include <glut.h>
#include <stdlib.h>
#include <stdio.h>

#define GL_CLAMP_TO_EDGE 0x812F
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 256
#define WINDOW_HEIGHT 8
#define WINDOW_WIDTH 8
#define WINDOW_BEZEL 2

enum TextureType
{
	WINDOWS,
	SIDEWALK
};

class Texture
{
public:
	GLuint id;
	Texture();
	Texture(GLuint name, TextureType type);
private:
	GLubyte * image;
	float randR, randG, randB;
	void setWhite(int row, int col);
	void setBlack(int row, int col);
	void setGreyscale(int row, int col, GLubyte color);
	void colorWindow(int xOffset, int yOffset);
	void createGLTexture();
	GLubyte randomColor();
};

void Texture::setGreyscale(int row, int col, GLubyte color)
{
	int offset = row * TEXTURE_WIDTH * 4 + col * 4;

	*(image + offset) = color * randR;
	*(image + (++offset)) = color * randG;
	*(image + (++offset)) = color * randB;
	*(image + (++offset)) = (GLubyte) 255;
}

void Texture::setBlack(int row, int col)
{
	setGreyscale(row, col, (GLubyte) 0);
}

void Texture::setWhite(int row, int col)
{
	setGreyscale(row, col, (GLubyte) 255);
}

GLubyte Texture::randomColor()
{
	int num = rand() % 100;
	if(num < 50)
	{
		return (GLubyte) 10;
	}
	else if (num < 60)
	{
		return (GLubyte) 80;
	}
	else if(num < 85)
	{
		return (GLubyte) 160;
	}
	else
	{
		return (GLubyte) 245;
	}
}

void Texture::createGLTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, 
		TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		image);
}

void Texture::colorWindow(int xOffset, int yOffset)
{
	int xStart = xOffset * WINDOW_HEIGHT;
	int yStart = yOffset * WINDOW_WIDTH;
	int xPos, yPos;
	GLubyte color = randomColor();
	GLubyte randMod;
	for(int row = 0; row < WINDOW_HEIGHT; row++)
	{
		for(int col = 0; col < WINDOW_WIDTH; col++)
		{
			xPos = xStart + row;
			yPos = yStart + col;
			if(row % WINDOW_HEIGHT < WINDOW_BEZEL)
			{
				setBlack(xPos, yPos);
			}
			else if(row % WINDOW_HEIGHT >= WINDOW_HEIGHT - WINDOW_BEZEL)
			{
				setBlack(xPos, yPos);
			}
			else
			{
				if(col % WINDOW_WIDTH < WINDOW_BEZEL)
				{
					setBlack(xPos, yPos);
				}
				else if(col % WINDOW_WIDTH >= WINDOW_WIDTH - WINDOW_BEZEL)
				{
					setBlack(xPos, yPos);
				}
				else
				{
					randMod = (GLubyte) ((color % col) / ((rand() % 4)+1))*color;
					setGreyscale(xPos, yPos, color-randMod);
				}
			} 
		}
	}
}

Texture::Texture(GLuint name, TextureType type)
{
	image = (GLubyte *)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT * 4);
	id = name;
	randR = ((float)rand())/RAND_MAX;
	randG = ((float)rand())/RAND_MAX;
	randB = ((float)rand())/RAND_MAX;
	for(int row = 0; row < TEXTURE_HEIGHT / WINDOW_HEIGHT; row++)
	{
		for(int col = 0; col < TEXTURE_WIDTH / WINDOW_WIDTH; col++)
		{
			colorWindow(row, col);
		}
	}
	createGLTexture();
	free(image);
}

Texture::Texture()
{
}

#endif