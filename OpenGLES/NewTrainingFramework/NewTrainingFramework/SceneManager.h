#include "..\Utilities\Math.h"

class Object;
class Model;
class Texture;
class Camera;
class ResourceManager;


//Class to store Light Properties
class Light{
public:
	Vector3	Pos;
	Vector3	Color;
	float radius, speed, angle;
	int	type, moving;

	enum moving{
		CIRCLE = 0
	};
	enum types{
		DIRECTIONAL = 0,
		POINT
	};
	Light():radius(0),speed(0),angle(0){}
};



//Class to store Fog Properties
class Fog{
public:
	float Color[4];
	float Start, Range;
	Fog(float f1, float f2, float f3){
		Color[0]=f1;
		Color[1]=f2;
		Color[2]=f3;
		Color[3]=1.0;
	}
	void setStart(float start){Start = start;}
	void setRange(float range){Range = range;}
	float* getColor(){return Color;}
};

class SceneManager{
private:
	static SceneManager * instance;

protected:
	SceneManager();
	~SceneManager();

	SceneManager& operator =( SceneManager &){ }

	unsigned int m_noObjects; 

	Object	**m_ObjectArr;

public:
	void Init(char * filename);  
	void Draw();
	void Update(float deltaTime);
	static SceneManager * getInstance();
	static void destroyInstance();

	unsigned int getObjectsCount();

	Object* getObjectByID(unsigned int i_id);

	Camera	*camera;
	Fog		*fog;
	float	updateTime;


	
	Vector3 Amb_Color;
	float	Amb_W;
	unsigned m_noLights;
	Light	**m_LightArr;
	Light	*getLightByID(unsigned int i_id);

	
};
#define SceneMan SceneManager::getInstance()



