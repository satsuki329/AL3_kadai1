#include "Viewprojection.h"
#include "WorldTransform.h"
#include "Model.h"

class skydome
{
public:

	void Initialize(Model* model, ViewProjection* viewprojection);

	void Update();

	void Draw();


private:

	WorldTransform worldTransformSkydome_;
	ViewProjection* viewprojection_ = nullptr;

	Model* model_ = nullptr;
};