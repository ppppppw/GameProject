#include "Scene.h"
class Level1 : public Scene {
public:
    int Live = 3;
    bool isEnd = false;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
