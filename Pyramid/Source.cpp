/*   OpenGL Pyramid Project

Displays a pyramid drawn from lines and draws 4 triangles occupying half the pyramid.

User controls:
Esc: Close window
W:Move camera forward
A:Move camera to the left
S:Move camera backwards
D:Move camera to the right
Q:Move camera downwards
E:Move camera upwards
---After right click
W/S:Rotate pyramid on X axis
A/D:Rotate pyramid on Y axis
Q/E:Rotate pyramid on Z axis

Camera follows mouse movement
*/
#include <string>
#include <iostream>
#include <fstream>
#include <memory.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>


#define M_PI       3.14159265358979323846

using namespace std;

//integers used in drawing pyramid
int side = 5, height = 10;
// Pyramid data
//data will be used to create a pyramid with square base 
//with side of length 2*side and height of length height centered on (0,0,0) point
float verticesPyramid[] = {
	//pyramid base
	-1.0f * side, 0.0f, 1.0f * side, 1.0f,
	1.0f *  side, 0.0f, 1.0f * side, 1.0f,

	-1.0f * side, 0.0f, -1.0f * side, 1.0f,
	1.0f * side, 0.0f, -1.0f * side, 1.0f,

	-1.0f * side, 0.0f, -1.0f * side, 1.0f,
	-1.0f * side, 0.0f, 1.0f * side, 1.0f,

	1.0f * side, 0.0f, 1.0f * side, 1.0f,
	1.0f * side, 0.0f, -1.0f * side, 1.0f,
	//pyramid sides
	-1.0f * side, 0.0f, 1.0f * side, 1.0f,
	0.0f, 1.0f*height,0.0f, 1.0f,

	-1.0f * side, 0.0f, -1.0f * side, 1.0f,
	0.0f, 1.0f*height,0.0f, 1.0f,

	1.0f * side, 0.0f, -1.0f * side, 1.0f,
	0.0f, 1.0f*height,0.0f, 1.0f,

	1.0f * side, 0.0f, 1.0f * side, 1.0f,
	0.0f, 1.0f*height,0.0f, 1.0f
};
///colour data of lines used in drawing the pyramid 
float colourPyramid[] = { 0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f };

// Triangles data
//data of 4 triangles which fill half the pyramid
float vertices[] = {
	-1.0f * side, 0.0f, -1.0f * side, 1.0f,
	1.0f * side, 0.0f, 1.0f * side, 1.0f,
	0.0f, 1.0f*height, 0.0f, 1.0f,

	1.0f * side, 0.0f, -1.0f * side, 1.0f,
	1.0f * side, 0.0f, 1.0f * side, 1.0f,
	0.0f, 1.0f*height, 0.0f, 1.0f,

	-1.0f * side, 0.0f, -1.0f * side, 1.0f,
	1.0f * side, 0.0f, -1.0f * side, 1.0f,
	0.0f, 1.0f*height, 0.0f, 1.0f,

	1.0f * side, 0.0f, -1.0f * side, 1.0f,
	1.0f * side, 0.0f, 1.0f * side, 1.0f,
	-1.0f * side, 0.0f, -1.0f * side, 1.0f, };
///data for triangle colours
float colours[] = {
	1.0f, 0.0f, 0.4f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.3f,0.8f, 0.5f, 1.0f,

	1.0f, 0.0f, 0.4f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.3f,0.8f, 0.5f, 1.0f,

	1.0f, 0.0f, 0.4f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.3f,0.8f, 0.5f, 1.0f,

	1.0f, 0.0f, 0.4f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.3f,0.8f, 0.5f, 1.0f };

// Program and Shader Identifiers
GLuint p, vs, fs;

// Vertex Attribute Locations
GLuint vertexLoc, colourLoc;

// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc, modelMatrixLoc;
// Vertex Array Objects Identifiers
GLuint vao[2];

// storage for Matrices
float projMatrix[16];
float viewMatrix[16];
float modelMatrix[16] = { 1.0f,0.0f,0.0f,0.0f,
0.0f,1.0f,0.0f,0.0f,
0.0f,0.0f,1.0f,0.0f,
0.0f,0.0f,0.0f,1.0f };

//variable incremented when right click is pressed to switch controls
int change_controls = 0;
//yaw and pitch angles of the camera
float yaw = 0.0f, pitch = 0.0f;
//camera posion, half the pyramid height on Y axis and size of pyramid base length on Z axis
float position[] = { 0.0f, height*0.5f, side*2.0f + side };
//window size
int windowWidth = 640, windowHeight = 480;
//positon where cursor will be warped
int Ox = windowWidth / 2, Oy = windowHeight / 2;
//boolean to check if loading shaders failed
bool loadFailed;

//update pitch by given value and keep it between -90 and 90 degrees
void updatePitchBy(float p) {
	pitch -= p;

	if (pitch < -90.0f) {
		pitch = -90.0f;
	}
	if (pitch > 90.0f) {
		pitch = 90.0f;
	}
}
//update yaw by given value and keep it between 0 and 360 degrees
void updateYawBy(float y) {

	yaw -= y;
	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

}

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix(float *mat, int size) {

	// fill matrix with 0s and diagonal with 1s
	for (int i = 0; i < size * size; ++i)
	{
		if (i % (size + 1) == 0)
			mat[i] = 1.0f;
		else
			mat[i] = 0.0f;

	}

}


//multiply  matrices and store value in matrix a
void multMatrix(float *a, float *b) {

	float res[16];

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[j * 4 + i] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				res[j * 4 + i] += a[k * 4 + i] * b[j * 4 + k];
			}
		}
	}
	memcpy(a, res, 16 * sizeof(float));

}

// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z) {

	setIdentityMatrix(mat, 4);
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
}
//scales matrix mat by passed values
void scaleMatrix(float* mat, float x, float y, float z) {
	float aux[16];
	setIdentityMatrix(aux, 4);
	aux[0] = x;
	aux[5] = y;
	aux[10] = z;
	multMatrix(mat, aux);
}
//rotates matrix mat on given axis by specified degrees
void rotateGivenAxis(float* mat, float* axis, float degrees) {
	//store cos and sin values
	float c = cos((float)((degrees * M_PI) / 180));
	float s = sin((float)((degrees * M_PI) / 180));

	float aux[16];
	setIdentityMatrix(aux, 4);
	aux[0] = (axis[0] * axis[0]) * (1.0f - c) + c;
	aux[1] = (axis[1] * axis[0]) * (1.0f - c) + (axis[2] * s);
	aux[2] = (axis[2] * axis[0]) * (1.0f - c) - (axis[1] * s);

	aux[4] = (axis[0] * axis[1]) * (1.0f - c) - (axis[2] * s);
	aux[5] = (axis[1] * axis[1]) * (1.0f - c) + c;
	aux[6] = (axis[2] * axis[1]) * (1.0f - c) + (axis[0] * s);

	aux[8] = (axis[0] * axis[2]) * (1.0f - c) + (axis[1] * s);
	aux[9] = (axis[1] * axis[2]) * (1.0f - c) - (axis[0] * s);
	aux[10] = (axis[2] * axis[2]) * (1.0f - c) + c;
	//rotate matrix
	multMatrix(mat, aux);

}
//rotate matrix mat on X axis by specified degrees
void rotate_X_Axis(float *mat, float degrees) {
	//store cos and sin values
	float c = cos((float)((degrees * M_PI) / 180));
	float s = sin((float)((degrees * M_PI) / 180));

	float aux1[16];
	setIdentityMatrix(aux1, 4);

	aux1[5] = c;
	aux1[6] = -s;
	aux1[9] = s;
	aux1[10] = c;
	//rotate matrix
	multMatrix(mat, aux1);

}
//rotate matrix mat on Y axis by specified degrees
void rotate_Y_Axis(float *mat, float degrees) {

	//store cos and sin values
	float c = cos((float)((degrees * M_PI) / 180));
	float s = sin((float)((degrees * M_PI) / 180));

	float aux2[16];
	setIdentityMatrix(aux2, 4);

	aux2[0] = c;
	aux2[2] = s;
	aux2[8] = -s;
	aux2[10] = c;
	//rotate matrix
	multMatrix(mat, aux2);
}
//rotate matrix mat on Z axis by specified degrees
void rotate_Z_Axis(float *mat, float degrees) {
	//store cos and sin values
	float c = cos((float)((degrees * M_PI) / 180));
	float s = sin((float)((degrees * M_PI) / 180));

	float aux3[16];
	setIdentityMatrix(aux3, 4);

	aux3[0] = c;
	aux3[1] = -s;
	aux3[4] = s;
	aux3[5] = c;
	//roate matrix
	multMatrix(mat, aux3);
}

//create projection matrix - perspective view
void buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {

	float f = 1.0f / tan(fov * (M_PI / 360.0));

	setIdentityMatrix(projMatrix, 4);

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
	projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;
}


//builds view matrix based on camera position, pitch and yaw values
void buildViewMatrix(float posX, float posY, float posZ) {

	float aux[16];
	setIdentityMatrix(viewMatrix, 4);
	setTranslationMatrix(aux, -posX, -posY, -posZ);

	rotate_X_Axis(viewMatrix, -pitch);
	rotate_Y_Axis(viewMatrix, -yaw);
	multMatrix(viewMatrix, aux);

}

//method called when the window is reshaped
void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	ratio = (1.0f * w) / h;
	//set clip view value so that the whole pyramid can be seen
	if (side > height)
		buildProjectionMatrix(53.13f, ratio, 1.0f, side*4.0f);
	else
		buildProjectionMatrix(53.13f, ratio, 1.0f, height*4.0f);

	//update values of integers
	windowWidth = w;
	windowHeight = h;
}

void setupBuffers() {

	GLuint buffers[2];
	glGenVertexArrays(2, vao);

	// VAO for triangles
	glBindVertexArray(vao[0]);
	// Generate two slots for the vertex and color buffers
	glGenBuffers(2, buffers);
	// bind buffer for vertices and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

	// bind buffer for colors and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colourLoc);
	glVertexAttribPointer(colourLoc, 4, GL_FLOAT, 0, 0, 0);


	//VAO for Pyramid
	glBindVertexArray(vao[1]);
	// Generate two slots for the vertex and color buffers
	glGenBuffers(2, buffers);
	// bind buffer for vertices and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPyramid), verticesPyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

	// bind buffer for colors and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourPyramid), colourPyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colourLoc);
	glVertexAttribPointer(colourLoc, 4, GL_FLOAT, 0, 0, 0);

}

void setUniforms() {

	// must be called after glUseProgram
	glUniformMatrix4fv(projMatrixLoc, 1, false, projMatrix);
	glUniformMatrix4fv(viewMatrixLoc, 1, false, viewMatrix);
	glUniformMatrix4fv(modelMatrixLoc, 1, false, modelMatrix);
}

void renderScene(void) {
	// clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set camera position
	buildViewMatrix(position[0], position[1], position[2]);

	glUseProgram(p);
	setUniforms();

	// draws triangles
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// draws pyramid
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_LINES, 0, 16);

	//  swap contents of backward and forward frame buffers
	glutSwapBuffers();
}
//reads key presses
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) {
		glDeleteVertexArrays(3, vao);
		glDeleteProgram(p);
		glDeleteShader(vs);
		glDeleteShader(fs);
		exit(0);
	}
	//if right click was pressed
	if (change_controls)
	{   //rotate pyramid on a certain axis based on key presses
		if (key == 'w')rotate_X_Axis(modelMatrix, 2);
		if (key == 's') rotate_X_Axis(modelMatrix, -2);
		if (key == 'a')rotate_Y_Axis(modelMatrix, 2);
		if (key == 'd')rotate_Y_Axis(modelMatrix, -2);
		if (key == 'q') rotate_Z_Axis(modelMatrix, 2);
		if (key == 'e') rotate_Z_Axis(modelMatrix, -2);

	}
	else {
		//initialize identity matrix
		float aux[16];
		setIdentityMatrix(aux, 4);

		//move forward
		if (key == 'w') {
			//take into account camera pitch and yaw
			rotate_X_Axis(aux, -pitch);
			rotate_Y_Axis(aux, -yaw);
			//decrease camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] -= aux[2 + 4 * i] * 0.2f;
			}
		}
		//move backwards
		if (key == 's') {
			//take into account camera pitch and yaw
			rotate_X_Axis(aux, -pitch);
			rotate_Y_Axis(aux, -yaw);
			//increase camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] += aux[2 + 4 * i] * 0.2f;
			}


		}
		//move left
		if (key == 'a') {
			//take into account camera yaw
			rotate_Y_Axis(aux, -yaw);
			//decrease camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] -= aux[0 + 4 * i] * 0.2f;
			}


		}
		//move right
		if (key == 'd') {
			//take into account camera yaw
			rotate_Y_Axis(aux, -yaw);
			//increase camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] += aux[0 + 4 * i] * 0.2f;
			}


		}
		//move downwards
		if (key == 'q') {
			//take into account camera yaw
			rotate_Y_Axis(aux, -yaw);
			//decrease camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] -= aux[1 + 4 * i] * 0.2f;
			}
		}
		//move upwards
		if (key == 'e') {
			//take into account camera yaw
			rotate_Y_Axis(aux, -yaw);
			//increase camera position by Z coordinates stored in the aux matrix
			//multiplied by movement speed
			for (int i = 0; i < 3; i++)
			{
				position[i] += aux[1 + 4 * i] * 0.2f;
			}
		}
	}
}
//makes camera follow mouse movement
void mouseMove(int x, int y) {
	//calculate distance from last postion of cursor to current one
	//and update camera yaw and pitch
	updateYawBy(-(x - Ox)*0.8f);
	updatePitchBy(-(y - Oy)*0.8f);

	//update last position of cursor
	Ox = x; Oy = y;

	//if mouse gets close to the window edge update cursor position and move mouse to window center 
	if ((x < 20 || x> windowWidth - 20) || (y < 20 || y> windowHeight - 20))
	{
		Ox = windowWidth / 2, Oy = windowHeight / 2;
		glutWarpPointer(Ox, Oy);

	}


}

//checks for mouse clicks 
void mouse(int button, int state, int x, int y)
{
	//if right click was pressed change user controls
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		change_controls++; change_controls %= 2;

	}
}

//load shader file from given path, returns true if load was succesful
bool LoadShaderFile(string from, string & into) {

	ifstream file;
	string temp;

	cout << " Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if (!file.is_open()) {
		cout << " File does not exist !" << endl;
		return false;

	}
	//read file contents
	while (!file.eof()) {
		getline(file, temp);
		into += temp + "\n";

	}

	file.close();
	cout << into << endl << endl;
	cout << " Loaded shader text !" << endl << endl;

	return true;
}

//return shader of specified type from passed source
GLuint GenerateShader(string from, GLenum type) {
	cout << " Compiling Shader ... " << endl;

	string load;
	if (!LoadShaderFile(from, load)) {
		cout << " Compiling failed !" << endl;
		loadFailed = true;
		return 0;

	}

	GLuint shader = glCreateShader(type);

	const char * chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		cout << "Error in shader" << endl;
		return false;
	}
	cout << " Compiling success !" << endl << endl;
	loadFailed = false;

	return shader;

}

//create shaders and bind them to the progam
//bind attributes and uniforms
GLuint setupShaders() {

	vs = GenerateShader("../Shaders/vertex.glsl", GL_VERTEX_SHADER);
	fs = GenerateShader("../Shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	p = glCreateProgram();
	glAttachShader(p, vs);
	glAttachShader(p, fs);

	glBindFragDataLocation(p, 0, "outputF");
	glLinkProgram(p);

	GLint  link_ok = GL_FALSE;
	glGetProgramiv(p, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		cout << "Error in glLinkProgram \n";
		return false;
	}

	char* vertexName = "position";
	vertexLoc = glGetAttribLocation(p, vertexName);
	if (vertexLoc == -1) {
		cout << "Could not bind attribute %s\n", vertexName;
		return false;
	}

	char* colourName = "colour";
	colourLoc = glGetAttribLocation(p, colourName);
	if (colourLoc == -1) {
		cout << "Could not bind attribute %s\n", colourName;
		return false;
	}

	projMatrixLoc = glGetUniformLocation(p, "projMatrix");
	viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	modelMatrixLoc = glGetUniformLocation(p, "modelMatrix");

	return(p);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//initialize window with size values
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Pyramid");

	//set callback methods
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMove);

	//move mouse to center of the window and make it invisible
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//set up shaders and buffers
	p = setupShaders();
	setupBuffers();

	glutMainLoop();


	return(0);
}