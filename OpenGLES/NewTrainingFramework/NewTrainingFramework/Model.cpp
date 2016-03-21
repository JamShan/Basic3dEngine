#include "stdafx.h"
#include "Model.h"
#include "Globals.h"


//usar buffer objects (vertex buffer e index buffer object)
	FILE *pFile;
	/* you need to keep always constructors very simple
	(only code like  variable = 0, pointer = NULL).  */
Model::Model(){
	
	pFile				= NULL;
	m_hVertexBuffer		= 0;
	m_hIndexBuffer		= 0;
	m_noIndices			= 0;
	m_noVertices		= 0;
}
     /* That’s because on some platforms you can’t access the call stack before main(),
	  so, if you have global objects (constructor will be called before main()),
	  it’s a nightmare to identify which of those global objects has problems.*/

   //Don’t forget to put on NULL the pointers after “delete pointer”
Model::~Model(){
	if(m_hVertexBuffer){
		glDeleteBuffers(1,&m_hVertexBuffer);
		m_hVertexBuffer = NULL;
	}

	if(m_hIndexBuffer){
		glDeleteBuffers(1,&m_hIndexBuffer);
		m_hVertexBuffer = NULL;
	}

}                          


	 /* You read .nfg file using fscanf_s(). You can “jump” over some
	 values which are not relevant for you. Example:
		fscanf_s( pFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];"
		,&verticesData.pos.x, &verticesData.pos.y, & verticesData.pos.z,……….);
	 Please notice that “*” allowed us to jump over a value (it is read from your file, but not assigned to a variable”.*/
void Model::InitModel(unsigned ID, char * filename){
	
	//capturamos el numero de vertices
	
	pFile = fopen(filename, "r");
    if (pFile == NULL) {
       printf("Error: no fue posible abrir el modelo \"%s\"\n",filename);
	   return;
    }

	printf("\nSuccesfully opened file: %s\n",filename);

	fscanf_s( pFile, "%*s %d",&m_noVertices);
	printf("noVertices: %d\n",m_noVertices);
	m_vArray = new Vertex[m_noVertices];
	unsigned int ind = 0;
	for ( ; ind < m_noVertices ; ind++ ){
		fscanf_s( pFile, "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
			&m_vArray[ind].pos.x,	 &m_vArray[ind].pos.y,	  &m_vArray[ind].pos.z,		 &m_vArray[ind].norm.x, &m_vArray[ind].norm.y, &m_vArray[ind].norm.z,
			&m_vArray[ind].binorm.x, &m_vArray[ind].binorm.y, &m_vArray[ind].binorm.z,	 &m_vArray[ind].tangent.x,  &m_vArray[ind].tangent.y,  &m_vArray[ind].tangent.z,
			&m_vArray[ind].uv.x,	 &m_vArray[ind].uv.y);

		/*printf("vertex[%d] = pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n", ind,
			m_vArray[ind].pos.x,	m_vArray[ind].pos.y,	m_vArray[ind].pos.z,	 m_vArray[ind].norm.x, m_vArray[ind].norm.y, m_vArray[ind].norm.z,
			m_vArray[ind].binorm.x, m_vArray[ind].binorm.y,	m_vArray[ind].binorm.z,	 m_vArray[ind].tangent.x,  m_vArray[ind].tangent.y,  m_vArray[ind].tangent.z,
			m_vArray[ind].uv.x,		m_vArray[ind].uv.y);
		*/
		//m_vArray[ind].color.w =  1.0f;  m_vArray[ind].color.x =  0.0f;	m_vArray[ind].color.y =  0.5f;  m_vArray[ind].color.z =  1.0f;
	}

	
	fscanf_s( pFile, "%*s %d",&m_noIndices);
	printf("noIndices: %d\n\n",m_noIndices);
	m_indArray = new int[m_noIndices];
	for ( ind = 0 ; ind < m_noIndices ; ind+=3 ){
		fscanf_s( pFile, "%*d. %d, %d, %d",
			&m_indArray[ind], &m_indArray[ind+1], &m_indArray[ind+2]);
		//printf("indice %d, %d, %d \n",m_indArray[ind], m_indArray[ind+1], m_indArray[ind+2]);
	}


	glGenBuffers(1, &m_hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_noVertices * sizeof(Vertex), m_vArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_hIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_noIndices*sizeof(int), m_indArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	SAFE_DELETE_ARR(m_vArray);
	SAFE_DELETE_ARR(m_indArray);

	pFile		= NULL;

}

	
	  // You create your vertex buffer object and your index buffer object (glGenBuffers() etc.)


