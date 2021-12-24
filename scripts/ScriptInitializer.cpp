#include "ScriptInitializer.h"

ObjectInfo makeTestTarget(float x, float y)
{
    std::string name = "Target", spriteName = "circle";
    int id = 2;
    float pos_x = x, pos_y = y;
    float rotation = 0.0f;
    float size_x = 70.0f, size_y = 70.0f;
    float vel_x = 0.0f, vel_y = -100.0f;
    float accel_x = 0.0f, accel_y = 0.0f;
    float color_r = 255.0f, color_g = 0.0f;
    float color_b = 0.0f, color_a = 1.0f;
    float minFireDelay = 0.0f;
    std::function<void(GameObject*, float)> updateFunction = [](GameObject*, float) 
    {

    };
    std::vector<float> hitboxInfo{ size_x / 2.0f };

    ObjectInfo info(name, id, spriteName,
        pos_x, pos_y, rotation, size_x, size_y,
        vel_x, vel_y, accel_x, accel_y, color_r,
        color_g, color_b, color_a, minFireDelay,
        updateFunction, HitboxShape::CIRCLE,
        hitboxInfo);

    return info;
}

ScriptCollection ScriptInitializer::createGameScripts()
{
    ScriptCollection collection("game script");



    return collection;
}

ScriptCollection ScriptInitializer::createTestScripts()
{
    ScriptCollection collection("test script");
    std::vector<ObjectInfo> objects;
    std::vector<Command> commands;

    objects.push_back(makeTestTarget(300, 400));
    objects.push_back(makeTestTarget(400, 500));
    objects.push_back(makeTestTarget(500, 600));

    commands.push_back(Command(ScriptCommand::PLACE_OBJECT, 0));
    commands.push_back(Command(ScriptCommand::WAIT_FOR, 5));
    commands.push_back(Command(ScriptCommand::PLACE_OBJECT, 0));
    commands.push_back(Command(ScriptCommand::WAIT_FOR, 5));
    commands.push_back(Command(ScriptCommand::PLACE_OBJECT, 0));

    collection.addScript(Script("make target", 1, 0, 0, objects, commands));

    return collection;
}

//std::vector<Polygon> starData{
//    Polygon(std::vector<glm::vec2> { glm::vec2(70, 0), glm::vec2(48.73, 46.09), glm::vec2(91.63, 46.09) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(140, 53.48), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(113.26, 140), glm::vec2(70, 116.09), glm::vec2(105, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(26.74, 140), glm::vec2(35, 89.35), glm::vec2(70, 116.09) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(0, 53.48), glm::vec2(48.37, 46.09), glm::vec2(35, 89.35) }),
//    Polygon(std::vector<glm::vec2> { glm::vec2(70, 116.09), glm::vec2(35, 89.35), glm::vec2(48.37, 46.09), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) })
//};