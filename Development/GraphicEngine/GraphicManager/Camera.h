#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace camera {
	class Camera {
	public:

		Camera(float, float);


		void SetPosition(const glm::vec3&);

		glm::vec3 GetPosition() const;

		void SetZoom(float);

		float GetZoom() const;

		glm::mat4 GetProjectionMatrix() const;

		glm::mat4 GetViewMatrix() const;

		glm::mat4 GetViewProjectionMatrix() const;

		void UpdateProjection();

		void UpdateView();

		void UpdateViewProjection();

		void Move(const glm::vec3&, float);

		void Resize(float, float);

		void Follow(const glm::vec3&, float);

		void CameraUpdate();

		glm::vec3 ScreenToWorldCoordinates(const glm::vec3&) const;

	public:
		glm::vec3 direction;
		glm::vec2 prevMouseCursor;

	private:
		glm::vec3 position{ 0,0,0 };             /*!< The current position of the camera. */
		float zoom;                    /*!< The current zoom level of the camera. */
		float aspectRatio;             /*!< The current aspect ratio of the viewport. */
		float viewportWidth;           /*!< The width of the viewport. */
		float viewportHeight;          /*!< The height of the viewport. */
		float nearClipPlane;           /*!< The near clipping plane for projection. */
		float farClipPlane;            /*!< The far clipping plane for projection. */
		glm::mat4 projectionMatrix;    /*!< The projection matrix of the camera. */
		glm::mat4 viewMatrix;          /*!< The view matrix of the camera. */
		glm::mat4 cameraMatrix;        /*!< The combined view-projection matrix. */
		glm::vec3 target;               /*!< The target position for the camera to follow. */
		bool isFollowing;              /*!< Flag indicating if the camera is following a target. */
		float followSpeed;             /*!< Speed of following the target. */
		bool toUpdate{ true };                 /*!< Flag indicating if matrices need updating. */
		float fovDegree;

	};
}


