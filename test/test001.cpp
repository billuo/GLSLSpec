#include <iostream>
#include <catch2/catch.hpp>
#include <Math/Transform.hpp>
#include <Math/Math.hpp>


TEST_CASE("Decompose transformation matrix M=SRT")
{
    using namespace glm;
    using namespace Math;
    vec3 S = vec3(1.0f, 2.0f, 3.0f);
    quat R = rotate(identity<quat>(), Pi / 2, vec3(0.0f, 1.0f, 0.0f));
    vec3 T(3.0f, 2.0f, 1.0f);
    GIVEN("All positive component scale") {
        auto ref = Transform{S, R, T};
        auto t = Transform{ref.to_mat4()};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
    }
    GIVEN("One negative component scale") {
        S.x = -S.x;
        auto ref = Transform{S, R, T};
        auto t = Transform{ref.to_mat4()};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S.x = -S.x;
    }
    GIVEN("Two negative component scale") {
        S = -S;
        S.x = -S.x;
        auto ref = Transform{S, R, T};
        auto t = Transform{ref.to_mat4()};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S.x = -S.x;
        S = -S;
    }
    GIVEN("All negative component scale") {
        S = -S;
        auto ref = Transform{S, R, T};
        auto t = Transform{ref.to_mat4()};
        REQUIRE(nearlyEqual(t.to_mat4(), ref.to_mat4()));
        S = -S;
    }
}

TEST_CASE("Transform rotation maxtrix")
{
    using namespace glm;
    using namespace Math;
    Transform T;
    GIVEN("Axis aligned rotation") {
        T.rotation = rotate(identity<quat>(), (90_deg).radians().value(), vec3(0.0, 1.0, 0.0));
        REQUIRE(nearlyEqual(mat3(T.rotation), mat3(0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0)));
    }
    GIVEN("Arbitrary rotation") {
        // TODO
    }
}
