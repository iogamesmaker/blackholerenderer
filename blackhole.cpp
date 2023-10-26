#include <GL/glut.h>
#include <cmath>
#include <iostream>

float grid=1;
int oHEIGHT,WIDTH=800;
int oWIDTH,HEIGHT=600;
int stepSize=9;
int downscale=8;
int iter;
float samples=1;
float strength=1;
float volumetric=0;
float maxIter=37*stepSize,acreation=130;
bool fullscreen,end,gravity=true,Acreation=true;

double xDir=0,yDir=0,zPos=0,xPos,yPos;
double vx,vy,vz,bri,dist,atanwastaken,xx,yy,zz,x,y,z,r,g,b;

void normalize(double &vx,double &vy,double &vz) {
    double vecLen=sqrtf((vx*vx)+(vy*vy)+(vz*vz));
    vx/=vecLen;
    vy/=vecLen;
    vz/=vecLen;
}

void keyboard(unsigned char key, int x, int y){
    if(key==27){exit(0);}
    else{
        switch(key){
            case 'w':
                zPos+=0.1;glutPostRedisplay();break;
            case 's':
                zPos-=0.1;glutPostRedisplay();break;
            case 'a':
                xPos+=0.1;glutPostRedisplay();break;
            case 'd':
                xPos-=0.1;glutPostRedisplay();break;
            case 'e':
                yPos-=0.1;glutPostRedisplay();break;
            case 'q':
                yPos+=0.1;glutPostRedisplay();break;
            case 'g':
                gravity=!gravity;glutPostRedisplay();break;
            case 'h':
                Acreation=!Acreation;glutPostRedisplay();break;
            case 'i':
                samples++;grid=sqrtf(samples);glutPostRedisplay();break;
            case 'o':
                samples--;if(samples==0){samples=1;grid=1;}else{grid=sqrtf(samples);}glutPostRedisplay();break;
        }
    }
}
void specialKey(int key,int x,int y){
    switch(key){
        case GLUT_KEY_F11: fullscreen=!fullscreen;if(fullscreen){oWIDTH=WIDTH;oHEIGHT=HEIGHT;glutFullScreen();}else{glutReshapeWindow(oWIDTH,oHEIGHT);}break;
        case GLUT_KEY_LEFT :
            xDir+=0.1;glutPostRedisplay();break;
        case GLUT_KEY_RIGHT:
            xDir-=0.1;glutPostRedisplay();break;
        case GLUT_KEY_UP   :
            yDir+=0.1;glutPostRedisplay();break;
        case GLUT_KEY_DOWN :
            yDir-=0.1;glutPostRedisplay();break;
        case GLUT_KEY_F1 :
            stepSize--;if(stepSize==0){stepSize=1;}maxIter=19*stepSize;glutPostRedisplay();break;
        case GLUT_KEY_F2 :
            stepSize++;maxIter=19*stepSize;glutPostRedisplay();break;
        case GLUT_KEY_F3 :
            acreation--;if(acreation==0){acreation=1;}glutPostRedisplay();break;
        case GLUT_KEY_F4 :
            acreation++;glutPostRedisplay();break;
        case GLUT_KEY_F5 :
            downscale/=2;if(downscale<1){downscale=1;}glutPostRedisplay();break;
        case GLUT_KEY_F6 :
            downscale*=2;if(downscale>128){downscale=128;}glutPostRedisplay();break;
        case GLUT_KEY_F7 :
            maxIter-=2;if(maxIter<20){maxIter=20;}glutPostRedisplay();break;
        case GLUT_KEY_F8 :
            maxIter+=2;glutPostRedisplay();break;
        case GLUT_KEY_F9 :
            strength-=0.1;if(strength<0){strength=0;}glutPostRedisplay();break;
        case GLUT_KEY_F10 :
            strength+=0.1;if(strength>2){strength=2;}glutPostRedisplay();break;
    }
}

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);
    for (double xx = -WIDTH/2; xx < WIDTH/2+1; xx+=1*downscale) {
    glPointSize(downscale);
    glBegin(GL_POINTS);
        for (double yy = -HEIGHT/2; yy < HEIGHT/2+1; yy+=1*downscale) {
            r=0;
            g=0;
            b=0;
            for(int zz=0;zz<samples;zz++){
                volumetric=0;
                int gridX = fmod(zz,grid);
                int gridY = zz / grid;
                double xOffset = (gridX + 0.5) / grid;
                double yOffset = (gridY + 0.5) / grid;
                x=0  +xPos;
                y=0.6+yPos;
                z=-10+zPos;
                vx=0-(((yy+yOffset) / 130) * 0.25 + ((xx+xOffset) / 130) * 0.75)+xDir;
                vy=((yy+yOffset) / 130) * 0.75 + ((xx+xOffset) / 130) * -0.25+yDir;
                vz=1.5+zPos;
                normalize(vx, vy, vz);
                b+=0.0;
                iter = 0;
                end = true;
                while (iter < maxIter) {
                    dist = (x*x) + (y*y) + (z*z);
                    if (dist < 2) {
                        end = false;
                        break;
                    }
                    dist +=dist;
                    if (-0.5/stepSize <y && y < 0.5/stepSize && dist < acreation&&Acreation){
                        volumetric++;
                        dist=sqrtf((x*x)+(y*y)+(z*z));
                        bri =50+3.75*(8-(8-dist)*(8-dist)/1.7);
                        atanwastaken=atan(x/z)+(180*(z<0));
                        bri =(bri * (8 - ((dist * dist) / 8))) / 3;
                        r+=abs(bri/31.875)/volumetric;
                        g+=abs(0.5*bri/31.875/volumetric)/1.8/volumetric;
                        if(volumetric==maxIter/stepSize){
                            end=false;
                            break;
                        }
                    }///*
                    if(gravity){
                        vx+=(0-x)/(dist*stepSize)*strength; //   |
                        vy+=(0-y)/(dist*stepSize)*strength; //   |- These 3 lines change gravity.
                        vz+=(0-z)/(dist*stepSize)*strength; //   |
                    }//*/
                    normalize(vx, vy, vz);
                    x+=vx/stepSize;
                    y+=vy/stepSize;
                    z+=vz/stepSize;
                    iter++;
                }
                if(end){
                    dist=(200-z)/vz;
                    if (abs(fmod((y + (vy * dist)) * 5, 180) - 90) < 5 || abs(fmod((x + (vx * dist)) * 5, 180) - 90) < 5 || abs(fmod((y + (vy * dist)) * 5, 180) + 90) < 5 || abs(fmod((x + (vx * dist)) * 5, 180) + 90) < 5) {
                        if(volumetric>1){
                        r+=0.2/volumetric;g+=0.2/volumetric;b+=0.2/volumetric;}else{r+=0.5;g+=0.5;b+=0.5;}
                    }
                }
            }
            glColor3f(r/samples,g/samples,b/samples);
            glVertex2i(xx+WIDTH/2, yy+HEIGHT/2);
        }
    glEnd();
    glFlush();
    }
    // glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	WIDTH = w;
	HEIGHT = h;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB/* | GLUT_MULTISAMPLE*/);
    glutInitWindowSize(WIDTH,HEIGHT);

    glutCreateWindow("zwarte gat dingie");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);



    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);

    glutMainLoop();
    return 0;
}
