#include "Camera.hpp"

Camera::Camera(CameraProjection projection, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up, f32 fovDegrees, f32 aspectRatio)
    : mPosition{position}
    , mLookAt{lookAt}
    , mUp{up}
    , mFovDegrees{fovDegrees}
{
    if (projection == CameraProjection::Orthographic)
    {
        Throw("Orthographic projection is not yet supported.");
    }

    mProjectionMatrix = Mat4f::Perspective(DEG_TO_RAD(fovDegrees), aspectRatio, 0.1f, 100.0f);
    mViewMatrix = Mat4f::LookAt(position, lookAt, up);
    mRight = Vec3f::Normalize(Vec3f::Cross(mLookAt, mUp));
}

auto Camera::GetProjectionMatrix() const -> const Mat4f&
{
    return mProjectionMatrix;
}

auto Camera::GetViewMatrix() const -> const Mat4f&
{
    return mViewMatrix;
}

void Camera::SetAspectRatio(f32 aspectRatio)
{
    mProjectionMatrix = Mat4f::Perspective(DEG_TO_RAD(mFovDegrees), aspectRatio, 0.1f, 100.0f);
}

auto Camera::Front() const -> const Vec3f&
{
    return mLookAt;
}

auto Camera::Right() const -> const Vec3f&
{
    return mRight;
}

void Camera::Rotate(f32 pitch, f32 yaw)
{
    mPitch += pitch;
    mYaw += yaw;

    // TODO: uncomment and fix
//    mPitch = std::clamp(mPitch, -std::numbers::pi_v<f32> / 2.0f, std::numbers::pi_v<f32> / 2.0f);
    mYaw = std::fmod(mYaw, 2.0f * std::numbers::pi_v<f32>);

    mLookAt = Vec3f::Normalize({
        std::cos(mYaw) * std::cos(mPitch),
        std::sin(mPitch),
        std::sin(mYaw) * std::cos(mPitch)
    });
    mRight = Vec3f::Normalize(Vec3f::Cross(mLookAt, mUp));
    mViewMatrix = Mat4f::LookAt(mPosition, mPosition + mLookAt, mUp);
}

void Camera::Translate(const Vec3f& translation)
{
    mPosition += translation;
    mViewMatrix = Mat4f::LookAt(mPosition, mPosition + mLookAt, mUp);
}