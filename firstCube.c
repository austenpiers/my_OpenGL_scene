//#include
#include <math.h>      // Math Func.
#include <GL/gl.h>     // 3-d Drawing
#include <GL/glu.h>    // Camera Control
#include <GL/glut.h>   // Windowing System
#include <stdio.h>

GLint i,j,w_w,w_h,iterate;
static GLuint SHAD_TEX, WOOD_TEX;
GLubyte *SHADOW_PIXELS;
unsigned char*   WOOD_PIXELS;
int tick=0,tock=0, tic=-1000,toc=0, startx=0,starty=0,moving=0,eye=1,LGT=1;
float Obj_P[]={0,0,0};
static void (*E[6])(void);
static void (*O[6])(void);
static void (*Es[6])(int);
static void (*Os[6])(int);
GLfloat A,B,C,Ez=50.0,Ey=0.0,Ex=0.0,ROTx=0,ROTy=0;
static GLfloat lightColor[] = {0.8,0.8,0.8, 1.0};
static GLfloat light_position[]={0,120,20,1};
static GLfloat g[] = {0.1, 0.76, 0.15, 0.1};
static GLfloat b[] = {0.1, 0.2, 0.73, 0.1};
static GLfloat o[] = {1, 0.4, 0, 0.1};
static GLfloat r[] = {0.86, 0.15, 0.1, 0.1};
static GLfloat y[] = {0.9, 0.8, 0, 0.1};
static GLfloat t[] = {0.1, 0.8, 1, 0.1};
static GLfloat p[] = {1, 0.1, 1, 0.1};
static GLfloat w[] = {1, 1, 1, 0.1};
static GLfloat n[] = {1, 1, 1, 0.1};
static GLfloat pu[] = {0.745, 0.4706, 0.175, 1.0};

GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };


void PPMREAD(char* fileName){
  FILE *fp; fp = fopen(fileName, "r"); int reading=3, rw=1,m; char c;
  if(fp==NULL) printf("didnt work mang\n");
  while(reading>0){
    fscanf(fp,"%c",&c); 
    //return;
    if(c=='\n'){
      reading-=1; continue;
    }
    if(reading>2) continue;
    if(reading>1){
      if(c==' ') {rw--;  continue;}
      if(rw>0) w_w=w_w*10+(c-48);
      else w_h=w_h*10+(c-48);
      continue;
    }
    else{
      m=m*10+(c-48);
    }
  }
  int t = w_w*w_h*3,i;
  WOOD_PIXELS = malloc(t);

  for(i=0;i<t;++i){
    fscanf(fp,"%c",&c); 
    WOOD_PIXELS[i]=c;
  }
}

void Shadow( void (*f)(int) ) {
	GLfloat V[]={Obj_P[0]/2-light_position[0],Obj_P[1]/2-light_position[1],Obj_P[0]/2-light_position[2]},
		V_Mag=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]),
		U[]={V[2]/V_Mag,0,-V[0]/V_Mag}, 
		W[]={-U[2],0,U[0]},
		flo=-4,
		Obj_Hi = Obj_P[1]-flo,
		Theta = acos(V[1]/V_Mag),
		W_Tmp = tan(Theta)*Obj_Hi,
		W_Mock = (V_Mag+sqrt(Obj_Hi*Obj_Hi+W_Tmp*W_Tmp))/V_Mag,
		W_Push = W_Tmp - 4/3*Theta*W_Mock;

	glPushMatrix();
	  glTranslatef(W[0]*W_Push+Obj_P[0], -Obj_Hi+Obj_P[1], -W[2]*W_Push+Obj_P[2]);
	  glScalef(W[0]*W_Push/20+1, 1, W[2]*W_Push/20+1);
          glRotated((GLint) ROTx,0,1,0);
    	  glRotated((GLint) ROTy,1,0,0);
	  glRotatef(Theta, U[0], U[1], U[2]);
	  //(*f)(3);
	glPopMatrix();
}

int isEven(int e){
  if(e%2==0) return(1);
  return(-1);
}

static void NullFunc(void){return;}
static void NullFunci(int col){return;}

static void drawRect(void){
  glShadeModel(GL_FLAT);
  A=1.0;
  glPushMatrix();
  glBegin(GL_QUAD_STRIP);
    
    glNormal3f(-1,0,0);
    glVertex3f(-A,A,0.25);
    glVertex3f(-A,A,-0.25);
    glVertex3f(-A,-A,0.25);
    glVertex3f(-A,-A,-0.25);

    glNormal3f(0,-1,0);
    glVertex3f(A,-A,0.25);
    glVertex3f(A,-A,-0.25);

    glNormal3f(1,0,0);
    glVertex3f(A,A,0.25);
    glVertex3f(A,A,-0.25);

    glNormal3f(0,1,0);
    glVertex3f(-A,A,0.25);
    glVertex3f(-A,A,-0.25);
  glEnd();
  glPushMatrix();
  for(i=-1;i<2;i+=2){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
      glVertex3f(A,-A,i*0.25);
      glVertex3f(A,A,i*0.25);
      glVertex3f(-A,A,i*0.25);
      glVertex3f(-A,-A,i*0.25);
    glEnd();
  }
  glPopMatrix();
  glPopMatrix();

}

static void drawTrap(void){
  glShadeModel(GL_FLAT);
  A=1.0;B=0.4;C=0.5;
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
      if(i==1)glNormal3f(0,0,1);
      else    glNormal3f(0,0,-1);
      glVertex3f(0.0,A,i*C);
      glVertex3f(B,2*B,i*C);
      glVertex3f(B,2*-B,i*C);
      glVertex3f(0.0,-A,i*C);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);

      glNormal3f(1,3,0);
      glVertex3f(0.0,A,C);
      glVertex3f(0.0,A,0.0);
      glVertex3f(B,2*B,C);
      glVertex3f(B,2*B,0.0);

      glNormal3f(1,0,0);
      glVertex3f(B,2*-B,C);
      glVertex3f(B,2*-B,0.0);

      glNormal3f(-1,-3,0);
      glVertex3f(0.0,-A,C);
      glVertex3f(0.0,-A,0.0);

      glNormal3f(-1,0,0);
      glVertex3f(0.0,A,C);
      glVertex3f(0.0,A,0.0);
  glEnd();
}

static void drawParl(void){
  glShadeModel(GL_FLAT);
  A=1.0;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
      glNormal3f(0,0,i);
      glVertex3f(-A,A,i*0.5);
      glVertex3f(0.0,A,i*0.5);
      glVertex3f(A,-A,i*0.5);
      glVertex3f(0.0,-A,i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
    glNormal3f(0,1,0);
    glVertex3f(-A,A,0.5);
    glVertex3f(-A,A,0.0);
    glVertex3f(0.0,A,0.5);
    glVertex3f(0.0,A,0.0);

    glNormal3f(3,1,0);
    glVertex3f(A,-A,0.5);
    glVertex3f(A,-A,0.0);

    glNormal3f(0,-1,0);
    glVertex3f(0.0,-A,0.5);
    glVertex3f(0.0,-A,0.0);

    glNormal3f(-3,-1,0);
    glVertex3f(-A,A,0.5);
    glVertex3f(-A,A,0.0);
  glEnd();
  glPopMatrix();
}

static void drawSq(void){
  glShadeModel(GL_FLAT);
  A=1.0;B=0.635;C=0.35;
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
      glNormal3f(0,0,i);
      glVertex3f(0.0,A,i*0.5);
      glVertex3f(-A,A,i*0.5);
      glVertex3f(0.0,-A,i*0.5);

      glVertex3f(A/4,-A,i*0.5);
      glVertex3f(B,-C,i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
    glNormal3f(0,1,0);
    glVertex3f(0.0,A,0.5);
    glVertex3f(0.0,A,0.0);
    glVertex3f(-A,A,0.5);
    glVertex3f(-A,A,0.0);

    glNormal3f(-3,-1,0);
    glVertex3f(0.0,-A,0.5);
    glVertex3f(0.0,-A,0.0);

    glNormal3f(0,-1,0);
    glVertex3f(A/4,-A,0.5);
    glVertex3f(A/4,-A,0.0);

    glNormal3f(1,-1,0);
    glVertex3f(B,-C,0.5);
    glVertex3f(B,-C,0.0);

    glNormal3f(3,1,0);
    glVertex3f(0.0,A,0.5);
    glVertex3f(0.0,A,0.0);
  glEnd();

}

static void drawDim(void){
  glShadeModel(GL_FLAT);
  A=1.0;B=0.635;C=0.35;
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
      glNormal3f(0,0,i);
      glVertex3f(0.0,A,i*0.5);
      glVertex3f(-A,A,i*0.5);
      glVertex3f(-A/4,A,i*0.5);
      glVertex3f(-B,C,i*0.5);
      glVertex3f(0.0,-A,i*0.5);
      glVertex3f(A/4,-A,i*0.5);
      glVertex3f(B,-C,i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
    glNormal3f(0,1,0);
    glVertex3f(0.0,A,0.5);
    glVertex3f(0.0,A,0.0);
    glVertex3f(-A/4,A,0.5);
    glVertex3f(-A/4,A,0.0);

    glNormal3f(-1,1,0);
    glVertex3f(-B,C,0.5);
    glVertex3f(-B,C,0.0);

    glNormal3f(-1,-2,0);
    glVertex3f(0.0,-A,0.5);
    glVertex3f(0.0,-A,0.0);

    glNormal3f(0,-1,0);
    glVertex3f(A/4,-A,0.5);
    glVertex3f(A/4,-A,0.0);

    glNormal3f(1,-1,0);
    glVertex3f(B,-C,0.5);
    glVertex3f(B,-C,0.0);

    glNormal3f(1,2,0);
    glVertex3f(0.0,A,0.5);
    glVertex3f(0.0,A,0.0);
  glEnd();
}
static void drawCirc(void){
  glShadeModel(GL_SMOOTH);
  A=0.39269;C=6.28;
  glPushMatrix();
  
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),0);
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawCircle(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  drawCirc();
}


static void drawTad(void){
  A=0.26;C=1.70;
  glShadeModel(GL_FLAT);
  glPushMatrix();

  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
    glVertex3f(0.0,1.0,i*0.5);
    glVertex3f(0.0,1.25,i*0.5);
    glVertex3f(1.35,1.25,i*0.5);
    glVertex3f(1.35,0.0,i*0.5);
    glVertex3f(1.2,0.0,i*0.5);
    glEnd();
  }
  
  glBegin(GL_QUAD_STRIP);
    glNormal3f(-1,0,0);
    glVertex3f(0.0,1.0,0.5);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.25,0.5);
    glVertex3f(0.0,1.25,0.0);
    
    glNormal3f(0,1,0);
    glVertex3f(1.35,1.25,0.5);
    glVertex3f(1.35,1.25,0.0);

    glNormal3f(1,0,0);
    glVertex3f(1.35,0.0,0.5);
    glVertex3f(1.35,0.0,0.0);

    glNormal3f(0,-1,0);
    glVertex3f(1.2,0.0,0.5);
    glVertex3f(1.2,0.0,0.0);

    glNormal3f(-1,-1,0);
    glVertex3f(0.0,1.0,0.5);
    glVertex3f(0.0,1.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawPlus(void){
  if(LGT) glColor4fv(p);
  else glColor4fv(n);
  glPushMatrix();
    glScalef(0.7,0.25,1);
    drawRect();
  glPopMatrix();

  glPushMatrix();
    glScalef(0.25,0.7,1);
    drawRect();
  glPopMatrix();
}
static void drawMinus(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glPushMatrix();
    glScalef(0.7,0.25,1);
    drawRect();
  glPopMatrix();
}
static void drawTimes(void){
  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  glPushMatrix();
    glRotated(45,0,0,1);
    drawPlus();
  glPopMatrix();
}
static void drawDiv(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  glPushMatrix();
    glScalef(0.7,0.125,1);
    drawRect();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0,0.5,-0.25);
    glScalef(0.25,0.25,1);
    drawCirc();
  glPopMatrix(); 
  glPushMatrix();
    glTranslatef(0,-0.5,-0.25);
    glScalef(0.25,0.25,1);
    drawCirc();
  glPopMatrix(); 
}
static void drawCross(){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0,0.5,0);
    glScalef(0.7,0.125,1);
    drawRect();
  glPopMatrix();
  glPushMatrix();
    glScalef(0.25,1,1);
    drawRect();
  glPopMatrix();
}

static void drawTriangle(void){
  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  glShadeModel(GL_SMOOTH);
  A=2.094395;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
      glNormal3f((GLdouble) -cos(B+A),(GLdouble) -sin(B+A),0);

    for(i=1;i>-1;--i){
//      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),0);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glNormal3f(1,-2,0);
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawHebStar(void){
  if(LGT) glColor4fv(o);
  else glColor4fv(n);
  glPushMatrix();
  glRotatef(30,0,0,1);
  drawTriangle();
  glPushMatrix();
    glScaled(-1,1,1);
    drawTriangle();
  glPopMatrix();
  glPopMatrix();
}
static void drawSquare(void){
  glShadeModel(GL_FLAT);
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  A=1.570796;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}

static void drawPentagon(void){
  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  glShadeModel(GL_FLAT);
  A=1.2566;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,(GLdouble) i);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawHexagon(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glShadeModel(GL_FLAT);
  A=1.0471976;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    glNormal3f(0,0,i);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawHeptagon(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  glShadeModel(GL_FLAT);
  A=0.897598;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    for(B=0; B<C; B+=A)
      glNormal3f(0,0,i);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawOctagon(void){
  if(LGT) glColor4fv(p);
  else glColor4fv(n);
  glShadeModel(GL_FLAT);
  A=0.78539816;C=6.28;
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    if(i==1) glNormal3f(0,0,1);
    else     glNormal3f(0,0,-1);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
    glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),0);
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawNonagon(void){
  A=0.6981317;C=6.28;
  glShadeModel(GL_FLAT);
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    for(B=0; B<C; B+=A)
      glNormal3f(0,0,i);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}
static void drawDecagon(void){
  A=0.6283185;C=6.28;
  glShadeModel(GL_FLAT);
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_POLYGON);
    for(B=0; B<C; B+=A)
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    for(i=1;i>-1;--i){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}

static void drawCurvedEdge(void){
  A=0.2617;C=1.66;
  glShadeModel(GL_FLAT);
  glPushMatrix();
  for(i=1;i>-1;--i){
    glBegin(GL_TRIANGLE_STRIP);
    for(B=0; B<C; B+=A){
      if(i==1){
	glNormal3f(0,0,1);
	glVertex3f((GLdouble) cos(B)*0.5,(GLdouble) sin(B)*0.5,(GLdouble) i*0.5);
	glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
	continue;
      }
      glNormal3f(0,0,-1);
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
      glVertex3f((GLdouble) cos(B)*0.5,(GLdouble) sin(B)*0.5,(GLdouble) i*0.5);
    }
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){

    glNormal3f((GLdouble) cos(B),(GLdouble) sin(B),0);
    for(i=1;i>-1;--i){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B),(GLdouble) i*0.5);
    }
  }
  glNormal3f(-1,0,0);
  for(B=B-A; B>-0.26; B-=A){
    for(i=1;i>-1;--i){
      glVertex3f((GLdouble) cos(B)*0.5,(GLdouble) sin(B)*0.5,(GLdouble) i*0.5);
    }
    glNormal3f((GLdouble) -cos(B),(GLdouble) -sin(B),0);
  }
  glNormal3f(0,-1,0);
  glVertex3f(1.0,0.0,0.5);
  glVertex3f(1.0,0.0,0.0);
  glEnd();
  glPopMatrix();
}

static void drawCurvedEdgeh(void){

    drawCurvedEdge();
    glPushMatrix();
      glTranslatef(0.01,0,0);
      glScaled(-1,1,1);
      drawCurvedEdge();
    glPopMatrix();
}

static void drawCurvedEdget(void){

    drawCurvedEdge();
    glPushMatrix();
      glScaled(1,-1,1);
      drawCurvedEdgeh();
    glPopMatrix();
}

static void drawCurvedEdgef(void){

    drawCurvedEdgeh();
    glPushMatrix();
      glScaled(1,-1,1);
      drawCurvedEdgeh();
    glPopMatrix();
}

static void drawEdgedParl(void){

    glPushMatrix();
      drawParl();
      drawCurvedEdgeh();
    glPopMatrix();
}

// ================ SRARTNUMBERS ============== 

static void draw0(void){

  if(LGT) glColor4fv(t);
  else glColor4fv(n);
    glPushMatrix();
      glScaled(1.0,1.5,1.0);
      drawCurvedEdgef();
    glPopMatrix();
}

static void draw9(void){
 
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
    glPushMatrix();
      glTranslatef(0.0,0.6,0.0);
      drawCurvedEdgef();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0,-0.5,0);
      glScaled(1,-1,1);
      drawCurvedEdgeh();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.75,0.0,0.25);
      glScalef(0.25,0.5,1);
      drawRect();
    glPopMatrix();
}
static void draw8(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  glTranslatef(0,-0.75,0);
  glScalef(0.9,0.9,1.0);
  
    drawCurvedEdgef();
    glPushMatrix();
      glTranslatef(0.0,1.5,0.0);
      glScalef(0.9,0.9,1.0);
      drawCurvedEdgef();
    glPopMatrix();
}

static void draw7(void){

  if(LGT) glColor4fv(y);
  else glColor4fv(n);
    glPushMatrix();
      glTranslatef(0.925,-0.075,0);
      glScalef(-1.25,1.25,1.0);
      drawCurvedEdge();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0,1.175,0.25);
      glScalef(0.925,0.3,1);
      drawRect();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.02,-0.825,0.25);
    glScalef(0.31,0.75,1);
    drawRect();
    glPopMatrix();
}

static void draw6(void){

  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glRotated(180,0,0,1);
  draw9();
}

static void draw5(void){

  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  glTranslatef(0.0,-0.5,0.0);
  glScalef(1.0,1.1,1.0);

  drawCurvedEdget();

  glPushMatrix();
  glScalef(0.4,0.25,1);
  glTranslatef(-0.9,3.0,0.25);
  drawRect();
  glPopMatrix();

  glPushMatrix();
  glScalef(0.25,0.5,1);
  glTranslatef(-2.6,2.0,0.25);
  drawRect();
  glPopMatrix();
  
  glPushMatrix();
  glScalef(0.9,0.25,1);
  glTranslatef(0.0,6.0,0.25);
  drawRect();
  glPopMatrix();
}
static void draw4(void){
  
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  glPushMatrix();
  glScalef(1.0,0.9,1.0);
  glTranslatef(-0.3,0.9,0.0);
  glRotated(-40,0,0,1);
  drawTrap();
  glPopMatrix();

  glPushMatrix();
  glScalef(0.25,1.5,1.0);
  glTranslatef(2.0,0.0,0.25);
  drawRect();
  glPopMatrix();
  
  glPushMatrix();
  glScalef(0.9,0.25,1.0);
  glTranslatef(0.0,-0.5,0.25);
  drawRect();
  glPopMatrix();
  glRotated(-45,0,0,1);
}

static void draw3(void){

  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  glPushMatrix();
    glTranslated(0.0,0.65,0.5);
    glScalef(1.0,0.9,1.0);
    glPushMatrix();
      glTranslatef(0.0,-1.5,-0.5);
      drawCurvedEdget();
    glPopMatrix();
  glPopMatrix();
  
  glPushMatrix();
    glTranslated(0,0.65,0);
    glScaled(1,-1,1);
    drawCurvedEdget();
  glPopMatrix();
}

static void draw2(void){

  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glPushMatrix();
  glTranslatef(0.01,-1,0);
    glScaled(-1,1,1);
    drawCurvedEdge();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.01,-1.25,0.25);
    glScalef(1.0,0.25,1);
    drawRect();
  glPopMatrix();
  
  glPushMatrix();
    glScaled(1,-1,1);
    glTranslatef(0,-0.5,0);
    drawCurvedEdget();
  glPopMatrix();
}

static void draw1(void){
  if(LGT) glColor4fv(t);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0.0,0.0,0.25);
  
    glScalef(1.2,1.2,1.0);

    glPushMatrix();
      glScalef(0.25,1.25,1);
      drawRect();
    glPopMatrix();

    glTranslatef(-0.77,1.25,-0.25);
    glScalef(1.0,-1.0,1.0);
    drawCurvedEdge();
  glPopMatrix();
}

static void draw10(void){

  draw1();glTranslatef(1.4,0.0,0.0);
  draw0();
}


// =========== ENDNUMBERS ============== \\


static void drawTopEdgew(void){
  glPushMatrix();
  glScalef(1.0,0.125,1.0);
  drawCirc();
  glPopMatrix();
}

static void drawTopEdget(void){
  glPushMatrix();
  glScalef(0.7,0.125,1.0);
  drawCirc();
  glPopMatrix();
}

static void drawOCurve(void){
  A=0.2617;C=1.66;
  glShadeModel(GL_FLAT);
  glPushMatrix();
  for(i=1;i>-2;i-=2){
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0,0,i);
    for(B=0,j=3; B<C; B+=A){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B)*(1+0.1),(GLdouble) i*0.25);
      glVertex3f((GLdouble) cos(B)*(0.5-j*0.075),(GLdouble) sin(B)*(0.5+0.1*j),(GLdouble) i*(0.25));
    }
    glEnd();
  }
  glBegin(GL_QUAD_STRIP);
  for(B=0; B<C; B+=A){
    glNormal3f((GLdouble) cos(B),(GLdouble) sin(B)*(1+0.1),0);
    for(i=1;i>-2;i-=2){
      glVertex3f((GLdouble) cos(B),(GLdouble) sin(B)*(1+0.1),(GLdouble) i*0.25);
    }
  }
  for(B=B-A; B>0; B-=A){
    glNormal3f((GLdouble) -cos(B)*(0.5-j*0.075),(GLdouble) -sin(B)*(0.5+0.1*j),0);
    for(i=1;i>-2;i-=2){
      glVertex3f((GLdouble) cos(B)*(0.5-j*0.075),(GLdouble) sin(B)*(0.5+0.1*j),(GLdouble) i*0.25);
    }
  }
  glVertex3f(0.5-j*0.075,0.0,0.25);
  glVertex3f(0.5-j*0.075,0.0,-0.25);
  glEnd();
  glPopMatrix();
}

static void drawOCurveh(void){
  glPushMatrix();
  drawOCurve();
  glPushMatrix();
    glScaled(1,-1,1);
    drawOCurve();
  glPopMatrix();
  glPopMatrix();
}

static void drawCParl1(void){
  glPushMatrix();
    glScaled(1.5,1.0,1);
    drawParl();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-0.75,1.0,0.0);
    drawTopEdgew();
  glPopMatrix();
}

static void drawCParl(void){
  glPushMatrix();
  drawCParl1();
  glPushMatrix();
    glTranslatef(0.75,-1,0.0);
    drawTopEdgew();
  glPopMatrix();
  glPopMatrix();
}

static void drawCTrap(void){
  glPushMatrix();
    glScaled(-1.5,1.0,1.0);
    glRotated(30,0,0,1);
    drawTrap();
  glPopMatrix();

  glPushMatrix();
    glTranslated(0.5,1.0,0.0);
    drawTopEdget();
  glPopMatrix();
}

static void drawCTrap1(void){
  glPushMatrix();
    glScaled(-1.5,1.0,1.0);
    drawTrap();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-0.25,0.9,0.0);
    drawTopEdget();
  glPopMatrix();
}

static void drawCSq(void){
  glPushMatrix();
    glScaled(1.25,1.0,1);
    drawSq();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-0.55,1.0,0.0);
    drawTopEdgew();
  glPopMatrix();
}

static void drawCSq1(void){
  glPushMatrix();
  glScalef(0.75,1.0,1.0);
  glRotated(180,0,0,1);
  drawCParl1();
  glPopMatrix();
}
static void drawCOC1(void){
  glPushMatrix();
    glTranslated(0.2,0,0);
    glScalef(1,-2.0,1);
    drawOCurve();
  glPopMatrix();

  glPushMatrix();
    glTranslated(0.9,0,-0.25);
    drawTopEdget();
  glPopMatrix();
}

static void drawCOC(void){
  glPushMatrix();
    glTranslated(0.05,-1.2,-0.25);
    glScalef(1.35,1,1);
    glRotated(180,0,0,1);
    drawCurvedEdge();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-0.95,-0.7,0);
    glScaled(0.35,0.6,1);
    drawRect();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-1,0.,-0.25);
    drawTopEdget();
  glPopMatrix();
}

static void drawCTad(void){
  drawTad();
  glPushMatrix();
    glRotated(-90,0,0,1);
    glTranslated(-0.6,1.25,0.0);
    glScaled(1.25,1.0,1.0);
    drawTopEdget();
  glPopMatrix();
}

static void drawBar(void){
  glPushMatrix();
  glScalef(0.375,1.0,1);
  drawRect();
  glPopMatrix();
}

static void drawBarf1(void){
  drawBar();
  glPushMatrix();
  glTranslatef(0.0,-1.0,-0.25);
  glScaled(0.625,1,1);
  drawTopEdgew();
  glPopMatrix();
}

static void drawBarf2(void){
  drawBarf1();
  glPushMatrix();
  glTranslatef(0.0,1.0,-0.25);
  glScaled(0.625,1,1);
  drawTopEdgew();
  glPopMatrix();
}
static void drawBarf1h(void){
  drawBarf1();
  glPushMatrix();
  glTranslatef(-0.25,0.875,-0.25);
  glScaled(0.625,1,1);
  drawTopEdget();
  glPopMatrix();
}
static void drawBarh(void){
  drawBar();
  glPushMatrix();
  glTranslatef(-0.25,-0.875,-0.25);
  glScaled(0.625,1,1);
  drawTopEdget();
  glPopMatrix();
}
static void drawBarh2(void){
  drawBarh();
  glPushMatrix();
  glTranslatef(-0.25,0.875,-0.25);
  glScaled(0.625,1,1);
  drawTopEdget();
  glPopMatrix();
}

// ====== STARTLETTERS ======== 


static void drawZ(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0.05,0,0);
    glScaled(-1,1,1);
    drawParl();
  glPopMatrix();

 
  glPushMatrix();
    glTranslatef(-0.05,0.4,0);
    glScalef(-0.5,0.5,1);
    drawCTad();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.15,-0.4,0);
    glScalef(0.5,-0.45,1);
    drawCTad();
  glPopMatrix();
}


static void drawY(void){
  
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  glPushMatrix();
  glTranslatef(-0.2,-0.15,0);
  glScalef(1.1,1.1,1);
  
  glPushMatrix();
    glTranslatef(-0.3,0.4,0);
    glScalef(-0.6,0.5,1);
    drawCTrap();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0,-0.3,0);
    glScalef(-0.6,-0.6,1);
    drawCTrap1();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.8,0.6,0);
    glScalef(0.5,0.4,1);
    drawCTrap();
  glPopMatrix();

  glPopMatrix();
}

static void drawX(void){

  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  glPushMatrix();
    glScalef(0.95,0.85,1);

  glPushMatrix();
    glScaled(0.55,1.,1);
    drawCParl();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.82,0.55,0);
    glScalef(0.45,0.5,1.0);
    drawCTrap();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-0.82,-0.55,0);
    glScalef(-0.45,-0.5,1.0);
    drawCTrap();
  glPopMatrix();

  glPopMatrix();
}

static void drawW(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  
  glPushMatrix();
  glTranslatef(0.25,0,0);
  glScalef(0.8,1,1);

  glPushMatrix();
    glTranslated(0,-0.2,0);
    glScaled(0.9,0.9,1);
    drawDim();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(-0.95,-0.15,0);
    glScalef(0.75,0.9,1);
    drawCSq();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.9,0.14,0);
    glScalef(0.45,0.6,1.0);
    drawCTrap();
  glPopMatrix();

  glPopMatrix();
}

static void drawV(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(-0.2,-0.1,0);
    glScalef(1,1.1,1);

  glPushMatrix();
    glTranslatef(0,-0.1,0);
    glScalef(0.75,0.85,1);
    drawCSq();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.95,0.2,0);
    glScalef(0.45,0.6,1.0);
    drawCTrap();
  glPopMatrix();
  
  glPopMatrix();
}

static void drawU(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0.1,0.8,0.25);
    glScalef(0.8,0.9,1);
    drawCOC();
    drawCOC1();
  glPopMatrix();
}

static void drawP(void){
  
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(-0.2,0,0.25);
    drawBarf1h();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.3,0.4,0.25);
    glScalef(0.85,0.5,1.0);
    drawOCurveh();
  glPopMatrix();
}
static void drawO(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0.1,0,0.25);
    drawOCurveh();
  glPushMatrix();
    glScaled(-1,1,1);
    glTranslatef(0.125,0,0);
    drawOCurveh();
  glPopMatrix();
  glPopMatrix();
}

static void drawM(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  
  glPushMatrix();
  glScaled(0.8,0.9,1);
 
  glPushMatrix();
    glTranslatef(1.1,0,0.25);
    glScalef(0.5,1,1);
    drawBarf2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(-0.5,0,0);
    glScalef(-0.8,1,1);
    drawCTrap();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(-1.1,-0.3,0);
    glScalef(-0.6,-0.9,1);
    drawCTrap1();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.4,0.1,0);
    glScalef(0.6,1,1);
    glRotated(-37,0,0,1);
    drawTrap();
  glPopMatrix();

  glPopMatrix();
}

static void drawN(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  
  glPushMatrix();
  glScaled(0.9,1,1);
  glPushMatrix();
    glScaled(0.75,1,1);
    drawCParl();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(1.1,0.25,0);
    glScalef(0.5,0.85,1.0);
    drawCTrap1();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-1.1,-0.25,0);
    glScalef(-0.5,-0.85,1.0);
    drawCTrap1();
  glPopMatrix();
  glPopMatrix();
}

static void drawH(void){
  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(-0.5,0,0.25);
    drawBarf2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.7,0,0.25);
    glScalef(-0.8,1,1);
    drawBarf2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.07,0,0.25);
    glScalef(0.25,0.25,1);
    drawRect();
  glPopMatrix();
}

static void drawG(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(0.05,0,0.25);
    glScalef(-1.25,1,1);
    drawOCurveh();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.15,0.35,0);
    glScalef(0.5,0.6,1);
    drawCTad();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.05,-0.2,0.25);
    glScalef(0.75,0.4,1);
    drawCOC1();
  glPopMatrix();
}
static void drawF(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(-0.25,0,0.25);
    glScaled(1,1,1);
    drawBarf1h();
  glPopMatrix();

 
  glPushMatrix();
    glTranslatef(0.25,0.4,0);
    glScalef(0.5,0.5,1);
    drawCTad();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.25,-0.25,0);
    glScalef(0.25,0.25,1);
    drawCTad();
  glPopMatrix();
}

static void drawE(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(-0.25,0,0.25);
    glScaled(1,1,1);
    drawBarh2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.25,0.4,0);
    glScalef(0.5,0.5,1);
    drawCTad();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.25,-0.45,0);
    glScalef(0.5,-0.45,1);
    drawCTad();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.25,-0.25,0);
    glScalef(0.25,0.25,1);
    drawCTad();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.25,0.25,0);
    glScalef(0.25,-0.25,1);
    drawCTad();
  glPopMatrix();
}

static void drawD(void){
  if(LGT) glColor4fv(g);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(-0.25,0,0.25);
    drawBarh2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.25,0,0.25);
    glScalef(1.0,0.9,1.0);
    drawOCurveh();
  glPopMatrix();
}

static void drawC(void){
  if(LGT) glColor4fv(y);
  else glColor4fv(n);
  glPushMatrix();
    glTranslatef(0.25,0,0.25);
    glScalef(-1.25,1,1);
    drawOCurveh();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.35,0.45,0);
    glScalef(0.5,0.5,1);
    drawCTad();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.35,-0.45,0);
    glScalef(0.5,-0.45,1);
    drawCTad();
  glPopMatrix();
}


static void drawB(void){
  if(LGT) glColor4fv(r);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslatef(-0.25,0,0.25);
    drawBarh2();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.25,0.5,0.25);
    glScalef(0.75,0.4,1.0);
    drawOCurveh();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0.25,-0.5,0.25);
    glScalef(0.75,0.5,1.0);
    drawOCurveh();
  glPopMatrix();
}

static void drawA(void){
  if(LGT) glColor4fv(b);
  else glColor4fv(n);
  
  glPushMatrix();
    glTranslated(0.5,0,0);
    glScalef(0.75,1,1);
    drawCSq1();
  glPopMatrix();

  glPushMatrix();
    glTranslated(-0.75,0,0);
    glScalef(-0.5,-1,1);
    drawCTrap();
  glPopMatrix();

  glPushMatrix();
    glTranslated(0,-0.4,0);
    glScaled(0.5,1,1);
    glRotated(90,0,0,1);
    drawTrap();
  glPopMatrix();
}

// ======= ENDLETTERS ======== 
static void drawBeveledCube(int col){
  A=2.0, B=1.5;
  glShadeModel(GL_SMOOTH);
  if(LGT==0){
    glColor4fv(n);
  for(i=0;i<3;++i){
    for(j=0;j<2;++j){
      glRotated(j*90,0,1,0);
      glBegin(GL_QUAD_STRIP);
          glNormal3f(0,0,1);
	  glVertex3f(B,-B,A);
	  glVertex3f(A,-A,A);
	  glVertex3f(-B,-B,A);
	  glVertex3f(-A,-A,A);

	  glVertex3f(-B,B,A);
	  glVertex3f(-A,A,A);

	  glVertex3f(B,B,A);
	  glVertex3f(A,A,A);

	  glVertex3f(B,-B,A);
	  glVertex3f(A,-A,A);
      glEnd();
      
      glBegin(GL_QUAD_STRIP);
          glNormal3f(-1,0,0);
	  glVertex3f(B,-B,-A);
	  glVertex3f(B,-B,-B);
	  glVertex3f(B,B,-A);
	  glVertex3f(B,B,-B);

          glNormal3f(0,-1,0);
	  glVertex3f(-B,B,-A);
	  glVertex3f(-B,B,-B);

          glNormal3f(1,0,0);
	  glVertex3f(-B,-B,-A);
	  glVertex3f(-B,-B,-B);

          glNormal3f(0,1,0);
	  glVertex3f(B,-B,-A);
	  glVertex3f(B,-B,-B);
       glEnd();
      glBegin(GL_QUAD_STRIP);
	  glNormal3f(0,0,1);
	  glVertex3f(B,-B,B);
	  glVertex3f(-B,-B,B);
	  glVertex3f(B,B,B);
	  glVertex3f(-B,B,B);
      glEnd();
    }
    glPushMatrix();
    glRotated(90,1,0,0);
    }
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  return;
  }
  
      
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w_w,w_h,0,GL_RGB,GL_UNSIGNED_BYTE,WOOD_PIXELS);
  //glBindTexture(GL_TEXTURE_2D,SHAD_TEX);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  for(i=0;i<3;++i){
    for(j=0;j<2;++j){
      glRotated(j*90,0,1,0);
      glBegin(GL_QUAD_STRIP);
      switch(col){
      case 1:
	glColor4fv(b);
	break;
      case 2:
	glColor4fv(y);
	break;
      case 3:
	glColor4fv(n);
	break;
      case 4:
	glColor4fv(p);
	break;
      case 5:
	glColor4fv(r);
	break;
      default:
	glColor4fv(r);
	break;
      }
          glNormal3f(0,0,1);
	  glVertex3f(B,-B,A);
	  glVertex3f(A,-A,A);
	  glVertex3f(-B,-B,A);
	  glVertex3f(-A,-A,A);

	  glVertex3f(-B,B,A);
	  glVertex3f(-A,A,A);

	  glVertex3f(B,B,A);
	  glVertex3f(A,A,A);

	  glVertex3f(B,-B,A);
	  glVertex3f(A,-A,A);
      glEnd();
  glEnable(GL_TEXTURE_2D);
      glColor4fv(w);
      glBegin(GL_QUADS);
          glNormal3f(-1,0,0);

	  glTexCoord2f(0.7,0.5);
	  glVertex3f(B,-B,-A);
	  glTexCoord2f(0.7,0.3);
	  glVertex3f(B,-B,-B);
	  glTexCoord2f(0.3,0.5);
	  glVertex3f(B,B,-B);
	  glTexCoord2f(0.3,0.3);
	  glVertex3f(B,B,-A);
       glEnd();

      glBegin(GL_QUADS);
          glNormal3f(0,-1,0);

	  glTexCoord2f(0.2,0.9);
	  glVertex3f(B,B,-A);
	  glTexCoord2f(0.4,0.9);
	  glVertex3f(B,B,-B);
	  glTexCoord2f(0.2,0.5);
	  glVertex3f(-B,B,-B);
	  glTexCoord2f(0.4,0.5);
	  glVertex3f(-B,B,-A);
       glEnd();

      glBegin(GL_QUADS);
          glNormal3f(1,0,0);

	  glTexCoord2f(0.7,0.9);
	  glVertex3f(-B,B,-A);
	  glTexCoord2f(0.7,0.7);
	  glVertex3f(-B,B,-B);
	  glTexCoord2f(0.3,0.9);
	  glVertex3f(-B,-B,-B);
	  glTexCoord2f(0.3,0.7);
	  glVertex3f(-B,-B,-A);
       glEnd();
      glBegin(GL_QUADS);
          glNormal3f(0,1,0);

	  glTexCoord2f(0.4,0.9);
	  glVertex3f(-B,-B,-A);
	  glTexCoord2f(0.6,0.9);
	  glVertex3f(-B,-B,-B);
	  glTexCoord2f(0.4,0.5);
	  glVertex3f(B,-B,-B);
	  glTexCoord2f(0.6,0.5);
	  glVertex3f(B,-B,-A);
       glEnd();
       /////////////////////

      glBegin(GL_QUAD_STRIP);
	  glNormal3f(0,0,1);

	  glTexCoord2f(0.7,0.7);
	  glVertex3f(B,-B,B);

	  glTexCoord2f(0.3,0.7);
	  glVertex3f(-B,-B,B);

	  glTexCoord2f(0.7,0.3);
	  glVertex3f(B,B,B);

	  glTexCoord2f(0.3,0.3);
	  glVertex3f(-B,B,B);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
    glPushMatrix();
    glRotated(90,1,0,0);
  }
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  
}

static void drawFloor(void)
{
  glPushMatrix();
  glLoadIdentity();
  gluLookAt(cos(Ey)*sin(Ex)*Ez,-sin(Ey)*Ez,cos(Ey)*cos(Ex)*Ez,   0.0f,1.0f,0.0f,   0,1,0);

  glEnable(GL_TEXTURE_2D);
  glColor4fv(w);
  
  glBegin(GL_QUADS);
    glNormal3f(0,1,0);

    glTexCoord2f(0.1,0.0);
    glVertex3f(-42,-5,60);

    glTexCoord2f(0.9,0.0);
    glVertex3f(42,-5,60);

    glTexCoord2f(0.9,1.0);
    glVertex3f(42,-5,-50);

    glTexCoord2f(0.1,1.0);
    glVertex3f(-42,-5,-50);

  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

static void Draw(void)
{
  glClearColor(1,1,1,0);
  
  if(LGT){LGT=0; Draw(); LGT+=1;}

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if((LGT))
    gluLookAt(cos(Ey)*sin(Ex)*Ez,-sin(Ey)*Ez,cos(Ey)*cos(Ex)*Ez,   0.0f,1.0f,0.0f,   0,1,0);
  else
    gluLookAt(light_position[0],light_position[1], light_position[2],    0.0f,1.0f,6.0f,   0,1,0);

  if(LGT){
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120);
  }
  else{
    glDisable(GL_LIGHT0);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
  }

  glColor3f(0.0, 0.0, 1.0);
  glTranslated(0,0,10);
  glScaled(2,2,2);

  // BACK
  glPushMatrix();
    if(LGT)
      glTranslated(0,0,-16.5);
    else
      glTranslated(0,0,-17.5);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);
    drawBeveledCube(5);
    Obj_P[0]=0; Obj_P[1]=0; Obj_P[2]=-20;
  glPopMatrix();

    // put_shapes
  glPushMatrix();
    if(LGT)
      glTranslated(0,0,-16.5);
    else
    glTranslatef(0,0,-17.5);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);    
    glPushMatrix();
      glTranslatef(0,0,1.5);
      drawM();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,0,1,0);
      glTranslatef(0,0,1.5);
      drawN();
    glPopMatrix();
    glPushMatrix();
      glRotated(180,0,1,0);
      glTranslatef(0,0,1.5);
      drawO();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,0,1,0);
      glTranslatef(0,0,1.5);
      drawP();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,1,0,0);
      glTranslatef(0,0,1.5);
      drawTriangle();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,1,0,0);
      glTranslatef(0,0,1.5);
      drawSquare();
    glPopMatrix();
  glPopMatrix();
  



  // MID LEFT
  glPushMatrix();
    if(LGT)
      glTranslated(-4.8,0,-12.7);
    else
      glTranslatef(-4.8,0,-13);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);
    drawBeveledCube(1);
  glPopMatrix();

    // put_shapes
  glPushMatrix();
    if(LGT)
      glTranslated(-4.8,0,-12.7);
    else
      glTranslatef(-4.8,0,-13);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);
    
    glPushMatrix();
      glTranslatef(0,0,1.5);
      drawA();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,0,1,0);
      glTranslatef(0,0,1.5);
      drawB();
    glPopMatrix();
    glPushMatrix();
      glRotated(180,0,1,0);
      glTranslatef(0,0,1.5);
      drawC();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,0,1,0);
      glTranslatef(0,0,1.5);
      drawD();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,1,0,0);
      glTranslatef(0,0,1.5);
      drawHebStar();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,1,0,0);
      glTranslatef(0,0,1.5);
      drawCross();
    glPopMatrix();
  glPopMatrix();



  // MID RIGHT
  glPushMatrix();
    if(LGT)
      glTranslated(4.8,0,-12.7);
    else
      glTranslated(4.8,0,-13);
    glRotated((GLint) -ROTx,0,1,0);
    glRotated((GLint) -ROTy,1,0,0);
    drawBeveledCube(1);
  glPopMatrix();

  // put_shapes
  glPushMatrix();
    if(LGT)
      glTranslated(4.8,0,-12.7);
    else
      glTranslatef(4.8,0,-13);
    glRotated((GLint) -ROTx,0,1,0);
    glRotated((GLint) -ROTy,1,0,0);
    
    glPushMatrix();
      glTranslatef(0,0,1.5);
      drawE();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,0,1,0);
      glTranslatef(0,0,1.5);
      drawF();
    glPopMatrix();
    glPushMatrix();
      glRotated(180,0,1,0);
      glTranslatef(0,0,1.5);
      drawG();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,0,1,0);
      glTranslatef(0,0,1.5);
      drawH();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,1,0,0);
      glTranslatef(0,0,1.5);
      drawDiv();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,1,0,0);
      glTranslatef(0,0,1.5);
      drawTimes();
    glPopMatrix();
  glPopMatrix();

  // FRONT LEFT
  glPushMatrix();
    glTranslated(-7.5,0,-3);
    glRotated((GLint) -ROTx,0,1,0);
    glRotated((GLint) -ROTy,1,0,0);
    drawBeveledCube(2);
  glPopMatrix();
  
  // put_shapes
  glPushMatrix();
    glTranslatef(-7.5,0,-3);
    glRotated((GLint) -ROTx,0,1,0);
    glRotated((GLint) -ROTy,1,0,0);
    
    glPushMatrix();
      glTranslatef(0,0,1.5);
      drawU();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,0,1,0);
      glTranslatef(0,0,1.5);
      drawV();
    glPopMatrix();
    glPushMatrix();
      glRotated(180,0,1,0);
      glTranslatef(0,0,1.5);
      drawW();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,0,1,0);
      glTranslatef(0,0,1.5);
      drawX();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,1,0,0);
      glTranslatef(0,0,1.5);
      drawHexagon();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,1,0,0);
      glTranslatef(0,0,1.5);
      drawOctagon();
    glPopMatrix();
  glPopMatrix();


  //FRONT RIGHT
  glPushMatrix();
    glTranslated(7.5,0,-3);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);
    drawBeveledCube(5);
  glPopMatrix();
  
  // put_shapes
  glPushMatrix();
    glTranslatef(7.5,0,-3);
    glRotated((GLint) ROTx,0,1,0);
    glRotated((GLint) ROTy,1,0,0);
    
    glPushMatrix();
      glTranslatef(0,0,1.5);
      drawY();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,0,1,0);
      glTranslatef(0,0,1.5);
      drawZ();
    glPopMatrix();
    glPushMatrix();
      glRotated(180,0,1,0);
      glTranslatef(0,0,1.5);
      drawPentagon();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,0,1,0);
      glTranslatef(0,0,1.5);
      drawCircle();
    glPopMatrix();
    glPushMatrix();
      glRotated(90,1,0,0);
      glTranslatef(0,0,1.5);
      drawMinus();
    glPopMatrix();
    glPushMatrix();
      glRotated(-90,1,0,0);
      glTranslatef(0,0,1.5);
      drawPlus();
    glPopMatrix();
  glPopMatrix();

  glPushMatrix();
    glRotatef(10,0,1,0);
    if(tick<1000){
      glTranslatef(0,0,-2);
      glRotatef((GLfloat)50-((GLfloat)tick)*0.05,0,1,0);
      glTranslatef(0,0, -15+(((GLfloat)tick))*0.015);
    }
    else
      glTranslatef(0,0, (((GLfloat)tick)/50)-22);
    (*O[tock])();
  glPopMatrix();

  if(tic>0){
  glPushMatrix();
    glRotatef(-10,0,1,0);
    if(tic<1000){
      glTranslatef(0,0,-2);
      glRotatef(((GLfloat)tic)*0.05-50,0,1,0);
      glTranslatef(0,0, -15+(((GLfloat)tic))*0.015);
    }
    else
      glTranslatef(0,0, (((GLfloat)tic)/50)-22);
    (*E[toc])();
  glPopMatrix();
  }
  if((LGT)){
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,500,500,0,GL_RGBA,GL_UNSIGNED_BYTE,SHADOW_PIXELS);
    //glBindTexture(GL_TEXTURE_2D,SHAD_TEX);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

    drawFloor();

    free(SHADOW_PIXELS);
    glutSwapBuffers();
  }
  else
    {
      SHADOW_PIXELS = malloc(4*500*500);
      glReadPixels(0,0,500,500,GL_RGBA,GL_UNSIGNED_BYTE,SHADOW_PIXELS);
      glGenTextures(1, &SHAD_TEX);
    }
}


void idle(void){
  ROTx = ROTx+0.2;
  if(ROTx>360)  ROTx-=360;
  tick++;
  tic++;
  if(tick>2000){ tick=0; tock+=1;}
  if(tic>2000){ tic=0; toc+=1;}
  if(toc==5 && tic>0) exit(1);

  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}
void motion(int x, int y)
{
  //printf("%d \t %d\n", (x-startx), (y-starty));
  if (moving) {
    Ex = Ex - ((GLfloat)x - (GLfloat)startx)/200*3.14;
    Ey = Ey - ((GLfloat)y - (GLfloat)starty)/400*3.14;
	 if(Ey<0) Ey+=6.28;
	 if(Ex<0) Ex+=6.28;
	 if(Ey>6.28) Ey-=6.28;
	 if(Ex>6.28) Ex-=6.28;
    startx = x;
    starty = y;
  }
  idle();
}

/* INPUT */
void keyboard (unsigned char key, int x, int y)
{ 
  if(key== 'a'){
         
   }if(key== 's'){
              
   }if(key== 'd'){
         
   }if(key== 'w'){
	 if(light_position[0]<-9) light_position[0]=-9;
              
   }if(key== 'b'){
    if(eye)
      eye=0;
    else
      eye=1;
   }if(key== 'x'){
     LGT=1;
   }if(key== 'z'){
     LGT=0;
   }
  if(key== 'l'){
         Ex+=0.131;
	 if(Ex>6.28) Ex-=6.28;
         
   }if(key== 'j'){
         Ex-=0.131;
	 if(Ex<0) Ex+=6.28;
              
   }if(key== 'k'){
         Ey+=0.131;
	 if(Ey>6.28) Ey-=6.28;
         
   }if(key== 'i'){
         Ey-=0.131;
	 if(Ey<0) Ey+=6.28;
         
   }if(key== ' '){
         Ex=0;
	 Ey=0;
         Ez=50.0;
	 light_position[0]=0;
	 light_position[2]=40; 
         //ROTx=0; 
         //ROTy=0;
	 return;
         
  }if(key== 27){
    exit(0);
  }
   idle();
}

// MAIN FUNCT
int main(int argc, char* argv){

  O[0]=draw1;
  E[0]=draw2;
  O[1]=draw3;
  E[1]=draw4;
  O[2]=draw5;
  E[2]=draw6;
  O[3]=draw7;
  E[3]=draw8;
  O[4]=draw9;
  E[4]=draw10;
  O[5]=NullFunc;
  E[6]=NullFunc;

  PPMREAD("wood.ppm");

  glutInit(&argc, &argv);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100, 80);
  glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB|GLUT_MULTISAMPLE);
  glutCreateWindow("Name: Austen Piers");
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glGenTextures(1, &WOOD_TEX);
  glGenTextures(1, &SHAD_TEX);

  glutDisplayFunc(Draw);
  glutIdleFunc(idle);

  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glViewport(0,0, 500, 500);
  gluPerspective(45, 1, .1, 400);
  glMatrixMode(GL_MODELVIEW);

  glutMainLoop();
  return 0;
}
