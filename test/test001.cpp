#include <iostream>
#include "catch2/catch.hpp"
#include "Model.hpp"
#include "Math.hpp"
#include "Debug.hpp"

using namespace glm;

TEST_CASE("Decompose transformation matrix M=SRT")
{
    Transform transform;
    vec3 S = vec3(1.0f, 2.0f, 3.0f);
    quat R = quat_cast(rotate(mat4(1.0f), Pi / 2, vec3(0.0f, 1.0f, 0.0f)));
    vec3 T(3.0f, 2.0f, 1.0f);
    transform.position = T;
    transform.rotation = R;
    transform.scale = S;
    auto M = static_cast<mat4>(transform);
    GIVEN("All positive scale") {
        transform = Transform(M);
        REQUIRE(nearlyEqual(transform.position, T));
        REQUIRE(nearlyEqual(transform.rotation, R));
        REQUIRE(nearlyEqual(transform.scale, S));
    }
}
