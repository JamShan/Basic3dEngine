

class Camera;

class InputManager{

private:
	//only 1 instance
	static InputManager * instance;
	long keyPressed;
	// transform raw key values to a minor values
	void InputManager::transformKeyVal( int & );

protected:
	// no external instatiation
	InputManager() : keyPressed(false){
	} 
	~InputManager(){
	}

	//no asignation 
	InputManager& operator =( InputManager &){}

public:

	static InputManager * getInstance();
	static void destroyInstance();

	void pressKey(int KeyVal);
	void releaseKey(int KeyVal);
	bool isKeyPressed(int KeyVal);
	bool isKeyPressedTrans(int KeyVal);
	void handleKeys(float deltaTime, Camera *camera);

};

#define InputMan InputManager::getInstance()