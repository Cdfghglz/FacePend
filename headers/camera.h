#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "opencv2/core/mat.hpp"
#include "common.h"

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	void viewToWorld(cv::Point3d* pt) {

		::Point<double> pt2 = {pt->x, pt->y};
		viewToWorld(&pt2, pt->z);

		pt->x = pt2.x;
		pt->y = pt2.y;

	}

	void viewToWorld(Point<double>* pt, const float projectionZ = 0) {

		glm::mat4 view = glm::lookAt(pos, pos + forward, up);
		glm::mat4 invVP = glm::inverse(view * projection);

		glm::vec4 pt_hom(pt->x, pt->y, 1.0f, 1.0f);
		glm::vec4 beamV4 = invVP * pt_hom;

		glm::vec3 plane(0.0f, 0.0f, 1.0f);
		glm::vec3 beamV3(beamV4.x, beamV4.y, beamV4.z);
		double scale = (pos.z - projectionZ)/(glm::dot(plane, beamV3));

		pt->x = beamV3.x * scale;
		pt->y = beamV3.y * scale;

	}

protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};

#endif
