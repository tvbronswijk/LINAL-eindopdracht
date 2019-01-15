#pragma once
#include <engine/rendering/3D/model.hpp>
#include"game/collision/collisionbox.hpp"

class spaceship {
public:
	spaceship(rendering::rendering3d::model model) : _model(model), _collisionbox(collisionbox(model)) {}
	void pitch(float f) { rotate(f, 0.0f, 0.0f); }
	void yaw(float f) { rotate(0.0f, f, 0.0f); }
	void roll(float f) { rotate(0.0f, 0.0f, f); }
	void move(float f) { 
		math::uvector3D<float> heading = _model.center() * f;
		translate(heading.x(), heading.y(), heading.z()); 
	}
	rendering::rendering3d::model& get_model() { return _model; }
	collisionbox& get_collisionbox() { return _collisionbox; }
private:
	void translate(float x, float y, float z) {
		_model.translate({ {x, y, z} });
	}
	void rotate(float x, float y, float z) {
		_model.rotate({ {x, y, z} }, _model.center());
	}
	rendering::rendering3d::model _model;
	collisionbox _collisionbox;

};