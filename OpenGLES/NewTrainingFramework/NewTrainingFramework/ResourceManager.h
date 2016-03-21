

class Texture;
class Model;
class Shaders;

class ResourceManager{

private:
	//only 1 instance
	static ResourceManager * instance;

protected:
	// no external instatiation
	ResourceManager();
	~ResourceManager();

	//no asignation 
	ResourceManager& operator =( ResourceManager &){ }

	unsigned int m_noModels;  
	unsigned int m_noTextures;
	unsigned int m_noQTextures;
	unsigned int m_noShaders;

	Model	**m_ModelArr;
	Texture **m_TextureArr;
	Texture **m_QTextureArr;
	Shaders	**m_ShaderArr;
	
public:
	void Init(char * filename);  
	static ResourceManager * getInstance();
	static void destroyInstance();

	unsigned int getModelsCount();
	unsigned int getTexturesCount();
	unsigned int getQTexturesCount();
	unsigned int getShadersCount();
	

	Model	*getModelByID(unsigned int i_id);
	Texture	*getTextureByID(unsigned int i_id);
	Texture *getQTextureByID(unsigned i_id);
	Shaders	*getShaderByID(unsigned int i_id);
	

};

#define ResourceMan ResourceManager::getInstance()