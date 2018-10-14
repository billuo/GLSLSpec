#ifndef CAMERA_CPP_5HOJB2CT
#define CAMERA_CPP_5HOJB2CT

#include "Camera.hpp"
#include "Debug.hpp"

const float Camera::ClipPlanes::Nearest = 0.01f;
const float Camera::ClipPlanes::Furthest = 1000.0f;

void Camera::SetView(float aspect_ratio, float FOV, float far, float near, KeepAspect fixed) {
    if (float(FOV) <= 0.0f) {
        const float default_fov = 75.0f;
        FOV = default_fov;
        DEBUG("FOV == 0.0f, set to %f.\n", default_fov);
    } else if (FOV > 120.0f) {
        FOV = 120.0f;
    }
    m_aspect_ratio = aspect_ratio;
    m_FOV = FOV;
    m_fixed = fixed;
    float half_width, half_height;
    if (fixed == FixWidth) {
        half_height = near * tan(RadianOfDegree(FOV) / 2);
        half_width = half_height * aspect_ratio;
    } else {
        half_width = near * tan(RadianOfDegree(FOV) / 2);
        half_height = half_width / aspect_ratio;
    }
    m_clip = ClipPlanes(half_width, half_height, far, near);
    m_cached = false;
}

Camera::ClipPlanes::ClipPlanes(float r, float l, float t, float b, float f, float n) :
        right(r),
        left(l),
        top(t),
        bottom(b),
        far(f),
        near(n) {
    if (near < Nearest) {
        DEBUG("Clip plane near too close, clamped.\n");
        near = Nearest;
    }
    if (right <= left) {
        DEBUG("Clip plane invalid: right<=left, reset.\n");
        *this = ClipPlanes();
        return;
    }
    if (top <= bottom) {
        DEBUG("Clip plane invalid: top<=bottom, reset.\n");
        *this = ClipPlanes();
        return;
    }
    if (far <= near) {
        DEBUG("Clip plane invalid: far<=near, reset.\n");
        *this = ClipPlanes();
        return;
    }
}

Camera::ClipPlanes::ClipPlanes(float half_width, float half_height, float far, float near) :
        right(half_width),
        left(-half_width),
        top(half_height),
        bottom(-half_height),
        far(far),
        near(near) {
    if (near < Nearest) {
        DEBUG("Clip plane near too close, clamped.\n");
        near = Nearest;
    }
    if (right <= left) {
        DEBUG("Clip plane invalid: right<=left, reset.\n");
        *this = ClipPlanes();
        return;
    }
    if (top <= bottom) {
        DEBUG("Clip plane invalid: top<=bottom, reset.\n");
        *this = ClipPlanes();
        return;
    }
    if (far <= near) {
        DEBUG("Clip plane invalid: far<=near, reset.\n");
        *this = ClipPlanes();
        return;
    }
}

#endif /* end of include guard: CAMERA_CPP_5HOJB2CT */
