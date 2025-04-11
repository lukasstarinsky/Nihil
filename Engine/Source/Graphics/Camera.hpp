#pragma once

#include "Math/Math.hpp"

enum class CameraProjection
{
    Orthographic = 0,
    Perspective
};

class NIHIL_API Camera
{
public:
    Camera() = default;
    Camera(CameraProjection projection, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up, f32 fovDegrees, f32 aspectRatio);

    auto GetProjectionMatrix() const -> const Mat4f&;
    auto GetViewMatrix() const -> const Mat4f&;

    void Translate(const Vec3f& translation);
    void Rotate(f32 pitch, f32 yaw);
private:
    auto GetRight() const -> Vec3f;
private:
    Mat4f mProjectionMatrix {};
    Mat4f mViewMatrix {};

    Vec3f mPosition {};
    Vec3f mLookAt {};
    Vec3f mUp {};

    f32 mPitch {};
    f32 mYaw {};
};