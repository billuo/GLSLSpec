#include <iostream>
#include "catch2/catch.hpp"
#include "Transform.hpp"
#include "Math.hpp"
#include "Debug.hpp"


TEST_CASE("Decompose transformation matrix M=SRT")
{
    using namespace glm;
    vec3 S = vec3(1.0f, 2.0f, 3.0f);
    quat R = quat_cast(rotate(mat4(1.0f), Pi / 2, vec3(0.0f, 1.0f, 0.0f)));
    vec3 T(3.0f, 2.0f, 1.0f);
    GIVEN("All positive component scale") {
        auto ref = Transform{S, R, T};
        auto t = Transform{ref.to_mat4()};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
    }
    GIVEN("One negative component scale") {
        S.x = -S.x;
        auto ref = Transform{S, R, T};
        auto t = Transform{mat4(ref)};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S.x = -S.x;
    }
    GIVEN("Two negative component scale") {
        S = -S;
        S.x = -S.x;
        auto ref = Transform{S, R, T};
        auto t = Transform{mat4(ref)};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S.x = -S.x;
        S = -S;
    }
    GIVEN("All negative component scale") {
        S = -S;
        auto ref = Transform{S, R, T};
        auto t = Transform{mat4(ref)};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S = -S;
    }
}
