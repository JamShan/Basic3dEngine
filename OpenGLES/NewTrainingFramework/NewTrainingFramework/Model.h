#ifndef __MODEL__
#define __MODEL__


#include "../Utilities/Math.h"
#include "Vertex.h"

class Model
{

public:
//ONSTRUCTOR
	/* you need to keep always constructors very simple
	(only code like  variable = 0, pointer = NULL).  */
       Model();
     /* That’s because on some platforms you can’t access the call stack before main(),
	  so, if you have global objects (constructor will be called before main()),
	  it’s a nightmare to identify which of those global objects has problems.*/

   //Don’t forget to put on NULL the pointers after “delete pointer”
     ~Model();                          

//FUNCTIONS
	 /* You read .nfg file using fscanf_s(). You can “jump” over some
	 values which are not relevant for you. Example:
		fscanf_s( pFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",&verticesData.pos.x, &verticesData.pos.y, & verticesData.pos.z,……….);
	 Please notice that “*” allowed us to jump over a value (it is read from your file, but not assigned to a variable”.*/
     void InitModel(unsigned int ID, char * filename);                        


//MEMBERS
	  // You create your vertex buffer object and your index buffer object (glGenBuffers() etc.)

    // you keep handles to your buffer objects
	GLuint m_hVertexBuffer, m_hIndexBuffer;    
	
	
	// you need it in glDrawElements().
	unsigned int m_noIndices;  
	unsigned int m_noVertices;
	
	
	// You don’t use for the moment this data member, but when you will write the engine,
	//here it will be kept the ID of the model from Resource Manager file.
	unsigned int m_ID;    


	//index's array
	int *m_indArray;

	//Vertex's array
	Vertex *m_vArray;
};
#endif