#pragma once
#include "GameObjectFactory.h"
#include "PlatformerObject.h"

//A class that allows us to have animations
class AnimatedGraphic : public PlatformerObject {
public:

	//Constructor/Destructor

	AnimatedGraphic();
	virtual ~AnimatedGraphic() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();

private:

	int m_animSpeed;
	int m_frameCount;
};

//A class that will create animated graphics
class AnimatedGraphicCreator : public BaseCreator {
public:

	virtual GameObject* createGameObject() const {
		return new AnimatedGraphic();
	}
};