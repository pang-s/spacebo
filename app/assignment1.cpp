//  ========================================================================
//  Pang Suwanaposee psu43
//  ========================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

//--Globals ---------------------------------------------------------------
GLUquadric *q;    //Required for creating cylindrical objects


float size = 50; //size for skybox


float  eye_x = -20,  eye_y = 10,  eye_z = 20;
float look_x = -19.13, look_y = 10, look_z = 18;    //"Look-at" point along -z direction

float  h_look_angle = -0.35;                              //Look angles
float  v_look_angle = 0;
int cam_step = 0;								   //camera motion
GLuint txId[10];   //Texture ids

// colours
float grey[4] = {0.2, 0.2, 0.2, 1.0};
float black[4] = {0};
float white[4]  = {1.0, 1.0, 1.0, 1.0};

// walking bot
int walk_theta = 20;
int walk_z = 0;
int walk_x = 0;
int walk_dir_angle = 0;

int animation_time = 0;
float ring_turning_angle = 0;
float dodec_angle = 0;

// for flying bot
float bot_move_x = 0;
float bot_move_y = 0;
float bot_move_z = 0;
float bot_turn_x = 0;
float bot_turn_y = 0;
float bot_turn_z = 0;
float bot_turn_after = 0;
bool fly_up = true;
bool fly_down = false;

// for moving rover
float rover_move_x = 0;
float rover_move_y = 0;
float rover_move_z = 0;
float rover_turn_x = 0;
float rover_turn_y = 0;
float rover_turn_z = 0;
float rover_turn_y_after = 0;

// for butterfly
float butterAngle = 0;
float butterTurn = 0;

//shadow
float shadow_angle = 0.0;  //Rotation angle

//--------------------------------------------------------------------------------



void loadTexture()
{
    glGenTextures(10, txId); 	// Create 6 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("criminal-impact_bk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("criminal-impact_ft.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("criminal-impact_lf.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("criminal-impact_rt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("criminal-impact_up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("criminal-impact_dn.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture
    loadTGA("tv.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture
    loadTGA("walk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    q = gluNewQuadric();
    
    loadTexture();
    glEnable(GL_TEXTURE_2D);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour
    glEnable(GL_LIGHTING);					//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    float mat_amb[4] = {0.0, 0.0, 1.0, 1.0};//blue
    float mat_dif[4] = {0.0, 0.0, 1.0, 1.0};//blue
    float mat_spe[4] = {1.0, 1.0, 1.0, 1.0};//white
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spe);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
    gluQuadricDrawStyle (q, GLU_FILL );
    gluQuadricNormals	(q, GLU_SMOOTH );
    gluQuadricTexture (q, GL_TRUE);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., 1., 1., 100.);
}

//--------------------SKY BOX AND FLOOR-----------------------------

void skybox() {
    glPushMatrix();
          glDisable(GL_LIGHTING);
            glColor4f(1.0, 1.0, 1.0, 0.9);
            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                // Back
                glBindTexture(GL_TEXTURE_2D, txId[0]);
                glBegin(GL_QUADS);
              glTexCoord2f(0,  1); glVertex3f(-size, size, -size);
              glTexCoord2f(0,  0); glVertex3f(-size, 0, -size);
              glTexCoord2f(1,  0); glVertex3f(size, 0, -size);
              glTexCoord2f(1,  1); glVertex3f(size, size, -size);
                glEnd();
                // Right
                glBindTexture(GL_TEXTURE_2D, txId[3]);
                glBegin(GL_QUADS);
              glTexCoord2f(0,  1); glVertex3f(size, size, -size);
              glTexCoord2f(0,  0); glVertex3f(size, 0, -size);
              glTexCoord2f(1,  0); glVertex3f(size, 0, size);
              glTexCoord2f(1,  1); glVertex3f(size, size, size);
                glEnd();
                // Front
                glBindTexture(GL_TEXTURE_2D, txId[1]);
                glBegin(GL_QUADS);
                glTexCoord2f(1,  1); glVertex3f(-size, size, size);
                glTexCoord2f(1,  0); glVertex3f(-size, 0, size);
                glTexCoord2f(0,  0); glVertex3f(size, 0, size);
                glTexCoord2f(0,  1); glVertex3f(size, size, size);
                glEnd();
                // Left
                glBindTexture(GL_TEXTURE_2D, txId[2]);
                glBegin(GL_QUADS);
              glTexCoord2f(1,  1); glVertex3f(-size, size, -size);
              glTexCoord2f(1,  0); glVertex3f(-size, 0, -size);
              glTexCoord2f(0,  0); glVertex3f(-size, 0, size);
              glTexCoord2f(0,  1); glVertex3f(-size, size, size);
                glEnd();
                // Top
                glBindTexture(GL_TEXTURE_2D, txId[4]);
                glBegin(GL_QUADS);
              glTexCoord2f(0,  1); glVertex3f(-size, size, -size);
              glTexCoord2f(1,  1); glVertex3f(-size, size, size);
              glTexCoord2f(1,  0); glVertex3f(size, size, size);
              glTexCoord2f(0,  0); glVertex3f(size, size, -size);
                glEnd();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
    glPopMatrix();
}

void drawFloor()
{
    glPushMatrix();
    float floor_height = -0.01;
    glColor4f(0.611, 0.925, 0.682, 1.0);
    glNormal3f(0.0, 1.0, 0.0);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black); //suppress specular reflections
    
    glBegin(GL_QUADS);
    for(int i = -100; i < 100; i++)
    {
        for(int j = -100;  j < 100; j++)
        {
            glVertex3f(i, floor_height, j);
            glVertex3f(i, floor_height, j+1);
            glVertex3f(i+1, floor_height, j+1);
            glVertex3f(i+1, floor_height, j);
        }
    }
    glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white); // set back
    glPopMatrix();

}


//--------------------------------------------------------------------------------

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    glNormal3f(nx, ny, nz);
}


//--------------------------------------------------------------------------------

//--Draw models constructed using GLUT objects ------------------

void drawCylinder(float radius, float height)
{
    glPushMatrix();
    float angle1,angle2, ca1,sa1, ca2,sa2;
    float x1, z1, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
  
    float width = 0.05; // was .5
    glBegin(GL_QUADS);
    for(int i = 0; i < 360; i += 5)    //5 deg intervals
    {
        angle1 = i * toRad;       //Computation of angles, cos, sin etc
        angle2 = (i+5) * toRad;
        ca1=cos(angle1); ca2=cos(angle2);
        sa1=sin(angle1); sa2=sin(angle2);
        x1=(radius-width)*sa1; z1=(radius-width)*ca1;
        x4=(radius-width)*sa2; z4=(radius-width)*ca2;
        
        glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
        glVertex3f(x1, 0.0, z1);
        glVertex3f(x1, height, z1);
        glNormal3f(-sa2 ,0.0, -ca2);
        glVertex3f(x4, height, z4);
        glVertex3f(x4, 0.0, z4);
      
    }
    glEnd();
    glPopMatrix();
}


void attachBoosters(bool shadow) {
    glPushMatrix();          // right BOOSTERS
    if(!shadow){
        glColor3f(1, 0.4, 0);
    }
    glTranslatef(-0.8, -2.2, 0);
    drawCylinder(0.6, 2);
    glPopMatrix();
    
    glPushMatrix();          // left BOOSTERS
    if(!shadow) {
        glColor3f(1, 0.4, 0);
    }
    glTranslatef(0.8, -2.2, 0);
    drawCylinder(0.6, 2);
    glPopMatrix();
    
}


void drawFlyingBot(bool shadow)
{
    
    if(!shadow){glColor3f(.85, 0.72, 0.63);	}	//Head
    glPushMatrix();
    glTranslatef(0, 8.5, 0);
    glScalef(2.8, 1.7, 1.5);
    glutSolidCube(2);
    glPopMatrix();
    
    if(!shadow){glColor3f(0, 1, 0);	}	//Left eye
    glPushMatrix();
    glTranslatef(0.9, 8.7, 1.3);
    glutSolidSphere(0.35, 10, 10);
    glPopMatrix();
    
    //glColor3f(0, 1, 0);		//Right eye
    glPushMatrix();
    glTranslatef(-0.9, 8.7, 1.3);
    glutSolidSphere(0.35, 10, 10);
    glPopMatrix();
    
    if(!shadow){glColor3f(0, 0, 0);	}	//Mouth
    glPushMatrix();
    glTranslatef(0.0, 7.6, 1.51);
    glBegin(GL_TRIANGLES); //Begin triangle coordinates
    glVertex3f(-0.4f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.4f, 0.0f, 0.0f);
    glEnd(); //End triangle coordinates
    glPopMatrix();
    
    if(!shadow){glColor3f(.85, 0.72, 0.63);		}		//Torso
    //glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, 5.2, 0);
    glScalef(3, 4, 1.8);
    glutSolidCube(1);
    glPopMatrix();
    
    if(!shadow){glColor3f(.85, 0.72, 0.63);	}			//Torso Skirt Front
    glPushMatrix();
    glTranslatef(0, 2.8, 1.2);
    glRotatef(-35, 1, 0, 0);
    glScalef(3, 1.3, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);				//Torso Skirt Back
    glPushMatrix();
    glTranslatef(0, 2.8, -1.2);
    glRotatef(35, 1, 0, 0);
    glScalef(3, 1.3, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);				//Torso Skirt Right
    glPushMatrix();
    glTranslatef(1.6, 2.7, 0);
    glRotatef(15, 0, 0, 1);
    glScalef(0.1, 1.3, 1.8);
    glutSolidCube(1);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);				//Torso Skirt Left
    glPushMatrix();
    glTranslatef(-1.6, 2.7, 0);
    glRotatef(-15, 0, 0, 1);
    glScalef(0.1, 1.3, 1.8);
    glutSolidCube(1);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);				//Right leg circle
    glPushMatrix();
    glTranslatef(-0.8, 3, 0);
    glutSolidSphere(.5, 10, 10);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);	      // right leg
    glPushMatrix();
    glTranslatef(-0.8, 1.3, 0);
    glScalef(1, 2.8, 1.4);
    glutSolidCube(1);
    glPopMatrix();
    
    //glColor3f(.85, 0.72, 0.63);				//Left leg circle
    glPushMatrix();
    glTranslatef(0.8, 3, 0);
    glutSolidSphere(.5, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8, 1.3, 0);  // left leg
    glScalef(1, 2.8, 1.4);
    glutSolidCube(1);
    glPopMatrix();
    
    //Right arm circle shoulder
    glPushMatrix();
    glTranslatef(-2, 6.5, 0);
    glutSolidSphere(0.4, 10, 10);
    glPopMatrix();
    
    //Right arm block shoulder
    glPushMatrix();
    glTranslatef(-1.5, 6.6, 0);
    glScalef(0.8, 0.8, 1);
    glutSolidCube(1);
    glPopMatrix();
    
    // right arm
    glPushMatrix();
    glTranslatef(-2, 4.4, 0);
    glScalef(.8, 3.5, 0.8);
    glutSolidCube(1);
    glPopMatrix();
    
    //Left arm circle shoulder
    glPushMatrix();
    glTranslatef(2, 6.5, 0);
    glutSolidSphere(0.4, 10, 10);
    glPopMatrix();
    
    //Left arm block shoulder
    glPushMatrix();
    glTranslatef(1.5, 6.6, 0);
    glScalef(0.8, 0.8, 1);
    glutSolidCube(1);
    glPopMatrix();
    
    // left arm
    glPushMatrix();
    glTranslatef(2, 4.4, 0);
    glScalef(.8, 3.5, 1);
    glutSolidCube(1);
    glPopMatrix();
    
    
}

void drawWalkingbot(bool shadow)
{
    if(!shadow) {
        glColor3f(0.839, 0.850, 0.862);
    }
    //Head
    glPushMatrix();
    glTranslatef(0, 8.5, 0);
    glScalef(2.8, 1.7, 1.5);
    glutSolidCube(2);
  	glPopMatrix();

    if(!shadow){
        glColor3f(0, 1, 0);
    }
    //Left eye
    glPushMatrix();
      glTranslatef(0.9, 8.7, 1.3);
      glutSolidSphere(0.35, 10, 10);
    glPopMatrix();

    
    //Right eye
    glPushMatrix();
      glTranslatef(-0.9, 8.7, 1.3);
      glutSolidSphere(0.35, 10, 10);
    glPopMatrix();

    if(!shadow){glColor3f(0, 0, 0);	}
    //Mouth
    glPushMatrix();
      glTranslatef(0.0, 7.6, 1.51);
      glBegin(GL_TRIANGLES); //Begin triangle coordinates
        glVertex3f(-0.4f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.4f, 0.0f, 0.0f);
      glEnd(); //End triangle coordinates
    glPopMatrix();

    if(!shadow){glColor3f(0.839, 0.850, 0.862);}
    //Torso
  	glPushMatrix();
  	  glTranslatef(0, 5.2, 0);
  	  glScalef(3, 4, 1.8);
  	  glutSolidCube(1);
  	glPopMatrix();
    
    // add front pad material
    glPushMatrix();
    if(!shadow){
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0, 2.5 , 1);
        glBindTexture(GL_TEXTURE_2D, txId[7]); // walk bots pic
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBegin(GL_POLYGON);
        glTexCoord2f(0,  0); glVertex3f(-1, 1, 0.0);
        glTexCoord2f(1,  0); glVertex3f(1, 1, 0.0);
        glTexCoord2f(1,  1); glVertex3f(1, 4, 0.0);
        glTexCoord2f(0,  1); glVertex3f(-1, 4, 0.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glTranslatef(0, 2.5 , 1);
        glBegin(GL_POLYGON);
         glVertex3f(-1, 1, 0.0);
        glVertex3f(1, 1, 0.0);
        glVertex3f(1, 4, 0.0);
        glVertex3f(-1, 4, 0.0);
        glEnd();
    }
    
    glPopMatrix();

    if(!shadow){glColor3f(0.839, 0.850, 0.862);		}
    //Torso Skirt Front
    glPushMatrix();
      glTranslatef(0, 2.8, 1.2);
      glRotatef(-35, 1, 0, 0);
      glScalef(3, 1.3, 0.1);
      glutSolidCube(1);
    glPopMatrix();

    //Torso Skirt Back
    glPushMatrix();
      glTranslatef(0, 2.8, -1.2);
      glRotatef(35, 1, 0, 0);
      glScalef(3, 1.3, 0.1);
      glutSolidCube(1);
    glPopMatrix();

    //Torso Skirt Right
    glPushMatrix();
      glTranslatef(1.6, 2.7, 0);
      glRotatef(15, 0, 0, 1);
      glScalef(0.1, 1.3, 1.8);
      glutSolidCube(1);
    glPopMatrix();

    //Torso Skirt Left
    glPushMatrix();
      glTranslatef(-1.6, 2.7, 0);
      glRotatef(-15, 0, 0, 1);
      glScalef(0.1, 1.3, 1.8);
      glutSolidCube(1);
    glPopMatrix();

    //Right leg circle
    glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(-walk_theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(-0.8, 3, 0);
      glutSolidSphere(.5, 10, 10);
    glPopMatrix();

    //Moved right leg
    glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(-walk_theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(-0.8, 1.3, 0);
      glScalef(1, 2.8, 1.4);
      glutSolidCube(1);
    glPopMatrix();

    //Left leg circle
    glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(walk_theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 3, 0);
      glutSolidSphere(.5, 10, 10);
    glPopMatrix();

    //Moved left leg
    glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(walk_theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 1.3, 0);
      glScalef(1, 2.8, 1.4);
      glutSolidCube(1);
    glPopMatrix();

    //Right arm circle shoulder
    glPushMatrix();
      glTranslatef(-2, 6.5, 0);
      glutSolidSphere(0.4, 10, 10);
    glPopMatrix();

    //Right arm block shoulder
    glPushMatrix();
      glTranslatef(-1.5, 6.6, 0);
      glScalef(0.8, 0.8, 1);
      glutSolidCube(1);
    glPopMatrix();

    //Moved right arm
    glPushMatrix();
      glTranslatef(2, 6.5, 0);
      glRotatef(walk_theta, 1, 0, 0);
      glTranslatef(-2, -6.5, 0);
      glTranslatef(-2, 4.4, 0);
      glScalef(.8, 3.5, 0.8);
      glutSolidCube(1);
    glPopMatrix();

    //Left arm circle shoulder
    glPushMatrix();
      glTranslatef(2, 6.5, 0);
      glutSolidSphere(0.4, 10, 10);
    glPopMatrix();

    //Left arm block shoulder
    glPushMatrix();
      glTranslatef(1.5, 6.6, 0);
      glScalef(0.8, 0.8, 1);
      glutSolidCube(1);
    glPopMatrix();

    //Moved left arm
    glPushMatrix();
      glTranslatef(2, 6.5, 0);
      glRotatef(-walk_theta, 1, 0, 0);
      glTranslatef(-2, -6.5, 0);
      glTranslatef(2, 4.4, 0);
      glScalef(.8, 3.5, 1);
      glutSolidCube(1);
    glPopMatrix();

}



void updateNormalLookXYZ() {
    float dir_x = -sin(h_look_angle), dir_y = sin(v_look_angle),  dir_z = -cos(h_look_angle);
    float d = 2.0;
    look_x = eye_x + d * dir_x;
    look_y = eye_y + d * dir_y;
    look_z = eye_z + d * dir_z;
}


void halfRing(float radius)
{
    glPushMatrix();
    float angle1,angle2, ca1,sa1, ca2,sa2;
    float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
    float height = 0.2; // was 1
    float width = 0.15; // was .5
    glBegin(GL_QUADS);
    for(int i = 0; i < 180; i += 5)    //5 deg intervals
    {
        angle1 = i * toRad;       //Computation of angles, cos, sin etc
        angle2 = (i+5) * toRad;
        ca1=cos(angle1); ca2=cos(angle2);
        sa1=sin(angle1); sa2=sin(angle2);
        x1=(radius-width)*sa1; z1=(radius-width)*ca1;
        x2=(radius+width)*sa1; z2=(radius+width)*ca1;
        x3=(radius+width)*sa2; z3=(radius+width)*ca2;
        x4=(radius-width)*sa2; z4=(radius-width)*ca2;
        
        glNormal3f(0., height, 0.);       //Quad 1 facing up
        glVertex3f(x1, height, z1);
        glVertex3f(x2, height, z2);
        glVertex3f(x3, height, z3);
        glVertex3f(x4, height, z4);
        
        glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
        glVertex3f(x1, 0.0, z1);
        glVertex3f(x1, height, z1);
        glNormal3f(-sa2 ,0.0, -ca2);
        glVertex3f(x4, height, z4);
        glVertex3f(x4, 0.0, z4);
        
        glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
        glVertex3f(x2, height, z2);
        glVertex3f(x2, 0.0, z2);
        glNormal3f(sa2, 0.0, ca2);
        glVertex3f(x3, 0.0, z3);
        glVertex3f(x3, height, z3);
    }
    glEnd();
    glPopMatrix();
}

// A single circular ring of specified radius
void ring(float radius)
{
    glPushMatrix();
    float angle1,angle2, ca1,sa1, ca2,sa2;
    float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
    float height = 0.2; // was 1
    float width = 0.05; // was .5
    glBegin(GL_QUADS);
    for(int i = 0; i < 360; i += 5)    //5 deg intervals
    {
        angle1 = i * toRad;       //Computation of angles, cos, sin etc
        angle2 = (i+5) * toRad;
        ca1=cos(angle1); ca2=cos(angle2);
        sa1=sin(angle1); sa2=sin(angle2);
        x1=(radius-width)*sa1; z1=(radius-width)*ca1;
        x2=(radius+width)*sa1; z2=(radius+width)*ca1;
        x3=(radius+width)*sa2; z3=(radius+width)*ca2;
        x4=(radius-width)*sa2; z4=(radius-width)*ca2;
        
        glNormal3f(0., height, 0.);       //Quad 1 facing up
        glVertex3f(x1, height, z1);
        glVertex3f(x2, height, z2);
        glVertex3f(x3, height, z3);
        glVertex3f(x4, height, z4);
        
        glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
        glVertex3f(x1, 0.0, z1);
        glVertex3f(x1, height, z1);
        glNormal3f(-sa2 ,0.0, -ca2);
        glVertex3f(x4, height, z4);
        glVertex3f(x4, 0.0, z4);
        
        glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
        glVertex3f(x2, height, z2);
        glVertex3f(x2, 0.0, z2);
        glNormal3f(sa2, 0.0, ca2);
        glVertex3f(x3, 0.0, z3);
        glVertex3f(x3, height, z3);
    }
    glEnd();
    glPopMatrix();
}


void ringAndCircle(float radius, bool shadow) {
    // draw circle that rotates around it
    if(!shadow){glColor4f(1.0, 1.0, 0.0, 1.0);}
    glPushMatrix();
    glRotatef(ring_turning_angle, 0.0, 1.0, 0.0);
    glTranslated(0, 0, radius); // move on track
    glutSolidSphere(0.6, 10, 10);
    glPopMatrix();
    
    // draw ring with radius 2
    glPushMatrix();
    if(!shadow){glColor4f(0.0, 0.0, 0.3, 1.0);}
    ring(radius);
    glPopMatrix();
}

void drawSweepModel(bool shadow) {
    glPushMatrix();
    const int N = 11;  // Total number of vertices on the base curve

    float vx_t[N] = {9.5, 8.2, 7, 6.2, 6, 6.2, 6.8, 7.6, 8.5, 8.7};
    float vy_t[N] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float vz_t[N] = {0};
    
    float wx[N], wy[N], wz[N];
    if(!shadow){glColor4f (0.5, 1.0, 1.0, 1.0);} // cyanish colour

    double theta = (-1 * M_PI)/180;
    glBegin(GL_TRIANGLE_STRIP);
    for(int s = 0; s < 360; s++){
        for(int i = 0; i < N; i++)
        {
            wx[i] = vx_t[i] * cos(theta) + vz_t[i] * sin(theta);
            wy[i] = vy_t[i];
            wz[i] = -vx_t[i] * sin(theta) + vz_t[i] * cos(theta);
        }

        for(int i = 0; i < N; i++)
        {

            if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
                             vx_t[i-1], vy_t[i-1], vz_t[i-1],
                             vx_t[i], vy_t[i], vz_t[i] );
            //glTexCoord2d(float(s)/360.0, float(i)/N);
            glVertex3f(vx_t[i], vy_t[i], vz_t[i]);


            if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],
                             vx_t[i], vy_t[i], vz_t[i],
                             wx[i], wy[i], wz[i] );

           // glTexCoord2f(float(s+1)/360.0, float(i)/N);
            glVertex3f(wx[i], wy[i], wz[i]);

        }

        for (int j = 0; j < N; j++){
            vx_t[j] = wx[j];
            vy_t[j] = wy[j];
            vz_t[j] = wz[j];
        }

    }
    glEnd();
    glPopMatrix();
}


void drawWing() {
    glPushMatrix();
    const int N = 16;		//Number of vertices of the base polygon
    float vy[N] = {0};
    float vx[N] = {0,1,2,3,3.8,4.1,4,3.8,3, 3.5 ,4, 3.6, 3,2,1,0};
    float vz[N] = {4.1, 4.8, 5, 5.2, 4.8, 4, 3.2, 2.8, 2.5, 2, 1.2, 0.6, 0.2, 0.3, 0.9, 1.3};
    
    GLfloat polygonVertices[] {
        0, 0, 4.1,
        1, 0, 4.8,
        2, 0, 5,
        3, 0, 5.2,
        3.8, 0, 4.8,
        4.1, 0, 4,
        4, 0, 3.2,
        3.8, 0, 2.8,
        3, 0, 2.5,
        4, 0, 1.2,
        3.6, 0, 0.6,
        3, 0, 0.2,
        2, 0, 0.3,
        1, 0, 0.9,
        0, 0, 1.3
        
    };
    float wx[N], wy[N], wz[N];
    double theta = (-1 * M_PI)/180;
    float height = 1;
    
    // wing cover top
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glNormal3f(0.0, 1.0, 0.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, polygonVertices);
    glDrawArrays(GL_POLYGON, 0, 15);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    
    // wing cover bottom
    glPushMatrix();
    glNormal3f(0.0, 1.0, 0.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, polygonVertices);
    glDrawArrays(GL_POLYGON, 0, 15);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    
    for(int j = 0; j < 1; j++){
        for(int i = 0; i < N; i++)
        {
            wx[i] = vx[i] * cos(theta) + vz[i] * sin(theta);
            wy[i] = vy[i] + height;
            wz[i] = -vx[i] * sin(theta) + vz[i] * cos(theta);
            
            if(i > 0) normal( vx[i-1], vy[i-1], vz[i-1],
                             vx[i], vy[i], vz[i],
                             wx[i], wy[i], wz[i] );
            
        }
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i < N; i++)
        {
            
            //glTexCoord2f(s[i], 0);
            glVertex3f(vx[i], vy[i], vz[i]);
            //glTexCoord2f(s[i], 1);
            glVertex3f(wx[i], wy[i], wz[i]);
            
            
        }
        
        //draw this when do extrra texture
        //glTexCoord2f(s[N], 0);
        //glVertex3f(vx[0], vy[0], vz[0]);
        //glTexCoord2f(s[N], 1);
        //glVertex3f(wx[0], wy[0], wz[0]);
        
        for(int i = 0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
        
        
        glEnd();
    }
    glPopMatrix();
    
    
}

void drawButterfly() {
    glPushMatrix();
    glRotatef(-butterAngle, 0, 0, 1);
    drawWing();
    glPopMatrix();
    
    // middle bit
    glPushMatrix();
    glTranslatef(0, 0.5, 3);
    glScalef(0.5, 0.5, 5);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glRotatef(-180 + butterAngle, 0, 0, 1);
    drawWing();
    glPopMatrix();
}

void drawStand() {
    glPushMatrix();
    
    const int N = 9;		//Number of vertices of the base polygon
    float vz[N] = {2, 1, 0, -1, -2, -1, 1, 2};
    float vx[N] = {0, 1, 1, 1, 0, -1, -1, 0};
    float vy[N] = {0};
    
    float wx[N], wy[N], wz[N];


    double theta = (0 * M_PI)/180;
    float height = 5;
    glNormal3f(0.0, 1.0, 0.0);
    //glEnable(GL_TEXTURE_2D);
    for(int j = 0; j < 1; j++){
        
        for(int i = 0; i < N; i++)
        {
            wx[i] = vx[i] * cos(theta) + vz[i] * sin(theta);
            wy[i] = vy[i] + height; // was 20
            wz[i] = -vx[i] * sin(theta) + vz[i] * cos(theta);
            
            if(i > 0) normal( vx[i-1], vy[i-1], vz[i-1],
                             vx[i], vy[i], vz[i],
                             wx[i], wy[i], wz[i] );
            
        }
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i < N; i++)
        {
            
            //glTexCoord2f(s[i], 0);
            glVertex3f(vx[i], vy[i], vz[i]);
            //glTexCoord2f(s[i], 1);
            glVertex3f(wx[i], wy[i], wz[i]);
            
            
        }
        
        //draw this when do extrra texture
        //glTexCoord2f(s[N], 0);
        //glVertex3f(vx[0], vy[0], vz[0]);
        //glTexCoord2f(s[N], 1);
        //glVertex3f(wx[0], wy[0], wz[0]);
        
        for(int i = 0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
        
        glEnd();
    }
    glPopMatrix();
    
}


void drawDodecPlatform(bool shadow) {
    // draw turning dodecahedron
    
    glPushMatrix();
    if(!shadow){glColor3f (1, 1, 1);} // white colour
    glTranslatef(0, 10, 0);
    glRotatef(dodec_angle, 0, 1, 0);
    glScalef(1.2, 1.2, 1.2);
    glutSolidDodecahedron();
    glPopMatrix();
    
    // draw honeycomb extruded object
    
    glPushMatrix();
    if(!shadow){glColor3f (1, 0, 0);} // red colour
    glScalef(1.2, 1.2, 1.2);
    drawStand();
    glPopMatrix();
    
    // draw flattened cube for top part
    glPushMatrix();
    glTranslatef(0, 6, 0);
    glScalef(1.9, 0.1, 1.9);
    glutSolidCube(3);
    glPopMatrix();
}

void drawTeaStand() {
    //teapot
    glPushMatrix();
    glColor3f(0.541, 0.686, 1);
    glTranslatef(0, 7.5, 0);
    glutSolidTeapot(2);
    glPopMatrix();
    
    // draw stand
    glPushMatrix();
    glColor3f (1, 0, 0); // red colour
    glScalef(1.2, 1.2, 1.2);
    drawStand();
    glPopMatrix();
    
    // draw flattened cube for top part
    glPushMatrix();
    glTranslatef(0, 6, 0);
    glScalef(1.9, 0.1, 1.9);
    glutSolidCube(3);
    glPopMatrix();
}

void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);
    glPushMatrix();
    
    glTranslatef(0.0, 4.0, 0.0);
    glScalef(20.0, 2.0, 10.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();					//Connector
    
    glTranslatef(11.0, 4.0, 0.0);
    glutSolidCube(2.0);
    glPopMatrix();
    
    //Wheels
    glColor4f(0.5, 0., 0., 1.0);
    glPushMatrix();
    
    glTranslatef(-8.0, 2.0, 5.1);
    gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
    
    glTranslatef(8.0, 2.0, 5.1);
    gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
    
    glTranslatef(-8.0, 2.0, -5.1);
    glRotatef(180.0, 0., 1., 0.);
    gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
    
    glTranslatef(8.0, 2.0, -5.1);
    glRotatef(180.0, 0., 1., 0.);
    gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
}

void stick() {
    glPushMatrix();
    glTranslatef(5, 30, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 1.0, 1.0, 20.0, 20, 5);
    glPopMatrix();
}


void drawRover()
{
    base();
    
    // the stick
    glPushMatrix();
    glTranslatef(11, 35, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 1.0, 1.0, 30.0, 20, 5);
    glPopMatrix();
    
    // the cone
    glPushMatrix();
    
    glTranslatef(18, 35, 0);
    glRotatef(-90, 0, 1, 0);
    gluCylinder(q, 10, 0, 10, 20, 8);
    glPopMatrix();

    // the light in cone
    glPushMatrix();
    glColor3f(0.996, 1, 0.639 );
    glTranslatef(18, 35, 0);
    glutSolidSphere(2, 10, 10);
    glPopMatrix();
    // the floaty
    glColor4f(0.0, 1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 10.0, 0.0);
    glScalef(18.0, 5.0, 10.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
}

void drawItem(bool shadow) {
    
    drawDodecPlatform(shadow);
    
    // draw sweep model
    glPushMatrix();
    glScalef(0.4, 0.6, 0.4);
    drawSweepModel(shadow);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 2.6, 0);
    ringAndCircle(3.5, shadow);
    glPopMatrix();
    
}



void drawParabola() {
    
    const int P = 41;
    float y1[P] = {-20.0, -18.05, -16.2, -14.45, -12.8, -11.25, -9.8, -8.45, -7.2, -6.05, -5.0, -4.05, -3.2, -2.45, -1.8, -1.25, -0.8, -0.45, -0.2, -0.05, -0.0, -0.05, -0.2, -0.45, -0.8, -1.25, -1.8, -2.45, -3.2, -4.05, -5.0, -6.05, -7.2, -8.45, -9.8, -11.25, -12.8, -14.45, -16.2, -18.05, -20.0};
    float x1[P] = {-20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    
    float width = 2;
    float move_up = 20; // set this to min/max y value positive so that rainbow can be shown on floor
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i < P; i++) {
        glVertex3f(x1[i], y1[i] + width + move_up, 0);
        glVertex3f(x1[i] + width, y1[i] + width + move_up, 0);
    }
    glEnd();
    
    
}


void drawRainbow() {
    glPushMatrix();
    glTranslatef(0, 0, -20);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    glColor3f(1, 0, 0);
    halfRing(26);
    glColor3f(1, 0.478, 0.019);
    halfRing(25);
    glColor3f(1, 1, 0);
    halfRing(24);
    glColor3f(0, 1, 0);
    halfRing(23);
    glColor3f(0,0,1);
    halfRing(22);
    glColor3f(0.294, 0, 0.509);
    halfRing(21);
    glColor3f(0.498, 0, 1);
    halfRing(20);
    glPopMatrix();
    
}

void drawTV(bool shadow) {
    
  
    //screen
    glPushMatrix();
    if(!shadow){
        glEnable(GL_TEXTURE_2D);
        glColor3f (1, 1, 1); // white colour
        glTranslatef(0, 0, 1.6);
        //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
        glBindTexture(GL_TEXTURE_2D, txId[6]); // tv picture
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
        glBegin(GL_POLYGON);
        glTexCoord2f(0,  0); glVertex3f(-5, 1, 0.0);
        glTexCoord2f(1,  0); glVertex3f(5, 1, 0.0);
        glTexCoord2f(1,  1); glVertex3f(5, 7.4, 0.0);
        glTexCoord2f(0,  1); glVertex3f(-5, 7.4, 0.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        //glEnable(GL_LIGHTING);
    }
    else {
        glTranslatef(0, 0, 1.6);
        
        glBegin(GL_POLYGON);
        glVertex3f(-5, 1, 0.0);
        glVertex3f(5, 1, 0.0);
        glVertex3f(5, 7.4, 0.0);
        glVertex3f(-5, 7.4, 0.0);
        glEnd();
        //glEnable(GL_LIGHTING);
    }
    
    glPopMatrix();
    
    // box
    glPushMatrix();
    if(!shadow) {
        glColor3f(1, 1, 1);
    }
    glTranslatef(0, 4, 0);
    glScalef(4, 3, 1);
    glutSolidCube(3);
    glPopMatrix();
    
    //sticks
    glPushMatrix();
    glTranslatef(5, 0, 0);
    glRotatef(45, 0, 0, 1);
    glScalef(0.1, 0.5, 0.1);
    stick();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-6, 0.7, 0);
    glRotatef(-45, 0, 0, 1);
    glScalef(0.1, 0.5, 0.1);
    stick();
    glPopMatrix();
    
    
}


void flyRobotTimer() {
    // walk 13 steps on x axis
    if (animation_time < 13) {
        bot_move_y++; //lift off
    }
    // then turn forward
    else if (animation_time == 13) {
        bot_turn_y -= 36; // turn to arc
    }
    else if (animation_time < 18) {
        bot_turn_y -= 36; // turn to arc more
    }
    else if (animation_time < 20) {
        bot_turn_x -= 40; // move body down under arc
    }
    else if (animation_time < 25) {
        bot_move_z -= 2; // move towards arc
    }
    else if (animation_time < 30) {
        bot_move_z -= 2; // move towards arc
        bot_turn_y += 35;
    }
    else if (animation_time < 34) {
        bot_move_z -= 2; // move towards arc
        bot_move_y++;
    }
    else if (animation_time < 44) {
        bot_turn_x += 10; // fly up to arch back wards
        bot_move_y++;
    }
    else if (animation_time < 46) {
        bot_turn_x += 5; // fly up to arch back wards
        bot_move_y+= 0.5;
    }
    else if (animation_time < 52) {
        bot_turn_x += 10; // fly up to arch back wards
        bot_move_y+= 0.5;
    }
    else if (animation_time < 54) {
        bot_move_z +=3;
        bot_turn_x += 5;
    }
    else if (animation_time < 58) {
        bot_turn_x += 10;
        bot_move_z ++;
        bot_move_y--;
    }
    else if (animation_time < 64) {
        bot_turn_x += 15;
        bot_move_z +=.5;
        bot_move_y--;
    }
    else if (animation_time < 68) {
        bot_turn_x += 15;
        bot_move_z -=.5;
        bot_move_y -= 1.3;
    }
    else if (animation_time < 74) {
        bot_move_z -= 1.5;
        bot_turn_x += 12;
        bot_move_y++;
    }
    else if (animation_time < 77) {
        bot_move_y++;
    }
    else if (animation_time < 127) {
        bot_move_y-=0.5;
    }
    else if (animation_time < 138) {
        
    }
    else if (animation_time < 170){
        bot_move_y++;
    }
    else if (animation_time < 180) {
        //bot_move_x++;
        bot_move_y+=0.2;
        bot_turn_z+=10;
    }
    else if( animation_time < 190){
        //bot_turn_z+=2;
        bot_turn_x+=7;
        bot_move_x-=0.2;
    }
    else{
        bot_turn_after += 3;
    }

}

void moveRoverTimer(int value) {
    
    
    if (animation_time < 10) {
        rover_move_x ++;
    }
    else if (animation_time < 14) {
        rover_turn_y ++;
        rover_move_x ++;
    }
    else if (animation_time < 20) {
        rover_turn_y--;
        rover_move_x++;
    }
    else if (animation_time < 40) {
        rover_move_x++;
    }
    else if (animation_time < 50) {
        rover_turn_y -= 19;
    }
    else if (animation_time < 60) {
        rover_move_x-=2;
    }
    else if (animation_time < 70) {
        rover_turn_y -= 20;
    }
    else if(animation_time < 80) {
        rover_move_x ++;
    }
    else if(animation_time < 90){
        rover_turn_y+=20;
    }
    else if(animation_time < 100) {
        rover_move_x-=2;
    }
    else if (animation_time < 110) {
        rover_turn_y += 30;
    }
    else if (animation_time < 126) {
        rover_move_z--;
    
    }
    else if (animation_time < 134) {
        rover_turn_y +=18;
    }
    else {
        rover_turn_y_after+= 2;
    }
}

void walkBotTimer() {
    if (animation_time < 13) {
        walk_x++;
    }
    // then turn forward
    else if (animation_time == 13) {
        walk_dir_angle = 0;
    }
    // walk few steps on z axis
    else if (animation_time < 20) {
        walk_z++;
    }
}


void myTimer(int value) {
    // walking bot
    //walkBotTimer();
    
    // fly robot
    flyRobotTimer();
    
    // move rover
    moveRoverTimer(value);
    
    // for the item
    ring_turning_angle+=5;
    dodec_angle+=50;
    butterTurn+=1;
    animation_time++; // increment timer of animation
    if(value == 0){
        walk_theta-=40;
        butterAngle-=30;
        glutPostRedisplay();
        glutTimerFunc(220, myTimer, 1);
    }
    else if(value == 1){
        walk_theta+=40;
        butterAngle+=30;
        glutPostRedisplay();
        glutTimerFunc(220, myTimer, 0);
    }
    else {
        glutPostRedisplay();
        glutTimerFunc(220, myTimer, 0);
    }

}

void drawShadowObjects(bool shadow) {

    // draw tv
    glPushMatrix();
    glTranslatef(-10, 0, -40);
    glScalef(0.8, 0.8, 0.8);
    drawTV(shadow);
    glPopMatrix();

    
    
    glPushMatrix();
    glTranslatef(0, 0, -20);
    glScalef(0.8, 0.8, 0.8);
    // draw robot flying
    glPushMatrix();
    glRotatef(bot_turn_after, 0, 1, 0);
    glTranslatef(bot_move_x, bot_move_y, bot_move_z);
    glRotatef(bot_turn_x, 1, 0, 0);
    glRotatef(bot_turn_y, 0, 1, 0);
    glRotatef(bot_turn_z, 0, 0, 1);
    attachBoosters(shadow);
    drawFlyingBot(shadow);
    glPopMatrix();
    
//    // draw rainbow
//    glPushMatrix();
//    drawRainbow();
//    glPopMatrix();
    
    glPopMatrix();
    
    
    glPopMatrix();
    // draw walking robot
    glPushMatrix();
    glTranslatef(walk_x + 10, 11.5, walk_z-23); // walk
    glRotatef(walk_dir_angle, 0, 1, 0);
    glScalef(0.7, 0.7, 0.7);
    drawWalkingbot(shadow);
    glPopMatrix();
    
    // space decor
    glPushMatrix();
    glTranslatef(10, 0, -23); // make sure walk robot got same z
    drawItem(shadow);
    glPopMatrix();
    
    
}

//------------SHADOWS----------------------------------------------
void makeShadows() {

    //  Draw Object
    glEnable(GL_LIGHTING);
    glColor3f(1, 0, 1);
    glPushMatrix();
    //glTranslatef(0, 3, 0);
    glRotatef(shadow_angle, 1, 0, 0);
    glRotatef(shadow_angle*2, 0, 1, 0);
    glColor3f(1, 0, 1);
    drawShadowObjects(false);
    glPopMatrix();
    // Draw the shadow
    
    float lx = 80; float ly = 80.0; float lz = 0;
    float shadowMat[16] = { ly,0,0,0, -lx,0,-lz,-1,
        0,0,ly,0,  0,0,0,ly };
    
    glDisable(GL_LIGHTING);
    glPushMatrix(); //Draw Shadow Object
    glMultMatrixf(shadowMat);
    
    // Transformations
    glColor4f(0.2, 0.2, 0.2, 1.0);
    //glEnable(GL_LIGHTING);
    glColor3f(0, 0, 0);
    glPushMatrix();
    //glTranslatef(0, 3, 0);
    glRotatef(shadow_angle, 1, 0, 0);
    glRotatef(shadow_angle*2, 0, 1, 0);
    //glColor3f(0, 0, 0);
    drawShadowObjects(true);
    glPopMatrix();
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawButterflies() {
    // butterfly
    glPushMatrix();
    glColor3f(0, 1, 0.4);
    glRotatef(butterTurn, 0, 1, 0);
    glTranslatef(0, 30, -40);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    drawButterfly();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 0, 1);
    glRotatef(-butterTurn, 0, 1, 0);
    glTranslatef(10, 30, -40);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    drawButterfly();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.7, 0.5, 1);
    glRotatef(butterTurn, 0, 1, 0);
    glTranslatef(-10, 29, -40);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    drawButterfly();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 0.5, 0.2);
    glRotatef(-butterTurn, 0, 1, 0);
    glTranslatef(10, 29, 40);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    drawButterfly();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 1, 0.2);
    glRotatef(butterTurn, 0, 1, 0);
    glTranslatef(15, 28, -20);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    drawButterfly();
    glPopMatrix();
    
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
    updateNormalLookXYZ();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    
    float lpos[4] = {10., 10., 10., 1.0};  //light's position

  	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //Set light position
  	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
    glEnable(GL_LIGHT0);


    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,20);

    makeShadows();
    drawButterflies();
    
    // light on rover
    float lgt1_pos[] = {0  , 5, 5.0f, 1.0f};  //light1 position
    float lgt1_dir[] = {-1.0, -1.0, 0.0};
    
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);   //light1 position
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt1_dir); //light1 direction
    glPopMatrix();
    
    // draw rover moving
    glPushMatrix();
    glRotatef(rover_turn_y_after, 0, 1, 0);
    glTranslatef(rover_move_x - 25, rover_move_y, rover_move_z - 10);
    glRotatef(rover_turn_x, 1, 0, 0);
    glRotatef(rover_turn_y, 0, 1, 0);
    glRotatef(rover_turn_z, 0, 0, 1);
    glScalef(0.1, 0.1, 0.1);
    drawRover();
    //its light
    glTranslatef(50, 0, 0);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);   //light1 position
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt1_dir); //light1 direction
    glPopMatrix();

    // teapot on stand
    glPushMatrix();
    glTranslatef(13, 0, -40);
    drawTeaStand();
    glPopMatrix();
    
    // rainbow
    glPushMatrix();
    glTranslatef(0, 0, -20);
    glScalef(0.8, 0.8, 0.8);
    drawRainbow();
    glPopMatrix();
    
    drawFloor();
    skybox();
    
    glutSwapBuffers(); //Useful for animation
}

void special(int key, int x, int y)
{
    float dir_x = -sin(h_look_angle);
    //float dir_y = 0; 
    float dir_z = -cos(h_look_angle);
    float s = 2; // was 0.1

    if(key == GLUT_KEY_LEFT) {
        h_look_angle += 0.1;   //in radians
    }
    else if(key == GLUT_KEY_RIGHT) {
        h_look_angle -= 0.1;
    }
    else if(key == GLUT_KEY_DOWN) {
      eye_x = eye_x - s * dir_x;
      //eye_y = eye_y - s * dir_y;
      eye_z = eye_z - s * dir_z;
    }
    else if(key == GLUT_KEY_UP) {
      eye_x = eye_x + s * dir_x;
      //eye_y = eye_y + s * dir_y;
      eye_z = eye_z + s * dir_z;
    }
    // check boundaries
    float bound = 20;
    if (eye_z > bound) {
        eye_z = bound;
    }
    if (eye_z < -bound) {
        eye_z = -bound;
    }
    if (eye_x > bound) {
        eye_x = bound;
    }
    if (eye_x < -bound) {
        eye_x = -bound;
    }
    updateNormalLookXYZ();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int xmouse, int ymouse){
    switch (key){
        case 'b':
            eye_x = -20;  eye_y = 30;  eye_z = 20;
            break;
            
        case 'g':
            eye_x = -20;  eye_y = 10;  eye_z = 20;
            break;
            
        default:
            break;
    }
    
}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (700, 700);
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Assignment 1");
    initialize();
    glutDisplayFunc(display);
    glutTimerFunc(220, myTimer, 0);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
