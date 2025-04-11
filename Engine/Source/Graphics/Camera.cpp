#include "Camera.hpp"

Camera::Camera(CameraProjection projection, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up, f32 fovDegrees, f32 aspectRatio)
    : mPosition{position}
    , mLookAt{lookAt}
    , mUp{up}
{
    if (projection == CameraProjection::Orthographic)
    {
        Throw("Orthographic projection is not yet supported.");
    }

    mProjectionMatrix = Mat4f::Perspective(DEG_TO_RAD(fovDegrees), aspectRatio, 0.1f, 100.0f);
    mViewMatrix = Mat4f::LookAt(position, lookAt, up);
}

auto Camera::GetProjectionMatrix() const -> const Mat4f&
{
    return mProjectionMatrix;
}

auto Camera::GetViewMatrix() const -> const Mat4f&
{
    return mViewMatrix;
}

void Camera::Translate(const Vec3f& translation)
{
    mPosition += translation;
    mViewMatrix = Mat4f::LookAt(mPosition, mLookAt, mUp);
}