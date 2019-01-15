#include <iostream>
#include <string>
#include <windows.h>
#include <engine/input/inputhandler.hpp>
#include <engine/math.hpp>
#include <engine/render.hpp>
#include "game/entities/entityfactory.hpp"
#include "game/entities/spaceship.hpp"
#include "game/entities/target.hpp"
#include "game/entities/bullet.hpp"

using namespace input;
using namespace rendering;
using namespace rendering::rendering3d;
using namespace math;

static const int WIDTH = 1280;
static const int HEIGHT = 640;

void demo() {
	renderer view{ WIDTH, HEIGHT };
	entityfactory factory{ view };
	spaceship ship{ factory.create_spaceship() };
	target diamond{ factory.create_target() };
	diamond.get_model().translate({ {200.0f, 200.0f, 300.0f} });

	inputhandler input{};
	rendering3d::view<float> view_angle = rendering3d::view<float>::xy;
	std::vector<bullet> bullets{};
	bool help = false;
	try {
		while (true) {
			if (input.on_event([&](std::string key) {
				if (key == "W") ship.pitch(-1.0f);
				else if (key == "S") ship.pitch(1.0f);
				else if (key == "A") ship.yaw(-1.0f);
				else if (key == "D") ship.yaw(1.0f);
				else if (key == "Q") ship.roll(-1.0f);
				else if (key == "E") ship.roll(1.0f);
				else if (key == "Left Shift") ship.move(1.0f);
				else if (key == "Left Ctrl") ship.move(-1.0f);
				else if (key == "1") view_angle = rendering3d::view<float>::xy;
				else if (key == "2") view_angle = rendering3d::view<float>::zy;
				else if (key == "3") view_angle = rendering3d::view<float>::xz;
				else if (key == "H") help = !help;
				else if (key == "Space") {
					bullet shot{ factory.create_bullet({{ship.get_model().center().x(), ship.get_model().center().y(), ship.get_model().center().z()}}) };
					shot.get_model().heading = ship.get_model().heading;
					bullets.push_back(shot);
				}
				else if (key == "Escape") return true;
				return false;
			})) break;

			view.display([&](int dt) {
				view.set_color(colors::RED);
				ship.get_model().render(view_angle, WIDTH / 2.0f, HEIGHT / 2.0f);
				view.set_color(colors::GREEN);
				diamond.get_model().render(view_angle, WIDTH / 2.0f, HEIGHT / 2.0f);
				std::for_each(bullets.begin(), bullets.end(), [&](auto& b) {
					b.get_model().render(view_angle, WIDTH / 2.0f, HEIGHT / 2.0f);
					b.update();
				});
				view.set_color(colors::BLUE);
				if(help) ship.get_model().render_angles(view_angle, WIDTH / 2.0f, HEIGHT / 2.0f);
				diamond.pulse(dt);
			});

			if (ship.get_collisionbox().calculate_collision(diamond.get_model())) {
				MessageBox(HWND_DESKTOP, "You dead off a error in the guiding system!", "You Lose", MB_OK);
				return;
			}
			for (int i = 0; i < bullets.size(); i++) {
				if (diamond.get_collisionbox().calculate_collision(bullets.at(i).get_model())) {
					MessageBox(HWND_DESKTOP, "Winner Winner chicken Dinner", "You win", MB_OK);
					return;
				}
			}
		}
	}
	catch (int e) {
		std::cout << "The program quit unexpectedly. ERROR " << std::to_string(e) << std::endl;
	}
}

int main(int argc, char* argv[])
{
	//logic
	demo();
	//memory leak detection
	_CrtDumpMemoryLeaks();
	//exit
	return 0;
}
