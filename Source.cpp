#include <glut.h>

GLfloat coordinateX = 0.0, coordinateY = -2.5, coordinateZ = -1.0, turn = 0.0, flyPosition = 0.0, kepitL = 45.0, kepitR = 45.0;
static int on = 0, floating = 1, shoulderR = 0, shoulderL = 0, shoulderRy = 0, shoulderLy = 0, elbowR = 0, elbowL = 0, elbowRy = 0, elbowLy = 0; //bien tinh: luu gia tri sau moi lan dc tinh toan
GLint fan = 0;
GLUquadricObj* quadratic; //bien tu giac 
GLUquadricObj* disk;

void init(void)
{
	glEnable(GL_DEPTH_TEST); //kiem tra do sau, cap nhat bo dem do sau 
	glEnable(GL_LIGHTING);	// bat che do sang, lien quan de mau sac cua vat lieu
	glEnable(GL_LIGHT0);	// chieu sang tu tren xuong
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0); //background colour

	quadratic = gluNewQuadric();
	disk = gluNewQuadric();
}

void fly()// bay len xuong lien tuc
{
	if (coordinateY < flyPosition + 0.3 && floating == 1) //toa do y=-2.5 < vi tri bay = 0.0 + 0.3 va do noi = 1
		coordinateY = coordinateY + 0.05;				//toa do y = y + 0.001
	else floating = 0;									//do noi bang 0

	if (coordinateY > flyPosition - 0.3 && floating == 0)
		coordinateY = coordinateY - 0.05;
	else floating = 1;
}

void fall() {	//tat bay 
	if (coordinateY > -2.5) {
		coordinateY = coordinateY - 0.05;
		floating = 1;
	}
	flyPosition = 0.0; //reset flyPosition value
}

// -- robot's part --
void cuboid(GLfloat length, GLfloat height, GLfloat width, GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue); //Color
	glScalef(length, height, width); //change cube -> cuboid
	glutSolidCube(1.0);				//khoi hop: size

	//transform back to original
	glScalef(1 / length, 1 / height, 1 / width);	//ti le ma tran: phong to or nho 
}

void sphere(GLdouble radius, GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue); //Color
	glutSolidSphere(radius, 20.0, 20.0); //khoi cau: bk, 
}

void cylinder(GLdouble radius, GLdouble length) {
	GLint slices = 20, stacks = 20;

	glColor3f(0.8, 0.8, 0.8); //color
	glTranslatef(0.0, 0.0, -length / 2); //roi hinh
	gluCylinder(quadratic, radius, radius, length, slices, stacks); // khoi tru doc theo truc z
	glTranslatef(0.0, 0.0, length);
	gluDisk(disk, 0, radius, slices, stacks); //ve khoi hinh dia: vit o tay 
	glTranslatef(0.0, 0.0, -length);	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(disk, 0, radius, slices, stacks);

	//transform back to original
	glRotatef(-180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, length / 2);
}

void eye(GLdouble radius) {
	GLdouble length = 1.0;
	GLint slices = 20, stacks = 20;

	glColor3f(0.8, 0.8, 0.3); //Color
	gluCylinder(quadratic, radius, radius, length, slices, stacks); // ong mat


	glTranslatef(0.0, 0.0, length);
	gluDisk(disk, radius - 0.05, radius, slices, stacks); //vien mat
	glColor3f(0, 0, 0); //Color
	glTranslatef(0.0, 0.0, -0.03);
	gluDisk(disk, 0, radius - 0.05, slices, stacks); //trong mat

	//transform back to original
	glTranslatef(0.0, 0.0, 0.03);
	glTranslatef(0.0, 0.0, -length);
}

void finger(float kepit) {
	glRotatef(kepit, 0, 0, 1);
	cylinder(0.1, 0.3); //(radius, length)
	glTranslatef(0.2, 0.0, 0.0);
	cuboid(0.4, 0.2, 0.2, 0.1, 0.1, 0.1);

	glTranslatef(0.2, 0.0, 0.0);
	glRotatef(-45, 0, 0, 1);

	glTranslatef(0.15, 0.0, 0.0);
	cuboid(0.4, 0.15, 0.15, 1, 1, 0);

	//transform back to original
	glTranslatef(-0.15, 0.0, 0.0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(-0.4, 0.0, 0.0);
	glRotatef(-kepit, 0, 0, 1);
}

void hand(float kepit) {
	cuboid(0.24, 0.6, 0.24, 0.1, 0.1, 0.1);
	glTranslatef(0.0, 0.2, 0.0);
	finger(kepit);

	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(180, 1, 0, 0);
	finger(kepit);

	//transform back to original
	glRotatef(-180, 1, 0, 0);
}
// -- end of robot's part ----------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// -- light --
	GLfloat DiffuseLight[] = { 0.3, 0.3, 0.3 }; //set color for GL_LIGHT2, GL_LIGHT3
	glLightfv(GL_LIGHT2, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, DiffuseLight);

	GLfloat AmbientLight[] = { 0.0, 0.0, 0.0 }; //AmbientLight[] = {r, g, b}
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);

	GLfloat LightPosition2[] = { 5.0, 10.0, 0.0, 0.0 }; //set light position - above-right vi tri chieu sang: phai tren 
	GLfloat LightPosition3[] = { -5.0, 10.0, 0.0, 0.0 }; //set light position - above-left vi tri chieu sang: trai tren
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
	glLightfv(GL_LIGHT3, GL_POSITION, LightPosition3);
	//============================== Robot =================================
	glPushMatrix();

	glTranslatef((GLfloat)coordinateX, (GLfloat)coordinateY, (GLfloat)coordinateZ); //di chuyen robot yb
	glRotatef((GLfloat)turn, 0.0, 1.0, 0.0); //xoay robot: gh 

	//---- Body ----
	sphere(1.0, 0.3, 0.3, 1.0); // than robot: khoi cau 

	//---- Eyes ----
	glPushMatrix(); //---- Left Eye ----
	glTranslatef(-0.35, 0.3, 0.0);
	eye(0.22); //---- Right Eye ----
	glTranslatef(0.7, 0.0, 0.0);
	eye(0.22);
	glPopMatrix();
	//---- End Eyes ----

	glPushMatrix(); //---- Mouth ----
	glTranslatef(0.0, -0.2, 0.95);
	glScalef(1.0, 0.5, 1.0);
	glColor3f(1.0, 1.0, 0.0); //Color
	glutSolidSphere(0.5, 4.0, 2.0);
	glPopMatrix();

	glPushMatrix(); //---- Kipas ----
	glTranslatef(0.0, 1.1, 0.0); //huong canh quat len tren 
	glRotatef((GLfloat)fan, 0.0, 1.0, 0.0); //xoay canh quat

	//blades
	cuboid(1.8, 0.05, 0.30, 0.8, 0.8, 0.0); //(length, height, width, red, green, blue) ve canh quat
	cuboid(0.30, 0.05, 1.8, 0.8, 0.8, 0.0); //(length, height, width, red, green, blue

	//fan's center
	glRotatef(90, 1.0, 0.0, 0.0); //trung tam canh quat
	cylinder(0.1, 0.2);			//khoi tru 

	glPopMatrix(); //---- End Kipas ----

	//-------------- Right Arm --------------
	glPushMatrix();	//---- Upper Arm ----
	glTranslatef(1.1, 0.0, 0.0); //move shoulder to right
	//di chuyen tay theo than 

	glPushMatrix();
	glRotatef((GLfloat)shoulderR, 0.0, 0.0, 1.0); // nang canh tay 
	sphere(0.3, 0.8, 0.8, 0.8); // vai phai

	glTranslatef(0.6, 0.0, 0.0); // keo dai bap tay 
	cuboid(1.2, 0.25, 0.25, 0.1, 0.1, 0.1); //ve bap tay 

	glPushMatrix();	//---- Lower Arm ----
	glTranslatef(0.6, 0.0, 0.0); //move elbow to right

	glPushMatrix();
	sphere(0.2, 0.8, 0.8, 0.8); //elbow - (radius, red, green,blue)

	glTranslatef(0.6, 0.0, 0.0); //move forearm to right
	cuboid(1.25, 0.24, 0.24, 0.1, 0.1, 0.1); //forearm - (length, height, width, red, green, blue)

	glPushMatrix(); //---- Hand ----
	glTranslatef(0.6, 0.0, 0.0);
	hand(kepitR);
	glPopMatrix(); //---- End Hand ----

	glPopMatrix();

	glPopMatrix();	//-- End Lower Arm --
	glPopMatrix();
	glPopMatrix();	//--- End Upper Arm ---
	//-------------- End Right Arm --------------

	//--------------- Left Arm --------------
	glPushMatrix();	//---- Upper Arm ----
	glTranslatef(-1.1, 0.0, 0.0); //move shoulder to left

	glPushMatrix();
	glRotatef((GLfloat)shoulderL, 0.0, 0.0, 1.0);	// lift/lower shoulder & elbow
	sphere(0.3, 0.8, 0.8, 0.8); //shoulder - (radius, red, green,blue)

	glTranslatef(-0.6, 0.0, 0.0); //move bicep to left
	cuboid(1.2, 0.25, 0.25, 0.1, 0.1, 0.1); //bicep - (length, height, width, red, green, blue)

	glPushMatrix();	//---- Lower Arm ----
	glTranslatef(-0.6, 0.0, 0.0); //move elbow to left

	glPushMatrix();
	sphere(0.2, 0.8, 0.8, 0.8); //elbow - (radius, red, green,blue)

	glTranslatef(-0.6, 0.0, 0.0); //move forearm to left
	cuboid(1.25, 0.24, 0.24, 0.1, 0.1, 0.1); //forearm - (length, height, width, red, green, blue)

	glPushMatrix(); //---- Hand ----
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(180, 0, 0, 1);
	hand(kepitL);
	glPopMatrix(); //---- End Hand ----

	glPopMatrix();

	glPopMatrix();	//-- End Lower Arm --

	glPopMatrix();
	glPopMatrix();	//--- End Upper Arm ---
	//-------------- End Left Arm --------------

	glPopMatrix();
	//============================== End Robot ==============================

	glutSwapBuffers();

	//fly or fall
	if (on == 1) {
		fly();
		fan = (fan + 4) % 360;
	}
	else fall();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); // fovy, aspect ratio, near, far, ti le khung hinh gan xa thiet lap phep chieu boi canh

	glMatrixMode(GL_MODELVIEW);
}

void input(int key, int x, int y)
{
	float k = 0.5;
	switch (key) {
	case GLUT_KEY_UP:
		coordinateY += k;
		flyPosition = coordinateY;
		glutPostRedisplay(); //cap nhat vi tri sau khi di chuyen 
		break;
	case GLUT_KEY_DOWN:
		if (coordinateY > -2.5 + 0.5) { //set lowest position downward
			coordinateY -= k;
			flyPosition = coordinateY;
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		coordinateX -= k;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		coordinateX += k;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// On/Off helicopter
	case 32: //Spacebar key
		if (on == 0) on = 1;
		else on = 0;
		break;
		// Turn body
	case 'g':
	case 'G':
		turn -= 1;
		break;
	case 'h':
	case 'H':
		turn += 1;
		break;
	case 'y':
		coordinateZ -= 0.5;
		glutPostRedisplay();
		break;
	case 'b':
		coordinateZ += 0.5;
		glutPostRedisplay();
		break;
		//=============== Right Arm ================
	case 'i': //lift right shoulder
		if (shoulderR < 90)
			shoulderR = shoulderR + 5;
		glutPostRedisplay();
		break;
	case 'k': //lower right shoulder
		if (shoulderR > -90)
			shoulderR = shoulderR - 5;
		glutPostRedisplay();
		break;
	case 'n': //kepit kanan
		if (kepitR == 0) kepitR = 45;
		else kepitR = 0;
		glutPostRedisplay();
		break;
		//------------ End Right Arm ---------------
		//=============== Left Arm =================
	case 'e': //lift left shoulder
		if (shoulderL > -90)
			shoulderL = shoulderL - 5;
		glutPostRedisplay();
		break;
	case 'd': //lower left shoulder
		if (shoulderL < 90)
			shoulderL = shoulderL + 5;
		glutPostRedisplay();
		break;
	case 'v': //kepit kiri
		if (kepitL == 0) kepitL = 45;
		else kepitL = 0;
		glutPostRedisplay();
		break;
		//------------- End Left Arm ---------------
		// Reset
	case 8: //Backspace key
		//coordinateX = 0.0; coordinateY = -2.5; flyPosition = 0.0; on = 0; floating = 1; fan = 0;
		shoulderR = 0; shoulderL = 0; shoulderRy = 0; shoulderLy = 0;
		elbowR = 0; elbowL = 0; elbowRy = 0; elbowLy = 0; turn = 0.0;
		break;
		// Quit
	case 27: //Esc key
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(10, 5);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(input);

	glutMainLoop();
	return 0;
}
